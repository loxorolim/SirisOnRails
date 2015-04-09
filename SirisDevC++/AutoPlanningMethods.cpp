#include "AutoPlanningMethods.h"


//Une dois vetores de inteiros sem repetição, nada demais.
vector<int> AutoPlanning::concatVectors(vector<int> &v1, vector<int> &v2)
{
	vector<int> ret;
	for (int i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	for (int i = 0; i < v2.size(); i++)
	{
		bool add = true;
		for (int j = 0; j < v1.size(); j++)
		if (v1[j] == v2[i])
		{
			add = false;
			break;
		}
		if (add)
			ret.push_back(v2[i]);
	}
	return ret;
}
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
vector<int> AutoPlanning::uncoverableMeters(vector<vector<int> > &SCP)
{
	
	vector<int> uncoverableMeters;
	for (int i = 0; i < meters.size(); i++)
	{
		int coverable = 0;
		for (int j = 0; j < poles.size(); j++)
		{			
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
			if (cov)
			{
				coverable = 1;
				break;
			}
		}
		if (!coverable)
			uncoverableMeters.push_back(meters[i]->index);
	}
	return uncoverableMeters;
}

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
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_TX, SRD);
			if (i != j && eff >= MARGIN_VALUE) //Se a eficiencia for superior a 90%(MARGIN_VALUE) então é vizinho!
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
	//Grid* g = new Grid(meters,poles, regionLimiter); //Primeiro cria-se um grid.
	Grid* g = new Grid(1000);
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
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (eff >= MARGIN_VALUE)//Se a eficiencia for superior a 90%(MARGIN_VALUE) então o poste cobre esse medidor.
				metersCovered.push_back(metersReduced[j]->index);
		}
		sM.push_back(metersCovered);
	}
	if (meshEnabled)
	{
		vector<vector<int> > nM = createMeterNeighbourhood(g); //Cria a matriz de vizinhança para se analisar o mesh
		for (int i = 0; i < sM.size(); i++)//Essa etapa incrementa a matriz de cobertura para considerar o mesh.
		{
			vector<int> neighbours = sM[i];//Conjunto de medidores que o poste "i" cobre.
			for (int j = 0; j < meshEnabled; j++)//Para cada salto
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)//Os vizinhos dos medidores que o poste cobre também serão
				//considerados como cobertos! Isso se repete X vezes, onde X é o número de saltos mesh.
				{
					int toFind = neighbours[k];
					int pos = 0;
					for (int i = 0; i < meters.size(); i++){ if (meters[i]->index == toFind){ pos = i; break; }}
					//Esse for aí em cima serve para localizar a posição do medidor com o respectivo index, pois
					// a matriz de vizinhança dos medidores não é organizada por index, mas sim 0,1,2...n.
					sM[i] = concatVectors(sM[i], nM[pos]);//Os vizinhos agora fazem parte da cobertura do poste!
					newNeighbours = concatVectors(newNeighbours, nM[pos]);//Novos vizinhos a serem analisados no
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

//Calcula sem usar o Grid, é obsoleto.
vector<vector<int> > AutoPlanning::createScpSemGrid()
{
	Grid* g = new Grid(10000);
	g->putPositions(meters);
	vector<int> aux;
	vector<vector<int> > sM;
	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		vector<Position*> metersReduced = g->getCell(poles[i]);
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(poles[i], metersReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}
		sM.push_back(metersCovered);
	}
	if (meshEnabled)
	{
		vector<vector<int> > nM = createMeterNeighbourhood(g);
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> neighbours = sM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{
					sM[i] = concatVectors(sM[i], nM[neighbours[k]]);
					newNeighbours = concatVectors(newNeighbours, nM[neighbours[k]]);
				}
				neighbours = newNeighbours;
			}
		}
	}
	delete g;
	return sM;
}
//Ignora esse método, passa pro saveGLPKFileReduced, que usa o Grid.
void AutoPlanning::saveGLPKFile(vector<vector<int> > &SCP)
{
	FILE *file;
		{

			//TEM Q MUDAR ESSE NEGÓCIO AQUI!
		    vector<int> uncMeters = uncoverableMeters(SCP);
			string resp;
			resp += "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" +rubyPath+ "/Results.txt\";\n data;\n";
			//fprintf_s(file, "%s", "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
			//ret += "set Z:= ";
			//fprintf_s(file, "set Z:= ");
			resp += "set Z:= ";
			for (int i = 0; i < meters.size(); i++)
			{
				int uncov = -1;
				uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
				if (!uncov)
					resp += "Z" + to_string(i + 1) + " ";
			}

			resp += ";\n";
			resp += "set Y:= ";

			for (int i = 0; i < poles.size(); i++)
				resp += "Y" + to_string(i + 1) + " ";

			resp += ";\n";

			resp += "param A : ";
	
			for (int i = 0; i < poles.size(); i++)
				resp += "Y" + to_string(i + 1) + " ";

			resp += ":= \n";
			for (int i = 0; i < meters.size(); i++) 
			{
				int uncov = -1;
				uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
				if (!uncov)
				{
					resp += "Z" + to_string(i + 1) + " ";
					//fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
					for (int j = 0; j < poles.size(); j++)
					{
						//bool um = false;
						//for (int k = 0; k < SCP.size(); k++)
						int cov = -1;
						cov = (find(SCP[j].begin(), SCP[j].end(), i) != SCP[j].end());
						if (cov)
							resp += "1 ";
						else
							resp += "0 ";

					}
				}
	
			}
			resp += "\n";
			resp += ";";
			resp += "end;\n";
		
			string filename = rubyPath + "/GlpkFile.txt";
			ofstream f(filename.c_str());

			f << resp;
			f.close();

	
	
		}
	
}
//Esse método monta o arquivo de entrada pro GLPK.
void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP)
{
		
		vector<int> uncMeters = uncoverableMeters(SCP);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
		//Pois se não, o método retornaria que a solução é impossível!
		//A formatação você ve no arquivo GlpkFile.txt
		string resp;
		resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath +"/Results.txt\";\n data;\n";
		resp += "set Z:= ";
		for (int i = 0; i < meters.size(); i++)
		{
			int uncov = -1;
			uncov = (find(uncMeters.begin(), uncMeters.end(), meters[i]->index) != uncMeters.end());
			if (!uncov)
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
			int uncov = -1;
			uncov = (find(uncMeters.begin(), uncMeters.end(), meters[i]->index) != uncMeters.end());
			if (!uncov)
			{
				//fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
				for (int j = 0; j < poles.size(); j++)
				{
					//bool um = false;
					//for (int k = 0; k < SCP.size(); k++)
					int cov = -1;
					cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
					if (cov)
						resp += "[Z" + to_string(meters[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1)+"] 1";

				}
			}

		}
		resp += "\n";
		resp += ";";
		resp += "end;\n";
		
		string filename = rubyPath + "/GlpkFile.txt";
		ofstream f(filename.c_str());

		f << resp;
		f.close();
}
//Esse método faz um system call ao GLPK
void AutoPlanning::executeGlpk(string filename)
{
	string access = rubyPath + "/glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 6000  > " + rubyPath +"/wow.txt";
	//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
	system(access.c_str());
}
//Pode ignorar esse método
float getMemUsageFromGlpkFile(string fname)
{
	ifstream f(fname.c_str());
	string str;
	//int cont = 1;
	float mem = -1;
	while (getline(f, str))
	{
		int c = str.find("Memory used: ");
		if (c >= 0)
		{
			vector<string> s = split(str, ' ');
			int pos = -1;
			for (int i = 0; i < s.size(); i++)
			{
				int c2 = s[i].find("Mb");
				if (c2 >= 0)
				{
					pos = i;
					break;
				}
			}
			mem = atof(s[pos-1].c_str());
		}
	}
	return mem;

}
//Ignora também.
float getTimeUsageFromGlpkFile(string fname)
{
	ifstream f(fname.c_str());
	string str;
	//int cont = 1;
	float time = -1;
	while (getline(f, str))
	{
		//getline(f, str);
		int c = str.find("Time used: ");
		if (c >= 0)
		{
		//	cont = 0;

			vector<string> s = split(str, ' ');
			int pos = -1;
			for (int i = 0; i < s.size(); i++)
			{
				int c2 = s[i].find("secs");
				if (c2 >= 0)
				{
					pos = i;
					break;
				}
			}
			time = atof(s[pos-1].c_str());
		}
	}
	return time;

}
//Essa aqui é minha heurística que faz o método exato pra cada célula.
string AutoPlanning::gridAutoPlanning()
{
	//Grid* metergrid = new Grid(meters, poles, gridLimiter);//cria o grid dos medidores, bla bla bla.
	Grid* metergrid = new Grid(100000);
	metergrid->putPositions(meters);
	//Grid* polegrid = new Grid(poles, meters, gridLimiter);//cria o grid dos postes
	Grid* polegrid = new Grid(100000);
	polegrid->putPositions(poles);
	vector<Position*> metersAux = meters, polesAux = poles;
	map<pair<int, int>, vector<Position*> > meterCells = metergrid->getCells();
	vector<string> chosenDaps;
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
		saveGLPKFileReduced(cellSCP);
		executeGlpk(rubyPath + "/GlpkFile.txt");
		ifstream f((rubyPath + "/Results.txt").c_str());
		string gridAnswer;
		getline(f, gridAnswer);

		vector<string> x = split(gridAnswer, ' ');
		int wow = x.size();
		for (int i = 0; i < x.size(); i++)
		{
			//string snum = x[i].substr(1);
			chosenDaps.push_back(x[i]);
		}
	}
	string str = "";
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		str += chosenDaps[i] + " ";
	}

	delete metergrid;
	delete polegrid;
	meters = metersAux;
	poles = polesAux;
	return str;




}

//Pode ignorar, usei pra testes.
string AutoPlanning::gridAutoPlanningTestMode(float* mtu, float* mmu)
{
	Grid* metergrid = new Grid(gridLimiter);
	metergrid->putPositions(meters);
	Grid* polegrid = new Grid(gridLimiter);
	polegrid->putPositions(poles);
	vector<Position*> metersAux = meters, polesAux = poles;
	map<pair<int, int>, vector<Position*> > meterCells = metergrid->getCells();
	vector<string> chosenDaps;
	//string str;
	int i = 1;
	float maximummemusage = -1;
	float maximumtimeusage = -1;

	for (map<pair<int, int>, vector<Position*> >::iterator it = meterCells.begin(); it != meterCells.end(); ++it)
	{

		vector<Position*> cellsMeters;
		vector<Position*> cellsPoles;

		cellsMeters = it->second; //Recebe todas as posições dos medidores que estão na célula
		cellsPoles = polegrid->getCell(cellsMeters[0]);//Pega a posição de um desses medidores e usa como referência pra pegar os postes da mesma célula e das células vizinhas.
		meters = cellsMeters;
		poles = cellsPoles;
		vector<vector<int> > cellSCP = createScp();
		saveGLPKFile(cellSCP);
		executeGlpk(rubyPath+"/GlpkFile.txt");
		ifstream f((rubyPath + "/Results.txt").c_str());
		string gridAnswer;
		getline(f, gridAnswer);

		vector<string> x = split(gridAnswer, ' ');
			int wow = x.size();
		for (int i = 0; i < x.size(); i++)
		{
			//string snum = x[i].substr(1);
			chosenDaps.push_back(x[i]);
		}
		float memusage = getMemUsageFromGlpkFile(rubyPath+"/wow.txt");
		float timeusage = getTimeUsageFromGlpkFile(rubyPath+"/wow.txt");
		if (memusage > maximummemusage)
		{
			maximummemusage = memusage;
		}
		if (timeusage > maximumtimeusage)
		{
			maximumtimeusage = timeusage;
		}

	}
	string str = "";
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		str += chosenDaps[i] + " ";
	}
	delete metergrid;
	delete polegrid;
	meters = metersAux;
	poles = polesAux;
	*mtu = maximumtimeusage;
	*mmu = maximummemusage;
	return str;
}
//Executa o planejamento automático
string AutoPlanning::executeAutoPlan()
{
	//vector<vector<int> > SCP = createScp();
	//saveGLPKFileReduced(SCP);
	return gridAutoPlanning();
	//executeGlpk("GlpkFile.txt");
	//ifstream f("Results.txt");
	//string str;
	//getline(f, str);
	//return str;
}
//Coisa de teste
string AutoPlanning::executeAutoPlanTestMode( string * res, double gridsize)
{

	gridLimiter = gridsize;
	//vector<vector<int> > SCP = createScp();
	//saveGLPKFileReduced(SCP);
	float mtu, mme;
	double secondsgp = -1;
	const clock_t begin_time = clock();
	string result = gridAutoPlanningTestMode(&mtu,&mme);
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;


	Position* aux = new Position(0, 0);
	Position* aux2 = new Position(0 + gridsize, 0);
	Position* aux3 = new Position(0, 0 + gridsize);
	double gheight = getDistance(aux,aux2);
	double gwidth = getDistance(aux,aux3);

	string ret =  "Grid height: " + to_string(gheight) + "\n Grid width: " + to_string(gwidth) + "\n\nGrid planning solution time: " + to_string(secondsgp) + "\n";
	ret += "Maximum Memory Used: " + to_string(mme) +"\n\n";
	ret += "Maximum Time Used in a Cell: " + to_string(mtu) + "\n\n";

	*res = ret;
	return result;
}
//////////////////////////////////////////MÉTODOS PRO GRASP////////////////////////////////////////////////
int iterations = 500;
double alpha = 0.9;
vector<int> generateRCL(vector<vector<int> > &scp, int* solution)
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

int* constructPhase(vector<vector<int> > scp,vector<vector<int> >& invertedSCP, int* solution)
{
	//vector<vector<int>> scpCopy = copyScp(scp);
	//vector<vector<int>> cMatrix = coverageMatrix(scp, size);
	int tam = invertedSCP.size();

	while (tam > 0)
	{
		vector<int> RCL = generateRCL(scp, solution);
		if (RCL.size() == 0)
			break;
		int cand = RCL[rand() % RCL.size()];
		solution[cand] = 1;
		tam -= scp[cand].size();
//		vector<vector<int>> scpCopy = scp;
		int fSize = scp[cand].size();
		for(int i = 0; i < scp[cand].size();i++)
		{
			vector<int> aaa = scp[cand];
			int sSize = invertedSCP[scp[cand][i]].size();
			for(int j = 0; j < sSize; j++)
			{
				vector<int> bbb = invertedSCP[scp[cand][i]];
				int tSize = scp[invertedSCP[scp[cand][i]][j]].size();
				if (invertedSCP[scp[cand][i]][j] != cand)
				{
					for (int k = 0; k < tSize; k++)
					{
						vector<int> ccc = scp[invertedSCP[scp[cand][i]][j]];
						if (scp[invertedSCP[scp[cand][i]][j]][k] == scp[cand][i])
						{
							scp[invertedSCP[scp[cand][i]][j]].erase(scp[invertedSCP[scp[cand][i]][j]].begin() + k);
							break;
						}
					}
				}
			}
			scp[cand].clear();
		}


//		removeCovered(scp, cMatrix, cand, &tam);
//		updateMatrix(scpCopy, cMatrix, cand);
	}

	return solution;
}


string AutoPlanning::graspAutoPlanning()
{

	vector<vector<int> > SCP = createScp();
	vector<vector<int> > invertedSCP;
	invertedSCP.resize(meters.size());
	for(int i = 0; i < SCP.size(); i++)
	{
		for(int j = 0; j < SCP[i].size(); j++)
		{
			invertedSCP[SCP[i][j]].push_back(i);
		}
	}
	int* solution = new int[poles.size()];
	int* newSolution;
	//double bestSolutionTime;
	//double totalTime;
	for (int i = 0; i < iterations; i++)
	{
		newSolution =  new int[poles.size()];
		newSolution = constructPhase(SCP,invertedSCP, newSolution);
		int count = 0;
		for(int z = 0; z < poles.size(); z++)
		{
			if(newSolution[z] == 1) count++;
		}
		cout<<count;
		//WalkSat(scp, newSolution, size);


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
//	evaluateSolution(scp, solution, size, cSatisfied, nColumns);
	//return solution;

	return "wow";


}
