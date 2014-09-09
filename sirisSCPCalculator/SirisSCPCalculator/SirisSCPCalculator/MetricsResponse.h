#ifndef _METRICSRESPONSE_H
#define _METRICSRESPONSE_H
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include "auxiliars.h"
using namespace std;


vector<int> removeRepeated(vector<int> &v);
bool contains(vector<sComponent*> v, int elem);
vector<vector<sComponent*>> statisticalList(vector<Position*> &daps, vector<Position*> &meters, int meshEnabled, int meshMaxJumps, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
vector<vector<int>> createNeighbourhoodMatrix(vector<Position*> &meters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
double averageDirectLinkQuality(vector<vector<sComponent*>> statisticalList);
double calcEfficiency(sComponent *meter);
vector<vector<double>> averageHops(vector<vector<sComponent*>> statisticalList, int meshMaxJumps);
vector<double> averageLinksPerDap(vector<vector<sComponent*>> statisticalList);
vector<sComponent*> noRepeat(vector<sComponent*> list);
vector<vector<double>> avaregeMeshMetersPerDap(vector<vector<sComponent*>> statisticalList, int meshMaxJumps);
vector<double> averageMetersPerDap(vector<vector<sComponent*>> statisticalList);
string executeMetricsOption();
#endif