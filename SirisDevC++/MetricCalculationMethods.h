#ifndef _METRIC_CALCULATION_METHODS_H
#define _METRIC_CALCULATION_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"


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

class MetricCalculation
{
	private:
			vector<Position*> meters;
			vector<Position*> daps;
			int scenario, technology, SRD, meshEnabled;
			double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			double regionLimiter ;
			int packetSize; //em Mb
			double perHopDelay;
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
			regionLimiter = 0;
			while (getHataSRDSuccessRate(regionLimiter, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD) > MARGIN_VALUE)
			{
				regionLimiter++;
			}
			regionLimiter++;
			packetSize = 400 * 8;
			perHopDelay = 2; //ms
		};
		~MetricCalculation()
		{
			for(int i = 0; i < meters.size();i++)
				delete meters[i];

			for(int i = 0; i <daps.size();i++)
				delete daps[i];
		};
		vector<sComponent*> statisticalList();
		vector<double> minMedMaxMetersPerDap(vector<vector<int> > cL);
		vector<double> minMedMaxRedundancyPerMeter(vector<vector<int> > cL);
		vector<int> meterPerHop(vector<sComponent*> sL);
		vector<double> linkQualityPerHop(vector<sComponent*> sL);
		vector<double> linkDelayPerHop(vector<sComponent*> sL);
		//vector<vector<sComponent*> > createStatisticalMeterNeighbourhood(Grid *g);
		vector<vector<int> > createMeterNeighbourhood(Grid *g);
		sComponent* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, vector<sComponent*> sC, int meshHop);
		sComponent* chooseDeviceToConnect(Position* meter, vector<Position*> &devices, vector<sComponent*> sC, int meshHop);
		string executeMetricCalculation();
		vector<vector<int> > coverageList();
		vector<int> concatVectors(vector<int> &v1, vector<int> &v2);


};

#endif

