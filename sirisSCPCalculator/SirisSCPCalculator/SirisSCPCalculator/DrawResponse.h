#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include "stdlib.h"
#include <vector>
#include "auxiliars.h"
#include "string"


DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD);
string executeDrawOption();
vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2);
vector<DrawInfo*> calculateDrawingInfo(vector<Position*> &meters, vector<Position*> &daps, int meshValue, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD);