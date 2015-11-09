#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "KMLMethods.h"
#include "HataSRD.h"
#include <stdio.h>
////
//#include "rice/Class.hpp"
///* run this program using the console pauser or add your own getch, system("pause") or input loop */
//
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
	if (option != HEATGRID)
	{
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
	}

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
		int redundancy;
		std::getline(f, line);
		redundancy = std::atoi(line.c_str());
		int limit;
		std::getline(f, line);
		limit = std::atoi(line.c_str());
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
			//printf("%f %f",lat,lng);
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
		int gridSize = 500;
		AutoPlanning* res = new AutoPlanning(meters, poles, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,gridSize,rubyPath);
		string ret = res->executeAutoPlan(redundancy,limit);
		//res->~AutoPlanning();
		//cout << ret;
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

		LinkCalculation* res = new LinkCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
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

		MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
		string ret = res->executeMetricCalculation();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;

	}
	if (option == KML)
	{
		int pLength;
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		vector<Position*> meters, daps, poles, coverageArea;
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng, i);

			meters.push_back(toAdd);
		}
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng, i);
			daps.push_back(toAdd);
		}
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			Position *toAdd = new Position(lat, lng, i);
			poles.push_back(toAdd);
		}
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		for (int i = 0; i < pLength; i++)
		{
			double lat, lng, val;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			val = std::atof(s[2].c_str());
			vector<int> opIds;
			for (int j = 3; j < s.size(); j++)
			{
				opIds.push_back(std::atoi(s[j].c_str()));
			}
			Position *toAdd = new Position(lat, lng, i, opIds);
			coverageArea.push_back(toAdd);
		}

		KMLMethods* res = new KMLMethods(meters, daps, poles, coverageArea, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD, meshEnabled, rubyPath);
		string ret = res->generateKML();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;
	}
	//if (option == HEATGRID)
	//{
	//	int pLength;
	//	std::getline(f, line);
	//	pLength = std::atoi(line.c_str());
	//	if (pLength == 0) return "";
	//	vector<Position*> points;
	//	for (int i = 0; i < pLength; i++)
	//	{
	//		double lat;
	//		double lng;
	//		double weight;
	//		std::getline(f, line);
	//		vector<string> s = split(line, ' ');
	//		lat = std::atof(s[0].c_str());
	//		lng = std::atof(s[1].c_str());
	//		weight = std::atof(s[2].c_str());

	//		Position *toAdd = new Position(lat, lng, weight);
	//		points.push_back(toAdd);
	//	}
	//	Heatgrid *res = new Heatgrid(points);
	//	string ret = res->executeHeatgrid();
	//	//std::cout << ret;
	//	//res->~AutoPlanning();
	//	delete res;
	//	return ret;

	//}
	if (option == GET_RANGE)
	{
		double dapdist = 0, step = 0.1, effs = 1;
		while (effs > MARGIN_VALUE)
		{
			dapdist += step;
			effs = getHataSRDSuccessRate(dapdist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);	
		}
		effs = 1;
		double meterdist = 0;
		while (effs > MARGIN_VALUE)
		{
			meterdist += step;
			effs = getHataSRDSuccessRate(meterdist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD);
		}
		return to_string(dapdist - step) + "\n" + to_string(meterdist - step);
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
//int main(int argc, char** argv)
//{
//	string line, rP, input="";
//	getline(cin, rP);
//	while (getline(cin, line))
//	{
//		input += line+"\n";
//	}
//	//rP = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails";
//	//input = "0\n1\n2\n3\n5\n6\n20\n1\n2\n1\n-1\n8\n-20.301143436776947 -40.28205871582031\n-20.3012516073493 -40.28202384710312\n-20.301357262254005 -40.28199166059494\n-20.3014276988171 -40.28217673301697\n-20.301246576161564 -40.282251834869385\n-20.301279278878937 -40.28225988149643\n-20.301161045945054 -40.28227597475052\n-20.301319528367745 -40.28220355510712\n3\n-20.30116859273076 -40.28216063976288\n-20.30129437243846 -40.282104313373566\n-20.30141763645291 -40.28206139802933\n";
//	//rubyPath = rP;
//	string ret = getResponse(input,rP );
//	cout << ret;
//
//}



//
//extern "C"
//
//void Init_Siris()
//{
//  Class rb_c = define_class("Siris")
//   .define_method("getResponse", &getResponse);
//
//}
//
