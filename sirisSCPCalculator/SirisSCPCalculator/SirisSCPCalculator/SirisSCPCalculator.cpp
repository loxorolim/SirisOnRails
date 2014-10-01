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

int main(int argc, char* argv[])
{
	funcConversaoDadosHomma("dadoshomma2.txt");
//	double wow = getHataSRDSuccessRate(50, 0, 0, 0.25, 0, 3, 5, 1);
//	double wow2 = 1 - wow;
	{ //essas chaves tao aki por causa do teste do memory leak
		propagationTable();
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



