#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "DrawResponse.h"
using namespace std;

vector<vector<int>> createMeterNeighbourhoodMatrix(vector<Position*> &uncoveredMeters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
vector<int> concatVectors(vector<int> &v1, vector<int> &v2);
vector<vector<int>> createScpMatrix(vector<Position*>& uncoveredMeters, vector<Position*>& poles, int meshEnabled, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
vector<vector<int>> createScpMatrixFromSorted(vector<Position*>& uncoveredMeters, vector<Position*>& poles, int meshEnabled, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD);
void saveGLPKFile(vector<vector<int>> &SCP, vector<Position*> &poles, string filename);
string executeAutoPlanOption();