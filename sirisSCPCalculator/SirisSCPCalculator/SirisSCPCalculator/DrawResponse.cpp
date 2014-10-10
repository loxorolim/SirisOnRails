#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include <algorithm>
#include "HataSRD.h"

using namespace std;

//vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2)
//{
//	//remove v2 do v1
//	vector<Position*> ret;
//	for (int i = 0; i < v1.size(); i++)
//	{
//		auto it = find(v2.begin(), v2.end(), v1[i]);
//		if (it == v2.end())
//		{
//			ret.push_back(v1[i]);
//		}
//	}
//	return ret;
//}
//
//DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD)
//{
//	double minDist = -1;
//	Position* meterToConnect = NULL;
//	for (int i = 0; i < connectedMeters.size(); i++)
//	{
//		double dist = getDistance(meter, connectedMeters[i]);
//		if (dist < minDist || minDist == -1)
//		{
//			minDist = dist;
//			meterToConnect = connectedMeters[i];
//		}
//	}
//	if (minDist != -1)
//	{
//		double dist = getDistance(meter, meterToConnect);
//		double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//		if (effs >= MARGIN_VALUE) {
//			DrawInfo* ret = new DrawInfo(meter, meterToConnect, effs, dist, 1);
//			return ret;
//		}
//	}
//	return NULL;
//}
//vector<DrawInfo*> calculateDrawingInfo(vector<Position*> &meters, vector<Position*> &daps, int meshValue, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD)
//{
//
//
//	vector<DrawInfo*> toCover;
//	vector<Position*> coveredMeters;
//	vector<Position*> allMarkers;
//	allMarkers.insert(allMarkers.end(), meters.begin(), meters.end());
//	//allMarkers.insert(allMarkers.end(),daps.begin(), daps.end());
//	//POR ENQUANTO SÓ COM METERS
//	for (int d = 0; d < daps.size(); d++)
//	{
//		for (int i = 0; i < allMarkers.size(); i++) {
//			double dist = getDistance(daps[d], allMarkers[i]);
//			double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//			if (effs >= MARGIN_VALUE)
//			{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO
//
//				DrawInfo* toAdd = new DrawInfo(daps[d], allMarkers[i], effs, dist, 0);
//				toCover.push_back(toAdd);
//				coveredMeters.push_back(allMarkers[i]);
//			}
//		}
//		//toCover = toCover.sort(function(a, b) { return a.value.distance - b.value.distance });
//		//for (int i = 0; i < toCover.size(); i++)
//		//	this.connect(toCover[i].marker, toCover[i].value);
//	}
//	if (meshValue)
//	{
//		sort(coveredMeters.begin(), coveredMeters.end());
//		coveredMeters.erase(unique(coveredMeters.begin(), coveredMeters.end()), coveredMeters.end());
//		vector<Position*> uncoveredMeters = removeVectorFromAnother(allMarkers, coveredMeters);
//		vector<Position*> aux = coveredMeters;
//		for (int i = 0; i < meshValue; i++)
//		{
//			vector<Position*> newCovered;
//			for (int j = 0; j < uncoveredMeters.size(); j++)
//			{
//				DrawInfo* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//				if (toAdd)
//				{
//					toCover.push_back(toAdd);
//					newCovered.push_back(uncoveredMeters[j]);
//				}
//			}
//			aux = newCovered;
//			uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
//		}
//
//	}
//	return toCover;
//}
//string executeDrawOption()
//{
//
//
//	int scenario = 0;
//	int technology = 0;
//	double H_TX = 0;
//	double H_RX = 0;
//	double BIT_RATE = 0;
//	double TRANSMITTER_POWER = 0;
//	int SRD = 0;
//	int meshEnabled;
//
//	readConfiguration(&scenario, &technology, &H_TX, &H_RX, &BIT_RATE, &TRANSMITTER_POWER, &SRD, &meshEnabled);
//
//
//	int metersLength;
//	scanf_s("%d", &metersLength);
//	//fscanf_s(file,"%d", &metersLength);
//	vector<Position*> meters;
//	vector<Position*> daps;
//	for (int i = 0; i < metersLength; i++)
//	{
//		double lat;
//		double lng;
//		scanf_s("%lf %lf", &lat, &lng);
//		//fscanf_s(file,"%lf %lf", &lat, &lng);
//		//Position *toAdd = (Position*)malloc(sizeof(Position));
//		Position *toAdd = new Position(lat, lng);
//		//toAdd.latitude = lat;
//		//toAdd.longitude = lng;
//		meters.push_back(toAdd);
//
//	}
//	int dapsLength = 0;
//	scanf_s("%d", &dapsLength);
//	//fscanf_s(file,"%d", &dapsLength);
//	for (int i = 0; i < dapsLength; i++)
//	{
//		double lat;
//		double lng;
//		scanf_s("%lf %lf", &lat, &lng);
//		//fscanf_s(file,"%lf %lf", &lat, &lng);
//		//Position *toAdd = (Position*)malloc(sizeof(Position));
//		Position *toAdd = new Position(lat, lng);
//		//toAdd.latitude = lat;
//		//toAdd.longitude = lng;
//		daps.push_back(toAdd);
//	}
//
//	vector<DrawInfo*> drawInfos = calculateDrawingInfo(meters, daps, meshEnabled, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//	string ret = "";
//	for (int i = 0; i < drawInfos.size(); i++)
//		ret += drawInfos[i]->toString() + " ";
//
//	for (int i = 0; i < drawInfos.size(); i++)
//		delete drawInfos[i];
//	drawInfos.clear();
//	for (int i = 0; i < meters.size(); i++)
//		delete meters[i];
//	meters.clear();
//	for (int i = 0; i < daps.size(); i++)
//		delete daps[i];
//	daps.clear();
//
//	//double ret = getHataSRDSuccessRate(distance, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//	return ret;
//}
