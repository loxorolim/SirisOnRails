#ifndef _METRIC_CALCULATION_METHODS_H
#define _METRIC_CALCULATION_METHODS_H

#include "FatherMethods.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"

struct MetricResult
{
	int numOfDaps, numOfMeters, uncoveredMeters;
	vector<double > linkQualityPerHop;
	vector<int> meterPerHop;
	vector<double> minMedMaxMetersPerDap;
	vector<double> minMedMaxRedundancyPerMeter;
	vector<double > linkDelayPerHop;
};

class sComponent
{
	public:
		int index;
		double distance;
		double efficiency;
		double delay;
		int hop;
		sComponent *meshFather;
		sComponent(int i, double d, double e, double dl, int h, sComponent * mf)
		{
			index = i;
			distance = d;
			efficiency = e;
			delay = dl;
			hop = h;
			meshFather = mf;
		}
};

class MetricCalculation: public FatherMethods
{
	private:
		vector<Position*> meters; // alias para o vec1, meters é a mesma coisa que o vec1
		vector<Position*> daps; // alias para o vec2, daps é a mesma coisa que o vec2
	public:
		MetricCalculation(vector<Position*> &meters, vector<Position*> &daps, int scenario, int technology, double bit_rate, double t_power, double h_tx, double h_rx, int srd, int mesh,string rubyPath)
		{
			this->meters = meters;
			this->daps = daps;
			this->scenario = scenario;
			this->technology = technology;
			this->bit_rate = bit_rate;
			this->t_power = t_power;
			this->h_tx = h_tx;
			this->h_rx = h_rx;
			this->srd = srd;
			this->mesh = mesh;
			this->rubyPath = rubyPath;
			this->regionLimiter = getRegionLimiter();

		};
		~MetricCalculation()
		{
			for (int i = 0; i < meters.size(); i++)
			{
				delete meters[i];
			}
			for (int i = 0; i < daps.size(); i++)
			{
				delete daps[i];
			}
		}
		vector<sComponent*> statisticalList();
		vector<double> minMedMaxMetersPerDap(vector<vector<int> > cL);
		vector<double> minMedMaxRedundancyPerMeter(vector<vector<int> > cL);
		vector<int> meterPerHop(vector<sComponent*> sL);
		vector<double> linkQualityPerHop(vector<sComponent*> sL);
		vector<double> linkDelayPerHop(vector<sComponent*> sL);
		vector<vector<int> > createMeterNeighbourhood(Grid *g);
		sComponent* chooseDeviceToConnect(Position* meter, vector<Position*> &devices, vector<sComponent*> sC, int meshHop);
		string executeMetricCalculation();
		MetricResult* executeMetricCalculationTest();
		string getMetricCalculationString();
		vector<vector<int> > coverageList();
};

#endif

