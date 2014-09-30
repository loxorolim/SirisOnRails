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
#include "HataSRD.h"
using namespace std;

vector<vector<int>> createMeterNeighbourhoodMatrix(vector<Position*> &uncoveredMeters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{

	// var points = metersToPoints(meters);

	vector<vector<int>> M;

	for (int i = 0; i < uncoveredMeters.size(); i++) {
		vector<int> pointsCovered;
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

vector<vector<int>> createScpMatrix(vector<Position*>& uncoveredMeters, vector<Position*>& poles, int meshEnabled, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{

	vector<int> aux;
	vector<vector<int>> sM;
	for (int i = 0; i < uncoveredMeters.size(); i++) 
	{
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

		vector<vector<int>> nM = createMeterNeighbourhoodMatrix(uncoveredMeters, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
		for (int i = 0; i < uncoveredMeters.size(); i++)
		{
			vector<int> neighbours = nM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{
					sM[i] = concatVectors(sM[i], sMCopy[neighbours[k]]);
					newNeighbours = concatVectors(newNeighbours, nM[neighbours[k]]);
				}
				//sM[i] = removeRepeated(sM[i]);
				//newNeighbours = removeRepeated(newNeighbours);
				neighbours = newNeighbours;

			}
		}
	}




	return sM;
}

//vector<vector<int>> createScpMatrix2(vector<Position*>& meters, vector<Position*>& poles, int meshMaxJumps, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
//{
//
//	vector<vector<int>> sM;
//	for (int i = 0; i < poles.size(); i++)
//	{
//		vector<int> coverage;
//		for (int j = 0; j < meters.size(); j++)
//		{
//			double dist = getDistance(poles[i], meters[j]);
//			double eff = getHataSRDSuccessRate(dist, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
//			if (eff >= MARGIN_VALUE)
//				coverage.push_back(j);
//		}
//		if (meshMaxJumps)
//		{
//			vector<vector<int>> nM = createMeterNeighbourhoodMatrix(meters, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
//			vector<int> begin = coverage;
//			//vector<int> toAdd;
//			for (int z = 0; z < meshMaxJumps; z++)
//			{
//				vector<int> neighbours;
//				for (int k = 0; k < begin.size(); k++)
//				for (int l = 0; l < nM[begin[k]].size(); l++)
//					neighbours.push_back(nM[coverage[k]][l]);
//				sort(neighbours.begin(), neighbours.end());
//				neighbours.erase(unique(neighbours.begin(), neighbours.end()), neighbours.end());
//				vector<int> notAdded;
//				
//				sort(coverage.begin(), coverage.end());
//				set_difference(neighbours.begin(), neighbours.end(), coverage.begin(), coverage.end(), notAdded.begin());
//				coverage.insert(coverage.end(), notAdded.begin(), notAdded.end());
//				//for (int k = 0; k < notAdded.size(); k++)
//				//{
//				//	toAdd.push_back(notAdded[k]);
//				//}
//				begin = notAdded;
//			}
//		}
//		sM.push_back(coverage);
//	}
//	
//	return sM;
//}

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
		fprintf_s(file, "%s", "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
		//ret += "set Z:= ";
		fprintf_s(file, "set Z:= ");
		for (int i = 0; i < Z; i++)
			fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
		fprintf_s(file, ";\n");
		fprintf_s(file, "set Y:= ");
		for (int i = 0; i < Y; i++)
			fprintf_s(file, "%s%d%s", "Y", (i + 1), " ");
		fprintf_s(file, ";\n");

		fprintf_s(file, "param A : ");

		for (int i = 0; i < Y; i++)
			fprintf_s(file, "%s%d%s", "Y", (i + 1), " ");
		fprintf_s(file, ":= \n");
		for (int i = 0; i < Z; i++) {
			fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
			for (int j = 0; j < Y; j++) {
				bool um = false;
				for (int k = 0; k < SCP[i].size(); k++)
				if (j == SCP[i][k])
					um = true;
				if (um)
					fprintf_s(file, "1 ");
				else
					fprintf_s(file, "0 ");

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
	int SRD = 0;
	int meshEnabled;

	readConfiguration(&scenario, &technology, &H_TX, &H_RX, &BIT_RATE, &TRANSMITTER_POWER, &SRD, &meshEnabled);
	//FILE *file;
	//fopen_s(&file, "C:\\Sites\\first_app\\teste.txt", "r");
	//int wow = 0;
	//for (int i = 0; i < 9; i++)
	//	fscanf_s(file, "%d",&wow);
	int metersLength;
	scanf_s("%d", &metersLength);
	//fscanf_s(file,"%d", &metersLength);
	vector<Position*> meters;
	vector<Position*> poles;
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
	int polesLength = 0;
	scanf_s("%d", &polesLength);
	//fscanf_s(file,"%d", &polesLength);
	for (int i = 0; i < polesLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		//fscanf_s(file,"%lf %lf", &lat, &lng);
		//Position *toAdd = (Position*)malloc(sizeof(Position));
		Position *toAdd = new Position(lat, lng);
		//toAdd.latitude = lat;
		//toAdd.longitude = lng;
		poles.push_back(toAdd);
	}
//	fclose(file);

	vector<vector<int>> SCP = createScpMatrix(meters, poles, meshEnabled, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
	saveGLPKFile(SCP, poles, "C:\\Sites\\first_app\\teste2.txt");
	system("C:\\Users\\Guilherme\\Downloads\\glpk-4.54\\w64\\glpsol.exe --math C:\\Sites\\first_app\\teste2.txt");


	//FILE* file;
	//fopen_s(&file, "C:\\Sites\\first_app\\Results.txt", "r");
	ifstream f("C:\\Sites\\first_app\\Results.txt");
	string str;
	getline(f, str);
	//string file_contents;
	//while (getline(f, str))
	//{
	//	file_contents += str;
	//	file_contents.push_back('\n');
	//}
	return str;



}
