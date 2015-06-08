#ifndef _HEATGRID_METHODS_H
#define _HEATGRID_METHODS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"
#include <time.h>
#include <stdio.h>


class Heatgrid
{
private:
	vector<Position*> meters;
	vector<Position*> poles;
	int scenario, technology, SRD, meshEnabled;
	double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
	double regionLimiter, gridLimiter;
	string rubyPath;
public:
	AutoPlanning(vector<Position*> &m, vector<Position*> &p, int s, int t, double B, double T, double h1, double h2, int srd, int me, string rp)
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
		regionLimiter = 0.01;
		gridLimiter = 10000000;


	};
	~AutoPlanning()
	{
		for (int i = 0; i < meters.size(); i++)
		{
			delete meters[i];
		}

		for (int i = 0; i < poles.size(); i++)
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
	string executeAutoPlanTestMode(string * res, double gridsize);
	vector<int> uncoverableMeters(vector<vector<int> > &SCP);
	void executeGlpk(string filename);
	string gridAutoPlanning();
	string gridAutoPlanningTestMode(float *mtu, float* mmu);
	vector<int> concatVectors(vector<int> &v1, vector<int> &v2);
	string graspAutoPlanning();

};


void RolimLocalSearch(vector<vector<int> > &scp, int * solution);
void RolimEGuerraLocalSearch(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, int * solution);
void WalkSat(vector<vector<int> > &scp, int * solution);

#endif