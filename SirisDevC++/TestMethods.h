#ifndef _TEST_METHODS_H
#define _TEST_METHODS_H

#define NO_FIX 0
#define FIXED_METERS 1
#define FIXED_POLES 2



#include <iostream>
#include <sstream>
#include "Grid.h"
#include "auxiliars.h"
#include "AutoPlanningMethods.h"
#include "LinkCalculationMethods.h"
#include "MetricCalculationMethods.h"
#include "HeatgridMethods.h"
#include "HataSRD.h"
#include <stdio.h>
extern "C"
{
#include <vl/kmeans.h>
#include <vl/generic.h>
#include <vl/host.h>
#include <vl/kdtree.h>
}
string const rubyPath =  "C:/Users/Guilherme/Documents/GitHub/SirisOnRails/";

struct DensityTestResult
{
	vector<vector<double> > results;

	vector<double> getDensitiesVector()
	{
		vector<double> ret;
		for (int i = 0; i < results.size(); i++)
		{
			ret.push_back(results[i][0]);
		}
		return ret;
	}
	double getTimeAtDensity(double d)
	{
		for (int i = 0; i < results.size(); i++)
		{
			if (results[i][0] == d)
				return results[i][1];
		}
		return -1;
	}
	double getMemAtDensity(double d)
	{
		for (int i = 0; i < results.size(); i++)
		{
			if (results[i][0] == d)
				return results[i][2];
		}
		return -1;
	}
	void saveToFile(string filename)
	{
		ofstream f(filename.c_str());
		for (int j = 0; j < results.size(); j++)
		{
			f << results[j][0] << " " << results[j][1] << " " << results[j][2] << "\n";
		}
		f.close();
	}
	~DensityTestResult()
	{
		
	}


};

#endif