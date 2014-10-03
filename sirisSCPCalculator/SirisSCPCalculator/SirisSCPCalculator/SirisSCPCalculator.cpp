// SirisSCPCalculator.cpp : Defines the entry point for the console application.
//
#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include "MetricsResponse.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "DrawResponse.h"
#include "AutoPlanResponse.h"
#include "MetricsResponse.h"
#include "HataSRD.h"
#include "Grid.h"
using namespace std;


string readFromPopen()
{
	int option = -1;
	scanf_s("%d", &option);

	switch (option)
	{
		case AUTOPLAN:
			return executeAutoPlanOption();
			break;
		case DRAW:
			return executeDrawOption();
			break;
		case METRIC:
			return executeMetricsOption();
			break;
		default:
			return "";
			break;
	}
}
// essa função aqui é só uma auxiliar, deletar depois!!!
float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
void funcConversaoDadosHomma(string arq)
{
	FILE* file;
	fopen_s(&file, "dadoshomma.txt", "r");
	if (file)
	{
		//while (true)
		//{
		//	ifstream f("dadoshomma.txt");
		//	string str;
		//	string str2;
		//	getline(f, str);
		//	getline(f, str2);
		//}
		FILE* filemeters;
		fopen_s(&filemeters, "filemeters.txt", "w");
		FILE* filepoles;
		fopen_s(&filepoles, "filepoles.txt", "w");
		
		
		//double poleX=-1, poleY=-1;
		int poleAux=-1;
		//double meterX = -1, meterY = -1;
		int meterAux=-1;
		//fscanf_s(file, "%f", &uc);
		vector<double> poleX, poleY,poleX2, poleY2;
		double meterX, meterY;
		//vector<double> poleY;
		int i = 0;
		while (true)
		{

			int uc = -1, fu = -1;
			double ramal = -1, x = -1, y = -1, x2 = -1, y2 = -1;
			fscanf_s(file, "%d", &uc);
			fscanf_s(file, "%d", &fu);
			
			fscanf_s(file, "%lf", &ramal);
			
			fscanf_s(file, "%lf", &x);
			fscanf_s(file, "%lf", &y);
			fscanf_s(file, "%lf", &x2);
			fscanf_s(file, "%lf", &y2);

			if (fu == poleAux || poleAux == -1)
			{
				poleX.push_back(x);
				poleY.push_back(y);
				poleX2.push_back(x2);
				poleY2.push_back(y2);
			}
			else
			{
				double xSum = 0, ySum = 0;
				for (int i = 0; i < poleX.size(); i++)
				{
					xSum += (poleX[i]+poleX2[i])/2;
					ySum += (poleY[i]+poleY2[i])/2;
				}
				double poleXToAdd = xSum / poleX.size();
				double poleYToAdd = ySum / poleY.size();
				for (int i = 0; i < poleX.size(); i++)
				{
					int r = 5;
					//double ang = (360 / poleX.size())*i;
					//double mx = poleXToAdd + r*cos(ang); 
					//double my = poleYToAdd + r*sin(ang);
					double mx = poleXToAdd + RandomNumber(-10, 10);
					
					double my = poleYToAdd + RandomNumber(-10, 10);
					fprintf(filemeters, "%lf %lf\n", mx,my);
				}
				poleX.clear();
				poleY.clear();
				poleX.push_back(x);
				poleY.push_back(y);
				poleX2.clear();
				poleY2.clear();
				poleX2.push_back(x2);
				poleY2.push_back(y2);
				fprintf(filepoles, "%lf %lf\n", poleXToAdd, poleYToAdd);
				//fazer fprintf aki
			}
			
			//if (ramal != meterAux && meterAux != -1)
			//	fprintf(filemeters, "%lf %lf\n", meterX,meterY);


			poleAux = fu;
			//meterAux = ramal;
			//meterX = x;
			//meterY = y;
			if (fu == -1)
				break;
			




		}
	}

	
	fclose(file);

}
int checkFeasibleTest(vector<vector<int>> &scp)
{
	int ret=0;
	for (int i = 0; i < scp.size(); i++)
	{
		if (scp[i].size() == 0)
			ret++;
	}
	return ret;
}

void functeste(vector<Position*> meters)
{
	int clusterSize = 500;
	vector<vector<Position*>> sets;
	Position* startPoint = meters[0];
	while (meters.size() != 0)
	{
		vector<Position*> aux;
		for (int i = 0; i < clusterSize; i++)
		{
			double minD = -1;
			int pos;
			if (meters.size() == 0)
				break;
			for (int j = 0; j < meters.size(); j++)
			{
				double d = getDistance(meters[j], startPoint);
				if (minD == -1 || d < minD)
				{
					pos = j;
					minD = d;
				}
					
			}
			if (i != clusterSize - 1)
			{
				aux.push_back(meters[pos]);
				meters.erase(meters.begin() + pos);
			}
			else
				startPoint = meters[pos];
		}
		sets.push_back(aux);
		printf("push");


		
	}
}


void createSCPTeste()
{

	FILE * file;
	fopen_s(&file, "filemetersconvertidos.txt", "r");
	FILE * file2;
	fopen_s(&file2, "filepolesconvertidos.txt", "r");


	vector<Position*> meters;
	vector<Position*> poles;
	while (true)
	{
		double lat = -1;
		double lng = -1;
		fscanf_s(file, "%lf %lf", &lat, &lng);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, meters.size());
		meters.push_back(toAdd);

	}

	while (true)
	{
		double lat = -1;
		double lng = -1;

		fscanf_s(file2, "%lf %lf", &lat, &lng);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}

	//sort(poles.begin(), poles.end(), compareByLatitude);
	vector<Position*> wow;

	Position* testez = new Position(0, 0);
	for (int i = 1; i < 3; i++)
	{
		Position* teste = new Position(0, i);
		Position* teste2 = new Position(i, 0);
		Position* teste3 = new Position(0, -i);
		Position* teste4 = new Position(-i, 0);
		Position* teste5 = new Position(i, i);
		Position* teste6 = new Position(i, -i);
		Position* teste7 = new Position(-i, i);
		Position* teste8 = new Position(-i, -i);
		wow.push_back(teste);
		wow.push_back(teste2);
		wow.push_back(teste3);
		wow.push_back(teste4);
		wow.push_back(teste5);
		wow.push_back(teste6);
		wow.push_back(teste7);
		wow.push_back(teste8);
	}

	//Grid* g = new Grid(meters,0.01);
	//int x = 0;
	//vector<Position*> vizinhos = g->getCell(meters[242]);
	sort(wow.begin(), wow.end(), compareByLatitude);
//	getRegionFromVector(wow, testez, 500);
	
	vector<vector<int>> SCP = createScpMatrixFromSorted(meters, poles, 0, 2, 0, 0, 15, 3, 5, 1);

	int r = checkFeasibleTest(SCP);
	printf("\n%d", r);
}

int main(int argc, char* argv[])
{
	//funcConversaoDadosHomma("dadoshomma2.txt");
//	double wow = getHataSRDSuccessRate(50, 0, 0, 0.25, 0, 3, 5, 1);
//	double wow2 = 1 - wow;
	{ //essas chaves tao aki por causa do teste do memory leak
		propagationTable();
		createSCPTeste();
		//functeste();
//		for (int i = 0; i < 200; i++)
//			double wow = getHataSRDSuccessRate(i, 0, 0, 0.25, 0, 3, 5, 1);
		string answer = "";
		answer = readFromPopen();
		printf_s("%s",answer.c_str());
	}

	FILE *pFile;
	freopen_s(&pFile, "C:\\Sites\\first_app\\MeMLeakTest.txt", "w", stdout);
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();

	fclose(pFile);



	
	return 0;
}



