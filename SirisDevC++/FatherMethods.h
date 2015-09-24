#ifndef _FATHER_METHODS_H
#define _FATHER_METHODS_H

#include "HataSRD.h"

class FatherMethods
{
	public:
		vector<Position*> vec1, vec2; //vec1 e vec2 serão medidores, postes ou daps dependendo da classe que implementar.
		int scenario, technology, srd, mesh;
		double h_tx, h_rx, bit_rate, t_power;
		double regionLimiter; // para criação do grid

		double getLinkQuality(double distance);
		double getLinkQualityBetweenMeters(double distance);


	virtual ~FatherMethods()
	{
		for (int i = 0; i < vec1.size(); i++)
		{
			delete vec1[i];
		}

		for (int i = 0; i < vec2.size(); i++)
		{
			delete vec2[i];
		}
	};


};







#endif