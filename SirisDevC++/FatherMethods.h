#ifndef _FATHER_METHODS_H
#define _FATHER_METHODS_H

#include "HataSRD.h"

class FatherMethods
{
	public:
		int scenario, technology, srd, mesh;
		double h_tx, h_rx, bit_rate, t_power;
		double regionLimiter; // para criação do grid
		string rubyPath;
		bool verbose;

		double getLinkQuality(double distance);
		double getLinkQualityBetweenMeters(double distance);
		double getRegionLimiter();
};







#endif