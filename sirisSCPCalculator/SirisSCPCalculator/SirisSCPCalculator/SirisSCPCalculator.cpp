// SirisSCPCalculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include "MetricsCalculator.h"
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
		fprintf_s(file,"%s","set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n fminimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
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

int main(int argc, char* argv[])
{
	argc = 3;
	argv[1] = "C:\\Sites\\first_app\\teste.txt";
	argv[2] = "C:\\Sites\\first_app\\teste2.txt";
	if (argc > 2)
	{
		 FILE *file;
		 //printf(argv[1]);
		 //printf("%s\n", argv[1]);
		 fopen_s(&file,argv[1], "r");
		// fopen_s(&file, "C:\\Sites\\first_app\\teste.txt", "r");
		/* fopen returns 0, the NULL pointer, on failure */
		if (file == 0)
		{
			printf("Could not open file\n");
		}
		else
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
			fscanf_s(file, "%d", &scenario);
			fscanf_s(file, "%d", &technology);
			fscanf_s(file, "%lf", &H_TX);
			fscanf_s(file, "%lf", &H_RX);
			fscanf_s(file, "%lf", &BIT_RATE);
			fscanf_s(file, "%lf", &TRANSMITTER_POWER);
			fscanf_s(file, "%d", &SRD);

			fscanf_s(file, "%d", &meshEnabled);
			//fscanf_s(file,"%lf", &reach);
			fscanf_s(file, "%d", &metersLength);
			vector<Position*> meters;
			vector<Position*> poles;
			for (int i = 0; i < metersLength; i++)
			{
				double lat;
				double lng;
				fscanf_s(file, "%lf %lf", &lat, &lng);
				//Position *toAdd = (Position*)malloc(sizeof(Position));
				Position *toAdd = new Position(lat,lng);
				//toAdd.latitude = lat;
				//toAdd.longitude = lng;
				meters.push_back(toAdd);

			}
			int polesLength = 0;
			fscanf_s(file, "%d", &polesLength);
			for (int i = 0; i < polesLength; i++)
			{
				double lat;
				double lng;
				fscanf_s(file, "%lf %lf", &lat, &lng);
				//Position *toAdd = (Position*)malloc(sizeof(Position));
				Position *toAdd = new Position(lat,lng);
				//toAdd.latitude = lat;
				//toAdd.longitude = lng;
				poles.push_back(toAdd);
			}
			
			fclose(file);
			vector<vector<int>> SCP = createScpMatrix(meters, poles,  meshEnabled,  scenario, technology,  BIT_RATE,  TRANSMITTER_POWER, H_TX,  H_RX,  SRD);
			saveGLPKFile(SCP, poles, argv[2]);
			
			vector<Position*> daps;
			Position* dap1 = new Position(-22.918997592942823, - 43.090025782585144);
			daps.push_back(dap1);

			//vector<vector<sComponent*>> sl = statisticalList(daps, meters, 1, 3, 0, 0, 6, -20, 3, 5, true);
			
			SCP.clear(); //CLEAR NAO DA DELETE!
			meters.clear();
			poles.clear();
		}
		
	}

	
	return 0;
}


