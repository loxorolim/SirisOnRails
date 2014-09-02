#ifndef _METRICSCALCULATOR_H
#define _METRICSCALCULATOR_H
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include "auxiliars.h"
using namespace std;


vector<int> removeRepeated(vector<int> &v);
vector<vector<int>> createNeighbourhoodMatrix(vector<Position*> &meters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
vector<vector<sComponent*>> statisticalList(vector<Position*> &daps, vector<Position*> &meters, int meshEnabled, int meshMaxJumps, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);

#endif