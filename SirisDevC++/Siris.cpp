#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "HataSRD.h"
#include <stdio.h>

//#include "rice/Class.hpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//using namespace Rice;



string getResponse(string req, string rubyPath)
{
	std::cout.precision(20);
	std::istringstream f(req);
	std::string line;

	int option,scenario,technology, SRD, meshEnabled;
	double H_TX,H_RX,BIT_RATE,TRANSMITTER_POWER;
	std::getline(f, line);
	option = std::atoi(line.c_str());
	std::getline(f, line);
	scenario = std::atoi(line.c_str());
	std::getline(f, line);
	technology = std::atoi(line.c_str());
	std::getline(f, line);
	H_TX = std::atof(line.c_str());
	std::getline(f, line);
	H_RX = std::atof(line.c_str());
	std::getline(f, line);
	BIT_RATE = std::atof(line.c_str());
	std::getline(f, line);
	TRANSMITTER_POWER = std::atof(line.c_str());
	std::getline(f, line);
	SRD = std::atoi(line.c_str());
	std::getline(f, line);
	meshEnabled = std::atoi(line.c_str());

	/*std::cout << scenario << "\n";
	std::cout << technology << "\n";
	std::cout << H_TX << "\n";
	std::cout << H_RX << "\n";
	std::cout << BIT_RATE << "\n";
	std::cout << TRANSMITTER_POWER<< "\n";
	std::cout << SRD << "\n";
	std::cout << meshEnabled << "\n";
	*/

	if(option == AUTOPLAN)
	{
		int mLength;
		std::getline(f, line);
		mLength = std::atoi(line.c_str());
		if(mLength == 0) return "";
		vector<Position*> meters;
		vector<Position*> poles;
		for (int i = 0; i < mLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());

			Position *toAdd = new Position(lat, lng,i);
			printf("%f %f",lat,lng);
			meters.push_back(toAdd);
		}
		std::getline(f, line);
		int pLength;
		pLength = std::atoi(line.c_str());
		if(pLength == 0) return "";
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng,i);
			poles.push_back(toAdd);
		}

		AutoPlanning* res = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
		string ret = res->executeAutoPlan();
		//res->~AutoPlanning();
		cout << ret;
		delete res;
		return ret;

	}
	if(option == DRAW)
	{
		int pLength;
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		if(pLength == 0) return "";
		vector<Position*> meters;
		vector<Position*> daps;
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng,i);

			meters.push_back(toAdd);
		}
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		if(pLength == 0) return "";
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng,i);
			daps.push_back(toAdd);
		}

		LinkCalculation* res = new LinkCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
		string ret = res->executeLinkCalculation();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;
	}
	if(option == METRIC)
	{
		int pLength;
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		if(pLength == 0) return "Não há medidores!";
		vector<Position*> meters;
		vector<Position*> daps;
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng,i);

			meters.push_back(toAdd);
		}
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		if(pLength == 0) return "Não há agregadores!";
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng,i);
			daps.push_back(toAdd);
		}

		MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
		string ret = res->executeMetricCalculation();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;

	}

	//while (std::getline(f, line))
	//{
	//	std::cout << line << std::endl;
	//}

//	vector<Position*> teste;
//	Grid *g = new Grid(teste,10);



//	string x = AutoPlanning::executeAutoPlan();
	return "";
}
string gridTest(vector<Position*> &meters, vector<Position*> &poles, int s, int t, double B, double T,double h1, double h2, int srd, int me, string rp, double gridSize)
{
	AutoPlanning* ap = new AutoPlanning(meters, poles, s, t, B, T,h1, h2, srd, me,rp);
	string gresult = "";
	string ret = ap->executeAutoPlanTestMode(&gresult, gridSize);
	vector<string> xgp = split(ret, ' ');
	vector<Position*> daps;
	for (int i = 0; i < xgp.size(); i++)
	{
		string snum = xgp[i].substr(1);
		Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude,poles[atoi(snum.c_str()) - 1]->longitude,poles[atoi(snum.c_str()) - 1]->index);;
		daps.push_back(dapToInsert);
	}
	MetricCalculation* mc = new MetricCalculation(meters, daps, s, t, B, T,h1, h2, srd, me,rp);
	string metricResult = mc->executeMetricCalculation();
	//delete mc;
	//delete ap;
	for(int i = 0; i < daps.size();i++)
	{
		delete daps[i];
	}
	//cout << gresult + metricResult;
	return "\n" + gresult + metricResult + "\n";
}
void testFromFile(string metersFile, string polesFile, int scenario, int technology, double BIT_RATE,  double TRANSMITTER_POWER, double H_TX,  double H_RX, int SRD, int meshEnabled, string rubyPath)
{

		string completeMetersFile = rubyPath + "/arqsTeste/"+ metersFile;
		string completePolesFile = rubyPath + "/arqsTeste/"+ polesFile;
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

		//AutoPlanning* res = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);

		string finalResult = "";

		//cout << "Iniciando 0.001 \n";
		//finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.001);
		//cout << "Finalizando 0.001 \n";
//		cout << "Iniciando 0.01 \n";
//		finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.01);
//		cout << "Finalizando 0.01 \n";
		cout << "Iniciando 0.1 \n";
		finalResult += gridTest(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath, 0.1);
		cout << "Finalizando 0.1 \n";

		for(int i =0; i < meters.size(); i++)
		{
			delete meters[i];
		}
		for(int i =0; i < poles.size(); i++)
		{
			delete poles[i];
		}
//		string gresult = "";
//		string ret = res->executeAutoPlanTestMode(&gresult, 0.001);
//		vector<string> xgp = split(ret, ' ');
//		vector<Position*> daps;
//		for (int i = 0; i < xgp.size(); i++)
//		{
//			string snum = xgp[i].substr(1);
//			Position* dapToInsert = new Position(poles[atoi(snum.c_str()) - 1]->latitude,poles[atoi(snum.c_str()) - 1]->longitude,poles[atoi(snum.c_str()) - 1]->index);;
//			daps.push_back(dapToInsert);
//		}
//		MetricCalculation* res2 = new MetricCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
//		string ret2 = res2->executeMetricCalculation();

//		delete res;
//		delete res2;

		string resultsFilePath = rubyPath + "/testResults/Result" + metersFile ;
		ofstream f(resultsFilePath.c_str());
		f << finalResult;
		f.close();


}
string RubyPathTest(string t)
{
	return t;
}
int main(int argc, char** argv)
{
	//string rubyPath = "C:/Users/Guilherme/Documents/GitHub/SirisOnRails";
	string rubyPath = "C:/Sites/first_app";
	testFromFile("filemeters10000.txt", "filepoles10000.txt", Rural, t802_11_g, 6,  20, 3,  5, 1, 0, rubyPath);
//	vector<Position*> teste;
//	Grid *g = new Grid(teste,10);

//	double val = getHataSRDSuccessRate(5, Rural, t802_11_g, 6, 20, 3, 5, 1);

//	string x = AutoPlanning::executeAutoPlan();
//	std::cout << val;
	return 0;
}

//extern "C"
//
//void Init_Siris()
//{
//  Class rb_c = define_class("Siris")
//   .define_method("getResponse", &getResponse);
//
//}

