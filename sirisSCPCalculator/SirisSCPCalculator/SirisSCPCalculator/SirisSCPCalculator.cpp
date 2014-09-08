// SirisSCPCalculator.cpp : Defines the entry point for the console application.
//
#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include "MetricsCalculator.h"
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;






//double rad (double x) 
//{
//	return x * M_PI / 180;
//}
//double getDistance(Position p1, Position p2)
//{
//	double R = 6378137; // Earth’s mean radius in meter
//	double dLat = rad(p2.latitude - p1.latitude);
//	double dLong = rad(p2.longitude - p1.longitude);
//	double a = sin(dLat / 2) * sin(dLat / 2) +
//		cos(rad(p1.latitude)) * cos(rad(p2.latitude)) *
//		sin(dLong / 2) * sin(dLong / 2);
//	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
//	double d = R * c;
//	return d; // returns the distance in meter
//}
vector<vector<int>> createMeterNeighbourhoodMatrix(vector<Position*> &uncoveredMeters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{
	
	// var points = metersToPoints(meters);

	vector<vector<int>> M;

	for (int i = 0; i < uncoveredMeters.size(); i++) {
		vector<int> pointsCovered ;
		for (int j = 0; j < uncoveredMeters.size(); j++)
		{
			double dist = getDistance(uncoveredMeters[i], uncoveredMeters[j]);
			double eff = getHataSRDSuccessRate(dist, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
			if (i != j && eff >= MARGIN_VALUE) 
				pointsCovered.push_back(j);
		
		}
		M.push_back(pointsCovered);
	}

	return M;
}
vector<int> concatVectors(vector<int> &v1, vector<int> &v2)
{
	vector<int> ret;
	for (int i = 0; i < v1.size(); i++)	
		ret.push_back(v1[i]);
	for (int i = 0; i < v2.size(); i++)
	{
		bool add = true;
		for (int j = 0; j < v1.size(); j++)
		if (v1[j] == v2[i])
		{
			add = false;
			break;
		}
		if (add)
			ret.push_back(v2[i]);
			
	}
	return ret;
		
	

}
//void removeRepeated(vector<int> &v)
//{
//	for (int i = 0; i < v.size(); i ++)
//		for (int j = i+1; j < v.size(); j++)
//		if (v[i] == v[j])
//			v.erase(v.begin() + j);
//		
//
//}
vector<vector<int>> createScpMatrix(vector<Position*>& uncoveredMeters, vector<Position*>& poles, int meshEnabled, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD){

	vector<vector<int>> sM;
	for (int i = 0; i < uncoveredMeters.size(); i++) {
		vector<int> polesThatCover;
		for (int j = 0; j < poles.size(); j++)
		{
			double dist = getDistance(uncoveredMeters[i], poles[j]);
			double eff = getHataSRDSuccessRate(dist, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
			if (eff >= MARGIN_VALUE)
				polesThatCover.push_back(j);
		}
					
		//if (polesThatCover.length > 0)
			sM.push_back(polesThatCover);
	}
	//Essa função depende de como o Mesh está implementado, talvez devesse estar no script do mesh?
	if (meshEnabled)
	{
		vector<vector<int>> sMCopy;
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> toAdd;
			for (int j = 0; j < sM[i].size(); j++)
				toAdd.push_back(sM[i][j]);
			sMCopy.push_back(toAdd);
		}

		vector<vector<int>> nM = createMeterNeighbourhoodMatrix(uncoveredMeters, env,  technology,  bit_rate,  transmitter_power,  h_tx,  h_rx,  SRD);
		for (int i = 0; i < uncoveredMeters.size(); i++)
		{
			vector<int> neighbours = nM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{
					sM[i] = concatVectors(sM[i], sMCopy[neighbours[k]]);
					newNeighbours = concatVectors(newNeighbours,nM[neighbours[k]]);
				}
				//sM[i] = removeRepeated(sM[i]);
				//newNeighbours = removeRepeated(newNeighbours);
				neighbours = newNeighbours;

			}
		}
	}




	return sM;
}
void saveGLPKFile(vector<vector<int>> &SCP, vector<Position*> &poles, string filename)
{
	FILE *file;
	fopen_s(&file, filename.c_str(), "w");
	if (file == 0)
	{
		printf("Could not open file\n");
	}
	else
	{
		int Z = SCP.size();
		int Y = poles.size();
		//TEM Q MUDAR ESSE NEGÓCIO AQUI!
		fprintf_s(file,"%s","set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
		//ret += "set Z:= ";
		fprintf_s(file, "set Z:= ");
		for (int i = 0; i < Z; i++)
			fprintf_s(file,"%s%d%s","Z" , (i + 1) , " ");
		fprintf_s(file, ";\n");
			fprintf_s(file,"set Y:= ");
		for (int i = 0; i < Y; i++)
			fprintf_s(file, "%s%d%s", "Y" , (i + 1) , " ");
		fprintf_s(file,";\n");

		fprintf_s(file,"param A : ");

		for (int i = 0; i < Y; i++)
			fprintf_s(file, "%s%d%s", "Y" , (i + 1) , " ");
		fprintf_s(file,":= \n");
		for (int i = 0; i < Z; i++) {
			fprintf_s(file,"%s%d%s", "Z" , (i + 1) , " ");
			for (int j = 0; j < Y; j++) {
				bool um = false;
				for (int k = 0; k < SCP[i].size(); k++)
				if (j == SCP[i][k])
					um = true;
				if (um)
					fprintf_s(file,"1 ");
				else
					fprintf_s(file,"0 ");

			}

		}
		fprintf_s(file, "\n");
		fprintf_s(file, ";");
		fprintf_s(file, "end;");
		fclose(file);
		

	}
	
}
string executeAutoPlanOption()
{


	int scenario = 0;
	int technology = 0;
	double H_TX = 0;
	double H_RX = 0;
	double BIT_RATE = 0;
	double TRANSMITTER_POWER = 0;
	bool SRD = 0;

	int meshEnabled = 0;
	//double reach = 0;
	int metersLength = 0;
	scanf_s("%d", &scenario);
	scanf_s("%d", &technology);
	scanf_s("%lf", &H_TX);
	scanf_s("%lf", &H_RX);
	scanf_s("%lf", &BIT_RATE);
	scanf_s("%lf", &TRANSMITTER_POWER);
	scanf_s("%d", &SRD);

	scanf_s("%d", &meshEnabled);
	//fscanf_s(file,"%lf", &reach);
	scanf_s("%d", &metersLength);
	vector<Position*> meters;
	vector<Position*> poles;
	for (int i = 0; i < metersLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		//Position *toAdd = (Position*)malloc(sizeof(Position));
		Position *toAdd = new Position(lat, lng);
		//toAdd.latitude = lat;
		//toAdd.longitude = lng;
		meters.push_back(toAdd);

	}
	int polesLength = 0;
	scanf_s("%d", &polesLength);
	for (int i = 0; i < polesLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		//Position *toAdd = (Position*)malloc(sizeof(Position));
		Position *toAdd = new Position(lat, lng);
		//toAdd.latitude = lat;
		//toAdd.longitude = lng;
		poles.push_back(toAdd);
	}


	vector<vector<int>> SCP = createScpMatrix(meters, poles, meshEnabled, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
	saveGLPKFile(SCP, poles, "C:\\Sites\\first_app\\teste2.txt");
	system("C:\\Users\\Guilherme\\Downloads\\glpk-4.54\\w64\\glpsol.exe --math C:\\Sites\\first_app\\teste2.txt");


	//FILE* file;
	//fopen_s(&file, "C:\\Sites\\first_app\\Results.txt", "r");
	ifstream file("C:\\Sites\\first_app\\Results.txt");
	string str;
	string file_contents;
	while (getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	return str;



}
DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD)
{
	double minDist = -1;
	Position* meterToConnect = NULL;
	for(int i = 0; i < connectedMeters.size(); i++) 
	{
		double dist = getDistance(meter, connectedMeters[i]);
		if (dist < minDist || minDist == -1) 
		{
			minDist = dist;
			meterToConnect = connectedMeters[i];
		}
	}
	if (minDist != -1) 
	{
		double dist = getDistance(meter, meterToConnect);
		double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
		if (effs >= MARGIN_VALUE) {
			DrawInfo* ret = new DrawInfo(meter, meterToConnect, effs, dist, 1);
			return ret;			
		}
	}
	return NULL;
}
vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2)
{
	//remove v2 do v1
	vector<Position*> ret;
	for (int i = 0; i < v1.size(); i++)
	{
		auto it = find(v2.begin(), v2.end(), v1[i]);
		if (it == v2.end())
		{
			ret.push_back(v1[i]);
		}
	}
	return ret;
}
vector<DrawInfo*> calculateDrawingInfo(vector<Position*> &meters, vector<Position*> &daps, int meshValue, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD)
{


	vector<DrawInfo*> toCover;
	vector<Position*> coveredMeters;
	vector<Position*> allMarkers;
	allMarkers.insert(allMarkers.end(), meters.begin(), meters.end());
	//allMarkers.insert(allMarkers.end(),daps.begin(), daps.end());
	//POR ENQUANTO SÓ COM METERS
	for (int d = 0; d < daps.size(); d++)
	{
		for (int i = 0; i < allMarkers.size(); i++) {
			double dist = getDistance(daps[d], allMarkers[i]);
			double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (effs >= MARGIN_VALUE ) 
			{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO
				
				DrawInfo* toAdd = new DrawInfo(daps[d], allMarkers[i], effs, dist, 0);
				toCover.push_back(toAdd);
				coveredMeters.push_back(allMarkers[i]);
			}
		}
		//toCover = toCover.sort(function(a, b) { return a.value.distance - b.value.distance });
		//for (int i = 0; i < toCover.size(); i++)
		//	this.connect(toCover[i].marker, toCover[i].value);
	}
	if (meshValue)
	{
		sort(coveredMeters.begin(), coveredMeters.end());
		coveredMeters.erase(unique(coveredMeters.begin(), coveredMeters.end()), coveredMeters.end());
		vector<Position*> uncoveredMeters = removeVectorFromAnother(allMarkers,coveredMeters);
		vector<Position*> aux = coveredMeters;
		for (int i = 0; i < meshValue; i++)
		{
			vector<Position*> newCovered;
			for (int j = 0; j < uncoveredMeters.size(); j++)
			{
				DrawInfo* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
				if (toAdd)
				{
					toCover.push_back(toAdd);
					newCovered.push_back(uncoveredMeters[j]);
				}
			}
			aux = newCovered;
			uncoveredMeters = removeVectorFromAnother(uncoveredMeters,newCovered);
		}
		
	}
	return toCover;
	



}
string executePropagationOption()
{
	//FILE *file;
	//	 fopen_s(&file, "C:\\Sites\\first_app\\teste.txt", "r");
		
	//double distance = 0;
	int scenario = 0;
	int technology = 0;
	double H_TX = 0;
	double H_RX = 0;
	double BIT_RATE = 0;
	double TRANSMITTER_POWER = 0;
	int SRD = 0;
	int meshEnabled;


	scanf_s("%d", &scenario);
	scanf_s("%d", &technology);
	scanf_s("%lf", &H_TX);
	scanf_s("%lf", &H_RX);
	scanf_s("%lf", &BIT_RATE);
	scanf_s("%lf", &TRANSMITTER_POWER);
	scanf_s("%d", &SRD);
	scanf_s("%d", &meshEnabled);

	//fscanf_s(file,"%d", &scenario);
	//fscanf_s(file, "%d", &technology);
	//fscanf_s(file, "%lf", &H_TX);
	//fscanf_s(file, "%lf", &H_RX);
	//fscanf_s(file, "%lf", &BIT_RATE);
	//fscanf_s(file, "%lf", &TRANSMITTER_POWER);
	//fscanf_s(file, "%d", &SRD);
	//fscanf_s(file, "%d", &meshEnabled);

	int metersLength;
	scanf_s("%d", &metersLength);
	//fscanf_s(file,"%d", &metersLength);
	vector<Position*> meters;
	vector<Position*> daps;
	for (int i = 0; i < metersLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		//fscanf_s(file,"%lf %lf", &lat, &lng);
		//Position *toAdd = (Position*)malloc(sizeof(Position));
		Position *toAdd = new Position(lat, lng);
		//toAdd.latitude = lat;
		//toAdd.longitude = lng;
		meters.push_back(toAdd);

	}
	int dapsLength = 0;
	scanf_s("%d", &dapsLength);
	//fscanf_s(file,"%d", &dapsLength);
	for (int i = 0; i < dapsLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		//fscanf_s(file,"%lf %lf", &lat, &lng);
		//Position *toAdd = (Position*)malloc(sizeof(Position));
		Position *toAdd = new Position(lat, lng);
		//toAdd.latitude = lat;
		//toAdd.longitude = lng;
		daps.push_back(toAdd);
	}

	vector<DrawInfo*> drawInfos = calculateDrawingInfo(meters, daps,meshEnabled, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
	string ret = "";
	for (int i = 0; i < drawInfos.size(); i++)
		ret += drawInfos[i]->toString() + " ";

	for (int i = 0; i < drawInfos.size(); i++)
		delete drawInfos[i];
	drawInfos.clear();
	for (int i = 0; i < meters.size(); i++)
		delete meters[i];
	meters.clear();
	for (int i = 0; i < daps.size(); i++)
		delete daps[i];
	daps.clear();

	//double ret = getHataSRDSuccessRate(distance, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
	return ret;
}
string executeMetricOption()
{
	return "";
}
string readFromPopen()
{
	int option = -1;
	scanf_s("%d", &option);

	switch (option)
	{
		case AUTOPLAN:
			return executeAutoPlanOption();
			break;
		case PROPAGATION:
			return executePropagationOption();
			break;
		case METRIC:
			return executeMetricOption();
			break;
		default:
			return "";
			break;
	}
}

int main(int argc, char* argv[])
{
	{ //essas chaves tao aki por causa do teste do memory leak
		string answer = "";
		answer = readFromPopen();
		printf_s("%s",answer);
	}

	FILE *pFile;
	freopen_s(&pFile, "C:\\Sites\\first_app\\MeMLeakTest.txt", "w", stdout);
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();

	fclose(pFile);



	
	return 0;
}


