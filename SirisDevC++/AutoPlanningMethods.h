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
#include "FatherMethods.h"
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
class AutoPlanning: public FatherMethods
{
	private:
		double gridLimiter;
		vector<Position*> meters, poles;
		vector<int> coveredMetersIndexes;
			//vector<Position*> meters;
			//vector<Position*> poles;
			//int scenario, technology, SRD, meshEnabled;
			//double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
			//double regionLimiter, gridLimiter ;
			//string rubyPath;
	public:
		AutoPlanning(vector<Position*> &meters, vector<Position*> &poles, int scenario, int technology, double bit_rate, double t_power,double h_tx, double h_rx, int srd, int mesh, double gridLimiter,string rubyPath, bool verbose = false)
		{
			this->meters = meters;
			this->poles = poles;
			this->scenario = scenario;
			this->technology = technology;
			this->bit_rate = bit_rate;
			this->t_power = t_power;
			this->h_tx = h_tx;
			this->h_rx = h_rx;
			this->srd = srd;
			this->mesh = mesh;
			this->gridLimiter = gridLimiter;
			this->rubyPath = rubyPath;
			this->verbose = verbose;
			this->regionLimiter = getRegionLimiter();

			////Delimitar o tamanho do grid para criação do SCP, esse tamanho deve ser maior ou igual que o alcance que estamos considerando. O tamanho ótimo é igual ao tamanho do alcance.
			//regionLimiter = 0;
			//while (getLinkQuality(regionLimiter) >= MARGIN_VALUE || getLinkQualityBetweenMeters(regionLimiter) >= MARGIN_VALUE)
			//{
			//	regionLimiter++;
			//}
			//regionLimiter++;
		
		};
		~AutoPlanning()
		{
			for (int i = 0; i < meters.size(); i++)			
				delete meters[i];			
			for (int i = 0; i < poles.size(); i++)			
				delete poles[i];						
		}
		
		//Funções de criação do SCP;
		vector<vector<int> > createScp();
		vector<vector<int> > createInvertedScp();
		vector<vector<int> > coverageList(vector<Position*> &daps);
		
		//Funções planejamento básico
		void saveGLPKFile(vector<vector<int> > &SCP, int redundancy);
		
		int getBestCellSize();
		
		//Planejamento com cluster (usa o K-MEANS).
		vector<int> clusterAutoPlanning(bool usePostOptimization, int redundancy);
		TestResult* clusterAutoPlanningTestMode(bool usePostOptimization, int redundancy);
		//Planejamento com células.
		vector<int> gridAutoPlanning(bool usePostOptimization, int redundancy);
		TestResult* gridAutoPlanningTestMode(bool usePostOptimization, int redundancy);
		//Planejamento com Grasp
		vector<int> graspAutoPlanning(int iterations, double alpha, int redundancy, bool usePostOptimization);
		TestResult* graspAutoPlanningTestMode(int iterations, double alpha, int redundancy, bool usePostOptimization);

		vector<vector<int> > createMeterNeighbourhood(Grid *g);

		string executeAutoPlan(int redundancy, int limit);
		//vector<Position*> getMetersThatSatisfyRedundancy(int redundancy, vector< vector< int > > invertedSCP);
	
		//Executa o solver GLPK
		vector<int> executeGlpk(string filename);
		vector<int> executeGlpk(string filename, double &maxmem, double &solverTime);

		
		//string planWithRedundancy(vector<vector<int> > &scp, int redundancy);
		//vector<int> concatVectors(vector<int> &v1, vector<int> &v2);


		//Metodos Teste
		TestResult* executeAutoPlanTestMode(int usePostOptimization, int redundancy);
		TestResult* executeClusterAutoPlanTestMode(int usePostOptimization, int redundancy);
		//TestResult* executeGraspAutoPlanTestMode(int iterations, double alpha, int redundancy,int usePostOptimization);
		//TestResult* graspAutoPlanningTestMode(int iterations, double alpha, int redundancy, bool usePostOptimization);
		int getMetersSize();
		int getPolesSize();
		void setGridSize(double gridSize);
		void setRegionLimiter(double rl);
		//void removeAlreadyCoveredMeters(vector<Position*> &daps);
		void setCoveredMeters(vector<Position*> &daps);
};

//void RolimLocalSearch(vector<vector<int> > &scp, int * solution);
//void RolimEGuerraLocalSearch(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, vector<int> solution);
void RolimEGuerraLocalSearchWithRedundancy(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, vector<int> &solution,int redundancy);
//void RolimEGuerraLocalSearchWithRedundancy2(vector<vector<int> > &scp, vector<vector<int> > &invertedSCP, int * &solution, int redundancy);
//void WalkSat(vector<vector<int> > &scp, int * solution);

#endif
