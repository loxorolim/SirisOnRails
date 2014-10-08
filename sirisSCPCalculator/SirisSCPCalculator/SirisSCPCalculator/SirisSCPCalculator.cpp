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
#include "Requisition.h"
using namespace std;


string readFromPopen()
{
	int option = -1;
	scanf_s("%d", &option);

	switch (option)
	{
		case AUTOPLAN:
		//	return executeAutoPlanOption();
			break;
		case DRAW:
		//	return executeDrawOption();
			break;
		case METRIC:
		//	return executeMetricsOption();
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
void funcConversaoDadosHomma(string arq, int mSize)
{
	FILE* file;
	fopen_s(&file, arq.c_str(), "r");
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
		string ms = to_string(mSize);
		string um = "arqsTeste//filemeters" + ms + ".txt";
		string dois = "arqsTeste//filepoles" + ms + ".txt";
		FILE* filemeters;
		fopen_s(&filemeters, um.c_str(), "w");
		FILE* filepoles;
		fopen_s(&filepoles, dois.c_str(), "w");
		
		
		//double poleX=-1, poleY=-1;
		int poleAux=-1;
		//double meterX = -1, meterY = -1;
		int meterAux=-1;
		//fscanf_s(file, "%f", &uc);
		vector<double> poleX, poleY;
		double meterX, meterY;
		//vector<double> poleY;
		int i = 0;
		while (i<mSize)
		{

			int uc = -1, fu = -1;
			double ramal = -1, x = -1, y = -1, x2 = -1, y2 = -1;
			fscanf_s(file, "%d", &uc);
			fscanf_s(file, "%d", &fu);
			
			fscanf_s(file, "%lf", &ramal);
			
			fscanf_s(file, "%lf", &x);
			fscanf_s(file, "%lf", &y);
			//fscanf_s(file, "%lf", &x2);
			//fscanf_s(file, "%lf", &y2);

			if (fu == poleAux || poleAux == -1)
			{
				poleX.push_back(x);
				poleY.push_back(y);
				//poleX2.push_back(x2);
				//poleY2.push_back(y2);
			}
			else
			{
				double xSum = 0, ySum = 0;
				for (int i = 0; i < poleX.size(); i++)
				{
					xSum += (poleX[i]);
					ySum += (poleY[i]);
				}
				double poleXToAdd = xSum / poleX.size();
				double poleYToAdd = ySum / poleY.size();
				for (int i = 0; i < poleX.size(); i++)
				{
					int r = 5;
					//double ang = (360 / poleX.size())*i;
					//double mx = poleXToAdd + r*cos(ang); 
					//double my = poleYToAdd + r*sin(ang);
					//1grau de latitude = aprox 111km = 111000metros
					//1
					double mx = poleXToAdd + RandomNumber(-0.0001, +0.0001);
					
					double my = poleYToAdd + RandomNumber(-0.0001, +0.0001);
					fprintf(filemeters, "%lf %lf\n", mx,my);
				}
				poleX.clear();
				poleY.clear();
				poleX.push_back(x);
				poleY.push_back(y);
				//poleX2.clear();
				//poleY2.clear();
				//poleX2.push_back(x2);
				//poleY2.push_back(y2);
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
			


			i++;

		}
		fclose(file);
		fclose(filemeters);
		fclose(filepoles);
	}

	
	

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
				//aux.push_back(meters[pos]);
				meters.erase(meters.begin() + pos);
			}
			else
				startPoint = meters[pos];
		}
		//sets.push_back(aux);
		printf("push");


		
	}
}


void createSCPTeste(string arqm, string arqp)
{

	FILE * file;
	fopen_s(&file, arqm.c_str(), "r");
	FILE * file2;
	fopen_s(&file2, arqp.c_str(), "r");


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

	Requisition* req = new Requisition();
	req->setConfig(3, 1, 0, 0, 15, 3, 5, 1);
	req->setMeters(meters);
	req->setPoles(poles);
	//vector<vector<int>> SCP2 = req->createScp2();
	vector<vector<int>> SCP =  req->createScp();
	//vector<vector<int>> SCP = createScpMatrixFromSorted(meters, poles, 0, 1, 0, 0, 15, 3, 5, 1);
	int r = checkFeasibleTest(SCP);
	printf("\n%d", r);
//	saveGLPKFile(SCP, poles, "C:\\Sites\\first_app\\teste2.txt");
//	system("C:\\Users\\Guilherme\\Downloads\\glpk-4.54\\w64\\glpsol.exe --math C:\\Sites\\first_app\\teste2.txt");

	
}

int main(int argc, char* argv[])
{
	
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv",1000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 2000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 3000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 4000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 5000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 10000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 9999999);
//	double wow = getHataSRDSuccessRate(50, 0, 0, 0.25, 0, 3, 5, 1);
//	double wow2 = 1 - wow;
	{ //essas chaves tao aki por causa do teste do memory leak

		propagationTable();
		createSCPTeste("arqsTeste//filemeters1000.txt", "arqsTeste//filepoles1000.txt");
		//createSCPTeste();
		//functeste();
//		for (int i = 0; i < 200; i++)
//			double wow = getHataSRDSuccessRate(i, 0, 0, 0.25, 0, 3, 5, 1);
		string answer = "";
		answer = readFromPopen();
		printf_s("%s",answer.c_str());
	}

	FILE *pFile;
	freopen_s(&pFile, "C:\\Sites\\first_app\\MeMLeakTest.txt", "w", stdout);
	if (pFile)
	{

		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtDumpMemoryLeaks();
		fclose(pFile);
	}





	
	return 0;
}



