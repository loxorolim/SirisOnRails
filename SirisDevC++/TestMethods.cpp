#include "TestMethods.h"
#include "MainMethods.h"


#define distFromHouseToStreet 40
#define distToRandom 20

double generateRandom()
{
	return rand() % (2 * distToRandom / 2 + 1) - distToRandom / 2;
}
double memoryTest(int x, int y)
{
	
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\nparam B{r in Z} default " + to_string(1) + ", integer;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=B[r];\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "Results.txt\";\n data;\n";
	resp += "set Z:= ";
	for (int i = 0; i < x; i++)
	{
		resp += "Z" + to_string(i) + " ";
	}

	resp += ";\n";
	resp += "set Y:= ";

	for (int i = 0; i < y; i++)
		resp += "Y" + to_string(i) + " ";
	resp += ";\n";
	resp += "param A := ";
	resp += "\n";
	resp += ";";
	resp += "\n";
	resp += "param B := ";
	resp += "\n";
	resp += "\n;\nend;\n";

	string filename = rubyPath + "GlpkFile.txt";
	ofstream f(filename.c_str());
	f << resp;
	f.close();
	//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800  > " + rubyPath + "wow.txt";
	//system(access.c_str());

	glp_prob *lp;
	glp_tran *tran;
	int err;
	int ret, count, cpeak;
	size_t total, tpeak;
	double totalInMb, tpeakInMb;
	glp_term_out(GLP_ON);
	lp = glp_create_prob();
	tran = glp_mpl_alloc_wksp();
	ret = glp_mpl_read_model(tran, filename.c_str(), 0);
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

	glp_mem_usage(&count, &cpeak, &total, &tpeak);
	//printf("%d memory block(s) are still allocated\n", count);
	totalInMb = ((double)total / (1024 * 1024));
	tpeakInMb = ((double)tpeak / (1024 * 1024));

skip: glp_mpl_free_wksp(tran);
	glp_delete_prob(lp);

	//ifstream fi(rubyPath + "wow.txt");
	//string str;
	//float mem = -1;
	//while (getline(fi, str))
	//{
	//	int c = str.find("Memory used: ");
	//	if (c >= 0)
	//	{
	//		vector<string> s = split(str, ' ');
	//		int pos = -1;
	//		for (int i = 0; i < s.size(); i++)
	//		{
	//			int c2 = s[i].find("bytes");
	//			if (c2 >= 0)
	//			{
	//				pos = i;
	//				break;
	//			}
	//		}
	//		mem = atof((s[pos - 1].substr(1)).c_str());
	//	}
	//}
	return totalInMb;
	//return mem;

}
vector<vector<Position*> > generateBlock(double latStart, double lngStart, int scenario, int blockWidth, int streetWidth, int polesPerSide, int n, vector<bool> sidesToPutPoles )
{
	
	//consideramos blocos de 100m x 100m
	vector<Position*> ret;
	vector<Position*> polesRet;
	double dist = blockWidth/(n-1);
	double latStart2 = latStart + getLatOfDistance(blockWidth);
	double lngStart2 = lngStart + getLongOfDistance(latStart, blockWidth);
	double lngStart3 = lngStart + getLongOfDistance(latStart2, blockWidth);
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart + getLatOfDistance(generateRandom()), lngStart + getLongOfDistance(latStart, j*dist + generateRandom()), 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart + getLatOfDistance(j*dist + generateRandom()), lngStart2 + getLongOfDistance(latStart,generateRandom()), 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart2 + getLatOfDistance(generateRandom()), lngStart3 - getLongOfDistance(latStart2, j*dist + generateRandom()), 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart2 - getLatOfDistance(j*dist + generateRandom()), lngStart + getLongOfDistance(latStart2, generateRandom()), 0);
		ret.push_back(p);
	}
	//Depois de inserir os medidores, prepara pra inserir os postes!
	n = polesPerSide;
	latStart = latStart - getLatOfDistance(streetWidth/2);
	lngStart = lngStart - getLongOfDistance(latStart, streetWidth/2);
	blockWidth = blockWidth + streetWidth;
	dist = (double) blockWidth / (polesPerSide - 1);
	latStart2 = latStart + getLatOfDistance(blockWidth);
	lngStart2 = lngStart + getLongOfDistance(latStart,blockWidth);
	lngStart3 = lngStart + getLongOfDistance(latStart2, blockWidth);
	if (sidesToPutPoles[0])
		for (int j = 0; j < n - 1; j++)
		{
			Position* p = new Position(latStart , lngStart + getLongOfDistance(latStart, j*dist), 0);
			if (j != 0)
			{
				for (int k = 0; k < n - 1; k++)
				{
					Position* p2 = new Position(latStart + getLatOfDistance(k*dist), lngStart + getLongOfDistance(latStart, j*dist), 0);
					polesRet.push_back(p2);
				}
			}
			polesRet.push_back(p);
		}
	if (sidesToPutPoles[1])
		for (int j = 0; j < n - 1; j++)
		{
			Position* p = new Position(latStart + getLatOfDistance(j*dist), lngStart2, 0);
			polesRet.push_back(p);
		}
	if (sidesToPutPoles[2])
		for (int j = 0; j < n - 1; j++)
		{
			Position* p = new Position(latStart2, lngStart3 - getLongOfDistance(latStart2, j*dist), 0);
			polesRet.push_back(p);
		}
	if (sidesToPutPoles[3])
		for (int j = 0; j < n - 1; j++)
		{
			Position* p = new Position(latStart2 - getLatOfDistance(j*dist), lngStart, 0);
			polesRet.push_back(p);
		}

	vector<vector<Position*> > pairRet;
	pairRet.push_back(ret);
	pairRet.push_back(polesRet);
		
	return pairRet;

	
}
vector<vector<Position*> > gridInstanceGenerator(double latStart, double lngStart, double streetWidth, int scenario, int numOfRuns, int blockWidth,int polesPerSide, int metersPerSide)
{
	vector<Position*> ret;
	vector<Position*> polesRet;

	int aux = 1;
	double latStartAux = latStart;
	
	while(numOfRuns>0)
	{
		for(int j = 0; j < (aux/2 + 1); j++)
		{
			vector<bool> sidesToDraw = {1,0,0,1};
			if (numOfRuns == 1)
			{
				sidesToDraw[2] = 1;
				if (j == (aux / 2))
					sidesToDraw[1] = 1;
			}

			vector<vector<Position*> > block = generateBlock(latStart, lngStart + getLongOfDistance(latStart, blockWidth + streetWidth)*j, scenario, blockWidth, streetWidth, polesPerSide, metersPerSide,sidesToDraw);
			vector<Position*> meterBlock = block[0]; vector<Position*> polesBlock = block[1];
			ret.insert(ret.end(), meterBlock.begin(), meterBlock.end());
			polesRet.insert(polesRet.end(), polesBlock.begin(), polesBlock.end());
		}
		for(int j = 0; j < aux/2; j++)
		{

			vector<bool> sidesToDraw = { 1, 0, 0, 1 };
			if (numOfRuns == 1)
				sidesToDraw[1] = 1;
			vector<vector<Position*> > block = generateBlock(latStartAux + getLatOfDistance(blockWidth + streetWidth)*(j), lngStart + getLongOfDistance(latStartAux + getLatOfDistance(blockWidth + streetWidth)*j, blockWidth + streetWidth)*(aux / 2), scenario, blockWidth, streetWidth, polesPerSide,metersPerSide, sidesToDraw);
			vector<Position*> meterBlock = block[0]; vector<Position*> polesBlock = block[1];
			ret.insert(ret.end(), meterBlock.begin(), meterBlock.end());
			polesRet.insert(polesRet.end(), polesBlock.begin(), polesBlock.end());
		}
		//vector<Position*> block = generateBlock(latStart, lngStart + getLongOfDistance(latStart + getLatOfDistance(blockWidth + streetWidth), blockWidth)*(aux / 2), scenario, blockWidth);
		//ret.insert(ret.end(), block.begin(), block.end());
		latStart += getLatOfDistance(blockWidth + streetWidth);

		
		aux += 2;//pq é um quadrado
		numOfRuns--;
	}
	vector<vector<Position*> > pairRet;
	pairRet.push_back(ret);
	pairRet.push_back(polesRet);
	return pairRet;

}
//void testGraspFromFile(string metersFile, string polesFile, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, int SRD, int meshEnabled, string rubyPath)
//{
//	string completeMetersFile = rubyPath + "arqsTeste/" + metersFile;
//	string completePolesFile = rubyPath + "arqsTeste/" + polesFile;
//	FILE * file;
//	file = fopen(completeMetersFile.c_str(), "r");
//	FILE * file2;
//	file2 = fopen(completePolesFile.c_str(), "r");
//
//
//	vector<Position*> meters;
//	vector<Position*> poles;
//
//	while (true)
//	{
//		double lat = -1;
//		double lng = -1;
//		fscanf(file, "%lf %lf", &lat, &lng);
//		if (lat == -1)
//			break;
//		Position *toAdd = new Position(lat, lng, meters.size());
//		meters.push_back(toAdd);
//	}
//	while (true)
//	{
//		double lat = -1;
//		double lng = -1;
//		fscanf(file2, "%lf %lf", &lat, &lng);
//		if (lat == -1)
//			break;
//		Position *toAdd = new Position(lat, lng, poles.size());
//		poles.push_back(toAdd);
//	}
//
//	fclose(file);
//	fclose(file2);
//
//	string finalResult = "";
//	int gridSize = 500;
//	AutoPlanning* ap = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, gridSize,rubyPath);
//
//	//string gresult = "";
//	string ret = ap->graspAutoPlanning();
//	//string gresult = "";
//	//string ret = ap->gridAutoPlanning();
//	for (int i = 0; i < meters.size(); i++)
//	{
//		delete meters[i];
//	}
//	for (int i = 0; i < poles.size(); i++)
//	{
//		delete poles[i];
//	}
//	delete ap;
//}
AutoPlanning* setAutoPlanningFromFile(string metersFilePath, string polesFilePath, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, int SRD, int meshEnabled, int verbose = 0)
{

	//string completeMetersFile = rubyPath + "arqsTeste/" + metersFile;
	//string completePolesFile = rubyPath + "arqsTeste/" + polesFile;
	FILE * file;
	file = fopen(metersFilePath.c_str(), "r");
	FILE * file2;
	file2 = fopen(polesFilePath.c_str(), "r");


	vector<Position*> meters;
	vector<Position*> poles;

	while (true)
	{
		double lat = -1;
		double lng = -1;
		fscanf(file, "%lf %lf", &lat, &lng);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, meters.size());
		meters.push_back(toAdd);
	}
	while (true)
	{
		double lat = -1;
		double lng = -1;
		fscanf(file2, "%lf %lf", &lat, &lng);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}

	fclose(file);
	fclose(file2);

	int gridSize = 500;
	AutoPlanning* res = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, gridSize, rubyPath,verbose);
	return res;


}
TestResult* gridPlanningTest(AutoPlanning *AP,int gridSize, int usePostOptimization, int redundancy)
{
	AP->setGridSize(gridSize);
	
	cout << "\nIniciando teste com célula de tamanho " << to_string(gridSize) << " com " << to_string(AP->getMetersSize()) << " medidores e " << to_string(AP->getPolesSize()) << " postes\n";
	TestResult* ret = AP->executeAutoPlanTestMode(usePostOptimization,redundancy);
	return ret;
}
void executeTest(string meterFile, string poleFile, string pathToSave,int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(),poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD,  mesh);

	string result = "";

	string fileToSave = pathToSave + "Meters" + to_string(AP->getMetersSize()) + "Poles" + to_string(AP->getPolesSize()) + "S" + to_string(scenario) + "T" + to_string(tech) + "Hops" + to_string(mesh + 1) + ".txt";
	ofstream f(fileToSave.c_str());
	//result += gridPlanningTest(AP, 100, false, 1);
	//result += gridPlanningTest(AP, 100, true, 1);
	//result += gridPlanningTest(AP, 500, false, 1);
	//result += gridPlanningTest(AP, 500, true, 1);
	//result += gridPlanningTest(AP, 1000, false, 1);
	//result += gridPlanningTest(AP, 1000, true, 1);
	//result += gridPlanningTest(AP, 5000, false, 1);
	//result += gridPlanningTest(AP, 5000, true, 1);
	//result += gridPlanningTest(AP, 10000, false, 1);
	//result + "\n------------------------------\n";
	//result += gridPlanningTest(AP, 100, 0, 2);
	//result += gridPlanningTest(AP, 100, 2, 2);
	//result += gridPlanningTest(AP, 500, 0, 2);
	//result += gridPlanningTest(AP, 500, 2, 2);
	//result += gridPlanningTest(AP, 1000, 0, 2);
	//result += gridPlanningTest(AP, 1000, 2, 2);
	//result += gridPlanningTest(AP, 5000, 0, 2);
	//result += gridPlanningTest(AP, 5000, 2, 2);
	//result += gridPlanningTest(AP, 10000, 2, 2);
	//result + "\n------------------------------\n";
	//result += gridPlanningTest(AP, 100, 0, 3);
	//result += gridPlanningTest(AP, 100, 2, 3);
	//result += gridPlanningTest(AP, 500, 0, 3);
	//result += gridPlanningTest(AP, 500, 2, 3);
	//result += gridPlanningTest(AP, 1000, 0, 3);
	//result += gridPlanningTest(AP, 1000, 2, 3);
	//result += gridPlanningTest(AP, 5000, 0, 3);
	//result += gridPlanningTest(AP, 5000, 2, 3);
	//result += gridPlanningTest(AP, 10000, 2, 3);
	//result + "\n------------------------------\n";
	
	f << result;
	f.close();

	delete AP;
	//result += gridPlanningTest(AP, 100000, 0, 2);
	//result += gridPlanningTest(AP, 100, 1, 1);
	



}
//0 cluster 1 guloso 2 grasp
string executePlanningTest(string rubyPath,string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh, int redundancy, int type, int iterations = 500, double alpha = 0.9)
{
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD, mesh,1);

	//	AP->saveGLPKFileWithLimit(scp,1, 5);

	string result = "";

	string fileToSave = to_string(AP->getMetersSize()) + "Poles" + to_string(AP->getPolesSize()) + "S" + to_string(scenario) + "T" + to_string(tech) + "Hops" + to_string(mesh + 1) + "Redundancy" + to_string(redundancy) + "MemLim" + to_string(MEM_LIMIT) + ".txt";

	TestResult* ret;
	int size=1;
	switch (type)
	{
	case 0:
		fileToSave = pathToSave + "Cluster" + fileToSave ;
		ret = AP->executeClusterAutoPlanTestMode(true, redundancy);
		result = ret->toString();
		delete(ret);
		break;
	case 1:
		fileToSave = pathToSave + "Guloso" + fileToSave;
		ret = AP->executeGraspAutoPlanTestMode(1, 1, redundancy, true);
		result = ret->toString();
		delete(ret);
		break;
	case 2:
		fileToSave = pathToSave + "Grasp" + fileToSave;
		ret = AP->executeGraspAutoPlanTestMode(iterations, alpha, redundancy, true);
		result = ret->toString();
		delete(ret);
		break;
	case 3:
		fileToSave = pathToSave + "Cell" + fileToSave;
		size = AP->getBestCellSize();
		ret = gridPlanningTest(AP, size, true, redundancy);
		
		result = ret->toString();
		delete(ret);
		break;
	case 4:
		fileToSave = pathToSave + "ClusterVariable" + fileToSave;
		ret = AP->executeClusterVariableAutoPlanTestMode(true, redundancy);
		result = ret->toString();
		delete(ret);
		break;
	default:
		break;
	}
	 

	ofstream f(fileToSave.c_str());
	f << result;
	f.close();

	delete AP;
	return result;
}
void kmeansTest(string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD, mesh);
	AP->executeClusterAutoPlanTestMode(true, 1);
}
void executionTimeTest(int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{

	ofstream f("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\executionTimeTestResults");
	int numOfCells = 1;
	while (true)
	{
	
		vector<vector<Position*> > teste = gridInstanceGenerator(0, 0, 10 + distFromHouseToStreet, scenario, numOfCells, 100 - distFromHouseToStreet, 4, 3);
		vector<Position*> meters;
		for(int i = 0; i < teste[0].size();i++)
		{
			Position* toAdd = new Position(teste[0][i]->latitude,teste[0][i]->longitude, i);
			meters.push_back(toAdd);
		}
		vector<Position*> poles;
		for (int i = 0; i < teste[1].size(); i++)
		{
			Position* toAdd = new Position(teste[1][i]->latitude, teste[1][i]->longitude, i);
			poles.push_back(toAdd);
		}
		string ret = "";
		int gridSize = 500;
		AutoPlanning* AP = new AutoPlanning(meters, poles, Suburbano, tech, bitrate, power, hx, rx, SRD, 3, gridSize, rubyPath);
		TestResult *res = AP->clusterAutoPlanningTestMode(false, 1);
		cout << to_string(AP->getMetersSize()) << "x" << to_string(AP->getPolesSize()) << " " << res->solverTime << "\n";
		f << to_string(AP->getMetersSize()) << "x" << to_string(AP->getPolesSize()) << " " << res->solverTime << "\n";
		delete res;
		delete AP;
		numOfCells++;
	}


	f.close();

}
vector<vector<int> > SCPGenerator(int mSize, int pSize, double density)
{
	int covNum = (mSize*pSize)*density;
	int i = 0;
	vector<vector<int> > ret;
	if (density == 1)
	{
		for (int i = 0; i < pSize; i++)
		{
			vector<int> toAdd;
			for (int j = 0; j < mSize; j++)
			{
				toAdd.push_back(j);
			}
			random_shuffle(toAdd.begin(), toAdd.end());
			ret.push_back(toAdd);
		}
		return ret;
	}
	for (int i = 0; i < pSize; i++)
	{
		vector<int> add;
		ret.push_back(add);
	}
	while (covNum)
	{
		start:
		int poleToInsert = rand()%pSize;
		vector<int> options;
		for (int j = 0; j < mSize; j++)
		{
			if (find(ret[poleToInsert%pSize].begin(), ret[poleToInsert%pSize].end(), j) == ret[poleToInsert%pSize].end())
				options.push_back(j);
		}
		if (options.size() == 0)
			goto start;
		int x = rand() % options.size();
		ret[poleToInsert].push_back(options[x]);
		covNum--;
		i++;
	}

	return ret;
}
vector<vector<int> > geographicSCPGenerator(int mSize, int pSize, double density, int range)
{
	int covNum = (mSize*pSize)*density;

	vector<vector<int> > ret;
	for (int i = 0; i < pSize; i++)
	{
		vector<int> add;
		ret.push_back(add);
	}
	int i = 0;
	while (covNum)
	{
		vector<int> options;
		for (int j = i%pSize; j < (i%pSize + range); j++)
		{
			if (find(ret[i%pSize].begin(), ret[i%pSize].end(), j) == ret[i%pSize].end())
				options.push_back(j%pSize);
		}
		if(options.size())
		{
			int x = rand() % options.size();
			ret[i%pSize].push_back(options[x]);
		}
		covNum--;
		i++;
	}

	return ret;
}
void increaseSCPDensity(vector<vector<int> > &SCP,int mSize, double newDensity)
{
	int covNum = 0;
 	for (int i = 0; i < SCP.size(); i++)
	{
		covNum += SCP[i].size();
	}
	int newCovNum = ((mSize*SCP.size())*newDensity) - covNum;
	int i = covNum % (SCP.size());
	cout << i;
	while (newCovNum)
	{
		vector<int> options;
		for (int j = 0; j < mSize; j++)
		{
			if (find(SCP[i%SCP.size()].begin(), SCP[i%SCP.size()].end(), j) == SCP[i%SCP.size()].end())
				options.push_back(j);
		}
		int x = rand() % options.size();
		SCP[i%SCP.size()].push_back(options[x]);
		i++;
		newCovNum--;
	}

}
void varySCPDensity(vector<vector<int> > &SCP, int mSize, double newDensity)
{
	int covNum = 0;
	for (int j = 0; j < SCP.size(); j++)
	{
		covNum += SCP[j].size();
	}
	int newCovNum = ((mSize*SCP.size())*newDensity) - covNum;
	int i = covNum % (SCP.size());

	//cout << i;
	while (newCovNum > 0)
	{
		start:
		vector<int> options;
		int poleToInsert = rand()%SCP.size();
		for (int j = 0; j < mSize; j++)
		{
			if (find(SCP[poleToInsert %SCP.size()].begin(), SCP[poleToInsert %SCP.size()].end(), j) == SCP[poleToInsert %SCP.size()].end())
				options.push_back(j);
		}
		if (options.size() == 0)
			goto start;
		int x = rand() % options.size();
		SCP[poleToInsert].push_back(options[x]);
		i++;
		newCovNum--;
	}
	while (newCovNum < 0)
	{
		i--;
		if (i < 0)
			i = SCP.size()-1;

		SCP[i%SCP.size()].pop_back();
		newCovNum++;
	}

}
void varySCPCoverage(vector<vector<int> > &SCP, int mSize, int toAddNum)
{

	while (toAddNum > 0)
	{
	start:
		vector<int> options;
		int poleToInsert = rand() % SCP.size();
		for (int j = 0; j < mSize; j++)
		{
			if (find(SCP[poleToInsert %SCP.size()].begin(), SCP[poleToInsert %SCP.size()].end(), j) == SCP[poleToInsert %SCP.size()].end())
				options.push_back(j);
		}
		if (options.size() == 0)
			goto start;
		int x = rand() % options.size();
		SCP[poleToInsert].push_back(options[x]);	
		toAddNum--;
	}
}
void increaseRangeTest(string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	

	string toSave = "";
	//AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Urbano, t802_15_4, bitrate, 0, hx, rx, SRD, mesh, rubyPath);
	AutoPlanning* AP;
	TestResult* ret;
	////Primeiro teste: Menor alcance considerado
	//AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Urbano, t802_15_4, bitrate, 0, hx, rx, SRD, mesh, rubyPath);
	//ret = AP->executeClusterAutoPlanTestMode(false, 1);
	//cout << ret->toString();
	////toSave += to_string(hataTestRange) + " " + to_string(ret->solverTime) + "\n";
	////cout << to_string(hataTestRange) << " " << to_string(ret->solverTime) << " " << to_string(ret->solutionQuality) << " " << to_string(ret->uncoveredMeters) << " " << to_string(ret->maxMem) << "\n";
	//delete ret;
	//delete AP;
	//Segundo teste: Médio alcance considerado
	AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Suburbano, t802_11_g, bitrate, 20, hx, rx, SRD, mesh);
	ret = AP->executeClusterAutoPlanTestMode(false, 1);
	//toSave += to_string(hataTestRange) + " " + to_string(ret->solverTime) + "\n";
	//cout << to_string(hataTestRange) << " " << to_string(ret->solverTime) << " " << to_string(ret->solutionQuality) << " " << to_string(ret->uncoveredMeters) << " " << to_string(ret->maxMem) << "\n";
	cout << ret->toString();
	delete ret;
	delete AP;
	//Terceiro teste: Maior alcance considerado
	 AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Rural, t802_11_g, bitrate, 22, hx, rx, SRD, mesh);
	ret = AP->executeClusterAutoPlanTestMode(false, 1);
	//toSave += to_string(hataTestRange) + " " + to_string(ret->solverTime) + "\n";
	//cout << to_string(hataTestRange) << " " << to_string(ret->solverTime) << " " << to_string(ret->solutionQuality) << " " << to_string(ret->uncoveredMeters) << " " << to_string(ret->maxMem) << "\n";
	cout << ret->toString();
	delete ret;
	delete AP;

	string filename = rubyPath + "arqsTeste/RangeTest.txt";
	ofstream f(filename.c_str());
	f << toSave;
	f.close();
}
void saveGLPKFile(vector<vector<int> > &SCP, int mSize, int pSize, int redundancy)
{
	//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
	//Pois se não, o método retornaria que a solução é impossível!
	//A formatação você ve no arquivo GlpkFile.txt

	
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\nparam B{r in Z} default " + to_string(redundancy) + ", integer;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=B[r];\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "Results.txt\";\n data;\n";
	resp += "set Z:= ";
	for (int i = 0; i < mSize; i++)
	{
		resp += "Z" + to_string(i) + " ";
	}

	resp += ";\n";
	resp += "set Y:= ";

	for (int i = 0; i < pSize; i++)
		resp += "Y" + to_string(i) + " ";
	resp += ";\n";
	resp += "param A := ";
	resp += "\n";
	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < pSize; j++)
		{
			//bool um = false;
			//for (int k = 0; k < SCP.size(); k++)
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), i) != SCP[j].end());
			if (cov)
				resp += "[Z" + to_string(i) + ",Y" + to_string(j) + "] 1";

		}
	}
	resp += "\n";
	resp += ";";
	resp += "\n";
	resp += "param B := ";
	resp += "\n";
	vector<int> aux(mSize, 0);
	for (int i = 0; i < SCP.size(); i++)
	{
		for (int j = 0; j < SCP[i].size(); j++)
		{
			aux[SCP[i][j]] = 1;
		}
		
		
	}
	for (int i = 0; i < aux.size(); i++)
	{
		if (aux[i] == 0)
		resp += "[Z" + to_string(i) + "] " + to_string(0);
	}
	
	resp += "\n;\nend;\n";

	string filename = rubyPath + "GlpkFile.txt";
	ofstream f(filename.c_str());

	f << resp;
	f.close();
}
//void get_mip_gap(glp_tree *T, void *info)
//{
//	double gap = glp_ios_mip_gap(T);
//	*(double*)info = gap;
//}
void executeGLPK(string filename,double timeLimit, double* solverTime, double* maxMem)
{
	glp_term_out(GLP_ON);
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
	glp_mem_limit(5800);
	double gap = 0;
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
//	parm.cb_func = get_mip_gap;
//	parm.cb_info = &gap;
	parm.presolve = GLP_ON;
	parm.tm_lim = timeLimit * 1000; //TEMPO LIMITE EM SEGUNDOS

	begin_time = clock();
	err = glp_intopt(lp, &parm);
	seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
	*solverTime = seconds;
	glp_mem_usage(&count, &cpeak, &total, &tpeak);
	//printf("%d memory block(s) are still allocated\n", count);
	totalInMb = ((double)total / (1024 * 1024));
	tpeakInMb = ((double)tpeak / (1024 * 1024));
	*maxMem = totalInMb;
	cout << gap;
skip: glp_mpl_free_wksp(tran);
	glp_delete_prob(lp);
}
void increaseDensityTest(int mSize, int pSize, string id, int timeLimit)
{
	string filename = rubyPath + "GlpkFile.txt";
	string fileOutput = rubyPath +"density_tests/DensityResult"+to_string(mSize)+"x"+to_string(pSize)+"-"+id+".txt";
	
	double density = 0.005;
	ofstream f(fileOutput.c_str());
	vector<vector<int> > scp = SCPGenerator(mSize, pSize, density);
	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (density <= 0.1)
	{
		double solverTime = -1, maxMem = -1;
		
		saveGLPKFile(scp, mSize, pSize, 1);
		//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
		//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
		//system(access.c_str());
		glp_term_out(GLP_ON);
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
		glp_mem_limit(5800);
		double gap = 0;
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
		//parm.cb_func = get_mip_gap;
	//	parm.cb_info = &gap;
		parm.presolve = GLP_ON;
		parm.tm_lim = timeLimit*1000; //TEMPO LIMITE DE 60 SEGUNDOS

		begin_time = clock();
		err = glp_intopt(lp, &parm);
		seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;
		solverTime = seconds;
		glp_mem_usage(&count, &cpeak, &total, &tpeak);
		//printf("%d memory block(s) are still allocated\n", count);
		totalInMb = ((double)total / (1024 * 1024));
		tpeakInMb = ((double)tpeak / (1024 * 1024));
		maxMem = totalInMb;
		cout << gap;
	skip: glp_mpl_free_wksp(tran);
		glp_delete_prob(lp);
		

		f << to_string(density) << " " << solverTime << " " << maxMem << "\n";
		density += 0.0005;
		increaseSCPDensity(scp, mSize, density);
		if (solverTime >= timeLimit)
			break;
	}
	f.close();
}
DensityTestResult* varyDensityTest(int mSize, int pSize, int timeLimit, double initDensity, double rate, bool adjustRate)
{
	string filename = rubyPath + "GlpkFile.txt";
	//string fileOutput = rubyPath + "density_tests/DensityResult" + to_string(mSize) + "x" + to_string(pSize) + "-" + id + ".txt";
	DensityTestResult* resultsRet = new DensityTestResult();
	vector<vector<int> > aux;
	//double density = 0.0005;
	//ofstream f(fileOutput.c_str());
	vector<vector<int> > scp = SCPGenerator(mSize, pSize, initDensity);

	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (true)
	{		
		double solverTime = -1, maxMem = -1;
		saveGLPKFile(scp, mSize, pSize, 1);
		//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
		//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
		//system(access.c_str());
		executeGLPK(filename, timeLimit, &solverTime, &maxMem);
	
		if (solverTime >= timeLimit && adjustRate)
		{
			initDensity -= rate;
			if (initDensity < 0)
				initDensity = 0;
			scp = aux;
			rate = rate / 10;
			
			if (rate <= 0.00001)
				goto end;
		}
		else
		{
			vector<double> toAdd;
			toAdd.push_back(initDensity);
			toAdd.push_back(solverTime);
			toAdd.push_back(maxMem);
			resultsRet->results.push_back(toAdd);
			//f << to_string(initDensity) << " " << solverTime << " " << maxMem << "\n";
		}

		initDensity += rate;
		if (initDensity > 1 || initDensity < 0)
			goto end;
		//if ((rate > 0 && initDensity > 0.5) || (rate < 0 && initDensity <= 0.5))
		//{
		//	goto end;
		//}
			
		aux = scp;
		varySCPDensity(scp, mSize, initDensity);
		
	}
end:	
	//f.close();
	return resultsRet;
}
DensityTestResult* varySCPCoverageTest(int mSize, int pSize, int timeLimit, int rate, int endVal)
{
	string filename = rubyPath + "GlpkFile.txt";
	//string fileOutput = rubyPath + "density_tests/DensityResult" + to_string(mSize) + "x" + to_string(pSize) + "-" + id + ".txt";
	DensityTestResult* resultsRet = new DensityTestResult();
	vector<vector<int> > aux;
	//double density = 0.0005;
	//ofstream f(fileOutput.c_str());
	vector<vector<int> > scp = SCPGenerator(mSize, pSize, 0);
	int currentVal = 0;

	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (true)
	{
		double solverTime = -1, maxMem = -1;
		saveGLPKFile(scp, mSize, pSize, 1);
		//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
		//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
		//system(access.c_str());
		executeGLPK(filename, timeLimit, &solverTime, &maxMem);


		vector<double> toAdd;
		toAdd.push_back(currentVal);
		toAdd.push_back(solverTime);
		toAdd.push_back(maxMem);
		resultsRet->results.push_back(toAdd);
			
		

		currentVal += rate;
		
		if (currentVal > endVal || currentVal>(mSize*pSize))
			goto end;


		varySCPCoverage(scp, mSize, rate);

	}
end:
	//f.close();
	return resultsRet;
}

void fullDensityTest(int mSize, int pSize, int timeLimit, int numOfInst, bool coverageIncrease)
{
	vector<DensityTestResult*> incResults;
	for (int i = 0; i < numOfInst; i++)
	{
		DensityTestResult * res;
		if (coverageIncrease)
			res = varySCPCoverageTest(mSize, pSize, timeLimit, 100,14400);
		else
			res = varyDensityTest(mSize, pSize, timeLimit, 0.00, +0.01,0);
		//DensityTestResult * res2 = varyDensityTest(mSize, pSize, rubyPath, timeLimit, 1, -0.01);
		string filename = rubyPath + "density_tests/DensityResult" + to_string(mSize) + "x" + to_string(pSize) + "-" + to_string(i) + ".txt";
		res->saveToFile(filename);
		incResults.push_back(res);
	}

	vector<double> incDensities;
	for (int i = 0; i < incResults.size(); i++)
	{
		vector<double> aux = incResults[i]->getDensitiesVector();
		incDensities.insert(incDensities.end(),aux.begin(),aux.end());
	}
	//for (int i = 0; i < decResults.size(); i++)
	//{
	//	vector<double> aux = decResults[i]->getDensitiesVector();
	//	incDensities.insert(incDensities.end(), aux.begin(), aux.end());
	//}
	sort(incDensities.begin(), incDensities.end());
	incDensities.erase(unique(incDensities.begin(), incDensities.end()), incDensities.end());
	vector<vector<double>> timeResults;
	vector<vector<double>> memResults;
	for (int i = 0; i < incDensities.size(); i++)
	{
		vector<double> toAddTime,toAddMem;
		toAddTime.push_back(incDensities[i]);
		toAddMem.push_back(incDensities[i]);
		for (int j = 0; j < incResults.size(); j++)
		{
			toAddTime.push_back(incResults[j]->getTimeAtDensity(incDensities[i]));
			toAddMem.push_back(incResults[j]->getMemAtDensity(incDensities[i]));
		}
		timeResults.push_back(toAddTime);
		memResults.push_back(toAddMem);
	}
	vector<double> avgTimeResults, minTimeResults, maxTimeResults, avgMemResults,avgStdDeviation,avgMemStdDeviation;
	for(int i = 0; i < timeResults.size(); i++)
	{
		double avg = 0, min = -1, max = -1, memAvg = 0, stdDeviation = 0, memStdDeviation = 0, numToDivide = 0;
		for (int j = 1; j < timeResults[i].size(); j++)
		{
			if (timeResults[i][j] != -1)
			{
				if (min == -1 || timeResults[i][j] < min)
					min = timeResults[i][j];
				if (max == -1 || timeResults[i][j] > max)
					max = timeResults[i][j];

				avg += timeResults[i][j];
				memAvg += memResults[i][j];
				numToDivide++;
			}
		}
		for (int j = 1; j < timeResults[i].size(); j++)
		{
			if (timeResults[i][j] != -1)
			{
				stdDeviation += pow(timeResults[i][j] - (avg / numToDivide), 2);
				memStdDeviation += pow(memResults[i][j] - (memAvg / numToDivide), 2);
			}
		}
		avgTimeResults.push_back(avg / numToDivide);
		avgMemResults.push_back(memAvg / numToDivide);
		minTimeResults.push_back(min);
		maxTimeResults.push_back(max);
		stdDeviation = sqrt(stdDeviation / numToDivide);
		memStdDeviation = sqrt(memStdDeviation / numToDivide);
		avgStdDeviation.push_back(stdDeviation);
		avgMemStdDeviation.push_back(memStdDeviation);		
	}
	string tmRes, memRes;
	for (int i = 0; i < timeResults.size(); i++)
	{
		tmRes += to_string(timeResults[i][0]) + " " + to_string(avgTimeResults[i] - avgStdDeviation[i]) + " " + to_string(avgTimeResults[i] + avgStdDeviation[i]) + " " + to_string(avgTimeResults[i]) + " " + to_string(minTimeResults[i]) + " " + to_string(maxTimeResults[i]) +   "\n";
		memRes += to_string(memResults[i][0]) + " " + to_string(avgMemResults[i] - avgMemStdDeviation[i]) + " " + to_string(avgMemResults[i] + avgMemStdDeviation[i]) + " " + to_string(avgMemResults[i]) + "\n";
		//for (int j = 0; j < timeResults[i].size(); j++)
		//{
		//	teste += to_string(timeResults[i][j]) + " ";
		//}
		//teste += "\n";
	}
	cout << tmRes;
	cout << memRes;
	string timeFileOutput = rubyPath + "density_tests/DensityResultTime" + to_string(mSize) + "x" + to_string(pSize)+ ".txt";
	string memFileOutput = rubyPath + "density_tests/DensityResultMem" + to_string(mSize) + "x" + to_string(pSize) + ".txt";
	ofstream f(timeFileOutput.c_str());
	ofstream f2(memFileOutput.c_str());
	f << tmRes;
	f2 << memRes;
	f.close();
	f2.close();
	for (int i = 0; i < incResults.size(); i++)
	{
		delete incResults[i];
	}

	
}
void memoryEstimationTest(int ini, int end, int variation)
{
	int start = ini;
	while (ini <= end)
	{
		//int max = 2000, min = 1;
		//double val1 = (rand() % (max - min)) + min, val2 = (rand() % (max - min));
		string toSave = rubyPath + "mem_estimation_tests/memEstimation"+to_string(ini)+"metersfixed.txt";
		string toSave2 = rubyPath + "mem_estimation_tests/memEstimation" + to_string(ini) + "polesfixed.txt";
		ofstream f(toSave.c_str());
		ofstream f2(toSave2.c_str());
		for (int i = start; i <= end; i+=variation)
		{
			double realmem = memoryTest(ini, i);
			double estMem = memEstimation(ini, i);
			f << i << " " << realmem << " " << estMem << "\n";

			realmem = memoryTest(i, ini);
			estMem = memEstimation(i, ini);
			f2 << i << " " << realmem << " " << estMem << "\n";

		}
		f.close();
		f2.close();
		ini += variation;
	}
}
void memoryEstimationTestWithFixedMeters(int fixedMeters,int ini, int variation)
{
	string toSave = rubyPath + "/mem_estimation_tests/memEstimation" + to_string(fixedMeters) + "metersfixed.txt";
	ofstream f(toSave.c_str());
	while (memEstimation(fixedMeters, ini)*MEM_EST_SAFETY < MEM_LIMIT)
	{
		double estMem = memEstimation(fixedMeters, ini);
		double realmem = memoryTest(fixedMeters, ini);
		f << ini << " " << realmem << " " << estMem << "\n";
		ini += variation;

	}
	f.close();
	
}
void memoryEstimationTestWithFixedPoles(int fixedPoles, int ini, int variation)
{
	string toSave = rubyPath + "/mem_estimation_tests/memEstimation" + to_string(fixedPoles) + "polesfixed.txt";
	ofstream f(toSave.c_str());
	while (memEstimation(ini, fixedPoles)*MEM_EST_SAFETY < MEM_LIMIT)
	{
		double realmem = memoryTest(ini, fixedPoles);
		double estMem = memEstimation(ini, fixedPoles);
		f << ini << " " << realmem << " " << estMem << "\n";
		ini += variation;
	}
	f.close();

}
void saveResults(string filesLocation, string metersFilename, string polesFilename, int scenario, int tech, int mesh)
{
	string toSave = filesLocation + "\\ResultsT" + to_string(tech)+"S"+to_string(scenario)+"Mesh"+to_string(mesh)+".txt";
	metersFilename = filesLocation + "\\" + metersFilename;
	polesFilename = filesLocation + "\\" + polesFilename;
	AutoPlanning* AP;
	if (tech == t802_15_4)
		AP = setAutoPlanningFromFile(metersFilename.c_str(), polesFilename.c_str(), scenario,tech, 0.250, 0, 3, 5, 1, mesh);
	else
		AP = setAutoPlanningFromFile(metersFilename.c_str(), polesFilename.c_str(), scenario,tech, 6, 20, 3, 5, 1, mesh);

	
	TestResult* ret = AP->executeClusterAutoPlanTestMode(false, 1);
	cout << ret->toString();
	
	ofstream f(toSave.c_str());
	f << ret->toString();
	f.close();
	delete ret;
	delete AP;
}
DensityTestResult* fixedDensityTest(double density, int mSize,int pSize, int variation, int timeLimit, double memLimit, int mode)
{
	string filename = rubyPath + "GlpkFile.txt";
	//string fileOutput = rubyPath + "density_tests/DensityResult" + to_string(mSize) + "x" + to_string(pSize) + "-" + id + ".txt";
	DensityTestResult* resultsRet = new DensityTestResult();
	vector<vector<int> > aux;
	//double density = 0.0005;
	//ofstream f(fileOutput.c_str());


	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (true)
	{
		if (memEstimation(mSize, pSize) > memLimit)
			break;

		vector<vector<int> > scp = SCPGenerator(mSize, pSize, density);
		double solverTime = -1, maxMem = -1;
		saveGLPKFile(scp, mSize, pSize, 1);
		//string access = rubyPath + "glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"wow.txt";
		//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
		//system(access.c_str());
		executeGLPK(filename, timeLimit, &solverTime, &maxMem);

		vector<double> toAdd;
		if (mode == NO_FIX)
		{
			toAdd.push_back(mSize);
			mSize += variation;
			pSize += variation;
		}
		if (mode == FIXED_METERS)
		{
			toAdd.push_back(pSize);
			pSize += variation;
		}
		if (mode == FIXED_POLES)
		{
			toAdd.push_back(mSize);
			mSize += variation;
		}
		toAdd.push_back(solverTime);
		toAdd.push_back(maxMem);
		resultsRet->results.push_back(toAdd);
		//f << to_string(initDensity) << " " << solverTime << " " << maxMem << "\n";
		//cout << to_string(size) << " " << solverTime << " " << maxMem << "\n";
		

		if (solverTime > timeLimit)
			break;

	}
	return resultsRet;
}
void fullFixedDensityTest(double density, int mSize,int pSize, int variation, int timeLimit, double memLimit,int numOfInst, int mode)
{
	vector<DensityTestResult*> incResults;
	for (int i = 0; i < numOfInst; i++)
	{
		DensityTestResult * res = fixedDensityTest(density, mSize,pSize, variation, timeLimit,memLimit, mode);
		string filename = "";
		if (mode == NO_FIX)
			filename = rubyPath + "density_tests/FixedDensityResultNoSizeFix" + to_string(density) + "Inst"+to_string(i)+".txt";
		if (mode == FIXED_METERS)
			filename = rubyPath + "density_tests/FixedDensityResult"+to_string(mSize) +"FixedMeters" + to_string(density) + "Inst" + to_string(i) + ".txt";
		if (mode == FIXED_POLES)
			filename = rubyPath + "density_tests/FixedDensityResult"+ to_string(pSize) + "FixedPoles" + to_string(density) + "Inst" + to_string(i) + ".txt";
		res->saveToFile(filename);
		incResults.push_back(res);
	}

	vector<double> incDensities;
	for (int i = 0; i < incResults.size(); i++)
	{
		vector<double> aux = incResults[i]->getDensitiesVector();
		incDensities.insert(incDensities.end(), aux.begin(), aux.end());
	}
	//for (int i = 0; i < decResults.size(); i++)
	//{
	//	vector<double> aux = decResults[i]->getDensitiesVector();
	//	incDensities.insert(incDensities.end(), aux.begin(), aux.end());
	//}
	sort(incDensities.begin(), incDensities.end());
	incDensities.erase(unique(incDensities.begin(), incDensities.end()), incDensities.end());
	vector<vector<double>> timeResults;
	vector<vector<double>> memResults;
	for (int i = 0; i < incDensities.size(); i++)
	{
		vector<double> toAddTime, toAddMem;
		toAddTime.push_back(incDensities[i]);
		toAddMem.push_back(incDensities[i]);
		for (int j = 0; j < incResults.size(); j++)
		{
			toAddTime.push_back(incResults[j]->getTimeAtDensity(incDensities[i]));
			toAddMem.push_back(incResults[j]->getMemAtDensity(incDensities[i]));
		}
		timeResults.push_back(toAddTime);
		memResults.push_back(toAddMem);
	}
	vector<double> avgTimeResults, minTimeResults, maxTimeResults, avgMemResults, avgStdDeviation, avgMemStdDeviation;
	for (int i = 0; i < timeResults.size(); i++)
	{
		double avg = 0, memAvg = 0,min=-1,max=-1, stdDeviation = 0, memStdDeviation = 0, numToDivide = 0;
		for (int j = 1; j < timeResults[i].size(); j++)
		{
			if (min == -1 || timeResults[i][j] < min)
				min = timeResults[i][j];
			if (max == -1 || timeResults[i][j] > max)
				max = timeResults[i][j];
			if (timeResults[i][j] != -1)
			{
				avg += timeResults[i][j];
				memAvg += memResults[i][j];
				numToDivide++;
			}
		}
		for (int j = 1; j < timeResults[i].size(); j++)
		{
			if (timeResults[i][j] != -1)
			{
				stdDeviation += pow(timeResults[i][j] - (avg / numToDivide), 2);
				memStdDeviation += pow(memResults[i][j] - (memAvg / numToDivide), 2);
			}
		}
		avgTimeResults.push_back(avg / numToDivide);
		avgMemResults.push_back(memAvg / numToDivide);
		stdDeviation = sqrt(stdDeviation / numToDivide);
		minTimeResults.push_back(min);
		maxTimeResults.push_back(max);
		memStdDeviation = sqrt(memStdDeviation / numToDivide);
		avgStdDeviation.push_back(stdDeviation);
		avgMemStdDeviation.push_back(memStdDeviation);
	}
	string tmRes, memRes;
	for (int i = 0; i < timeResults.size(); i++)
	{
		tmRes += to_string(timeResults[i][0]) + " " + to_string(avgTimeResults[i] - avgStdDeviation[i]) + " " + to_string(avgTimeResults[i] + avgStdDeviation[i]) + " " + to_string(avgTimeResults[i]) + " " + to_string(minTimeResults[i]) + " " + to_string(maxTimeResults[i]) + "\n";;
		memRes += to_string(memResults[i][0]) + " " + to_string(avgMemResults[i] - avgMemStdDeviation[i]) + " " + to_string(avgMemResults[i] + avgMemStdDeviation[i]) + " " + to_string(avgMemResults[i]) + "\n";
		//for (int j = 0; j < timeResults[i].size(); j++)
		//{
		//	teste += to_string(timeResults[i][j]) + " ";
		//}
		//teste += "\n";
	}
	cout << tmRes;
	cout << memRes;
	string timeFileOutput = "";
	string memFileOutput = "";
	if (mode == NO_FIX)
	{
		timeFileOutput = rubyPath + "density_tests/FixedDensityResultTimeNoSizeFix" + to_string(density) + ".txt";
		memFileOutput = rubyPath + "density_tests/FixedDensityResultMemNoSizeFix" + to_string(density) + ".txt";
	}
	if (mode == FIXED_METERS)
	{
		timeFileOutput = rubyPath + "density_tests/FixedDensityResultTime" + to_string(mSize) + "FixedMeters" + to_string(density) + ".txt";
		memFileOutput = rubyPath + "density_tests/FixedDensityResultMem" + to_string(mSize) + "FixedMeters" + to_string(density) + ".txt";
	}
	if (mode == FIXED_POLES)
	{
		timeFileOutput = rubyPath + "density_tests/FixedDensityResultTime" + to_string(pSize) + "FixedPoles" +to_string(density) + ".txt";
		memFileOutput = rubyPath + "density_tests/FixedDensityResultMem" + to_string(pSize) + "FixedPoles" + to_string(density) + ".txt";
	}
	ofstream f(timeFileOutput.c_str());
	ofstream f2(memFileOutput.c_str());
	f << tmRes;
	f2 << memRes;
	f.close();
	f2.close();
	for (int i = 0; i < incResults.size(); i++)
	{
		delete incResults[i];
	}
}
void VaryMemLimTest(int init, int var, int max,  string metersFile, string polesFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh, int redundancy)
{
	int aux = MEM_LIMIT;
	MEM_LIMIT = init;
	cout << MEM_LIMIT;
	int meterSize=0, poleSize=0;
	

	vector<vector<double>> vec;
	while (true)
	{
		cout << "\nTESTANDO PARA MEM_LIM = " + to_string(MEM_LIMIT);
		AutoPlanning* AP = setAutoPlanningFromFile(metersFile.c_str(), polesFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD, mesh);
		TestResult* ret = AP->executeClusterAutoPlanTestMode(true, redundancy);
		//resp += to_string(MEM_LIMIT) + " " + to_string(ret->solutionQuality) + " " + to_string(ret->time) + " " + to_string(ret->poSolutionQuality) + " " + to_string(ret->poTime) + " " + to_string(ret->subProblemStats.size()) + "\n";
		vector<double> res;
		res.push_back(MEM_LIMIT);
		res.push_back(ret->solutionQuality);
		res.push_back(ret->time); 
		res.push_back(ret->poSolutionQuality);
		res.push_back(ret->poTime);
		res.push_back(ret->subProblemStats.size());
		vec.push_back(res);
		meterSize = AP->getMetersSize(); poleSize = AP->getPolesSize();
		delete AP;
		delete ret;
		if (MEM_LIMIT >= max)
			break;
		MEM_LIMIT += var;
	}
	int best_solution = vec[vec.size()-1][1];
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i].push_back(vec[i][3] / best_solution);
	}
	MEM_LIMIT = aux;
	string fileToSave = pathToSave+ "ClusterMeters"+to_string(meterSize) + "Poles" + to_string(poleSize) + "S" + to_string(scenario) + "T" + to_string(tech) + "Hops" + to_string(mesh + 1) + "Redundancy" + to_string(redundancy) + ".txt";
	ofstream f(fileToSave.c_str());
	string resp = "";
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec[i].size(); j++)
		{
			resp += to_string(vec[i][j]) + " ";
		}
		resp += "\n";
	}
	f << resp;
	f.close();
}
void MemoryTests()
{
	//memoryEstimationTestWithFixedMeters(1000, 1, 5000);
//	memoryEstimationTestWithFixedMeters(2000, 1, 1000);
	//memoryEstimationTestWithFixedMeters(3000, 1, 1000);
	//memoryEstimationTestWithFixedMeters(4000, 1, 1000);

//	memoryEstimationTestWithFixedPoles(1000, 1, 5000);
//	memoryEstimationTestWithFixedPoles(2000, 1, 1000);
	memoryEstimationTestWithFixedPoles(3000, 1, 1000);
	memoryEstimationTestWithFixedPoles(4000, 1, 1000);
	
}
void DensityTests()
{
	fullFixedDensityTest(0.001, 500, 500, 500, 360, 5000, 5, NO_FIX);
	fullFixedDensityTest(0.005, 500, 500, 500, 360, 5000, 5, NO_FIX);
	fullFixedDensityTest(0.01, 500, 500, 500, 360, 5000, 5, NO_FIX);

	fullFixedDensityTest(0.001, 1000, 500, 5000, 360, 5000, 5, FIXED_METERS);
	fullFixedDensityTest(0.005, 2500, 500, 2500, 360, 5000, 5, FIXED_METERS);
	fullFixedDensityTest(0.01, 5000, 500, 1000, 360, 5000, 5, FIXED_METERS);

	fullFixedDensityTest(0.001, 500, 1000, 1000, 360, 5000, 5, FIXED_POLES);
	fullFixedDensityTest(0.005, 500, 2500, 1000, 360, 5000, 5, FIXED_POLES);
	fullFixedDensityTest(0.01, 500, 5000, 1000, 360, 5000, 5, FIXED_POLES);
}
void SolutionTests()
{
	int hops = 0;
	int maxHops = 3;
	string metersFile = "", polesFile = "";
	metersFile = rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt";
	polesFile = rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt";
	executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 50, 1, 0);
	return;
	for (hops; hops <= maxHops; hops++)
	{
		cout << "HOPS: " << hops;
		for (int i = 1; i < 4; i++)
		{
			metersFile = rubyPath + "/Instances/NikitiMeters3666.txt";
			polesFile = rubyPath + "/Instances/NikitiPoles1030.txt";
		//	executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.9);
		//	executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);

			//metersFile = rubyPath + "/Instances/GridInstanceSuburbanoMeters3200.txt";
			//polesFile = rubyPath + "/Instances/GridInstanceSuburbanoPoles4920.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.9);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);

			metersFile = rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt";
			polesFile = rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.8);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);

			metersFile = rubyPath + "/Instances/FloripaMetersCompleto29002.txt";
			polesFile = rubyPath + "/Instances/FloripaPolesCompleto12140.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.8);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);
		}
	}
}
void HopVariationTest(string metersFile, string polesFile, int scenario, int tech, int startHop)
{
	int currentSol = 0;
	while (true)
	{
		cout << "HOPS: " << startHop;
		string result = executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", scenario, tech, 6, 20, 3, 5, 1, startHop, 1, 0);
		int val = -1;
		string aux = "";
		int pos = result.find("Solution Quality: ");
		aux = result.substr(pos);
		aux = aux.substr(aux.find(":")+2);
		aux = aux.substr(0, aux.find("\n"));
		val = stoi(aux);
		
		int pos2 = result.find("P.O Solution Quality: ");
		if (pos2 != -1)
		{
			aux = result.substr(pos2);
			aux = aux.substr(aux.find(":") + 2);
			aux = aux.substr(0, aux.find("\n"));
			val = stoi(aux);
		}
		if (val == currentSol)
			break;
		currentSol = val;		
		startHop++;

	}
	
}
void HopVariationTestMaxHops(string metersFile, string polesFile, int scenario, int tech, int startHop, int maxHops)
{
	//int currentSol = 0;
	while (startHop <= maxHops)
	{
		cout << "HOPS: " << startHop;
		string result = executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", scenario, tech, 6, 20, 3, 5, 1, startHop, 1, 0);
		int val = -1;
		string aux = "";
		int pos = result.find("Solution Quality: ");
		aux = result.substr(pos);
		aux = aux.substr(aux.find(":") + 2);
		aux = aux.substr(0, aux.find("\n"));
		val = stoi(aux);

		int pos2 = result.find("P.O Solution Quality: ");
		if (pos2 != -1)
		{
			aux = result.substr(pos2);
			aux = aux.substr(aux.find(":") + 2);
			aux = aux.substr(0, aux.find("\n"));
			val = stoi(aux);
		}

		startHop++;

	}

}
void RedundancyVariationTestmaxRedundancy(string metersFile, string polesFile, int scenario, int tech, int startRedundancy, int maxRedundancy)
{
	//int currentSol = 0;
	while (startRedundancy <= maxRedundancy)
	{
		cout << "Redundancy: " << startRedundancy;
		string result = executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", scenario, tech, 6, 20, 3, 5, 1, 0, startRedundancy, 0);
		int val = -1;
		string aux = "";
		int pos = result.find("Solution Quality: ");
		aux = result.substr(pos);
		aux = aux.substr(aux.find(":") + 2);
		aux = aux.substr(0, aux.find("\n"));
		val = stoi(aux);

		int pos2 = result.find("P.O Solution Quality: ");
		if (pos2 != -1)
		{
			aux = result.substr(pos2);
			aux = aux.substr(aux.find(":") + 2);
			aux = aux.substr(0, aux.find("\n"));
			val = stoi(aux);
		}

		startRedundancy++;

	}

}

double GetSubInstancesSolvingTime(string path)
{

	string line;
	ifstream myfile(path.c_str());
	double ret = 0;
	if (myfile.is_open())
	{
		double solving_time = 0;
		while (getline(myfile, line))
		{
			if (!line.find("Tempo utilizado: "))
			{
				int pos = line.find(":");
				string str = line.substr(pos + 1);
				ret += stod(str);
				//cout << pos;
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return ret;
}
double GetSolutionQualityFromFile(string path)
{
	string line;
	ifstream myfile(path.c_str());
	double ret = 0;
	if (myfile.is_open())
	{
		double solving_time = 0;
		while (getline(myfile, line))
		{
			if (!line.find("Solution Quality: ") || !line.find("P.O Solution Quality: "))
			{
				int pos = line.find(":");
				string str = line.substr(pos + 1);
				ret = stod(str);
				//cout << pos;
			}

		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return ret;
}
double GetExecutionTimeFromFile(string path)
{
	string line;
	ifstream myfile(path.c_str());
	double ret = 0;
	if (myfile.is_open())
	{
		double solving_time = 0;
		while (getline(myfile, line))
		{
			if (!line.find("Time: ") || !line.find("P.O Time : "))
			{
				int pos = line.find(":");
				string str = line.substr(pos + 1);
				ret = stod(str);
				//cout << pos;
			}

		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return ret;
}
double GetFromFile(string att, string path)
{
	string line;
	ifstream myfile(path.c_str());
	double ret = 0;
	if (myfile.is_open())
	{
		double solving_time = 0;
		while (getline(myfile, line))
		{
			if (!line.find(att))
			{
				int pos = line.find(":");
				string str = line.substr(pos + 1);
				ret = stod(str);
				//cout << pos;
			}

		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return ret;
}
void HeuristicsComparison(string path, int meters_size, int poles_size, int sce, int tech)
{
	vector<vector<double>> SolResults, ExResults;
	for (int j = 1; j <= 3; j++)
	{
		vector<double> aux1, aux2;
		for (int i = 1; i <= 4; i++)
		{
			double GulosoExTime = GetExecutionTimeFromFile(path + "\\Guloso" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");
			double GulosoSolQty = GetSolutionQualityFromFile(path + "\\Guloso" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");

			double CellExTime = GetExecutionTimeFromFile(path + "\\Cell" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");
			double CellSolQty = GetSolutionQualityFromFile(path + "\\Cell" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");

			double ClusterExTime = GetExecutionTimeFromFile(path + "\\Cluster" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");
			double ClusterSolQty = GetSolutionQualityFromFile(path + "\\Cluster" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(i) + "Redundancy" + to_string(j) + "MemLim6000.000000.txt");


			aux1.push_back(GulosoSolQty); aux1.push_back(CellSolQty); aux1.push_back(ClusterSolQty);
			aux2.push_back(GulosoExTime); aux2.push_back(CellExTime); aux2.push_back(ClusterExTime);
		}
		SolResults.push_back(aux1);
		ExResults.push_back(aux2);
	}
	for (int i = 0; i < SolResults.size(); i++)
	{
		for (int j = 0; j < SolResults[i].size(); j++)
		{
			cout << SolResults[i][j] << " ";
			if ((j + 1) % 3 == 0)
				cout << "\t\t";
		}
		cout << "\n";
		for (int j = 0; j < SolResults[i].size(); j++)
		{
			cout << ExResults[i][j] << " ";
			if ((j + 1) % 3 == 0)
				cout << "\t";
		}
		cout << "\n\n";
	}
}
int TestMain(int argc, char** argv)
{

	srand(time(NULL));
	string path = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\testResults";
	//int meters_size = 3666, poles_size = 1030, sce = Suburbano, tech = t802_11_g ;
	//int meters_size = 29002, poles_size = 12140, sce = Urbano, tech = t802_11_g;
	//int meters_size = 8000, poles_size = 12200, sce = Urbano, tech = t802_11_g;
	int meters_size = 3200, poles_size = 4920, sce = Suburbano, tech = t802_11_g;
	string wow = "";
	for (int i = 1; i < 26; i++)
	{
		double exTime = GetSolutionQualityFromFile(path + "\\Cluster" + to_string(meters_size) + "Poles" + to_string(poles_size) + "S" + to_string(sce) + "T" + to_string(tech) + "Hops" + to_string(1) + "Redundancy" + to_string(i) + "MemLim6000.000000.txt");
		cout << i << ": " << exTime << "\n";
		wow += to_string(exTime) + "\n";
	}
	string filename = "C:\\Users\\Guilherme\\Desktop\\wow.txt";
	ofstream f(filename.c_str());

	f << wow;
	f.close();
	//cout << "wow";
	//SolutionTests();
	//HopVariationTest(rubyPath + "/Instances/NikitiMeters3666.txt", rubyPath + "/Instances/NikitiPoles1030.txt", Suburbano, t802_11_g, 4);
	//HopVariationTest(rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt", rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt", Urbano, t802_11_g, 4);
	//HopVariationTest(rubyPath + "/Instances/GridInstanceSuburbanoMeters3200.txt", rubyPath + "/Instances/GridInstanceSuburbanoPoles4920.txt", Suburbano, t802_11_g, 1);
	//HopVariationTestMaxHops(rubyPath + "/Instances/GridInstanceSuburbanoMeters3200.txt", rubyPath + "/Instances/GridInstanceSuburbanoPoles4920.txt", Suburbano, t802_11_g, 0,20);
	//HopVariationTest(rubyPath + "/Instances/FloripaMetersCompleto29002.txt", rubyPath + "/Instances/FloripaPolesCompleto12140.txt", Urbano, t802_11_g, 4);
	
	//RedundancyVariationTestmaxRedundancy(rubyPath + "/Instances/NikitiMeters3666.txt", rubyPath + "/Instances/NikitiPoles1030.txt", Suburbano, t802_11_g, 1, 20);
	//RedundancyVariationTestmaxRedundancy(rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt", rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt", Urbano, t802_11_g, 1, 20);
	//RedundancyVariationTestmaxRedundancy(rubyPath + "/Instances/GridInstanceSuburbanoMeters3200.txt", rubyPath + "/Instances/GridInstanceSuburbanoPoles4920.txt", Suburbano, t802_11_g, 1, 20);
	//RedundancyVariationTestmaxRedundancy(rubyPath + "/Instances/FloripaMetersCompleto29002.txt", rubyPath + "/Instances/FloripaPolesCompleto12140.txt", Urbano, t802_11_g, 1, 20);
	return 0;
	//MemoryTests();
	//return 0;
	//double estMem = memEstimation(29002, 12140);
	string metersFile = "", polesFile = "";
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";

	//metersFile = rubyPath + "/Instances/FloripaMetersCompleto29002.txt";
	//polesFile = rubyPath + "/Instances/FloripaPolesCompleto12140.txt";
	//metersFile = rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt";
	//polesFile = rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt";
	//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0, 1, 4);
	//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0, 1, 0);
	//return 0;
	//metersFile = rubyPath + "/Instances/NikitiMeters3666.txt";
	//polesFile = rubyPath + "/Instances/NikitiPoles1030.txt";
	//VaryMemLimTest(10, 10, 700, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResultsMemLimVariation/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3, 1);
	//return 0;
	//metersFile = rubyPath + "/Instances/FloripaMeters1000.txt";
	//polesFile = rubyPath + "/Instances/FloripaPoles1000.txt";
	//for (int i = 3; i < 4; i++)
	//{
	//	cout << "\nINICIANDO TESTE MEM_LIM HOP: " + i;
	//	VaryMemLimTest(10, 10, 700, rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResultsMemLimVariation/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, i, 1);
	//}
	//return 0;
	//metersFile = rubyPath + "/Instances/FloripaMeters15000.txt";
	//polesFile = rubyPath + "/Instances/FloripaPoles15000.txt";

	//for (int i = 0; i < 4; i++)
	//{
	//	cout << "\nINICIANDO TESTE MEM_LIM HOP: " + i;
	//	VaryMemLimTest(100, 100, 6000, rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResultsMemLimVariation/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3, 1);
	//}
	//return 0;

	
	int hops = 0;
	int maxHops = 3;
	for (hops; hops <= maxHops; hops++)
	{
		cout << "HOPS: " << hops;
		for (int i = 1; i < 4; i++)
		{
			//metersFile = rubyPath + "/Instances/NikitiMeters3666.txt";
			//polesFile = rubyPath + "/Instances/NikitiPoles1030.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.9);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);
			
			//metersFile = rubyPath + "/Instances/GridInstanceSuburbanoMeters3200.txt";
			//polesFile = rubyPath + "/Instances/GridInstanceSuburbanoPoles4920.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.9);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);

			metersFile = rubyPath + "/Instances/GridInstanceUrbanoMeters8000.txt";
			polesFile = rubyPath + "/Instances/GridInstanceUrbanoPoles12200.txt";
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.8);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);

			metersFile = rubyPath + "/Instances/FloripaMetersCompleto29002.txt";
			polesFile = rubyPath + "/Instances/FloripaPolesCompleto12140.txt";
			executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 0);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 1);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 2, 1000, 0.8);
			//executePlanningTest(rubyPath, metersFile.c_str(), polesFile.c_str(), rubyPath + "/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, hops, i, 3);
		}
	}
	
	return 0;

	//fullDensityTest(100, 100, rubyPath, 500, 5, false);
	//fullDensityTest(150, 150, rubyPath, 500, 5, false);
	//fullDensityTest(125, 125, rubyPath, 500, 5, false);

	//fullFixedDensityTest(0.001, 500,500, 500, 360, 5000, 5, rubyPath,NO_FIX);
	//fullFixedDensityTest(0.005, 500, 500, 500, 360, 5000, 5, rubyPath, NO_FIX);
	//fullFixedDensityTest(0.01, 500, 500, 500, 360, 5000, 5, rubyPath, NO_FIX);

	//fullFixedDensityTest(0.001, 1000, 500, 5000, 360, 5000, 5, rubyPath, FIXED_METERS);
	//fullFixedDensityTest(0.005, 2500, 500, 2500, 360, 5000, 5, rubyPath, FIXED_METERS);
	//fullFixedDensityTest(0.01, 5000, 500, 1000, 360, 5000, 5, rubyPath, FIXED_METERS);
	//
	//fullFixedDensityTest(0.001, 500, 1000, 1000, 360, 5000, 5, rubyPath, FIXED_POLES);
	//fullFixedDensityTest(0.005, 500, 2500, 1000, 360, 5000, 5, rubyPath, FIXED_POLES);
	//fullFixedDensityTest(0.01, 500, 5000, 1000, 360, 5000, 5, rubyPath, FIXED_POLES);


	//memoryEstimationTest(1, 5500, 500, rubyPath);
	int ini = 0;
	double sum = 0;
	double maxGap=-1, minGap=-1, maxGapVal=-1, minGapVal=-1;
	double stdDeviation;
	int repetitions = 100;
	while (ini < repetitions)
	{
		cout << "---------------------/////" << ini << "/////---------------------\n";
		int max = 5000, min = 500;
		double val1 = (rand() % (max - min)) + min, val2 = (rand() % (max - min));
		double realmem = memoryTest(val1, val2);
		double estMem = memEstimation(val1, val2);

		double gapValDiff = abs(realmem - estMem);
		if (maxGapVal == -1 || gapValDiff > maxGapVal)
			maxGapVal = gapValDiff;
		if (minGapVal == -1 || gapValDiff < minGapVal)
			minGapVal = gapValDiff;
		double gapDiff = abs(1 - (realmem / estMem));
		if (maxGap == -1 || gapDiff > maxGap)
			maxGap = gapDiff;
		if (minGap == -1 || gapDiff < minGap)
			minGap = gapDiff;

		cout << realmem << " " << estMem << "\n";
		cout << abs(1-(realmem / estMem));
		sum += abs(1-(realmem / estMem));
		ini++;
	}
	double avg = sum / ini;
	cout << "\navg:" << avg;
	cout << "\nmaxGapVal:" << maxGapVal;
	cout << "\nminGapVal:" << minGapVal;
	cout << "\nmaxGap:" << maxGap;
	cout << "\nminGap:" << minGap;
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Suburbano, t802_11_g, 0, rubyPath);
	//double estMem = memEstimation(fixedMeters, i);
	//memoryEstimationTestWithFixedMeters(500, 3000, 3000, 5000, rubyPath);
	//memoryEstimationTestWithFixedMeters(1000, 1, 35001, 5000, rubyPath);
	//memoryEstimationTestWithFixedMeters(2000, 1, 15001, 5000, rubyPath);
	//memoryEstimationTestWithFixedMeters(3000, 1, 10001, 5000, rubyPath);

//	saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Nikiti", "metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Suburbano, t802_11_g, 3, rubyPath);
//	saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Floripa", "filemeters15000.txt", "filepoles15000.txt", Urbano, t802_11_g, 3, rubyPath);
//	saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Urbano, t802_11_g, 3, rubyPath);
//	saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Rural, t802_11_g, 0, rubyPath);
//	saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Rural, t802_11_g, 3, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Nikiti", "metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Rural, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Floripa", "filemeters15000.txt", "filepoles15000.txt", Rural, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Rural, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Nikiti", "metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Urbano, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Floripa", "filemeters15000.txt", "filepoles15000.txt", Urbano, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Urbano, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Nikiti", "metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Suburbano, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\Floripa", "filemeters15000.txt", "filepoles15000.txt", Suburbano, t802_11_g, 0, rubyPath);
	//saveResults("C:\\Users\\Guilherme\\SkyDrive\\PSCC\\Instancias\\UrbanGrid", "gridmeterurbanopscc.txt", "gridpoleurbanopscc.txt", Suburbano, t802_11_g, 0, rubyPath);

	//vector<vector<Position*> > teste = gridInstanceGenerator(0, 0, 10 + distFromHouseToStreet, Urbano, 10, 100 - distFromHouseToStreet, 6, 6);
	//string ret = "";
	//for(int i = 0; i < teste[0].size();i++)
	//{
	//	ret += to_string(teste[0][i]->latitude) + " " + to_string(teste[0][i]->longitude) + "\n";
	//}
	//ofstream f("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\gridmeterurbanopscc.txt");
	//f << ret;
	//f.close();
	//ret = "";
	//for (int i = 0; i < teste[1].size(); i++)
	//{
	//	ret += to_string(teste[1][i]->latitude) + " " + to_string(teste[1][i]->longitude) + "\n";
	//}
	//ofstream f2("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\gridpoleurbanopscc.txt");
	//f2 << ret;
	//f2.close();
	//memoryEstimationTest(1000, 3000, 500, rubyPath);

	//fullDensityTest(130, 130, rubyPath, 500, 1,true);
	//fullDensityTest(150,150, rubyPath, 360,5);
	//fullDensityTest(50, 150, rubyPath, 360, 5);
	//fullDensityTest(150, 50, rubyPath, 360, 5);
	//varyDensityTest(100, 100, rubyPath, "Incremental2", 360, 0, +0.01);
	//varyDensityTest(100, 100, rubyPath, "Incremental3", 360, 0, +0.01);
	//varyDensityTest(100, 100, rubyPath, "Incremental4", 360, 0, +0.01);
	//varyDensityTest(100, 100, rubyPath, "Incremental5", 360, 0, +0.01);
	return 0;
	//increaseDensityTest(1000,1000, rubyPath, "TesteGap", 60);
	//increaseDensityTest(3000, 3000, rubyPath, "Incremental2", 360);
	//increaseDensityTest(3000, 3000, rubyPath, "Incremental3", 360);
	//increaseDensityTest(3000, 3000, rubyPath, "Incremental4", 360);
	//increaseDensityTest(3000, 3000, rubyPath, "Incremental5", 360);
	//increaseDensityTest(3000, 3000, rubyPath, "1");
	//increaseDensityTest(3000, 3000, rubyPath, "2");
	//increaseDensityTest(3000, 3000, rubyPath, "3");
	//increaseDensityTest(3000, 3000, rubyPath, "4");
	//increaseDensityTest(3000, 3000, rubyPath, "5");
	//increaseDensityTest(10000, 500, rubyPath, "1");
	//increaseDensityTest(10000, 500, rubyPath, "2");
	//increaseDensityTest(10000, 500, rubyPath, "3");
	//increaseDensityTest(10000, 500, rubyPath, "4");
	//increaseDensityTest(10000, 500, rubyPath, "5");
	//increaseDensityTest(2000, 4000, rubyPath, "1");
	//increaseDensityTest(2000, 4000, rubyPath, "2");
	//increaseDensityTest(2000, 4000, rubyPath, "3");
	//increaseDensityTest(2000, 4000, rubyPath, "4");
	//increaseDensityTest(2000, 4000, rubyPath, "5");

	//return 0;
	//vector<vector<int> > scp = SCPGenerator(300, 300,0.10);
	//saveGLPKFile(scp, 300, 300, 1, "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails");
	
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(368619);
	{

	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/instanciagridmetersSuburbano.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/instanciagridpolesSuburbano.txt";

	//	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filemeters5000.txt";
	//	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filepoles5000.txt";
	//increaseRangeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0,rubyPath);
	//
	//float aux = 0;
	//string v = "";
	//float mem=1;
	//int i = 1, init = 100;
	//while (mem > 0)
	//{	
	//	mem = memoryTest(i,i, rubyPath);
	//	if (mem<0)
	//		break;
	//	cout << i << "x" << i << ": " << mem<< "\n";
	//	v += to_string(i) + " " + to_string(mem) +"\n";
	//	aux = mem;
	//	if (i < 1001)
	//		i += 10;
	//	else 
	//		i += 500;

	//}
	}


	return 0;

}