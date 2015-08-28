#include "TestMethods.h"


#define distFromHouseToStreet 40
#define distToRandom 20
//string gridTest(vector<Position*> &meters, vector<Position*> &poles, int s, int t, double B, double T, double h1, double h2, int srd, int me, string rp, double gridSize)
//{
//	AutoPlanning* ap = new AutoPlanning(meters, poles, s, t, B, T, h1, h2, srd, me, rp);
//	string gresult = "";
//	string ret = ap->executeAutoPlanTestMode(&gresult, gridSize);
//	vector<string> xgp = split(ret, ' ');
//	vector<Position*> daps;
//	for (int i = 0; i < xgp.size(); i++)
//	{
//		string snum = xgp[i].substr(1);
//		Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude, poles[atoi(snum.c_str()) - 1]->longitude, poles[atoi(snum.c_str()) - 1]->index);;
//		daps.push_back(dapToInsert);
//	}
//	MetricCalculation* mc = new MetricCalculation(meters, daps, s, t, B, T, h1, h2, srd, me, rp);
//	string metricResult = mc->executeMetricCalculation();
//	//delete mc;
//	//delete ap;
//	for (int i = 0; i < daps.size(); i++)
//	{
//		delete daps[i];
//	}
//	//cout << gresult + metricResult;
//	return "\n" + gresult + metricResult + "\n";
//}
double generateRandom()
{
	return rand() % (2 * distToRandom / 2 + 1) - distToRandom / 2;
}
double memoryTest(int x, int y, string rubyPath)
{
	
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\nparam B{r in Z} default " + to_string(1) + ", integer;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=B[r];\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "/Results.txt\";\n data;\n";
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

	string filename = rubyPath + "/GlpkFile.txt";
	ofstream f(filename.c_str());
	f << resp;
	f.close();
	//string access = rubyPath + "/glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800  > " + rubyPath + "/wow.txt";
	//system(access.c_str());

	glp_prob *lp;
	glp_tran *tran;
	int err;
	int ret, count, cpeak;
	size_t total, tpeak;
	double totalInMb, tpeakInMb;
	glp_term_out(GLP_OFF);
	lp = glp_create_prob();
	tran = glp_mpl_alloc_wksp();
	ret = glp_mpl_read_model(tran, filename.c_str(), 0);

	glp_mem_limit(5800);
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

	//ifstream fi(rubyPath + "/wow.txt");
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
void testGraspFromFile(string metersFile, string polesFile, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, int SRD, int meshEnabled, string rubyPath)
{
	string completeMetersFile = rubyPath + "/arqsTeste/" + metersFile;
	string completePolesFile = rubyPath + "/arqsTeste/" + polesFile;
	FILE * file;
	file = fopen(completeMetersFile.c_str(), "r");
	FILE * file2;
	file2 = fopen(completePolesFile.c_str(), "r");


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

	string finalResult = "";
	AutoPlanning* ap = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);

	//string gresult = "";
	string ret = ap->graspAutoPlanning();
	//string gresult = "";
	//string ret = ap->gridAutoPlanning();
	for (int i = 0; i < meters.size(); i++)
	{
		delete meters[i];
	}
	for (int i = 0; i < poles.size(); i++)
	{
		delete poles[i];
	}
	delete ap;
}
AutoPlanning* setAutoPlanningFromFile(string metersFilePath, string polesFilePath, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, int SRD, int meshEnabled, string rubyPath)
{

	//string completeMetersFile = rubyPath + "/arqsTeste/" + metersFile;
	//string completePolesFile = rubyPath + "/arqsTeste/" + polesFile;
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


	AutoPlanning* res = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
	return res;


}
string gridPlanningTest(AutoPlanning *AP,int gridSize, int usePostOptimization, int redundancy)
{
	AP->setGridSize(gridSize);
	
	cout << "\nIniciando teste com célula de tamanho " << to_string(gridSize) << " com " << to_string(AP->getMetersSize()) << " medidores e " << to_string(AP->getPolesSize()) << " postes\n";
	TestResult* ret = AP->executeAutoPlanTestMode(usePostOptimization,redundancy);
	return "";
}
void executeTest(string meterFile, string poleFile, string pathToSave,int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(),poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD,  mesh, rubyPath);

//	AP->saveGLPKFileReducedWithLimit(scp,1, 5);

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
	result += gridPlanningTest(AP, 10000, false, 1);
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
void executeClusterTest(string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD, mesh, rubyPath);

	//	AP->saveGLPKFileReducedWithLimit(scp,1, 5);

	string result = "";

	string fileToSave = pathToSave + "ClusterPlanningMeters" + to_string(AP->getMetersSize()) + "Poles" + to_string(AP->getPolesSize()) + "S" + to_string(scenario) + "T" + to_string(tech) + "Hops" + to_string(mesh + 1) + ".txt";
	ofstream f(fileToSave.c_str());
	TestResult* ret = AP->executeClusterAutoPlanTestMode(true, 1);
	result = ret->toString();
	delete(ret);

	f << result;
	f.close();

	delete AP;
}
void kmeansTest(string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";

	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD, mesh, rubyPath);
	AP->executeClusterAutoPlanTestMode(true, 1);
}
void executionTimeTest(int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh, string rubyPath)
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
		AutoPlanning* AP = new AutoPlanning(meters,poles, Suburbano, tech, bitrate, power, hx, rx, SRD, 3, rubyPath);
		TestResult *res = AP->clusterAutoPlanning(false, 1);
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
	for (int i = 0; i < pSize; i++)
	{
		vector<int> add;
		ret.push_back(add);
	}
	while (covNum)
	{
		vector<int> options;
		for (int j = 0; j < mSize; j++)
		{
			if (find(ret[i%pSize].begin(), ret[i%pSize].end(), j) == ret[i%pSize].end())
				options.push_back(j);
		}
		int x = rand() % options.size();
		ret[i%pSize].push_back(options[x]);
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
	for (int i = 0; i < SCP.size(); i++)
	{
		covNum += SCP[i].size();
	}
	int newCovNum = ((mSize*SCP.size())*newDensity) - covNum;
	int i = covNum % (SCP.size());

	cout << i;
	while (newCovNum > 0)
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
	while (newCovNum < 0)
	{
		SCP[i%SCP.size()].pop_back();
		i--;
		if (i < 0)
			i = SCP.size();
		newCovNum++;
	}

}
void increaseRangeTest(string meterFile, string poleFile, string pathToSave, int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh,string rubyPath)
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
	AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Suburbano, t802_11_g, bitrate, 20, hx, rx, SRD, mesh, rubyPath);
	ret = AP->executeClusterAutoPlanTestMode(false, 1);
	//toSave += to_string(hataTestRange) + " " + to_string(ret->solverTime) + "\n";
	//cout << to_string(hataTestRange) << " " << to_string(ret->solverTime) << " " << to_string(ret->solutionQuality) << " " << to_string(ret->uncoveredMeters) << " " << to_string(ret->maxMem) << "\n";
	cout << ret->toString();
	delete ret;
	delete AP;
	//Terceiro teste: Maior alcance considerado
	 AP = setAutoPlanningFromFile(meterFile.c_str(), poleFile.c_str(), Rural, t802_11_g, bitrate, 22, hx, rx, SRD, mesh, rubyPath);
	ret = AP->executeClusterAutoPlanTestMode(false, 1);
	//toSave += to_string(hataTestRange) + " " + to_string(ret->solverTime) + "\n";
	//cout << to_string(hataTestRange) << " " << to_string(ret->solverTime) << " " << to_string(ret->solutionQuality) << " " << to_string(ret->uncoveredMeters) << " " << to_string(ret->maxMem) << "\n";
	cout << ret->toString();
	delete ret;
	delete AP;

	string filename = rubyPath + "/arqsTeste/RangeTest.txt";
	ofstream f(filename.c_str());
	f << toSave;
	f.close();
}
void saveGLPKFileReduced(vector<vector<int> > &SCP, int mSize, int pSize, int redundancy, string rubyPath)
{
	//vector<int> uncMeters = uncoverableMeters(SCP, redundancy);//Só consideramos os medidores que são cobríveis(existe essa palavra?)
	//Pois se não, o método retornaria que a solução é impossível!
	//A formatação você ve no arquivo GlpkFile.txt

	
	string resp;
	resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\nparam B{r in Z} default " + to_string(redundancy) + ", integer;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=B[r];\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"" + rubyPath + "/Results.txt\";\n data;\n";
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

	string filename = rubyPath + "/GlpkFile.txt";
	ofstream f(filename.c_str());

	f << resp;
	f.close();
}
void get_mip_gap(glp_tree *T, void *info)
{
	double gap = glp_ios_mip_gap(T);
	*(double*)info = gap;
}
void increaseDensityTest(int mSize, int pSize, string rubyPath, string id, int timeLimit)
{
	string filename = rubyPath + "/GlpkFile.txt";
	string fileOutput = rubyPath +"/density_tests/DensityResult"+to_string(mSize)+"x"+to_string(pSize)+"-"+id+".txt";
	
	double density = 0.005;
	ofstream f(fileOutput.c_str());
	vector<vector<int> > scp = SCPGenerator(mSize, pSize, density);
	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (density <= 0.1)
	{
		double solverTime = -1, maxMem = -1;
		
		saveGLPKFileReduced(scp, mSize, pSize, 1, rubyPath);
		//string access = rubyPath + "/glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"/wow.txt";
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
		parm.cb_func = get_mip_gap;
		double gap = 0;
		parm.cb_info = &gap;
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
void varyDensityTest(int mSize, int pSize, string rubyPath, string id, int timeLimit, double initDensity, double rate)
{
	string filename = rubyPath + "/GlpkFile.txt";
	string fileOutput = rubyPath + "/density_tests/DensityResult" + to_string(mSize) + "x" + to_string(pSize) + "-" + id + ".txt";

	//double density = 0.0005;
	ofstream f(fileOutput.c_str());
	vector<vector<int> > scp = SCPGenerator(mSize, pSize, initDensity);
	//vector<vector<int> > scp = geographicSCPGenerator(mSize, pSize, density, 7);
	while (true)
	{
		double solverTime = -1, maxMem = -1;

		saveGLPKFileReduced(scp, mSize, pSize, 1, rubyPath);
		//string access = rubyPath + "/glpk-4.54/w64/glpsol.exe  --math " + filename + " --memlim 5800 > " + rubyPath +"/wow.txt";
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
		parm.cb_func = get_mip_gap;
		void* gap = NULL;
		parm.cb_info = gap;
		parm.presolve = GLP_ON;
		parm.tm_lim = timeLimit * 1000; //TEMPO LIMITE DE 60 SEGUNDOS

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


		f << to_string(initDensity) << " " << solverTime << " " << maxMem << "\n";
		initDensity += rate;
		varySCPDensity(scp, mSize, initDensity);
		if (solverTime >= timeLimit || initDensity > 1 || initDensity < 0)
			break;
	}
	f.close();
}
int main(int argc, char** argv)
{
	srand(time(NULL));
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";

<<<<<<< HEAD
	varyDensityTest(100,100, rubyPath, "Incremental1", 360, 0,+0.01);
=======
	increaseDensityTest(1000,1000, rubyPath, "TesteGap", 60);
>>>>>>> origin/master
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
	//saveGLPKFileReduced(scp, 300, 300, 1, "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails");
	string metersFile = "", polesFile = "";
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(368619);
	{

	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/instanciagridmetersSuburbano.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/instanciagridpolesSuburbano.txt";

		metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filemeters5000.txt";
		polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filepoles5000.txt";
	increaseRangeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0,rubyPath);
	
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
	//string filename = rubyPath + "/arqsTeste/ix"+to_string(i)+"MemTest.txt";
	//ofstream f(filename.c_str());
	//f << v;
	//f.close();


	//srand(time(NULL));
	//double x = 3501;
	//double wow = -1E-19;

	////for (int x = 1; x < 10002; x+=500)
	//	//cout << x << " : " << 5.77032E-13*pow(x, 4) + 8.52842E-09*pow(x, 3) + 0.000109763 * pow(x, 2) + 0.092964718*x - 62.48144596 << "\n";
	//	//cout << x << " : " << 0.0001530206*pow(x,2) + 0.0072606821*x - 10.7817256808 << "\n";
	////string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	//for (int i = 0; i < 100; i++)
	//{
	//	int max = 5000, min = 1;
	//	double val1 = (rand() % (max - min)) + min, val2 = (rand() % (max - min));
	//	double realmem = memoryTest(val1, val2, rubyPath);
	//	double estMem = memEstimation(val1, val2);
	//	cout << "Dimension: " << val1 << "x" << val2<< " Real: " << realmem << " Estimation: " << estMem << " Error: " << realmem / estMem << "\n";
	//}

	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters1000.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles1000.txt";
	//executeTest(metersFile.c_str(), polesFile.c_str() , "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters5000.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles5000.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters10000.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles10000.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters15000.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles15000.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters9999999.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles9999999.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters10000.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles10000.txt";
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters15000.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles15000.txt";
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filemeters9999999.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/filepoles9999999.txt";
	//kmeansTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3);

	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/metersInstanciaMédia3666.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/polesInstanciaMédia1030.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3);

	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/metersInstanciaPequena1576.txt";
	polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/polesInstanciaPequena453.txt";
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	executeClusterTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0);
	//
	//metersFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/instanciagridmetersSuburbano.txt";
	//polesFile = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/arqsTeste/instanciagridpolesSuburbano.txt";
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 3);
	//executeTest(metersFile.c_str(), polesFile.c_str(), "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/testResults/", Suburbano, t802_11_g, 6, 20, 3, 5, 1, 0);


	//vector<vector<Position*> > teste = gridInstanceGenerator(0, 0, 10 + distFromHouseToStreet, Suburbano, 20, 100 - distFromHouseToStreet, 4, 3);
	//string ret = "";
	//for(int i = 0; i < teste[0].size();i++)
	//{
	//	ret += to_string(teste[0][i]->latitude) + " " + to_string(teste[0][i]->longitude) + "\n";
	//}
	//ofstream f("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\instanciagridmetersSuburbano.txt");
	//f << ret;
	//f.close();
	//ret = "";
	//for (int i = 0; i < teste[1].size(); i++)
	//{
	//	ret += to_string(teste[1][i]->latitude) + " " + to_string(teste[1][i]->longitude) + "\n";
	//}
	//ofstream f2("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\instanciagridpolesSuburbano.txt");
	//f2 << ret;
	//f2.close();
	//string rubyPath = "C:/Sites/first_app";
	//testFromFile("filemeters1000.txt", "filepoles1000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters2000.txt", "filepoles2000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters3000.txt", "filepoles3000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters4000.txt", "filepoles4000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters5000.txt", "filepoles5000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters10000.txt", "filepoles10000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters15000.txt", "filepoles15000.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 3, rubyPath);
	//testFromFile("filemeters9999999.txt", "filepoles9999999.txt", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3, rubyPath);
	//testGraspFromFile("filemeters9999999.txt", "filepoles9999999.txt", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3, rubyPath);
	//	testFromFile("metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 0, rubyPath)//;
	//testGraspFromFile("metersInstanciaPequena1576.txt", "polesInstanciaPequena453.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 0, rubyPath);
	//testGraspFromFile("metersInstanciaMédia3666.txt", "polesInstanciaMédia1030.txt", Urbano, t802_11_g, 6,  20, 3,  5, 1, 0, rubyPath);
	//	vector<Position*> teste;
	//	Grid *g = new Grid(teste,10);

	//	double val = getHataSRDSuccessRate(5, Rural, t802_11_g, 6, 20, 3, 5, 1);

	//	string x = AutoPlanning::executeAutoPlan();
	//	std::cout << val;
	}

	//_CrtDumpMemoryLeaks();
	return 0;

}