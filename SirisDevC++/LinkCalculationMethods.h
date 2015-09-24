#ifndef _LINK_CALCULATION_METHODS_H
#define _LINK_CALCULATION_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"

#include "FatherMethods.h"
#include "Grid.h"

//DA PRA USAR O GRID AQUI PROVAVELMENTE!!!!!!!!!!!!!

class LinkCalculation: public FatherMethods
{
	private:
		vector<Position*>& meters = vec1; // alias para o vec1, meters é a mesma coisa que o vec1
		vector<Position*>& daps = vec2; // alias para o vec2, daps é a mesma coisa que o vec2

	public:
		LinkCalculation(vector<Position*> &meters, vector<Position*> &daps, int scenario, int technology, double bit_rate, double t_power, double h_tx, double h_rx, int srd, int mesh)
		{
			this->vec1 = meters;
			this->vec2 = daps;
			this->scenario = scenario;
			this->technology = technology;
			this->bit_rate = bit_rate;
			this->t_power = technology;
			this->h_tx = h_tx;
			this->h_rx = h_rx;
			this->srd = srd;
			this->mesh = mesh;

			
			//Delimitar o tamanho do grid para criação do SCP, esse tamanho deve ser maior ou igual que o alcance que estamos considerando. O tamanho ótimo é igual ao tamanho do alcance.
			regionLimiter = 0;
			while (getLinkQuality(regionLimiter) > MARGIN_VALUE)
			{
				regionLimiter++;
			}
			regionLimiter++;
			this->regionLimiter = regionLimiter;
		};

		vector<DrawInfo2*> calculateDrawingInfo();
		string executeLinkCalculation();
		DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters);
		DrawInfo2* chooseDeviceToConnect(Position* meter, vector<Position*> &devices,int isFirstHop);

};




#endif
