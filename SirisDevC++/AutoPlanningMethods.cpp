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
vector<int> AutoPlanning::uncoverableMeters(vector<vector<int> > &SCP,int redundancy)
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
				coverable++;
			if (coverable >= redundancy)
				break;
			
		}
		if (coverable < redundancy)
			uncoverableMeters.push_back(meters[i]->index);
	}
	return uncoverableMeters;
}
vector<int> AutoPlanning::coverableMeters(vector<vector<int> > &SCP, int redundancy)
{

	vector<int> coverableMeters;
	for (int i = 0; i < meters.size(); i++)
	{
		int coverable = 0;
		for (int j = 0; j < poles.size(); j++)
		{
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
			if (cov)
				coverable++;
			if (coverable >= redundancy)
			{
				coverableMeters.push_back(meters[i]->index);
				break;
			}
		}		
	}
	return coverableMeters;
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
	Grid* g = new Grid(regionLimiter);
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
vector<vector<int> > AutoPlanning::createInvertedScp()
{
	//Grid* g = new Grid(meters,poles, regionLimiter); //Primeiro cria-se um grid.
	Grid* gMeters = new Grid(regionLimiter);
	gMeters->putPositions(meters);//Adiciona-se ao grid os medidores.
	Grid* g = new Grid(regionLimiter);
	g->putPositions(poles);//Adiciona-se ao grid os medidores.
	vector<int> aux;
	vector<vector<int> > sM;
	for (int i = 0; i < meters.size(); i++)
	{
		vector<int> polesCovered;
		vector<Position*> polesReduced = g->getCell(meters[i]);//Pega os medidores das células vizinhas e da sua
		// própria célula para fazer a análise.
		for (int j = 0; j < polesReduced.size(); j++)
		{
			double dist = getDistance(meters[i], polesReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (eff >= MARGIN_VALUE)//Se a eficiencia for superior a 90%(MARGIN_VALUE) então o poste cobre esse medidor.
				polesCovered.push_back(polesReduced[j]->index);
		}
		sM.push_back(polesCovered);
	}
	if (meshEnabled)
	{
		vector<vector<int> > nM = createMeterNeighbourhood(gMeters); //Cria a matriz de vizinhança para se analisar o mesh
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> alreadyChecked;
			vector<int> neighbours = nM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{
					newNeighbours = concatVectors(newNeighbours, nM[neighbours[k]]);
					sM[i] = concatVectors(sM[i], sM[neighbours[k]]);
				}
				sort(newNeighbours.begin(), newNeighbours.end());
				newNeighbours.erase(unique(newNeighbours.begin(), newNeighbours.end()), newNeighbours.end());
				concatVectors(alreadyChecked, neighbours);
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
//Esse método monta o arquivo de entrada pro GLPK.
void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP, vector<Position*> metersToConsider, vector<Position*> polesToDisconsider, int redundancy)
{
		//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
		//Pois se não, o método retornaria que a solução é impossível!
		//A formatação você ve no arquivo GlpkFile.txt
		string resp;
		resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>="+to_string(redundancy)+";\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath +"/Results.txt\";\n data;\n";
		resp += "set Z:= ";
		for (int i = 0; i < metersToConsider.size(); i++)
		{
				resp += "Z" + to_string(metersToConsider[i]->index + 1) + " ";
		}
		
		resp += ";\n";
		resp += "set Y:= ";

		for (int i = 0; i < poles.size(); i++)
			resp += "Y" + to_string(poles[i]->index + 1) + " ";
		resp += ";\n";
		resp += "param A := ";
		resp += "\n";
		for (int i = 0; i < metersToConsider.size(); i++)
		{
				for (int j = 0; j < poles.size(); j++)
				{
					//bool um = false;
					//for (int k = 0; k < SCP.size(); k++)
					int cov = -1;
					cov = (find(SCP[j].begin(), SCP[j].end(), metersToConsider[i]->index) != SCP[j].end());
					int covP = 1;
					for (int k = 0; k < polesToDisconsider.size(); k++){ if (polesToDisconsider[k]->index == poles[j]->index){ covP = -1; break; } }
					if (cov && covP)
						resp += "[Z" + to_string(metersToConsider[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1)+"] 1";

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
void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP, int redundancy)
{
	vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
	//Pois se não, o método retornaria que a solução é impossível!
	//A formatação você ve no arquivo GlpkFile.txt
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=" + to_string(redundancy) + ";\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "/Results.txt\";\n data;\n";
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
					resp += "[Z" + to_string(meters[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1) + "] 1";

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
string AutoPlanning::planWithRedundancy(vector<vector<int> > &scp, int redundancy)
{
	vector<int> chosenPoles;
	vector<int> aux = coverableMeters(scp,redundancy);
	//vector<Position*> polesToDisconsider;
	vector<Position*> selectedPoles;
	vector<Position*> metersToConsider;
	for (int i = 0; i < aux.size(); i++)
	{
		metersToConsider.push_back(meters[aux[i]]);
	}
	saveGLPKFileReduced(scp, metersToConsider,selectedPoles, redundancy);
	executeGlpk(rubyPath + "/GlpkFile.txt");
	ifstream f((rubyPath + "/Results.txt").c_str());
	string gridAnswer;
	getline(f, gridAnswer);
	vector<string> xgp = split(gridAnswer, ' ');

	vector<Position*> metersCopy;
	for (int i = 0; i < xgp.size(); i++)
	{
		string snum = xgp[i].substr(1);
		Position* selected = new Position(poles[atoi(snum.c_str()) - 1]->latitude, poles[atoi(snum.c_str()) - 1]->longitude, poles[atoi(snum.c_str()) - 1]->index);;
		selectedPoles.push_back(selected);
	}


	//lembrar dos FREES


	return "wow";
}
//Essa aqui é minha heurística que faz o método exato pra cada célula.
string AutoPlanning::gridAutoPlanning(int redundancy)
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
		saveGLPKFileReduced(cellSCP,redundancy);
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
string AutoPlanning::gridAutoPlanningTestMode(float* mtu, float* mmu, bool usePostOptimization, int redundancy)
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
		saveGLPKFileReduced(cellSCP,redundancy);
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
	meters = metersAux;
	poles = polesAux;
	string str = "";
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		str += chosenDaps[i] + " ";
	}
	if (usePostOptimization)//PÓS OTIMIZAÇÃO
	{	

		int* chosen = new int[poles.size()];
		for (int i = 0; i < poles.size(); i++)
			chosen[i] = 0;
		for (int i = 0; i < chosenDaps.size(); i++)
		{
			string snum = chosenDaps[i].substr(1);
			const char * c = snum.c_str();
			int val = atoi(c) - 1;
			chosen[val] = 1;
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
		RolimEGuerraLocalSearch(scp, invertedSCP, chosen);
		string resultPosOpt = "";
		for (int i = 0; i < poles.size(); i++)
		{
			if (chosen[i] == 1)
				resultPosOpt += "Y"+to_string(i + 1) + " ";
		}
		str = resultPosOpt;
	}


	delete metergrid;
	delete polegrid;

	*mtu = maximumtimeusage;
	*mmu = maximummemusage;
	return str;
}
//Executa o planejamento automático
string AutoPlanning::executeAutoPlan()
{
	//vector<vector<int> > SCP = createScp();
	//saveGLPKFileReduced(SCP);
	return gridAutoPlanning(1);
	//executeGlpk("GlpkFile.txt");
	//ifstream f("Results.txt");
	//string str;
	//getline(f, str);
	//return str;
}
vector<Position*> AutoPlanning::getMetersThatSatisfyRedundancy(int redundancy, vector< vector< int > > invertedSCP)
{	
	vector<Position*> metersThatSatisfy;
	for (int i = 0; i < invertedSCP.size(); i++)
	{
		if (invertedSCP[i].size() >= redundancy)
			metersThatSatisfy.push_back(meters[i]);
	}
	return metersThatSatisfy;
}
string AutoPlanning::executeAutoPlan(int redundancy)
{

	return gridAutoPlanning(redundancy);

}
//Coisa de teste
string AutoPlanning::executeAutoPlanTestMode( bool usePostOptimization)
{
	//vector<vector<int> > SCP = createScp();
	//saveGLPKFileReduced(SCP);
	float mtu, mme;
	double secondsgp = -1;
	const clock_t begin_time = clock();
	string result = gridAutoPlanningTestMode(&mtu, &mme, usePostOptimization,1);
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;

	string ret =  "Grid size: " + to_string(gridLimiter) + "\n\nGrid planning solution time: " + to_string(secondsgp) + "\n";
	ret += "Maximum Memory Used: " + to_string(mme) +"\n\n";
	ret += "Maximum Time Used in a Cell: " + to_string(mtu) + "\n\n";

	vector<string> xgp = split(result, ' ');
	vector<Position*> daps;
	vector<Position*> metersCopy;
	for (int i = 0; i < xgp.size(); i++)
	{
		string snum = xgp[i].substr(1);
		Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude, poles[atoi(snum.c_str()) - 1]->longitude, poles[atoi(snum.c_str()) - 1]->index);;
		daps.push_back(dapToInsert);
	}
	for (int i = 0; i < meters.size(); i++)
	{
		Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
		metersCopy.push_back(copy);
	}
	MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
	string metricResult = mc->executeMetricCalculation();
	delete mc;

	ret += metricResult;
	return ret;
}
string AutoPlanning::executeAutoPlanTestMode(bool usePostOptimization, int redundancy)
{
	//vector<vector<int> > SCP = createScp();
	//saveGLPKFileReduced(SCP);
	float mtu, mme;
	double secondsgp = -1;
	string result;
	const clock_t begin_time = clock();
	if (redundancy > 1)
	{
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
		vector<Position*> metersThatSatisfy = getMetersThatSatisfyRedundancy(redundancy, invertedSCP);
		vector<Position*> metersThatSatisfyCopy;
		vector<Position*> polesCopy;
		for (int i = 0; i < metersThatSatisfy.size(); i++)
		{
			Position* copy = new Position(metersThatSatisfy[i]->latitude, metersThatSatisfy[i]->longitude, metersThatSatisfy[i]->index);
			metersThatSatisfyCopy.push_back(copy);
		}
		for (int i = 0; i < poles.size(); i++)
		{
			Position* copy = new Position(poles[i]->latitude, poles[i]->longitude, poles[i]->index);
			polesCopy.push_back(copy);
		}
		AutoPlanning* res = new AutoPlanning(metersThatSatisfyCopy, polesCopy, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
		string ret = res->executeAutoPlan(redundancy);
		delete res;

		vector<string> xgp = split(ret, ' ');
		vector<Position*> daps;
		vector<Position*> metersCopy;
		for (int i = 0; i < xgp.size(); i++)
		{
			string snum = xgp[i].substr(1);
			Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude, poles[atoi(snum.c_str()) - 1]->longitude, poles[atoi(snum.c_str()) - 1]->index);;
			daps.push_back(dapToInsert);
		}
		for (int i = 0; i < metersThatSatisfy.size(); i++)
		{
			Position* copy = new Position(metersThatSatisfy[i]->latitude, metersThatSatisfy[i]->longitude, i);
			metersCopy.push_back(copy);
		}
		MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
		string metricResult = mc->executeMetricCalculation();
		cout << metricResult;
		delete mc;

	}
	else
	{
		result = gridAutoPlanningTestMode(&mtu, &mme, usePostOptimization,1);
	}
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;

	string ret = "Grid size: " + to_string(gridLimiter) + "\n\nGrid planning solution time: " + to_string(secondsgp) + "\n";
	ret += "Maximum Memory Used: " + to_string(mme) + "\n\n";
	ret += "Maximum Time Used in a Cell: " + to_string(mtu) + "\n\n";

	vector<string> xgp = split(result, ' ');
	vector<Position*> daps;
	vector<Position*> metersCopy;
	for (int i = 0; i < xgp.size(); i++)
	{
		string snum = xgp[i].substr(1);
		Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude, poles[atoi(snum.c_str()) - 1]->longitude, poles[atoi(snum.c_str()) - 1]->index);;
		daps.push_back(dapToInsert);
	}
	for (int i = 0; i < meters.size(); i++)
	{
		Position* copy = new Position(meters[i]->latitude, meters[i]->longitude, meters[i]->index);
		metersCopy.push_back(copy);
	}
	MetricCalculation* mc = new MetricCalculation(metersCopy, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
	string metricResult = mc->executeMetricCalculation();
	delete mc;

	ret += metricResult;
	return ret;
}
//////////////////////////////////////////MÉTODOS PRO GRASP////////////////////////////////////////////////
int iterations = 500;
double p = 0.5;
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

	while (true)
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
		}
		scp[cand].clear();


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
void RolimEGuerraLocalSearch(vector<vector<int> > &scp, vector<vector<int> > &invertedScp,  int * solution)
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
void WalkSat(vector<vector<int> > &scp, int * solution)
{
	//	int* newSolution;
	//vector<vector<int> > cMatrix = coverageMatrix(scp, size);
	int bestClausesSatisfied;
	int bestNumberOfColumns;
	evaluateSolution(scp, solution, &bestClausesSatisfied, &bestNumberOfColumns);
	int* solCopy = new int[scp.size()];
	for (int i = 0; i < scp.size(); i++)
		solCopy[i] = solution[i];
	for (int i = 0; i < 50 ; i++)
	{

		float r = rand() % 100 + 1;
		r = r / 100;
		if (r < p)
		{
			int cSatisfied;
			int nColumns;
			BestFlip(scp, solution, &cSatisfied, &nColumns);
			if (cSatisfied > bestClausesSatisfied)
			{
				bestClausesSatisfied = cSatisfied;
				bestNumberOfColumns = nColumns;
				for (int i = 0; i < scp.size(); i++)
					solCopy[i] = solution[i];
			}
			else if (cSatisfied == bestClausesSatisfied)
			{
				if (nColumns < bestNumberOfColumns)
				for (int i = 0; i < scp.size(); i++)
					solCopy[i] = solution[i];
			}
		}
		else
		{
			RandomFlip(solution, scp.size());
			int cSatisfied;
			int nColumns;
			evaluateSolution(scp, solution, &cSatisfied, &nColumns);
			if (cSatisfied > bestClausesSatisfied)
			{
				bestClausesSatisfied = cSatisfied;
				bestNumberOfColumns = nColumns;
				for (int i = 0; i < scp.size(); i++)
					solCopy[i] = solution[i];
			}
			else if (cSatisfied == bestClausesSatisfied)
			{
				if (nColumns < bestNumberOfColumns)
				for (int i = 0; i < scp.size(); i++)
					solCopy[i] = solution[i];
			}


		}
	}
	for (int i = 0; i < scp.size(); i++)
		solution[i] = solCopy[i];
	free(solCopy);

	//	return solution;
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
	int winner = -1;
	for (int i = 0; i < iterations; i++)
	{
		newSolution = new int[poles.size()];
		for (int z = 0; z < poles.size(); z++)
			newSolution[z] = 0;
		newSolution = constructPhase(SCP,invertedSCP, newSolution);
		int count = 0;
		for (int z = 0; z < poles.size(); z++)
		{
			if (newSolution[z] == 1) count++;
		}
		cout << count << "\n";
		if (count < winner || winner == -1)
			winner = count;
	
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
	cout << "VENCEDOR: " << winner;
	//	evaluateSolution(scp, solution, size, cSatisfied, nColumns);
	//return solution;

	return "wow";


}
void AutoPlanning::setGridSize(double gridSize)
{
	gridLimiter = gridSize;
}
