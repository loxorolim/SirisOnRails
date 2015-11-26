#ifndef _FATHER_METHODS_H
#define _FATHER_METHODS_H

#include "HataSRD.h"

class FatherMethods
{
	public:
		//vector<vector<Position*>> elementsGroups; //vec1 e vec2 serão medidores, postes ou daps dependendo da classe que implementar.
		int scenario, technology, srd, mesh;
		double h_tx, h_rx, bit_rate, t_power;
		double regionLimiter; // para criação do grid
		string rubyPath;
		bool verbose;

		double getLinkQuality(double distance);
		double getLinkQualityBetweenMeters(double distance);


	/*virtual ~FatherMethods()
	{
		for (int i = 0; i < elementsGroups.size(); i++)
		{
			for (int j = 0; j < elementsGroups[i].size(); j++)
			{
				delete elementsGroups[i][j];
			}
		}
	};*/


};







#endif