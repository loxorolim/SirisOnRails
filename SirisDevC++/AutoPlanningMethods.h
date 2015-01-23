#ifndef _AUTO_PLANNING_METHODS_H
#define _AUTO_PLANNING_METHODS_H

#include <iostream>
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
			double regionLimiter ;
			string rubyPath;
	public:
		AutoPlanning(vector<Position*> &m, vector<Position*> &p, int s, int t, double B, double T,double h1, double h2, int srd, int me, string rp)
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
			rubyPath = rp;
			regionLimiter = 0.001;
		};
		~AutoPlanning()
		{
			for(int i = 0; i < meters.size();i++)
			{
				delete meters[i];
			}

			for(int i = 0; i < poles.size();i++)
			{
				delete poles[i];
			}
		};
		
		 vector<vector<int> > createScpSemGrid();
		 vector<vector<int> > createScp();
		 void saveGLPKFile(vector<vector<int> > &scp);
		 void saveGLPKFileReduced(vector<vector<int> > &SCP);
		 vector<vector<int> > createMeterNeighbourhood(Grid *g);
		 string executeAutoPlan();
		 vector<int> uncoverableMeters(vector<vector<int> > &SCP);
		 void executeGlpk(string filename);
		 string gridAutoPlanning();
		 vector<int> concatVectors(vector<int> &v1, vector<int> &v2);

};




#endif
