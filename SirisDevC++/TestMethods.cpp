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
string gridPlanningTest(AutoPlanning *AP,int gridSize, bool usePostOptimization, int redundancy)
{
	AP->setGridSize(gridSize);
	string ret = AP->executeAutoPlanTestMode(usePostOptimization,redundancy);
	return ret;
}
void executeTest()
{
	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/SirisOnRails";
	AutoPlanning* AP = setAutoPlanningFromFile(rubyPath+"/arqsTeste/filemeters1000.txt", rubyPath+"/arqsTeste/filepoles1000.txt", Urbano, t802_11_g, 6, 20, 3, 5, 1, 3, rubyPath);
	vector<vector<int> > scp = AP->createScp();
	AP->saveGLPKFileReducedWithLimit(scp,1, 5);

	string result = "";
	//result+=gridPlanningTest(AP, 100, true);
	result+=gridPlanningTest(AP, 100, false,1);
	result += gridPlanningTest(AP, 100, false, 2);
	result += gridPlanningTest(AP, 100, false, 3);
	cout << result;


}

int main(int argc, char** argv)
{

	string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	executeTest();
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