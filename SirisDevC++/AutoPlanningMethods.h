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
#include <time.h>
#include <stdio.h>
#include "MetricCalculationMethods.h"
#include "glpk.h"
#include <thread>
#include <future>
extern "C" {
#include <vl/kmeans.h>
#include <vl/generic.h>
#include <vl/host.h>
#include <vl/kdtree.h>
}

struct ClusterProblem
{
	vector<Position*> meters, poles;
};
struct subProblem
{
	int numOfMeters, numOfPoles;
	double solverTime, memUsed, density, avgCoverage, coverageDeviation, numOfCoverage;
	string toString()
	{
		string ret = "";
		ret += to_string(numOfMeters) + "x" + to_string(numOfPoles) + ":\n";
		ret += "Número de 1's na Matriz de Cobertura" + to_string(numOfCoverage) + ":\n";
		ret += "Densidade: " + to_string(density) + "\n";
		ret += "Cobertura média por DAP: " + to_string(avgCoverage) + "\n";
		ret += "Desvio padrão da cobertura: " + to_string(coverageDeviation) + "\n";
		ret += "Memória utilizada: " + to_string(memUsed) + "\n";
		ret += "Tempo utilizado: " + to_string(solverTime) + "\n";
		return ret;
	}
};
struct TestResult
{
	//Pros dois
	int gridSize, numMeters, numPoles, uncoveredMeters;
	float maxMem;
	double solverTime;
	vector<subProblem*> subProblemStats;
	//Sem post-opt
	int solutionQuality=-1;
	float time;
	vector<int> chosenPoles;
	vector<int> metersPerHop;
	vector<double> metersPerDap;
	vector<double> qualityPerHop;
	vector<double>  redundancy;

	//Com post-opt
	int poSolutionQuality=-1;
	float poTime;
	vector<int> poChosenPoles;
	vector<int> poMetersPerHop;
	vector<double> poMetersPerDap;
	vector<double> poQualityPerHop;
	vector<double>  poRedundancy;


	string toString()
	{
		string ret = "";
		ret += "Dimension: " + to_string(numMeters) + " x " + to_string(numPoles) + "\n";
		ret += "Grid Size: " + to_string(gridSize) + "\n";
		ret += "Uncovered meters: " + to_string(uncoveredMeters) + "\n";
		ret += "\nSolution Quality: " + to_string(solutionQuality) + "\n";
		ret += "Time: " + to_string(time) + "\n";
		for (int i = 0; i < metersPerHop.size(); i++)
		{
			ret += "Hop" + to_string(i + 1) + "Quantity: " + to_string(metersPerHop[i]) + "\n";
		}
		for (int i = 0; i < qualityPerHop.size(); i++)
		{
			ret += "Hop" + to_string(i + 1) + "Quality: " + to_string(qualityPerHop[i]) + "\n";
		}
		ret += "Min Meters per DAP: " + to_string(metersPerDap[0]) + "\n";
		ret += "Med Meters per DAP: " + to_string(metersPerDap[1]) + "\n";
		ret += "Max Meters per DAP: " + to_string(metersPerDap[2]) + "\n";
		ret += "Min Redundancy: " + to_string(redundancy[0]) + "\n";
		ret += "Med Redundancy: " + to_string(redundancy[1]) + "\n";
		ret += "Max Redundancy: " + to_string(redundancy[2]) + "\n";
		if (poSolutionQuality != -1)
		{

			ret += "\nP.O Solution Quality: " + to_string(poSolutionQuality) + "\n";
			ret += "P.O Time: " + to_string(poTime) + "\n";
			for (int i = 0; i < poMetersPerHop.size(); i++)
			{
				ret += "Hop" + to_string(i + 1) + "Quantity: " + to_string(poMetersPerHop[i]) + "\n";
			}
			for (int i = 0; i < poQualityPerHop.size(); i++)
			{
				ret += "Hop" + to_string(i + 1) + "Quality: " + to_string(poQualityPerHop[i]) + "\n";
			}
			ret += "Min Meters per DAP: " + to_string(poMetersPerDap[0]) + "\n";
			ret += "Med Meters per DAP: " + to_string(poMetersPerDap[1]) + "\n";
			ret += "Max Meters per DAP: " + to_string(poMetersPerDap[2]) + "\n";
			ret += "Min Redundancy: " + to_string(poRedundancy[0]) + "\n";
			ret += "Med Redundancy: " + to_string(poRedundancy[1]) + "\n";
			ret += "Max Redundancy: " + to_string(poRedundancy[2]) + "\n";
		}
		for (int i = 0; i < subProblemStats.size(); i++)
			ret += subProblemStats[i]->toString();
		return ret;
	}
	~TestResult()
	{
		for (int i = 0; i < subProblemStats.size(); i++)
			delete subProblemStats[i];
	}


};
class AutoPlanning
{
	private:
			vector<Position*> meters;
			vector<Position*> poles;
			int scenario, technology, SRD, meshEnabled;
			double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			double regionLimiter, gridLimiter ;
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
			gridLimiter = 10000000;
			
			//Delimitar o tamanho do grid para criação do SCP, esse tamanho deve ser maior ou igual que o alcance que estamos considerando. O tamanho ótimo é igual ao tamanho do alcance.
			regionLimiter = 0;
			while (getHataSRDSuccessRate(regionLimiter, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD) > MARGIN_VALUE)
			{
				regionLimiter++;
			}
			regionLimiter++;
		
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
		

		vector<vector<int> > createScpWithLimit(int limit);
		vector<vector<int> > createScp();
		vector<vector<int> > createInvertedScp();
		void saveGLPKFile(vector<vector<int> > &scp);
		void saveGLPKFileReduced(vector<vector<int> > &SCP, int redundancy);
		void saveGLPKFileReducedWithLimit(vector<vector<int> > &SCP, int redundancy, int limit);
		void saveGLPKFileReduced(vector<vector<int> > &SCP, vector<Position*> metersToConsider, vector<Position*> polesToConsider, int redundancy);
		vector<vector<int> > createMeterNeighbourhood(Grid *g);
		string executeAutoPlan();
		string executeAutoPlan(int redundancy, int limit);
		vector<Position*> getMetersThatSatisfyRedundancy(int redundancy, vector< vector< int > > invertedSCP);
		vector<int> uncoverableMeters(vector<vector<int> > &SCP, int redundancy);
		vector<int> coverableMeters(vector<vector<int> > &SCP, int redundancy);
		vector<int> executeGlpk(string filename);
		string gridAutoPlanning(int redundancy, int limit);
		string planWithRedundancy(vector<vector<int> > &scp, int redundancy);
		TestResult* clusterAutoPlanning(bool usePostOptimization, int redundancy);
		vector<int> concatVectors(vector<int> &v1, vector<int> &v2);
		string graspAutoPlanning();

		//Metodos Teste
		void setGridSize(double gridSize);
		TestResult* executeAutoPlanTestMode(int usePostOptimization, int redundancy);
		TestResult* executeClusterAutoPlanTestMode(int usePostOptimization, int redundancy);
		TestResult* gridAutoPlanningTestMode(bool usePostOptimization, int redundancy);
		vector<int> executeGlpk(string filename, double &maxmem, double &solverTime);
		int getMetersSize();
		int getPolesSize();
		void setRegionLimiter(double rl);

};



void RolimLocalSearch(vector<vector<int> > &scp, int * solution);
void RolimEGuerraLocalSearch(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, vector<int> solution);
void RolimEGuerraLocalSearchWithRedundancy(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, vector<int> &solution,int redundancy);
void RolimEGuerraLocalSearchWithRedundancy2(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, int * &solution, int redundancy);
void WalkSat(vector<vector<int> > &scp, int * solution);

#endif
