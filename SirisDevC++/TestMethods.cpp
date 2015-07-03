#include "TestMethods.h"

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

vector<vector<Position*> > generateBlock(double latStart, double lngStart, int scenario, int blockWidth, int streetWidth, int polesPerSide, int n, vector<bool> sidesToPutPoles )
{
	
	//consideramos blocos de 100m x 100m
	vector<Position*> ret;
	vector<Position*> polesRet;
	
	int dist = blockWidth/(n-1);
	double latStart2 = latStart + getLatOfDistance(blockWidth);
	double lngStart2 = lngStart + getLongOfDistance(latStart, blockWidth);
	double lngStart3 = lngStart + getLongOfDistance(latStart2, blockWidth);
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart, lngStart + getLongOfDistance(latStart, j*dist), 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart + getLatOfDistance(j*dist), lngStart2, 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart2, lngStart3 - getLongOfDistance(latStart2, j*dist), 0);
		ret.push_back(p);
	}
	for (int j = 0; j < n - 1; j++)
	{
		Position* p = new Position(latStart2 - getLatOfDistance(j*dist), lngStart, 0);
		ret.push_back(p);
	}
	//Depois de inserir os medidores, prepara pra inserir os postes!
	n = polesPerSide;
	latStart = latStart - getLatOfDistance(streetWidth/2);
	lngStart = lngStart - getLongOfDistance(latStart, streetWidth/2);
	blockWidth = blockWidth + streetWidth;
	dist = blockWidth / (polesPerSide - 1);
	latStart2 = latStart + getLatOfDistance(blockWidth);
	lngStart2 = lngStart + getLongOfDistance(latStart,blockWidth);
	lngStart3 = lngStart + getLongOfDistance(latStart2, blockWidth);
	if (sidesToPutPoles[0])
		for (int j = 0; j < n - 1; j++)
		{
			Position* p = new Position(latStart , lngStart + getLongOfDistance(latStart, j*dist), 0);
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

	//string finalResult = "";
	////1000x1000 m
	//finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath, 100);

	////cout << "Iniciando 0.001 \n";
	////finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.001);
	////cout << "Finalizando 0.001 \n";
	////cout << "Iniciando 0.005 \n";
	////finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.005);
	////cout << "Finalizando 0.005 \n";
	////cout << "Iniciando 0.01 \n";
	////finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.01);
	////cout << "Finalizando 0.01 \n";
	////cout << "Iniciando 0.05 \n";
	////finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.05);
	////cout << "Finalizando 0.05 \n";
	////cout << "Iniciando 0.1 \n";
	////finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.1);
	////cout << "Finalizando 0.1 \n";


	//for (int i = 0; i < meters.size(); i++)
	//{
	//	delete meters[i];
	//}
	//for (int i = 0; i < poles.size(); i++)
	//{
	//	delete poles[i];
	//}
	////		string gresult = "";
	////		string ret = res->executeAutoPlanTestMode(&gresult, 0.001);
	////		vector<string> xgp = split(ret, ' ');
	////		vector<Position*> daps;
	////		for (int i = 0; i < xgp.size(); i++)
	////		{
	////			string snum = xgp[i].substr(1);
	////			Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude,poles[atoi(snum.c_str()) - 1]->longitude,poles[atoi(snum.c_str()) - 1]->index);;
	////			daps.push_back(dapToInsert);
	////		}
	////		MetricCalculation* res2 = new MetricCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
	////		string ret2 = res2->executeMetricCalculation();

	////		delete res;
	////		delete res2;

	//string resultsFilePath = rubyPath + "/testResults/Result" + "S" + to_string(scenario) + "T" + to_string(technology) + "P" + to_string(TRANSMITTER_POWER) + "Hop" + to_string(meshEnabled + 1) + to_string(metersFile);
	//ofstream f(resultsFilePath.c_str());
	//f << finalResult;
	//f.close();
}
string gridPlanningTest(AutoPlanning *AP,int gridSize, int usePostOptimization, int redundancy)
{
	AP->setGridSize(gridSize);
	string ret = AP->executeAutoPlanTestMode(usePostOptimization,redundancy);
	return ret;
}
void executeTest(string meterFile, string poleFile, string pathToSave,int scenario, int tech, int bitrate, int power, int hx, int rx, int SRD, int mesh)
{
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	AutoPlanning* AP = setAutoPlanningFromFile(meterFile.c_str(),poleFile.c_str(), scenario, tech, bitrate, power, hx, rx, SRD,  mesh, rubyPath);
	//vector<vector<int> > scp = AP->createScp();
//	AP->saveGLPKFileReducedWithLimit(scp,1, 5);

	string result = "";

	string fileToSave = pathToSave + "S"+to_string(scenario)+"T"+to_string(tech)+"Hops"+to_string(mesh+1)+".txt";
	ofstream f(fileToSave.c_str());
	result += gridPlanningTest(AP, 100, 0, 1);
	result += gridPlanningTest(AP, 100, 2, 1);
	//result += gridPlanningTest(AP, 500, 0, 1);
	//result += gridPlanningTest(AP, 500, 2, 1);
	//result += gridPlanningTest(AP, 1000, 0, 1);
	//result += gridPlanningTest(AP, 1000, 2, 1);
	//result += gridPlanningTest(AP, 5000, 0, 1);
	//result += gridPlanningTest(AP, 5000, 2, 1);
	//result += gridPlanningTest(AP, 10000, 2, 1);
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

int main(int argc, char** argv)
{

	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	//executeTest("C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filemeters1000.txt", "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/arqsTeste/filepoles1000.txt", "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails/testResults/", Urbano, t802_11_g,6, 20, 3, 5, 1, 3);
	vector<vector<Position*> > teste = gridInstanceGenerator(0, 0, 10, Suburbano, 3, 100, 4,3);
	string ret = "";
	for(int i = 0; i < teste[0].size();i++)
	{
		ret += to_string(teste[0][i]->latitude) + " " + to_string(teste[0][i]->longitude) + "\n";
	}
	ofstream f("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\SirisOnRails\\instanciagridmeters.txt");
	f << ret;
	f.close();
	ret = "";
	for (int i = 0; i < teste[1].size(); i++)
	{
		ret += to_string(teste[1][i]->latitude) + " " + to_string(teste[1][i]->longitude) + "\n";
	}
	ofstream f2("C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\SirisOnRails\\instanciagridpoles.txt");
	f2 << ret;
	f2.close();
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
	return 0;
}