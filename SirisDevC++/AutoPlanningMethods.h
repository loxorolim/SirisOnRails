#ifndef _AUTO_PLANNING_METHODS_H
#define _AUTO_PLANNING_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"


class AutoPlanning
{
	private:
			vector<Position*> meters;
			vector<Position*> poles;
			int scenario, technology, SRD, meshEnabled;
			double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			double regionLimiter;
	public:
		AutoPlanning(vector<Position*> m, vector<Position*> p, int s, int t, double B, double T,double h1, double h2, int srd, int me)
		{
			meters = m;
			poles = p;
			scenario = s;
			technology = t;
			BIT_RATE = B;
			TRANSMITTER_POWER = T;
			H_TX = h1;
			H_RX = h2;
			SRD = srd;
			meshEnabled = me;
		};
		~AutoPlanning();
		
		 vector<vector<int> > createScpSemGrid();
		 vector<vector<int> > createScp();
		 void saveGLPKFile(vector<vector<int> > &scp);
		 void saveGLPKFileReduced(vector<vector<int> > &SCP);
		 vector<vector<int> > createMeterNeighbourhood(Grid *g);
		 string executeAutoPlan();
		 vector<int> uncoverableMeters(vector<vector<int> > &SCP);

};




#endif
