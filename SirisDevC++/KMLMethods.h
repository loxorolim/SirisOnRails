#ifndef _KML_FUNCTIONS_METHODS_H
#define _KML_FUNCTIONS_METHODS_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>
#include "auxiliars.h"
#include "FatherMethods.h"
#include "MetricCalculationMethods.h"
#include "LinkCalculationMethods.h"
#include "pugixml/pugixml.hpp"

const string MetersTag = "Medidores";
const string DAPsTag = "Agregadores";
const string PolesTag = "Postes";
const string SignalsTag = "Mapa de Calor";
const string LinksTag = "Enlaces";

const string meter_name = "Medidor";
const string DAP_name = "Agregador";
const string pole_name = "Poste";
const string signal_name = "Ponto de Coleta";
const string link_name = "Enlace";

class KMLMethods : public FatherMethods
{
private:
	vector<Position*> meters, daps, poles, coverageArea;
public:
	KMLMethods(vector<Position*> &meters, vector<Position*> &daps, vector<Position*> &poles, vector<Position*> &coverageArea, int scenario, int technology, double bit_rate, double t_power, double h_tx, double h_rx, int srd, int mesh, string rubyPath)
	{
		this->meters = meters;
		this->daps = daps;
		this->poles = poles;
		this->coverageArea = coverageArea;
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
	~KMLMethods()
	{
		for (int i = 0; i < meters.size(); i++)
		{
			delete meters[i];
		}
		for (int i = 0; i < daps.size(); i++)
		{
			delete daps[i];
		}
		for (int i = 0; i < poles.size(); i++)
		{
			delete poles[i];
		}
		for (int i = 0; i < coverageArea.size(); i++)
		{
			delete coverageArea[i];
		}

	}
	string generateKML();
	string getMetersKMLFormat();
	string getDAPsKMLFormat();
	string getMetricKMLFormat();
	string getPolesKMLFormat();
	string getLinksKMLFormat();
	string getHeatmapKMLFormat();
	



};
int readKML(string inputFilename, vector<Position*>& daps, vector<Position*>& meters, vector<Position*>& poles, vector<Position*>& coverageArea);


#endif