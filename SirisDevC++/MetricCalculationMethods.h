#ifndef _METRIC_CALCULATION_METHODS_H
#define _METRIC_CALCULATION_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"


class sComponent
{
	public:
		int index;
		double distance;
		double efficiency;
		int hop;
		sComponent *meshFather;
		sComponent(int i, double d, double e, int h, sComponent * mf)
		{
			index = i;
			distance = d;
			efficiency = e;
			hop = h;
			meshFather = mf;
		}
};

class MetricCalculation
{
	private:
			vector<Position*> meters;
			vector<Position*> daps;
			int scenario, technology, SRD, meshEnabled;
			double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			double regionLimiter ;
			string rubyPath;
	public:
			MetricCalculation(vector<Position*> &m, vector<Position*> &d, int s, int t, double B, double T,double h1, double h2, int srd, int me, string rp)
		{
			meters = m;
			daps = d;
			scenario = s;
			technology = t;
			BIT_RATE = B;
			TRANSMITTER_POWER = T;
			H_TX = h1;
			H_RX = h2;
			SRD = srd;
			meshEnabled = me;
			rubyPath = rp;
			regionLimiter = 0.001;
		};
		~MetricCalculation()
		{
			for(int i = 0; i < meters.size();i++)
				delete meters[i];

			for(int i = 0; i <daps.size();i++)
				delete daps[i];
		};
		vector<sComponent*> statisticalList();
		vector<double> minMedMaxMetersPerDap();
		vector<int> minMedMaxRedundancyPerMeter();
		vector<int> meterPerHop(vector<sComponent*> sL);
		vector<double> linkQualityPerHop(vector<sComponent*> sL);
		vector<vector<sComponent*> > createMeterNeighbourhood(Grid *g);
		sComponent* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, vector<sComponent*> sC, int meshHop);
		string executeMetricCalculation();


};

#endif

