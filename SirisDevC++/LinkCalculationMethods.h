#ifndef _LINK_CALCULATION_METHODS_H
#define _LINK_CALCULATION_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"

//DA PRA USAR O GRID AQUI PROVAVELMENTE!!!!!!!!!!!!!

class LinkCalculation
{
	private:
			vector<Position*> meters;
			vector<Position*> daps;
			int scenario, technology, SRD, meshEnabled;
			double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			double regionLimiter ;
			string rubyPath;
	public:
		LinkCalculation(vector<Position*> &m, vector<Position*> &d, int s, int t, double B, double T,double h1, double h2, int srd, int me, string rp)
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
		~LinkCalculation()
		{
			for(int i = 0; i < meters.size();i++)
				delete meters[i];

			for(int i = 0; i <daps.size();i++)
				delete daps[i];
		};
		vector<DrawInfo*> calculateDrawingInfo();
		string executeLinkCalculation();
		DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters);

};




#endif
