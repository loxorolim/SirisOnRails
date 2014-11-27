#ifndef _REQUISITION_H
#define _REQUISITION_H

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include "auxiliars.h"
#include "HataSRD.h"
#include "Grid.h"

using namespace std;


#define DAP 0
#define METER 1
#define POLE 2
const int memlimit = (getTotalSystemMemory() / (1000000))*0.8;
bool compareByLongitude(Position* a, Position *b);
bool compareByLatitude(Position* a, Position *b);
class Requisition
{
	private:
		int option;
		int scenario, technology, SRD, meshEnabled;
		double H_TX, H_RX, BIT_RATE, TRANSMITTER_POWER;
		vector<Position*> meters, poles, daps;
		double regionLimiter = 0.001;
	
	public:
		Requisition()
		{
			//readConfiguration();
		}
		~Requisition()
		{
			for (int i = 0; i < meters.size(); i++)
			{

				delete meters[i];
			}
			for (int i = 0; i < poles.size(); i++)
			{
				delete poles[i];
			}
			for (int i = 0; i < daps.size(); i++)
			{
				delete daps[i];
			}
		}

		void readConfiguration();
		void readPositions(int id);
		vector<int> uncoverableMeters(vector<vector<int>> &SCP);
		void dapsToNs3File(vector<vector<int>> &scp, vector<int> &chosenDaps);
		void saveGLPKFile(vector<vector<int>> &scp);
		void saveGLPKFileReduced(vector<vector<int>> &SCP);
		vector<vector<sComponent*>> statisticalList();
		vector<vector<int>> createScp();
		vector<vector<int>> createScp2();
		vector<DrawInfo*> calculateDrawingInfo();
		string getResponse();
		string getDrawResponse();
		string getMetricResponse();
		string getAutoPlanResponse();
		string gridAutoPlanning();
		string exactAutoPlanning();

		//SÓ PRA TESTES
		void getTestResponse(string fname); //ESSA REQUISIÇÃO É PARA SALVAR EM UM ARQUIVO DADOS DO PLANEJAMENTO, NÃO DEVE ESTAR NA VERSÃO FINAL!
		//void getGraspSol(vector<vector<int>> &SCP);
		//void getGreedySol(vector<vector<int>> &SCP);
		//void getExactSol(vector<vector<int>> &SCP);
		//TESSSSSTEEEEEES!!!


		vector<Position*> getActiveRegion(vector<Position*> &sorted,Position* ref);
		vector<vector<int>> createMeterNeighbourhood(Grid *g);
		void setConfig(int meshEnabled, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, int SRD)
		{
			this->scenario = scenario; this->technology = technology; this->H_TX = H_TX; this->H_RX = H_RX; this->BIT_RATE = BIT_RATE;
			this->TRANSMITTER_POWER = TRANSMITTER_POWER; this->SRD = SRD; this->meshEnabled = meshEnabled;
		}
		void setMeters(vector<Position*> &v)
		{
			//sort(v.begin(), v.end(), compareByLatitude);
			meters = v;
			
		}
		void setPoles(vector<Position*> &v)
		{
			//sort(v.begin(), v.end(), compareByLatitude);
			poles = v;
		}
		void setDAPs(vector<Position*> v)
		{
			//sort(v.begin(), v.end(), compareByLatitude);
			daps = v;

		}
		void setRegionLimiter(double v)
		{
			regionLimiter = v;
		}
		

};
#endif