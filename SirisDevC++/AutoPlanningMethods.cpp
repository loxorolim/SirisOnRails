#include "AutoPlanningMethods.h"

int AutoPlanning::getMetersSize()
{
	return meters.size();
}
int AutoPlanning::getPolesSize()
{
	return poles.size();
}
//Une dois vetores de inteiros sem repetição, nada demais.
//vector<int> AutoPlanning::concatVectors(vector<int> &v1, vector<int> &v2)
//{
//
//	vector<int> ret;
//	for (int i = 0; i < v1.size(); i++)
//		ret.push_back(v1[i]);
//	for (int i = 0; i < v2.size(); i++)
//	{
//		bool add = true;
//		for (int j = 0; j < v1.size(); j++)
//		if (v1[j] == v2[i])
//		{
//			add = false;
//			break;
//		}
//		if (add)
//			ret.push_back(v2[i]);
//	}
//	return ret;
//}
//Retorna um vetor com os índices dos medidores que não estão cobertos de acordo com a matriz de cobertura (SCP).
//A matriz de cobertura relaciona os medidores e postes e quem cobre quem.
//Por exemplo:
//     0 1 2
//  0| 0 1 1
//  1| 1 0 0
//  2| 0 0 0
//  3| 1 1 0
// As linhas são postes e as colunas são medidores. Isso significa que, o poste com índice 0 é capaz de cobrir
// os medidores 1 e 2, mas não o medidor 0. 0 = Não cobre e 1 = Cobre
// Esse método verifica essa matriz para verificar quais medidores tem a coluna toda zerada, o que significa que
// ninguém o cobre!
// PS: A matriz de cobertura não é uma matriz! É um vetor de vetores. Para o caso acima a real face da matriz de
// cobertura é:
// 0 - 1, 2
// 1 - 0,
// 2 -
// 3 - 0, 1
//vector<int> AutoPlanning::uncoverableMeters(vector<vector<int> > &SCP,int redundancy)
//{
//	
//	vector<int> uncoverableMeters;
//	for (int i = 0; i < meters.size(); i++)
//	{
//		int coverable = 0;
//		for (int j = 0; j < poles.size(); j++)
//		{			
//			int cov = -1;
//			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
//			if (cov)			
//				coverable++;
//			if (coverable >= redundancy)
//				break;
//			
//		}
//		if (coverable < redundancy)
//			uncoverableMeters.push_back(meters[i]->index);
//	}
//	return uncoverableMeters;
//}
//vector<int> AutoPlanning::coverableMeters(vector<vector<int> > &SCP, int redundancy)
//{
//
//	vector<int> coverableMeters;
//	for (int i = 0; i < meters.size(); i++)
//	{
//		int coverable = 0;
//		for (int j = 0; j < poles.size(); j++)
//		{
//			int cov = -1;
//			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
//			if (cov)
//				coverable++;
//			if (coverable >= redundancy)
//			{
//				coverableMeters.push_back(meters[i]->index);
//				break;
//			}
//		}		
//	}
//	return coverableMeters;
//}

//Esse método cria uma matriz que relaciona quais medidores alcançam outors medidores.
//A matriz é bem parecida à matriz de cobertura do SCP.
//A informação dessa matriz será utilizada nos cálculos Mesh para que já tenha pré-calculado quem alcança quem.
//Por exemplo:
//
// 0 - 1, 2
// 1 - 0
// 2 - 0
// No caso, o medidor de índice 0 alcança os medidores 1 e 2 e os medidores 1 e 2 alcançam 0. Obviamente, se um
// medidor "a" alcança "b", "b" deve alcançar "a".
vector<vector<int> > AutoPlanning::createMeterNeighbourhood(Grid *g)
{
	vector<vector<int> > M;
	for (int i = 0; i < meters.size(); i++) 
	{
		vector<int> pointsCovered;
		vector<Position*> meterRegion = g->getCell(meters[i]); //IMPORTANTE: Obviamente um medidor não precisa verificar
		//TODOS os outros. Por isso que se utiliza o GRID! Esse meterRegion são células adjacentes a celula do
		//medidor que está sendo analisado. Assim ele só analisa uma quantidade limitada de medidores e o processamento
		//fica mais rápido
		for (int j = 0; j < meterRegion.size(); j++)
		{
			double dist = getDistance(meters[i], meterRegion[j]);
			double eff = getLinkQuality(dist);
			if (meters[i]->index != meterRegion[j]->index && eff >= MARGIN_VALUE) //Se a eficiencia for superior a 90%(MARGIN_VALUE) então é vizinho!
				pointsCovered.push_back(meterRegion[j]->index);
		}
		M.push_back(pointsCovered);
	}
	return M;
}
//Esse método cria a matriz de cobertura que será utilizada no solver e assim obter o resultado do planejamento
//automático.
vector<vector<int> > AutoPlanning::createScp()
{
	Grid* g = new Grid(meters,poles, regionLimiter); //Primeiro cria-se um grid.
	//Grid* g = new Grid(regionLimiter);
	g->putPositions(meters);//Adiciona-se ao grid os medidores.
	vector<int> aux;
	vector<vector<int> > sM;
	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		vector<Position*> metersReduced = g->getCell(poles[i]);//Pega os medidores das células vizinhas e da sua
		// própria célula para fazer a análise.
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(poles[i], metersReduced[j]);
			double eff = getLinkQuality(dist);
			if (eff >= MARGIN_VALUE)//Se a eficiencia for superior a 90%(MARGIN_VALUE) então o poste cobre esse medidor.
				metersCovered.push_back(metersReduced[j]->index);
		}
		sM.push_back(metersCovered);
	}
	if (mesh)
	{
		vector<vector<int> > nM = createMeterNeighbourhood(g); //Cria a matriz de vizinhança para se analisar o mesh
		for (int i = 0; i < sM.size(); i++)//Essa etapa incrementa a matriz de cobertura para considerar o mesh.
		{
			vector<int> neighbours = sM[i];//Conjunto de medidores que o poste "i" cobre.
			for (int j = 0; j < mesh; j++)//Para cada salto
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)//Os vizinhos dos medidores que o poste cobre também serão
				//considerados como cobertos! Isso se repete X vezes, onde X é o número de saltos mesh.
				{
					int toFind = neighbours[k];
					int pos = 0;
					for (int l = 0; l < meters.size(); l++){ if (meters[l]->index == toFind){ pos = l; break; }}
					//Esse for aí em cima serve para localizar a posição do medidor com o respectivo index, pois
					// a matriz de vizinhança dos medidores não é organizada por index, mas sim 0,1,2...n.
					//sM[i] = concatVectors(sM[i], nM[pos]);//Os vizinhos agora fazem parte da cobertura do poste!
					sM[i].insert(sM[i].end(), nM[pos].begin(), nM[pos].end());
					sort(sM[i].begin(), sM[i].end());
					sM[i].erase(unique(sM[i].begin(), sM[i].end()), sM[i].end());
					//newNeighbours = concatVectors(newNeighbours, nM[pos]);//Novos vizinhos a serem analisados no
					newNeighbours.insert(newNeighbours.end(), nM[pos].begin(), nM[pos].end());
					sort(newNeighbours.begin(), newNeighbours.end());
					newNeighbours.erase(unique(newNeighbours.begin(), newNeighbours.end()), newNeighbours.end());
					//próximo salto. Esse newNeighbours vai ser a união de todos os vizinhos de todos os medidores
					//do salto em questão. Por exemplo, para o primeiro salto newNeighbours começa vazio, pois será
					//analisado os vizinhos dos medidores que o poste cobre. Para o segundo salto, o método tem que
					//analisar o vizinho desses vizinhos. Por isso que o neighbours recebe newNeighbours. newNeighbours
					// é a junção de todos os vizinhos sem repetição.
				}
				neighbours = newNeighbours;
			}
		}
	}
	delete g;
	return sM;
}

vector<vector<int> > AutoPlanning::createScpWithLimit(int limit)
{
	vector<vector<int> > completeSCP = createScp();
	int* covInfo = new int[meters.size()];
	for (int i = 0; i < meters.size(); i++){ covInfo[i] = 0; }
	for (int i = 0; i < completeSCP.size(); i++)
	{
		for (int j = 0; j < completeSCP[i].size(); j++)
		{
			int pos = -1;
			for (int k = 0; k < meters.size(); k++){ if (meters[k]->index == completeSCP[i][j]){ pos = k; break; } }
			covInfo[pos]++;
		}
	}
	for (int i = 0; i < completeSCP.size(); i++)
	{
		vector<int> coverageReduced;
		vector<int> orderedByDistance;
		vector<int> aux = completeSCP[i];
		while (aux.size() > 0)
		{
			int toAdd = 0;
			double dist = getDistance(poles[i], meters[aux[0]]);;
			for (int j = 1; j < aux.size(); j++)
			{
				double d = getDistance(poles[i], meters[aux[j]]);
				if (d < dist)
				{
					toAdd = j;
					dist = d;
				}
			}
			orderedByDistance.push_back(aux[toAdd]);
			aux.erase(aux.begin() + toAdd);
		}

		for (int j = 0; j < orderedByDistance.size(); j++)
		{
			if (coverageReduced.size() == limit)
			{
				break;
			}
			else if (covInfo[orderedByDistance[j]] == 1)
			{
				coverageReduced.push_back(orderedByDistance[j]);
				orderedByDistance.erase(orderedByDistance.begin() + j);
				j = 0;
			}
		}
		if (coverageReduced.size() < limit)
		{
			int aux = limit - coverageReduced.size();
			for (int j = 0; (j < aux) && orderedByDistance.size() > 0; j++)
			{
				coverageReduced.push_back(orderedByDistance[0]);
				orderedByDistance.erase(orderedByDistance.begin());
			}
		}
		completeSCP[i] = coverageReduced;
	}
	delete covInfo;
	return completeSCP;
}


void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP, int redundancy)
{
	//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
	//Pois se não, o método retornaria que a solução é impossível!
	//A formatação você ve no arquivo GlpkFile.txt

	vector<int> covInfo;
	for (int i = 0; i < meters.size(); i++){ covInfo.push_back(0); }
	for (int i = 0; i < SCP.size(); i++)
	{
		for (int j = 0; j < SCP[i].size(); j++)
		{
			int pos=-1;
			for (int k = 0; k < meters.size(); k++){
				if (meters[k]->index == SCP[i][j])
				{ 
					pos = k; break;
				} 
			}
			covInfo[pos]++;
		}
	}
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\nparam B{r in Z} default "+to_string(redundancy)+", integer;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=B[r];\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "Results.txt\";\n data;\n";
	resp += "set Z:= ";
	for (int i = 0; i < meters.size(); i++)
	{
		resp += "Z" + to_string(meters[i]->index) + " ";
	}

	resp += ";\n";
	resp += "set Y:= ";

	for (int i = 0; i < poles.size(); i++)
		resp += "Y" + to_string(poles[i]->index) + " ";
	resp += ";\n";
	resp += "param A := ";
	resp += "\n";
	for (int i = 0; i < meters.size(); i++)
	{
		for (int j = 0; j < poles.size(); j++)
		{
			//bool um = false;
			//for (int k = 0; k < SCP.size(); k++)
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
			if (cov)
				resp += "[Z" + to_string(meters[i]->index) + ",Y" + to_string(poles[j]->index) + "] 1";

		}
	}
	resp += "\n";
	resp += ";";
	resp += "\n";
	resp += "param B := ";
	resp += "\n";
	for (int i = 0; i < meters.size(); i++)
	{
		if (covInfo[i] < redundancy)
			resp += "[Z" + to_string(meters[i]->index) + "] "+to_string(covInfo[i]);
	}
	resp += "\n;\nend;\n";

	string filename = rubyPath + "GlpkFile.txt";
	ofstream f(filename.c_str());

	f << resp;
	f.close();
}
void AutoPlanning::saveGLPKFileReducedWithLimit(vector<vector<int> > &SCP, int redundancy, int limit)
{
	//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
	//Pois se não, o método retornaria que a solução é impossível!
	//A formatação você ve no arquivo GlpkFile.txt

	int* covInfo = new int[meters.size()];
	for (int i = 0; i < meters.size(); i++){ covInfo[i] = 0; }
	for (int i = 0; i < SCP.size(); i++)
	{
		for (int j = 0; j < SCP[i].size(); j++)
		{
			int pos = -1;
			for (int k = 0; k < meters.size(); k++){ if (meters[k]->index == SCP[i][j]){ pos = k; break; } }
			covInfo[pos]++;
		}
	}
	string resp;
	resp += "set Z;\n";
	resp += "set Y;\n";
	resp += "param A{ r in Z, m in Y } default 0, binary;\n";
	resp += "param B{r in Z} default "+to_string(redundancy)+", integer;\n";
	resp += "var Route{ m in Y }, binary;\n";
	resp += "var Link{ r in Z, m in Y }, binary;\n";
	resp += "minimize cost: (sum{m in Y} Route[m])/(card(Y)+1) + (card(Z)-sum{r in Z, m in Y} Link[r,m]);\n";
	resp += "subject to covers{ r in Z }: sum{ m in Y } A[r, m] * Route[m] >= B[r];\n";
	resp += "subject to linked{ m in Y }: sum{ r in Z } Link[r, m] <= "+to_string(limit)+";\n";
	resp += "subject to decis{ r in Z, m in Y }: Link[r, m] <= A[r, m] * Route[m];\n";
	resp += "subject to decis3{ r in Z }: sum{ m in Y } Link[r, m] <= 1;\n ";
	resp += "solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "Results.txt\";\n data;\n";
	resp += "set Z:= ";
	for (int i = 0; i < meters.size(); i++)
	{
		resp += "Z" + to_string(meters[i]->index + 1) + " ";
	}

	resp += ";\n";
	resp += "set Y:= ";

	for (int i = 0; i < poles.size(); i++)
		resp += "Y" + to_string(poles[i]->index + 1) + " ";
	resp += ";\n";
	resp += "param A := ";
	resp += "\n";
	for (int i = 0; i < meters.size(); i++)
	{
		for (int j = 0; j < poles.size(); j++)
		{
			//bool um = false;
			//for (int k = 0; k < SCP.size(); k++)
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
			if (cov)
				resp += "[Z" + to_string(meters[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1) + "] 1";

		}
	}
	resp += "\n";
	resp += ";";
	resp += "\n";
	resp += "param B := ";
	resp += "\n";
	for (int i = 0; i < meters.size(); i++)
	{
		if (covInfo[i] < redundancy)
			resp += "[Z" + to_string(meters[i]->index + 1) + "] " + to_string(covInfo[i]);
	}
	resp += "\n;\nend;\n";
	delete covInfo;

	string filename = rubyPath + "GlpkFile.txt";
	ofstream f(filename.c_str());

	f << resp;
	f.close();
}
////Esse método monta o arquivo de entrada pro GLPK.
//void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP, vector<Position*> metersToConsider, vector<Position*> polesToDisconsider, int redundancy)
//{
//		//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
//		//Pois se não, o método retornaria que a solução é impossível!
//		//A formatação você ve no arquivo GlpkFile.txt
//		string resp;
//		resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>="+to_string(redundancy)+";\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath +"Results.txt\";\n data;\n";
//		resp += "set Z:= ";
//		for (int i = 0; i < metersToConsider.size(); i++)
//		{
//				resp += "Z" + to_string(metersToConsider[i]->index + 1) + " ";
//		}
//		
//		resp += ";\n";
//		resp += "set Y:= ";
//
//		for (int i = 0; i < poles.size(); i++)
//			resp += "Y" + to_string(poles[i]->index + 1) + " ";
//		resp += ";\n";
//		resp += "param A := ";
//		resp += "\n";
//		for (int i = 0; i < metersToConsider.size(); i++)
//		{
//				for (int j = 0; j < poles.size(); j++)
//				{
//					//bool um = false;
//					//for (int k = 0; k < SCP.size(); k++)
//					int cov = -1;
//					cov = (find(SCP[j].begin(), SCP[j].end(), metersToConsider[i]->index) != SCP[j].end());
//					int covP = 1;
//					for (int k = 0; k < polesToDisconsider.size(); k++){ if (polesToDisconsider[k]->index == poles[j]->index){ covP = -1; break; } }
//					if (cov && covP)
//						resp += "[Z" + to_string(metersToConsider[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1)+"] 1";
//
//				}			
//		}
//		resp += "\n";
//		resp += ";";
//		resp += "end;\n";
//		
//		string filename = rubyPath + "GlpkFile.txt";
//		ofstream f(filename.c_str());
//
//		f << resp;
//		f.close();
//}

//Esse método faz um system call ao GLPK
vector<int> AutoPlanning::executeGlpk(string filename)
{
	//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
	//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
	//system(access.c_str());
	vector<int> answer;
	glp_term_out(GLP_OFF);
	glp_prob *lp;
	glp_tran *tran;
	int ret, count, cpeak;
	size_t total, tpeak;
	int err;
	double totalInMb, tpeakInMb;
	lp = glp_create_prob();
	tran = glp_mpl_alloc_wksp();
	ret = glp_mpl_read_model(tran, filename.c_str(), 0);
	glp_mem_limit(MEM_LIMIT);

	if (ret != 0)
	{
		fprintf(stderr, "Error on translating model\n");
		goto skip;
	}

	ret = glp_mpl_generate(tran, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "Error on generating model\n");
		goto skip;
	}

	glp_mpl_build_prob(tran, lp);
	glp_iocp parm;
	glp_init_iocp(&parm);
	parm.presolve = GLP_ON;
	err = glp_intopt(lp, &parm);
	for (int i = 1; i < poles.size() + 1; i++)
	{
		if (glp_mip_col_val(lp, i))
			answer.push_back(poles[i - 1]->index);
	}
	glp_mem_usage(&count, &cpeak, &total, &tpeak);
	printf("%d memory block(s) are still allocated\n", count);
	totalInMb = ((double)total / (1024 * 1024));
	tpeakInMb = ((double)tpeak / (1024 * 1024));
skip: glp_mpl_free_wksp(tran);
	glp_delete_prob(lp);
	return answer;
}
vector<int> AutoPlanning::executeGlpk(string filename, double &maxMem, double &solverTime)
{
	//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
	//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
	//system(access.c_str());

	glp_term_out(GLP_OFF);
	if (verbose) { cout << "\n"; glp_term_out(GLP_ON); }
	double seconds;
	clock_t begin_time = 0;
	vector<int> answer;
	glp_prob *lp;
	glp_tran *tran;
	int err;
	int ret, count, cpeak;
	size_t total, tpeak;
	double totalInMb, tpeakInMb;
	lp = glp_create_prob();
	tran = glp_mpl_alloc_wksp();
	ret = glp_mpl_read_model(tran, filename.c_str(), 0);
	glp_mem_limit(MEM_LIMIT);
	
	if (ret != 0)
	{
		fprintf(stderr, "Error on translating model\n");
		goto skip;
	}

	ret = glp_mpl_generate(tran, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "Error on generating model\n");
		goto skip;
	}
	glp_mpl_build_prob(tran, lp);
	glp_iocp parm;
	glp_init_iocp(&parm);
	parm.presolve = GLP_ON;
	parm.tm_lim = TIME_LIMIT;
	
	begin_time = clock();
	err = glp_intopt(lp, &parm);
	seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
	solverTime = seconds;
	for (int i = 1; i < poles.size() + 1; i++)
	{
		if (glp_mip_col_val(lp, i))
			answer.push_back(poles[i - 1]->index);
	}
	glp_mem_usage(&count, &cpeak, &total, &tpeak);
	//printf("%d memory block(s) are still allocated\n", count);
	totalInMb = ((double)total / (1024 * 1024));
	tpeakInMb = ((double)tpeak / (1024 * 1024));
	maxMem = totalInMb;
skip: glp_mpl_free_wksp(tran);
	glp_delete_prob(lp);
	return answer;
}


vector<pair<double,double> > getClusters(double *data, int numData)
{
	VlRand rand;
	vl_size dimension = 2;
	vl_size numCenters = 2;
	vl_size maxiter = 100;
	vl_size maxComp = 100;
	vl_size maxrep = 1;
	vl_size ntrees = 1;


	//VlKMeansAlgorithm algorithm = VlKMeansANN ;
	//VlKMeansAlgorithm algorithm = VlKMeansLloyd;
	VlKMeansAlgorithm algorithm = VlKMeansElkan ;
	VlVectorComparisonType distance = VlDistanceL2;
	VlKMeans * kmeans = vl_kmeans_new(VL_TYPE_DOUBLE, distance);

	vl_rand_init(&rand);
	vl_rand_seed(&rand, 1000);
	vl_kmeans_set_verbosity(kmeans, 1);
	vl_kmeans_set_max_num_iterations(kmeans, maxiter);
	vl_kmeans_set_max_num_comparisons(kmeans, maxComp);
	vl_kmeans_set_num_repetitions(kmeans, maxrep);
	vl_kmeans_set_num_trees(kmeans, ntrees);
	vl_kmeans_set_algorithm(kmeans, algorithm);
	vl_kmeans_init_centers_plus_plus(kmeans, data, dimension, numData, numCenters);
	//struct timeval t1,t2;
	//gettimeofday(&t1, NULL);
	vector<pair<double, double> > ret;
	vl_kmeans_cluster(kmeans, data, dimension, numData, numCenters);
	double* centers = (double*)vl_kmeans_get_centers(kmeans);
	int i, j;
	for (i = 0; i < numCenters; i++) {
		pair<double, double> toAdd(centers[dimension * i + 0], centers[dimension * i + 1] );
		ret.push_back(toAdd);
	}
	//vl_free(centers);

	//gettimeofday(&t2, NULL);

	//VL_PRINT("elapsed vlfeat: %f s\n",(double)(t2.tv_sec - t1.tv_sec) + ((double)(t2.tv_usec - t1.tv_usec))/1000000.);	
	vl_kmeans_delete(kmeans);

	return ret;
}

vector<ClusterProblem*> clusterizeProblem(vector<Position*> meters, vector<Position*> poles, vector<vector<int> > &invertedScp)
{
	double* data = (double*)vl_malloc(sizeof(double) * 2 * meters.size());
	for (int i = 0; i < meters.size(); i++)
	{
		data[i * 2 + 0] = meters[i]->latitude;
		data[i * 2 + 1] = meters[i]->longitude;
	}
	//for (int i = 0; i < poles.size(); i++)
	//{
	//	data[(meters.size() + i) * 2 + 0] = poles[i]->latitude;
	//	data[(meters.size() + i) * 2 + 1] = poles[i]->longitude;
	//}
	vector<pair<double, double> > clusters = getClusters(data, meters.size());
	vector<ClusterProblem*> subProblems;
	for (int i = 0; i < clusters.size(); i++)
		subProblems.push_back(new ClusterProblem);
	for (int i = 0; i < meters.size(); i++)
	{
		int chosenCluster = -1;
		double dist = -1;
		for (int j = 0; j < clusters.size(); j++)
		{
			double distancex = pow((clusters[j].first - meters[i]->latitude), 2);
			double distancey = pow((clusters[j].second - meters[i]->longitude), 2);
			double calcdistance = sqrt(distancex + distancey);
			if (calcdistance < dist || dist == -1)
			{
				dist = calcdistance;
				chosenCluster = j;
			}
		}
		subProblems[chosenCluster]->meters.push_back(meters[i]);
	}
	for (int i = 0; i < subProblems.size(); i++)
	{
		vector<int> toAdd;
		for (int j = 0; j < subProblems[i]->meters.size(); j++)
		{
			for (int k = 0; k < invertedScp[subProblems[i]->meters[j]->index].size(); k++)
			{
				toAdd.push_back(invertedScp[subProblems[i]->meters[j]->index][k]);
			}
		}
		sort(toAdd.begin(), toAdd.end());
		toAdd.erase(unique(toAdd.begin(), toAdd.end()), toAdd.end());
		for (int j = 0; j < toAdd.size(); j++)
		{
			subProblems[i]->poles.push_back(poles[toAdd[j]]);
		}
	}
	//for (int i = 0; i < poles.size(); i++)
	//{
	//	int chosenCluster = -1;
	//	double dist = -1;
	//	for (int j = 0; j < clusters.size(); j++)
	//	{
	//		double distancex = pow((clusters[j].first - poles[i]->latitude), 2);
	//		double distancey = pow((clusters[j].second - poles[i]->longitude), 2);
	//		double calcdistance = sqrt(distancex + distancey);
	//		if (calcdistance < dist || dist == -1)
	//		{
	//			dist = calcdistance;
	//			chosenCluster = j;
	//		}
	//	}
	//	subProblems[chosenCluster]->poles.push_back(poles[i]);
	//}
	vl_free(data);
	return subProblems;
}
void evaluateSCP(vector<vector<int> > &SCP, int metersSize, subProblem* sp )
{
	int numOfMeters, nummOfPoles;
	double solverTime, memUsed, density, avgCoverage, coverageDeviation;
	sp->numOfMeters = metersSize;
	sp->numOfPoles = SCP.size();
	int numOfCov = 0;
	for (int i = 0; i < SCP.size(); i++)
		numOfCov += SCP[i].size();
	sp->numOfCoverage = numOfCov;
	sp->density = (double)numOfCov / (SCP.size()*metersSize);
	sp->avgCoverage = (double)numOfCov / (SCP.size());
	double variance = 0;
	for (int i = 0; i < SCP.size(); i++)
		variance += pow(SCP[i].size() - sp->avgCoverage, 2);
	variance = variance / SCP.size();
	sp->coverageDeviation = sqrt(variance);
}
vector<int> AutoPlanning::clusterAutoPlanning(bool usePostOptimization, int redundancy)
{
	if (verbose) cout << "\n Iniciando planejamento por clusters";
	const clock_t begin_time = clock();
	double solverTime;
	if (verbose) cout << "\n Criando matriz de cobertura";
	vector<vector<int> > scp = createScp();
	if (verbose)
	{
		cout << "\n Matriz de cobertura criada";
		cout << "\n Criando matriz de cobertura invertida";
	}
	vector<vector<int> > invertedSCP;
	invertedSCP.resize(meters.size());
	for (int i = 0; i < scp.size(); i++)
	{
		for (int j = 0; j < scp[i].size(); j++)
		{
			invertedSCP[scp[i][j]].push_back(i);
		}
	}
	if (verbose) cout << "\n Matriz de cobertura invertida criada";
	vector<ClusterProblem*> subProblems;
	ClusterProblem* init = new ClusterProblem;
	init->meters = meters; init->poles = poles;
	subProblems.push_back(init);
	bool cont = true;
	while (true)
	{
		bool success = false;
		vector<int> toRemove;
		vector<ClusterProblem*> toConcat;
		for (int i = 0; i < subProblems.size(); i++)
		{
			double memEst = memEstimation(subProblems[i]->meters.size(), subProblems[i]->poles.size());
			if (memEst*MEM_EST_SAFETY >= MEM_LIMIT)
			{
				if (verbose) cout << "\n Instancia " + to_string(i + 1) + " pode estourar o limite de memoria.\n Dividindo-a em duas. ";
				vector<ClusterProblem*> clusterized = clusterizeProblem(subProblems[i]->meters, poles, invertedSCP);
				toConcat.insert(toConcat.end(), clusterized.begin(), clusterized.end());
				//subProblems.erase(subProblems.begin() + i);
				toRemove.push_back(i);
				success = true;
			}
		}
		for (int i = 0; i < toRemove.size(); i++)
		{
			free(subProblems[toRemove[i]]);
			subProblems.erase(subProblems.begin() + toRemove[i]);
			for (int j = 0; j < toRemove.size(); j++)
				toRemove[j]--;
		}
		subProblems.insert(subProblems.end(), toConcat.begin(), toConcat.end());
		if (!success)
			break;
	}
	double maxMem = -1;
	vector<int> chosenDaps;
	vector<Position*> metersAux = meters, polesAux = poles;
	if (verbose) cout << "\n A instancia inicial foi dividida em  " + to_string(subProblems.size()) + " sub-instancias ";
	for (int i = 0; i < subProblems.size(); i++)
	{
		if (verbose) cout << "\n Iniciando planejamento para sub-instancia de numero " + to_string(i + 1);
		meters = subProblems[i]->meters;
		poles = subProblems[i]->poles;
		if (verbose) cout << "\n Criando matriz de cobertura para a sub-instancia de numero " + to_string(i + 1);
		vector<vector<int> > cellSCP = createScp();
		if (verbose) cout << "\n Matriz de cobertura para sub-instancia de numero " + to_string(i + 1) + " criada";
		subProblem* sp = new subProblem();
		evaluateSCP(cellSCP, meters.size(), sp);
		if (verbose) cout << "\n Salvando arquivo GLPK para a sub-instancia de numero " + to_string(i + 1);
		saveGLPKFileReduced(cellSCP, redundancy);
		if (verbose) cout << "\n Arquivo GLPK para a sub-instancia de numero " + to_string(i + 1) + " salvo";
		double memUsageInCell = -1;
		if (verbose) cout << "\n Executando metodo exato no GLPK para a sub-instancia de numero " + to_string(i + 1);
		vector<int> answer = executeGlpk(rubyPath + "GlpkFile.txt", memUsageInCell, solverTime);
		if (verbose) cout << "\n Solucao obitada para a sub-instancia de numero " + to_string(i + 1);
		if (memUsageInCell > maxMem)
			maxMem = memUsageInCell;
		for (int i = 0; i < answer.size(); i++)
			chosenDaps.push_back(answer[i]);

	}
	meters = metersAux;
	poles = polesAux;
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());

	if (usePostOptimization && subProblems.size() > 1)//PÓS OTIMIZAÇÃO
	{
		if(verbose) cout << "\n Iniciando pós otimização\n";
		vector<int> chosen;
		for (int i = 0; i < poles.size(); i++)
			chosen.push_back(0);
		for (int i = 0; i < chosenDaps.size(); i++)
		{
			chosen[chosenDaps[i]] = 1;
		}

		RolimEGuerraLocalSearchWithRedundancy(scp, invertedSCP, chosen, redundancy);

		vector<int> resultPosOpt;
		for (int i = 0; i < poles.size(); i++)
		{
			if (chosen[i] == 1)
				resultPosOpt.push_back(i);
		}
		chosenDaps = resultPosOpt;

	}
	for (int i = 0; i < subProblems.size(); i++)
	{
		delete(subProblems[i]);
	}
	if (verbose) cout << "\n Planejamento concluido";
	return chosenDaps;
	
}
TestResult* AutoPlanning::clusterAutoPlanningTestMode(bool usePostOptimization, int redundancy)
{
	const clock_t begin_time = clock();
	double solverTime;
	if (verbose) cout << "\n Criando matriz de cobertura";
	vector<vector<int> > scp = createScp();
	if (verbose)
	{
		cout << "\n Matriz de cobertura criada";
		cout << "\n Criando matriz de cobertura invertida";
	}
	vector<vector<int> > invertedSCP;
	invertedSCP.resize(meters.size());
	for (int i = 0; i < scp.size(); i++)
	{
		for (int j = 0; j < scp[i].size(); j++)
		{
			invertedSCP[scp[i][j]].push_back(i);
		}
	}
	if (verbose) cout << "\n Matriz de cobertura invertida criada";
	vector<ClusterProblem*> subProblems;
	ClusterProblem* init = new ClusterProblem;
	init->meters = meters; init->poles = poles;
	subProblems.push_back(init);
	bool cont = true;
	while (true)
	{
		bool success = false;
		vector<int> toRemove;
		vector<ClusterProblem*> toConcat;
		for (int i = 0; i < subProblems.size(); i++)
		{
			double memEst = memEstimation(subProblems[i]->meters.size(), subProblems[i]->poles.size());
			if (memEst*MEM_EST_SAFETY >= MEM_LIMIT)
			{
				if (verbose) cout << "\n Instancia " + to_string(i+1) + " pode estourar o limite de memoria.\n Dividindo-a em duas. ";
				vector<ClusterProblem*> clusterized = clusterizeProblem(subProblems[i]->meters,poles,invertedSCP);
				toConcat.insert(toConcat.end(), clusterized.begin(), clusterized.end());
				//subProblems.erase(subProblems.begin() + i);
				toRemove.push_back(i);
				success = true;
			}
		}
		for (int i = 0; i < toRemove.size(); i++)
		{
			free(subProblems[toRemove[i]]);
			subProblems.erase(subProblems.begin() + toRemove[i]);
			for (int j = 0; j < toRemove.size(); j++)
				toRemove[j]--;
		}
		subProblems.insert(subProblems.end(), toConcat.begin(), toConcat.end());
		if (!success)
			break;
	}
	double maxMem = -1;
	vector<int> chosenDaps;
	vector<Position*> metersAux = meters, polesAux = poles;
	TestResult* result = new TestResult();
	if (verbose) cout << "\n A instancia inicial foi dividida em  " + to_string(subProblems.size()) + " sub-instancias ";
	for (int i = 0; i < subProblems.size();i++)
	{
		if (verbose) cout << "Iniciando planejamento para sub-instancia de numero " + to_string(i+1);
			
		
		meters = subProblems[i]->meters;
		poles = subProblems[i]->poles;
		if (verbose) cout << "\n Criando matriz de cobertura para a sub-instancia de numero " + to_string(i + 1);
		vector<vector<int> > cellSCP = createScp();
		if (verbose) cout << "\n Matriz de cobertura para sub-instancia de numero " + to_string(i + 1) + " criada";
		subProblem* sp = new subProblem();
		evaluateSCP(cellSCP, meters.size(), sp);
		if (verbose) cout << "\n Salvando arquivo GLPK para a sub-instancia de numero " + to_string(i + 1);
		saveGLPKFileReduced(cellSCP, redundancy);
		if (verbose) cout << "\n Arquivo GLPK para a sub-instancia de numero " + to_string(i + 1)+ " salvo";
		double memUsageInCell = -1;
		if (verbose) cout << "\n Executando metodo exato no GLPK para a sub-instancia de numero " + to_string(i + 1);
		vector<int> answer = executeGlpk(rubyPath + "GlpkFile.txt", memUsageInCell, solverTime);
		if (verbose) cout << "\n Solucao obitada para a sub-instancia de numero " + to_string(i + 1);
		sp->solverTime = solverTime;
		sp->memUsed = memUsageInCell;
		result->subProblemStats.push_back(sp);
		result->solverTime = solverTime;
		if (memUsageInCell > maxMem)
			maxMem = memUsageInCell;
		for (int i = 0; i < answer.size(); i++)
			chosenDaps.push_back(answer[i]);

	}
	meters = metersAux;
	poles = polesAux;
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());

	result->solutionQuality = chosenDaps.size();
	result->chosenPoles = chosenDaps;
	double secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	result->time = secondsgp;
	if (usePostOptimization && subProblems.size() > 1)//PÓS OTIMIZAÇÃO
	{
		if(verbose) cout << "\nIniciando pós otimização";
		vector<int> chosen;
		for (int i = 0; i < poles.size(); i++)
			chosen.push_back(0);
		for (int i = 0; i < chosenDaps.size(); i++)
		{
			chosen[chosenDaps[i]] = 1;
		}

		RolimEGuerraLocalSearchWithRedundancy(scp, invertedSCP, chosen, redundancy);

		vector<int> resultPosOpt;
		for (int i = 0; i < poles.size(); i++)
		{
			if (chosen[i] == 1)
				resultPosOpt.push_back(i);
		}
		result->poChosenPoles = resultPosOpt;
		result->poSolutionQuality = resultPosOpt.size();
	}
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	result->poTime = secondsgp;
	result->maxMem = maxMem;
	for (int i = 0; i < subProblems.size(); i++)
	{
		delete(subProblems[i]);
	}
	//cout << result->toString();
	//	return str;
	return result;
}
TestResult* AutoPlanning::executeClusterAutoPlanTestMode(int usePostOptimization, int redundancy)
{
	if (verbose) cout << "\n Iniciando planejamento por clusters";
	TestResult* result;
	//cout << "Iniciando planejamento\n";
	result = clusterAutoPlanningTestMode(usePostOptimization, redundancy);
	result->gridSize = gridLimiter;
	result->numMeters = meters.size();
	result->numPoles = poles.size();
	vector<int> xgp = result->chosenPoles;
	vector<Position*> daps;
	vector<Position*> metersCopy;

	for (int i = 0; i < meters.size(); i++)
	{
		Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
		metersCopy.push_back(copy);
	}
	for (int i = 0; i < xgp.size(); i++)
	{
		Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
		daps.push_back(dapToInsert);
	}
	////Calcula métricas sem pos-opt
	MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	MetricResult* metricResult = mc->executeMetricCalculationTest();
	result->metersPerHop = metricResult->meterPerHop;
	result->qualityPerHop = metricResult->linkQualityPerHop;
	result->redundancy = metricResult->minMedMaxRedundancyPerMeter;
	result->metersPerDap = metricResult->minMedMaxMetersPerDap;
	result->uncoveredMeters = metricResult->uncoveredMeters;
	delete metricResult;
	delete mc;
	//Calcula métricas com pos-opt
	metersCopy.clear();
	daps.clear();
	if (usePostOptimization)
	{
		xgp = result->poChosenPoles;
		for (int i = 0; i < meters.size(); i++)
		{
			Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
			metersCopy.push_back(copy);
		}
		for (int i = 0; i < xgp.size(); i++)
		{
			Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
			daps.push_back(dapToInsert);
		}
		mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
		metricResult = mc->executeMetricCalculationTest();
		result->poMetersPerHop = metricResult->meterPerHop;
		result->poQualityPerHop = metricResult->linkQualityPerHop;
		result->poRedundancy = metricResult->minMedMaxRedundancyPerMeter;
		result->poMetersPerDap = metricResult->minMedMaxMetersPerDap;
		delete metricResult;
		delete mc;
	}
	//cout << result->toString();
	if (verbose) cout << "\n Planejamento por clusters finalizado";
	return result;
}
//Essa aqui é minha heurística que faz o método exato pra cada célula.
string AutoPlanning::gridAutoPlanning(int redundancy, int limit)
{
	Grid* metergrid = new Grid(meters, poles, gridLimiter);//cria o grid dos medidores, bla bla bla.
	//Grid* metergrid = new Grid(100000);
	metergrid->putPositions(meters);
	Grid* polegrid = new Grid(poles, meters, gridLimiter);//cria o grid dos postes
	//Grid* polegrid = new Grid(100000);
	polegrid->putPositions(poles);
	vector<Position*> metersAux = meters, polesAux = poles;
	map<pair<int, int>, vector<Position*> > meterCells = metergrid->getCells();
	vector<int> chosenDaps;
	int i = 1;
	//Mais informação, ler meu artigo da SBRC ;)
	for (map<pair<int, int>, vector<Position*> >::iterator it = meterCells.begin(); it != meterCells.end(); ++it)
	{

		vector<Position*> cellsMeters;
		vector<Position*> cellsPoles;

		cellsMeters = it->second; //Recebe todas as posições dos medidores que estão na célula
		cellsPoles = polegrid->getCell(cellsMeters[0]);//Pega a posição de um desses medidores e usa como referência pra pegar os postes da mesma célula e das células vizinhas.
		meters = cellsMeters;
		poles = cellsPoles;
		vector<vector<int> > cellSCP = createScp();
		if (limit == -1)
			saveGLPKFileReduced(cellSCP, redundancy);
		else
			saveGLPKFileReducedWithLimit(cellSCP, redundancy, limit);
		vector<int> answer = executeGlpk(rubyPath + "GlpkFile.txt");
		ifstream f((rubyPath + "Results.txt").c_str());
		string gridAnswer;
		getline(f, gridAnswer);

		for (int i = 0; i < answer.size(); i++)
		{
			//string snum = x[i].substr(1);
			chosenDaps.push_back(answer[i]);
		}
	}
	string str = "";
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		str += to_string(chosenDaps[i]) + " ";
	}

	delete metergrid;
	delete polegrid;
	meters = metersAux;
	poles = polesAux;
	return str;




}

//Pode ignorar, usei pra testes.
TestResult* AutoPlanning::gridAutoPlanningTestMode( bool usePostOptimization, int redundancy)
{

	TestResult* result = new TestResult();
	double solverTime;
	const clock_t begin_time = clock();

	Grid* metergrid = new Grid(meters,poles,gridLimiter);
	metergrid->putPositions(meters);
	Grid* polegrid = new Grid(meters,poles,gridLimiter);
	polegrid->putPositions(poles);
	vector<Position*> metersAux = meters, polesAux = poles;
	map<pair<int, int>, vector<Position*> > meterCells = metergrid->getCells();
	vector<int> chosenDaps;
	int numCel = 1;
	double maxMem = -1;

	for (map<pair<int, int>, vector<Position*> >::iterator it = meterCells.begin(); it != meterCells.end(); ++it)
	{
		cout << "Planejando célula " << to_string(numCel) << " de " << to_string(meterCells.size()) << "\n";
		numCel++;
		vector<Position*> cellsMeters;
		vector<Position*> cellsPoles;

		cellsMeters = it->second; //Recebe todas as posições dos medidores que estão na célula
		//cellsPoles = polesAux;
		cellsPoles = polegrid->getCell(cellsMeters[0]);//Pega a posição de um desses medidores e usa como referência pra pegar os postes da mesma célula e das células vizinhas.
		meters = cellsMeters;
		poles = cellsPoles;
		vector<vector<int> > cellSCP = createScp();
		saveGLPKFileReduced(cellSCP,redundancy);
		double memUsageInCell= -1;
		vector<int> answer = executeGlpk(rubyPath + "GlpkFile.txt", memUsageInCell, solverTime);
		if (memUsageInCell > maxMem)
			maxMem = memUsageInCell;
		for (int i = 0; i < answer.size(); i++)
			chosenDaps.push_back(answer[i]);
		
	}
	meters = metersAux;
	poles = polesAux;
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());

	result->solutionQuality = chosenDaps.size();
	result->chosenPoles = chosenDaps;
	double secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	result->time = secondsgp;
	if (usePostOptimization && meterCells.size() > 1)//PÓS OTIMIZAÇÃO
	{	
		cout << "Iniciando pós otimização\n";
		vector<int> chosen ;
		for (int i = 0; i < poles.size(); i++)
			chosen.push_back(0);
		for (int i = 0; i < chosenDaps.size(); i++)
		{
			chosen[chosenDaps[i]] = 1;
		}
		vector<vector<int> > scp = createScp();
		vector<vector<int> > invertedSCP;
		invertedSCP.resize(meters.size());
		for (int i = 0; i < scp.size(); i++)
		{
			for (int j = 0; j < scp[i].size(); j++)
			{
				invertedSCP[scp[i][j]].push_back(i);
			}
		}
		RolimEGuerraLocalSearchWithRedundancy(scp, invertedSCP, chosen,redundancy);

		vector<int> resultPosOpt;
		for (int i = 0; i < poles.size(); i++)
		{
			if (chosen[i] == 1)
				resultPosOpt.push_back(i);
		}
		result->poChosenPoles = resultPosOpt;
		result->poSolutionQuality = resultPosOpt.size();
	}
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	result->poTime = secondsgp;

	delete metergrid;
	delete polegrid;

	result->maxMem = maxMem;
//	return str;
	return result;
}
//Executa o planejamento automático
//string AutoPlanning::executeAutoPlan()
//{
//	//vector<vector<int> > SCP = createScp();
//	//saveGLPKFileReduced(SCP);
//	return gridAutoPlanning(1,-1);
//	//executeGlpk("GlpkFile.txt");
//	//ifstream f("Results.txt");
//	//string str;
//	//getline(f, str);
//	//return str;
//}
//vector<Position*> AutoPlanning::getMetersThatSatisfyRedundancy(int redundancy, vector< vector< int > > invertedSCP)
//{	
//	vector<Position*> metersThatSatisfy;
//	for (int i = 0; i < invertedSCP.size(); i++)
//	{
//		if (invertedSCP[i].size() >= redundancy)
//			metersThatSatisfy.push_back(meters[i]);
//	}
//	return metersThatSatisfy;
//}
string AutoPlanning::executeAutoPlan(int redundancy,int limit)
{

	//return gridAutoPlanning(redundancy,limit);
	vector<int> chosenDaps = clusterAutoPlanning(true, redundancy);
	Document document;
	document.SetObject();
	Value array(rapidjson::kArrayType);
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		Value v;
		v.SetInt(chosenDaps[i]);
		array.PushBack(v, allocator);
	}
	document.AddMember("ChosenDAPs", array, allocator);
	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);
	return strbuf.GetString();
}
//Coisa de teste
TestResult* AutoPlanning::executeAutoPlanTestMode(int usePostOptimization, int redundancy)
{
	TestResult* result;

	cout << "Iniciando planejamento\n";
	result = gridAutoPlanningTestMode(usePostOptimization,redundancy);
	result->gridSize = gridLimiter;
	result->numMeters = meters.size();
	result->numPoles = poles.size();
	vector<int> xgp = result->chosenPoles;
	vector<Position*> daps;
	vector<Position*> metersCopy;

	for (int i = 0; i < meters.size(); i++)
	{
		Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
		metersCopy.push_back(copy);
	}
	cout << "Calculando métricas\n";
	for (int i = 0; i < xgp.size(); i++)
	{
		Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
		daps.push_back(dapToInsert);
	}
	//Calcula métricas sem pos-opt
	MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	MetricResult* metricResult = mc->executeMetricCalculationTest();
	result->metersPerHop = metricResult->meterPerHop;
	result->qualityPerHop = metricResult->linkQualityPerHop;
	result->redundancy = metricResult->minMedMaxRedundancyPerMeter;
	result->uncoveredMeters = metricResult->uncoveredMeters;
	delete metricResult;
	delete mc;
	//Calcula métricas com pos-opt
	metersCopy.clear();
	daps.clear();
	if (usePostOptimization)
	{
		xgp = result->poChosenPoles;
		for (int i = 0; i < meters.size(); i++)
		{
			Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
			metersCopy.push_back(copy);
		}
		for (int i = 0; i < xgp.size(); i++)
		{
			Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
			daps.push_back(dapToInsert);
		}
		mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
		metricResult = mc->executeMetricCalculationTest();
		result->poMetersPerHop = metricResult->meterPerHop;
		result->poQualityPerHop = metricResult->linkQualityPerHop;
		result->poRedundancy = metricResult->minMedMaxRedundancyPerMeter;
		delete metricResult;
		delete mc;
	}
	cout << result->toString();
	return result;
}
vector<vector<int> > AutoPlanning::coverageList(vector<Position*> &daps)
{
	Grid* g = new Grid(meters, daps, regionLimiter);
	//Grid* g = new Grid(regionLimiter);
	g->putPositions(meters);
	vector<int> aux;
	vector<vector<int> > sM;
	//sM.reserve(meters.size());

	for (int i = 0; i < daps.size(); i++)
	{
		vector<int> metersCovered;
		vector<Position*> metersReduced = g->getCell(daps[i]);
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(daps[i], metersReduced[j]);
			double eff = getLinkQuality(dist);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}
		sM.push_back(metersCovered);
	}
	if (mesh)
	{

		vector<vector<int> > nM = createMeterNeighbourhood(g);
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> neighbours = sM[i];
			for (int j = 0; j < mesh; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{

					int toFind = neighbours[k];
					int pos = 0;
					for (int l = 0; l < meters.size(); l++){ if (meters[l]->index == toFind){ pos = l; break; } }

					//sM[i] = concatVectors(sM[i], nM[pos]);
					sM[i].insert(sM[i].end(), nM[pos].begin(), nM[pos].end());
					sort(sM[i].begin(), sM[i].end());
					sM[i].erase(unique(sM[i].begin(), sM[i].end()), sM[i].end());

					//newNeighbours = concatVectors(newNeighbours, nM[pos]); //ESSA PARTE AQUI É PASSÍVEL DE OTIMIZAÇÃO! DIMINUIR NEWNEIGHBOURS DE SM[I]!!!
					newNeighbours.insert(newNeighbours.end(), nM[pos].begin(), nM[pos].end());
					sort(newNeighbours.begin(), newNeighbours.end());
					newNeighbours.erase(unique(newNeighbours.begin(), newNeighbours.end()), newNeighbours.end());
				}
				neighbours = newNeighbours;
			}
		}

	}
	delete g;
	return sM;
}
void AutoPlanning::removeAlreadyCoveredMeters(vector<Position*> &DAPs)
{
	vector<vector<int>> covList = coverageList(DAPs);
	vector<int> toRemove;
	for (int i = 0; i < covList.size(); i++)
	{
		for (int j = 0; j < covList[i].size(); j++)
		{
			toRemove.push_back(meters[covList[i][j]]->index);
		}
	}
	sort(toRemove.begin(), toRemove.end());
	toRemove.erase(unique(toRemove.begin(), toRemove.end()), toRemove.end());
	for (int i = 0; i < toRemove.size(); i++)
	{
		for (int j = 0; j < meters.size(); j++)
		{ 
			if (meters[j]->index == toRemove[i])
			{ 
				meters.erase(meters.begin()+j); 
				break;
			} 
		}
	}
}


//////////////////////////////////////////MÉTODOS PRO GRASP////////////////////////////////////////////////
//int iterations = 500;
double p = 0.5;
//double alpha = 0.9;
vector<int> generateRCL(vector<vector<int> > &scp, int* solution, double alpha)
{
	vector<pair<int,int> > numSatisfied;
	int max = 0;
	for (int i = 0; i < scp.size(); i++)
	{
		if (solution[i] != 1)
		{
			int s = scp[i].size();
			if (s > alpha*max && s != 0)
			{
				pair<int,int> pair;
				pair.first = i;
				pair.second = s;
				numSatisfied.push_back(pair);
				if (s > max)
					max = s;
			}
		}
	}
	float L = alpha*max;
	vector<int> RCL;
	for (int i = 0; i < numSatisfied.size(); i++)
	{
		if (numSatisfied[i].second >= L)
			RCL.push_back(numSatisfied[i].first);
	}
	return RCL;
}
vector<int> generateRCL2(vector<int> &polesCoverageValue, int* solution, double alpha)
{
	vector<pair<int, int> > numSatisfied;
	int max = 0;
	for (int i = 0; i < polesCoverageValue.size(); i++)
	{
		if (solution[i] != 1)
		{
			int s = polesCoverageValue[i];
			if (s > alpha*max && s != 0)
			{
				pair<int, int> pair;
				pair.first = i;
				pair.second = s;
				numSatisfied.push_back(pair);
				if (s > max)
					max = s;
			}
		}
	}
	float L = alpha*max;
	vector<int> RCL;
	for (int i = 0; i < numSatisfied.size(); i++)
	{
		if (numSatisfied[i].second >= L)
			RCL.push_back(numSatisfied[i].first);
	}
	return RCL;
}

int* constructPhase(vector<vector<int> > scp,vector<vector<int> >& invertedSCP, int* solution,double alpha, int redundancy = 1)
{
	//vector<vector<int>> scpCopy = copyScp(scp);
	//vector<vector<int>> cMatrix = coverageMatrix(scp, size);
	int tam = invertedSCP.size();
	vector<int> redundancyInfo;

	for (int i = 0; i < invertedSCP.size(); i++)
	{
		redundancyInfo.push_back(0);
	}
	for (int i = 0; i < scp.size(); i++)
	{
		for (int j = 0; j < scp[i].size(); j++)
		{
			if (redundancyInfo[scp[i][j]] < redundancy)
				redundancyInfo[scp[i][j]]++;
		}
	}
	while (true)
	{
		vector<int> RCL = generateRCL(scp, solution,alpha);
		if (RCL.size() == 0)
			break;
		int cand = RCL[rand() % RCL.size()];
		solution[cand] = 1;

		int sizeToReduce = 0;
		for (int i = 0; i < scp[cand].size(); i++)
		{
			redundancyInfo[scp[cand][i]]--;
			if (redundancyInfo[scp[cand][i]] == 0)
				sizeToReduce++;
		}
		tam -= sizeToReduce;

//		vector<vector<int>> scpCopy = scp;
		int fSize = scp[cand].size();
		for(int i = 0; i < scp[cand].size();i++)
		{
			//vector<int> aaa = scp[cand];
			int sSize = invertedSCP[scp[cand][i]].size();
			for(int j = 0; j < sSize; j++)
			{
				//vector<int> bbb = invertedSCP[scp[cand][i]];
				int tSize = scp[invertedSCP[scp[cand][i]][j]].size();
				if (invertedSCP[scp[cand][i]][j] != cand)
				{
					for (int k = 0; k < tSize; k++)
					{
						//vector<int> ccc = scp[invertedSCP[scp[cand][i]][j]];
						if (scp[invertedSCP[scp[cand][i]][j]][k] == scp[cand][i])
						{
							if (redundancyInfo[scp[invertedSCP[scp[cand][i]][j]][k]] == 0)
							{
								scp[invertedSCP[scp[cand][i]][j]].erase(scp[invertedSCP[scp[cand][i]][j]].begin() + k);
								break;
							}
						}
					}
				}
			}
		}
		scp[cand].clear();


//		removeCovered(scp, cMatrix, cand, &tam);
//		updateMatrix(scpCopy, cMatrix, cand);
	}

	return solution;
}
int* constructPhase2(vector<vector<int> > &scp, vector<vector<int> >& invertedSCP, int* solution, double alpha, int redundancy = 1)
{
	//vector<vector<int>> scpCopy = copyScp(scp);
	//vector<vector<int>> cMatrix = coverageMatrix(scp, size);
	int tam = invertedSCP.size();
	vector<int> redundancyInfo;
	vector<int> polesCoverageValue;
	for (int i = 0; i < scp.size(); i++)
	{
		polesCoverageValue.push_back(scp[i].size());
	}
	for (int i = 0; i < invertedSCP.size(); i++)
	{
		redundancyInfo.push_back(0);
	}
	for (int i = 0; i < scp.size(); i++)
	{
		for (int j = 0; j < scp[i].size(); j++)
		{
			if (redundancyInfo[scp[i][j]] < redundancy)
				redundancyInfo[scp[i][j]]++;
		}
	}
	while (true)
	{
		vector<int> RCL = generateRCL2(polesCoverageValue, solution, alpha);
		if (RCL.size() == 0)
			break;
		int cand = RCL[rand() % RCL.size()];
		solution[cand] = 1;
		
		int sizeToReduce = 0;
		for (int i = 0; i < scp[cand].size(); i++)
		{
			redundancyInfo[scp[cand][i]]--;
			if (redundancyInfo[scp[cand][i]] == 0)
				sizeToReduce++;
		}
		tam -= sizeToReduce;

		//		vector<vector<int>> scpCopy = scp;
		int fSize = scp[cand].size();
		for (int i = 0; i < scp[cand].size(); i++)
		{
			//vector<int> aaa = scp[cand];
			int sSize = invertedSCP[scp[cand][i]].size();
			for (int j = 0; j < sSize; j++)
			{
				//vector<int> bbb = invertedSCP[scp[cand][i]];
				int tSize = scp[invertedSCP[scp[cand][i]][j]].size();
				if (invertedSCP[scp[cand][i]][j] != cand)
				{
					for (int k = 0; k < tSize; k++)
					{
						//vector<int> ccc = scp[invertedSCP[scp[cand][i]][j]];
						if (scp[invertedSCP[scp[cand][i]][j]][k] == scp[cand][i])
						{
							if (redundancyInfo[scp[invertedSCP[scp[cand][i]][j]][k]] == 0)
							{
								polesCoverageValue[invertedSCP[scp[cand][i]][j]]--;
								//scp[invertedSCP[scp[cand][i]][j]].erase(scp[invertedSCP[scp[cand][i]][j]].begin() + k);
								break;
							}
						}
					}
				}
			}
		}
		//scp[cand].clear();


		//		removeCovered(scp, cMatrix, cand, &tam);
		//		updateMatrix(scpCopy, cMatrix, cand);
	}

	return solution;
}
void evaluateSolution(vector<vector<int> > &scp, int * solution, int* cSatisfied, int* numColumns)
{
	int num = 0;
	int numC = 0;
	vector<int> mCovered;
	for (int i = 0; i < scp.size(); i++)
	{
		if (solution[i] == 1)
		{
			mCovered.insert(mCovered.end(), scp[i].begin(), scp[i].end());
			numC++;
		}
	}
	sort(mCovered.begin(), mCovered.end());
	mCovered.erase(unique(mCovered.begin(), mCovered.end()), mCovered.end());

	*numColumns = numC;
	*cSatisfied = mCovered.size();
}
void BestFlip(vector<vector<int> > &scp, int* solut,  int* csat, int* ncol)
{
	int bestCSatisfied = -1, bestNumColumns = -1;
	//evaluateSolution(scp, solut, &bestCSatisfied, &bestNumColumns);
	int toFlip = -1;
	for (int i = 0; i < scp.size(); i++)
	{
		solut[i] = !solut[i];
		int cSatisfied = -1, numColumns = -1;
		evaluateSolution(scp, solut, &cSatisfied, &numColumns);
		if (bestCSatisfied == -1)
		{
			bestCSatisfied = cSatisfied;
			bestNumColumns = numColumns;
			toFlip = i;
		}
		else
		{
			if (cSatisfied > bestCSatisfied)
				toFlip = i;
			else
				if (cSatisfied == bestCSatisfied)
					if (numColumns < bestNumColumns)
						toFlip = i;
		}		
		solut[i] = !solut[i];
	}
	solut[toFlip] = !solut[toFlip];
	*csat = bestCSatisfied;
	*ncol = bestNumColumns;

}
void RandomFlip(int * solution, int size)
{
	int pos = rand() % size;
	solution[pos] = !solution[pos];
}

void RolimLocalSearch(vector<vector<int> > &scp, int * solution)
{
	int succeeded = 1;
	while (succeeded)
	{
		int count=0;
		for (int x = 0; x < scp.size(); x++)
		{
			if (solution[x] == 1)
				count++;
		}
		cout << count;
		succeeded = 0;
		for (int i = 0; i < scp.size(); i++)
		{
			vector<int> aux;
			if (solution[i] == 0)
			{
				aux = scp[i];
				sort(aux.begin(), aux.end());
				vector<int> removable;
				for (int j = 0; j < scp.size(); j++)
				{
					if (j != i && solution[j] == 1)
					{
						vector<int> toCheck = scp[j];
						sort(toCheck.begin(), toCheck.end());
						if (includes(aux.begin(), aux.end(), toCheck.begin(), toCheck.end()))
							removable.push_back(j);
					}

				}
				if (removable.size() >= 2)
				{
					solution[i] = 1;
					for (int z = 0; z < removable.size(); z++)
						solution[removable[z]] = 0;
					succeeded = 1;
					break;
				}
			}			
		}
	}
}
void RolimEGuerraLocalSearch(vector<vector<int> > &scp, vector<vector<int> > &invertedScp,  vector<int> solution)
{
	int succeeded = 1;

	while (succeeded)
	{
		int count = 0;
		for (int x = 0; x < scp.size(); x++)
		{
			if (solution[x] == 1)
				count++;
		}
		cout << count;
		succeeded = 0;
		for (int i = 0; i < scp.size(); i++)
		{
			vector<int> aux;
			if (solution[i] == 0)
			{
				aux = scp[i];
				sort(aux.begin(), aux.end());
				vector<int> removable;
				vector<int> polesToCheck;
				for (int z = 0; z < aux.size(); z++)
				{
					polesToCheck.insert(polesToCheck.end(), invertedScp[aux[z]].begin(), invertedScp[aux[z]].end());
				}
				sort(polesToCheck.begin(), polesToCheck.end());
				polesToCheck.erase(unique(polesToCheck.begin(), polesToCheck.end()), polesToCheck.end());

				for (int j = 0; j < polesToCheck.size(); j++)
				{
					if (polesToCheck[j] != i && solution[polesToCheck[j]] == 1)
					{
						vector<int> toCheck = scp[polesToCheck[j]];
						
						sort(toCheck.begin(), toCheck.end());

						if (includes(aux.begin(), aux.end(), toCheck.begin(), toCheck.end()))
							removable.push_back(polesToCheck[j]);
					}

				}
				if (removable.size() >= 2)
				{			
					solution[i] = 1;
					for (int z = 0; z < removable.size(); z++)
						solution[removable[z]] = 0;
					succeeded = 1;
					break;
				}
			}
		}
	}
}
int intersectionSize(vector<int> &v1, vector<int> &v2)
{

	vector<int> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3.size();
}
vector<int> intersection(vector<int> &v1, vector<int> &v2)
{

	vector<int> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}
vector<int> difference(vector<int> &v1, vector<int> &v2)
{

	vector<int> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}
bool canBeRemoved(int checkedPole, vector<int> polesToCheck, int* covInfo, vector<vector<int> > &scp, int redundancy)
{
	bool ret = true;
	vector<int> checked;
	for (int i = 0; i < polesToCheck.size(); i++)
	{
		for (int j = 0; j < scp[polesToCheck[i]].size(); j++)
		{
			covInfo[scp[polesToCheck[i]][j]]--;
			if (find(scp[checkedPole].begin(), scp[checkedPole].end(), scp[polesToCheck[i]][j]) != scp[checkedPole].end())
			{
				if (covInfo[scp[polesToCheck[i]][j]]+1 < redundancy)
					ret = false;
			}
			else
			{
				if (covInfo[scp[polesToCheck[i]][j]] < redundancy)
					ret = false;
			}

		}
		checked.push_back(i);
		if (ret == false)
			break;		
	}
	for (int i = 0; i < checked.size(); i++)
	{
		for (int j = 0; j < scp[checked[i]].size(); j++)
		{
			covInfo[scp[checked[i]][j]]++;
		}
	}
	return ret;
}
vector<int> postOptReduction(int checkedPole, vector<int> polesToCheck, int* covInfo, vector<vector<int> > &scp, int redundancy)
{
	vector<vector<int> > toBeChecked;
	toBeChecked.push_back(polesToCheck);
	while (toBeChecked.size() > 0)
	{
		vector<vector<int> > nextToBeChecked;
		for (int i = 0; i < toBeChecked.size(); i++)
		{
			if (canBeRemoved(checkedPole,toBeChecked[i], covInfo, scp, redundancy))
				return toBeChecked[i];
			else
			{
				for (int j = 0; j < toBeChecked[i].size(); j++)
				{
					if (toBeChecked[i].size() > 1)
					{
						vector<int> toAdd = toBeChecked[i];
						toAdd.erase(toAdd.begin() + j);
						nextToBeChecked.push_back(toAdd);
					}
				}
			}
		}
		sort(nextToBeChecked.begin(), nextToBeChecked.end());
		nextToBeChecked.erase(unique(nextToBeChecked.begin(), nextToBeChecked.end()), nextToBeChecked.end());
		toBeChecked = nextToBeChecked;
	}
	return vector<int>();

}
void RolimEGuerraLocalSearchWithRedundancy(vector<vector<int> > &scp, vector<vector<int> > &invertedScp, vector<int> &solution, int redundancy)
{
	int succeeded = 1;
	//int* covInfo = new int[invertedScp.size()];
	vector<int> covInfo;
	for (int i = 0; i < invertedScp.size(); i++){ covInfo.push_back(0); }
	for (int i = 0; i < scp.size(); i++)
	{
		if (solution[i])
		{
			for (int j = 0; j < scp[i].size(); j++)
			{
				covInfo[scp[i][j]]++;
			}
		}
	}

	while (succeeded)
	{
		int count = 0;
		for (int x = 0; x < scp.size(); x++)
		{
			if (solution[x] == 1)
				count++;
		}
		cout << count;
		succeeded = 0;
		for (int i = 0; i < scp.size(); i++)
		{

			if (solution[i] == 0)
			{
				vector<int> aux = scp[i];
				sort(aux.begin(), aux.end());
				vector<int> removable;
				vector<int> polesToCheck;
				for (int z = 0; z < aux.size(); z++)
				{
					for (int a = 0; a < invertedScp[aux[z]].size(); a++)
					{
						if (solution[invertedScp[aux[z]][a]])
							polesToCheck.push_back(invertedScp[aux[z]][a]);
					}
					//polesToCheck.insert(polesToCheck.end(), invertedScp[aux[z]].begin(), invertedScp[aux[z]].end());
				}
				sort(polesToCheck.begin(), polesToCheck.end());
				polesToCheck.erase(unique(polesToCheck.begin(), polesToCheck.end()), polesToCheck.end());
				int posFound = find(polesToCheck.begin(), polesToCheck.end(), i) - polesToCheck.begin();
				if (posFound != polesToCheck.size())
					polesToCheck.erase(polesToCheck.begin() + posFound);
				if (polesToCheck.size() < 2)
					continue;
				//vector<int> checkedCoverage = scp[i];

				if (redundancy > 1)
				{
					for (int j = 0; j < polesToCheck.size() - 1; j++)//ordenada básica de acordo com o tamanho de medidores em interseção com o poste analisado.
					{
						int aux = intersectionSize(scp[polesToCheck[j]], scp[i]);
						int pos = -1;
						int posSize = -1;
						for (int k = j + 1; k < polesToCheck.size(); k++)
						{
							int aux2 = intersectionSize(scp[polesToCheck[k]], scp[i]);
							if (posSize == -1 || aux2 < posSize)
							{
								pos = k;
								posSize = aux2;
							}
						}
						if (posSize < aux)
						{
							int aux3 = polesToCheck[j];
							polesToCheck[j] = polesToCheck[pos];
							polesToCheck[pos] = aux3;
						}
					}//ORDENEI DE MENOR PRA MAIOR A QNT DE MEDIDORES EM INTERSEÇÃO COM O POSTE QUE TO ANALISANDO
				}
				vector<int> toRemove;
				for (int j = 0; j < polesToCheck.size(); j++)
				{
					vector<int> setDifference = difference(scp[polesToCheck[j]], scp[i]);
					vector<int> setIntersection = intersection(scp[polesToCheck[j]], scp[i]);
					bool canRemove = true;
					for (int k = 0; k < setDifference.size(); k++)
					{
						if (covInfo[setDifference[k]] <= redundancy)
						{
							canRemove = false;
							break;
						}
					}
					if (canRemove)
					{
						for (int k = 0; k < setIntersection.size(); k++)
						{
							if (covInfo[setIntersection[k]] < redundancy)
								canRemove = false;
						}
					}
					if (canRemove)
					{

						toRemove.push_back(polesToCheck[j]);
						for (int a = 0; a < scp[polesToCheck[j]].size(); a++)
						{
							covInfo[scp[polesToCheck[j]][a]]--;
						}
					}
				}
				

				if (toRemove.size() >= 2)
				{
					solution[i] = 1;
					for (int z = 0; z < toRemove.size(); z++)
					{
						solution[toRemove[z]] = 0;
						//for (int a = 0; a < scp[toRemove[z]].size(); a++)
						//{
						//	covInfo[scp[toRemove[z]][a]]--;
						//}
					}
					for (int a = 0; a < scp[i].size(); a++)
					{
						covInfo[scp[i][a]]++;
					}
					succeeded = 1;
					break;
				
				}
				else
				{
					for (int z = 0; z < toRemove.size(); z++)
					{
						for (int a = 0; a < scp[toRemove[z]].size(); a++)
						{
							covInfo[scp[toRemove[z]][a]]++;
						}
					}
				}
			}
		}
	}
}
//void RolimEGuerraLocalSearchWithRedundancy2(vector<vector<int> > &scp, vector<vector<int> > &invertedScp, int * solution, int redundancy)
//{
//	int succeeded = 1;
//	int* covInfo = new int[invertedScp.size()];
//	for (int i = 0; i < invertedScp.size(); i++){ covInfo[i] = 0; }
//	for (int i = 0; i < scp.size(); i++)
//	{
//		if (solution[i])
//		{
//			for (int j = 0; j < scp[i].size(); j++)
//			{
//				covInfo[scp[i][j]]++;
//			}
//		}
//	}
//
//	while (succeeded)
//	{
//		int count = 0;
//		for (int x = 0; x < scp.size(); x++)
//		{
//			if (solution[x] == 1)
//				count++;
//		}
//		cout << count;
//		succeeded = 0;
//		for (int i = 0; i < scp.size(); i++)
//		{
//			
//			if (solution[i] == 0)
//			{
//				vector<int> aux = scp[i];
//				sort(aux.begin(), aux.end());
//				//vector<int> removable;
//				vector<int> polesToCheck;
//				for (int z = 0; z < aux.size(); z++)
//				{
//					for (int a = 0; a < invertedScp[aux[z]].size(); a++)
//					{
//						if (solution[invertedScp[aux[z]][a]])
//							polesToCheck.push_back(invertedScp[aux[z]][a]);
//					}
//				}
//				sort(polesToCheck.begin(), polesToCheck.end());
//				polesToCheck.erase(unique(polesToCheck.begin(), polesToCheck.end()), polesToCheck.end());
//				int posFound = find(polesToCheck.begin(), polesToCheck.end(), i) - polesToCheck.begin();
//				if (posFound != polesToCheck.size())
//					polesToCheck.erase(polesToCheck.begin() + posFound);
//
//				vector<int> toRemove = postOptReduction(i, polesToCheck, covInfo, scp, redundancy);
//
//				if (toRemove.size() >= 2)
//				{
//					solution[i] = 1;
//					for (int z = 0; z < toRemove.size(); z++)
//					{
//						solution[toRemove[z]] = 0;
//						for (int a = 0; a < scp[toRemove[z]].size(); a++)
//						{
//							covInfo[scp[toRemove[z]][a]]--;
//						}
//					}
//					for (int a = 0; a < scp[i].size(); a++)
//					{
//						covInfo[scp[i][a]]++;
//					}
//					succeeded = 1;
//					break;
//				}
//			}
//		}
//	}
//}
//void WalkSat(vector<vector<int> > &scp, int * solution)
//{
//	//	int* newSolution;
//	//vector<vector<int> > cMatrix = coverageMatrix(scp, size);
//	int bestClausesSatisfied;
//	int bestNumberOfColumns;
//	evaluateSolution(scp, solution, &bestClausesSatisfied, &bestNumberOfColumns);
//	int* solCopy = new int[scp.size()];
//	for (int i = 0; i < scp.size(); i++)
//		solCopy[i] = solution[i];
//	for (int i = 0; i < 50 ; i++)
//	{
//
//		float r = rand() % 100 + 1;
//		r = r / 100;
//		if (r < p)
//		{
//			int cSatisfied;
//			int nColumns;
//			BestFlip(scp, solution, &cSatisfied, &nColumns);
//			if (cSatisfied > bestClausesSatisfied)
//			{
//				bestClausesSatisfied = cSatisfied;
//				bestNumberOfColumns = nColumns;
//				for (int i = 0; i < scp.size(); i++)
//					solCopy[i] = solution[i];
//			}
//			else if (cSatisfied == bestClausesSatisfied)
//			{
//				if (nColumns < bestNumberOfColumns)
//				for (int i = 0; i < scp.size(); i++)
//					solCopy[i] = solution[i];
//			}
//		}
//		else
//		{
//			RandomFlip(solution, scp.size());
//			int cSatisfied;
//			int nColumns;
//			evaluateSolution(scp, solution, &cSatisfied, &nColumns);
//			if (cSatisfied > bestClausesSatisfied)
//			{
//				bestClausesSatisfied = cSatisfied;
//				bestNumberOfColumns = nColumns;
//				for (int i = 0; i < scp.size(); i++)
//					solCopy[i] = solution[i];
//			}
//			else if (cSatisfied == bestClausesSatisfied)
//			{
//				if (nColumns < bestNumberOfColumns)
//				for (int i = 0; i < scp.size(); i++)
//					solCopy[i] = solution[i];
//			}
//
//
//		}
//	}
//	for (int i = 0; i < scp.size(); i++)
//		solution[i] = solCopy[i];
//	free(solCopy);
//
//	//	return solution;
//}
TestResult* AutoPlanning::executeGraspAutoPlanTestMode(int iterations, double alpha, int redundancy, int usePostOptimization)
{
	TestResult* result;
	//cout << "Iniciando planejamento\n";
	result = graspAutoPlanningTestMode(iterations, alpha, redundancy, usePostOptimization);
	result->gridSize = gridLimiter;
	result->numMeters = meters.size();
	result->numPoles = poles.size();
	vector<int> xgp = result->chosenPoles;
	vector<Position*> daps;
	vector<Position*> metersCopy;

	for (int i = 0; i < meters.size(); i++)
	{
		Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
		metersCopy.push_back(copy);
	}
	for (int i = 0; i < xgp.size(); i++)
	{
		Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
		daps.push_back(dapToInsert);
	}
	////Calcula métricas sem pos-opt
	MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
	MetricResult* metricResult = mc->executeMetricCalculationTest();
	result->metersPerHop = metricResult->meterPerHop;
	result->qualityPerHop = metricResult->linkQualityPerHop;
	result->redundancy = metricResult->minMedMaxRedundancyPerMeter;
	result->metersPerDap = metricResult->minMedMaxMetersPerDap;
	result->uncoveredMeters = metricResult->uncoveredMeters;
	delete metricResult;
	delete mc;
	//Calcula métricas com pos-opt
	metersCopy.clear();
	daps.clear();
	if (usePostOptimization)
	{
		xgp = result->poChosenPoles;
		for (int i = 0; i < meters.size(); i++)
		{
			Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
			metersCopy.push_back(copy);
		}
		for (int i = 0; i < xgp.size(); i++)
		{
			Position* dapToInsert = new Position(poles[xgp[i]]->latitude, poles[xgp[i]]->longitude, i);
			daps.push_back(dapToInsert);
		}
		mc = new MetricCalculation(metersCopy, daps, scenario, technology, bit_rate, t_power, h_tx, h_rx, srd, mesh, rubyPath);
		metricResult = mc->executeMetricCalculationTest();
		result->poMetersPerHop = metricResult->meterPerHop;
		result->poQualityPerHop = metricResult->linkQualityPerHop;
		result->poRedundancy = metricResult->minMedMaxRedundancyPerMeter;
		result->poMetersPerDap = metricResult->minMedMaxMetersPerDap;
		delete metricResult;
		delete mc;
	}
	//cout << result->toString();
	if (verbose) cout << "\n Planejamento por clusters finalizado";
	return result;
}
string AutoPlanning::graspAutoPlanning(int iterations, double alpha)
{
	string ret = "";
	int retCount = -1;
	vector<vector<int> > SCP = createScp();
	vector<vector<int> > invertedSCP;
	invertedSCP.resize(meters.size());
	for (int i = 0; i < SCP.size(); i++)
	{
		for (int j = 0; j < SCP[i].size(); j++)
		{
			invertedSCP[SCP[i][j]].push_back(i);
		}
	}
	int* solution = new int[poles.size()];
	int* newSolution;
	//double bestSolutionTime;
	//double totalTime;
	int winner = -1;
	for (int i = 0; i < iterations; i++)
	{
		if (verbose) cout << "\n Iteracao " + to_string(i);
		newSolution = new int[poles.size()];
		for (int z = 0; z < poles.size(); z++)
			newSolution[z] = 0;
		newSolution = constructPhase(SCP, invertedSCP, newSolution, alpha);
		int count = 0;
		string currentSol = "";
		for (int z = 0; z < poles.size(); z++)
		{
			if (newSolution[z] == 1)
			{
				currentSol += to_string(z) + " ";
				count++;
			}
		}
		if (retCount == -1 || count < retCount)
		{
			ret = currentSol;
			retCount = count;
		}
		//cout << count << "\n";
		//if (count < winner || winner == -1)
		//	winner = count;
	
		//RolimLocalSearch(SCP, newSolution);
		////WalkSat(SCP, newSolution);

		//count = 0;
		//for (int z = 0; z < poles.size(); z++)
		//{
		//	if (newSolution[z] == 1) count++;
		//}
		//cout << "Melhorado:" << count << "\n";


//
//		if (isBetterSolution(scp, newSolution, solution, size) == 1) //MUDAAAAAAAR
//		{
//
//			delete[] solution;
//			solution = newSolution;
//			int cS = 0, nC = 0;
//			evaluateSolution(scp, solution, size, &cS, &nC);
//			//printf("%f \n", bestSolutionTime);
//			//printf("%d \n", cS);
//			//printf("%d \n", nC);
//		}
//		else
//		{
//			delete[] newSolution;
//		}


	}
	//cout << "VENCEDOR: " << winner;
	//	evaluateSolution(scp, solution, size, cSatisfied, nColumns);
	//return solution;
	return ret;
}
TestResult* AutoPlanning::graspAutoPlanningTestMode(int iterations, double alpha, int redundancy, bool usePostOptimization)
{
	TestResult* tresult = new TestResult();
	const clock_t begin_time = clock();
	double secondsgp = 0;
	vector<int> ret;
	int retCount = -1;
	vector<vector<int> > SCP = createScp();
	vector<vector<int> > invertedSCP;
	invertedSCP.resize(meters.size());
	for (int i = 0; i < SCP.size(); i++)
	{
		for (int j = 0; j < SCP[i].size(); j++)
		{
			invertedSCP[SCP[i][j]].push_back(i);
		}
	}
	int* solution = new int[poles.size()];
	int* newSolution;
	//double bestSolutionTime;
	//double totalTime;
	int winner = -1;
	for (int i = 0; i < iterations; i++)
	{
		cout << "\n Iteracao " + to_string(i);
		newSolution = new int[poles.size()];
		for (int z = 0; z < poles.size(); z++)
			newSolution[z] = 0;
		newSolution = constructPhase2(SCP, invertedSCP, newSolution, alpha,redundancy);
		int count = 0;
		vector<int> currentSol;
		for (int z = 0; z < poles.size(); z++)
		{
			if (newSolution[z] == 1)
			{
				currentSol.push_back(z);
			}
		}
		if (retCount == -1 || count < retCount)
		{
			ret = currentSol;
			retCount = count;
		}
	}
	sort(ret.begin(), ret.end());
	ret.erase(unique(ret.begin(), ret.end()), ret.end());
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	tresult->solutionQuality = ret.size();
	tresult->chosenPoles = ret;
	tresult->time = secondsgp;
	if (usePostOptimization)//PÓS OTIMIZAÇÃO
	{
		cout << "\n Iniciando pós otimização\n";
		vector<int> chosen;
		for (int i = 0; i < poles.size(); i++)
			chosen.push_back(0);
		for (int i = 0; i < ret.size(); i++)
		{
			chosen[ret[i]] = 1;
		}

		RolimEGuerraLocalSearchWithRedundancy(SCP, invertedSCP, chosen, redundancy);

		vector<int> resultPosOpt;
		for (int i = 0; i < poles.size(); i++)
		{
			if (chosen[i] == 1)
				resultPosOpt.push_back(i);
		}
		ret = resultPosOpt;
		secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
		tresult->poSolutionQuality = resultPosOpt.size();
		tresult->poChosenPoles = resultPosOpt;
		tresult->poTime = secondsgp;
	}
	return tresult;
}
void AutoPlanning::setGridSize(double gridSize)
{
	gridLimiter = gridSize;
}
void AutoPlanning::setRegionLimiter(double rl)
{
	regionLimiter = rl;
}