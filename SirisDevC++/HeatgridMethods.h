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

//NÃO ESTOU USANDO MAIS ESSA CLASSE!
class Heatgrid
{
private:
	vector<Position*> points;

public:
	Heatgrid(vector<Position*> &m)
	{
		points = m;
	};
	~Heatgrid()
	{
		for (int i = 0; i < points.size(); i++)
		{
			delete points[i];
		}
	};
	string calculateHeatgridInfo();
	string executeHeatgrid();
};

#endif
