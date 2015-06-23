#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "HeatgridMethods.h"
#include "HataSRD.h"
#include <stdio.h>

//#include "rice/Class.hpp"
///* run this program using the console pauser or add your own getch, system("pause") or input loop */
//
//using namespace Rice;
//


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
		string ret = res->executeAutoPlan(redundancy);
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

		MetricCalculation* res = new MetricCalculation(meters, daps, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD, meshEnabled,rubyPath);
		string ret = res->executeMetricCalculation();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;

	}
	if (option == HEATGRID)
	{
		int pLength;
		std::getline(f, line);
		pLength = std::atoi(line.c_str());
		if (pLength == 0) return "";
		vector<Position*> points;
		for (int i = 0; i < pLength; i++)
		{
			double lat;
			double lng;
			double weight;
			std::getline(f, line);
			vector<string> s = split(line, ' ');
			lat = std::atof(s[0].c_str());
			lng = std::atof(s[1].c_str());
			weight = std::atof(s[2].c_str());

			Position *toAdd = new Position(lat, lng, weight);
			points.push_back(toAdd);
		}
		Heatgrid *res = new Heatgrid(points);
		string ret = res->executeHeatgrid();
		//std::cout << ret;
		//res->~AutoPlanning();
		delete res;
		return ret;

	}
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

//extern "C"
//
//void Init_Siris()
//{
//  Class rb_c = define_class("Siris")
//   .define_method("getResponse", &getResponse);
//
//}

