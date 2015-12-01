#ifndef _LINK_CALCULATION_METHODS_H
#define _LINK_CALCULATION_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "auxiliars.h"

#include "FatherMethods.h"
#include "Grid.h"



class DrawInfo
{
public:
	int dashed;
	int a, b, hopnumber;
	double efficiency, delay, distance;
	DrawInfo(int mainIndex, int toConnectIndex, int hn, double eff, double del, double d, int da)
	{
		a = mainIndex;
		b = toConnectIndex;
		hopnumber = hn;
		efficiency = eff;
		delay = del;
		distance = d;
		dashed = da;
	};
	string toString()
	{
		string ret;
		ret += to_string(a);
		ret += "/";
		ret += to_string(b);
		ret += "/";
		ret += to_string(hopnumber);
		ret += "/";
		ret += to_string(efficiency);
		ret += "/";
		ret += to_string(delay);
		ret += "/";
		ret += to_string(distance);
		ret += "/";
		ret += to_string(dashed);
		//ret += "/";
		return ret;
	}
};


class LinkCalculation: public FatherMethods
{
	private:
		vector<Position*> meters; // alias para o vec1, meters é a mesma coisa que o vec1
		vector<Position*> daps; // alias para o vec2, daps é a mesma coisa que o vec2

	public:
		LinkCalculation(vector<Position*> &meters, vector<Position*> &daps, int scenario, int technology, double bit_rate, double t_power, double h_tx, double h_rx, int srd, int mesh, string rubyPath)
		{
			this->meters = meters;
			this->daps = daps;
			this->scenario = scenario;
			this->technology = technology;
			this->bit_rate = bit_rate;
			this->t_power = t_power;
			this->h_tx = h_tx;
			this->h_rx = h_rx;
			this->srd = srd;
			this->mesh = mesh;
			this->rubyPath = rubyPath;

			
			//Delimitar o tamanho do grid para criação do SCP, esse tamanho deve ser maior ou igual que o alcance que estamos considerando. O tamanho ótimo é igual ao tamanho do alcance.
			regionLimiter = 0;
			while (getLinkQuality(regionLimiter) > MARGIN_VALUE)
			{
				regionLimiter++;
			}
			regionLimiter++;
			this->regionLimiter = regionLimiter;
		};
		~LinkCalculation()
		{
			for (int i = 0; i < meters.size(); i++)
			{
				delete meters[i];
			}
			for (int i = 0; i < daps.size(); i++)
			{
				delete daps[i];
			}
		}

		vector<DrawInfo*> calculateDrawingInfo();
		string executeLinkCalculation();
		DrawInfo* chooseDeviceToConnect(Position* meter, vector<Position*> &devices,int isFirstHop);

};






#endif
