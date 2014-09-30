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
		fopen_s(&file, "dadoshomma.txt", "r");
		FILE* filepoles;
		fopen_s(&file, "dadoshomma.txt", "r");
		
		int uc = -1, fu = -1;
		double ramal = -1, x = -1, y = -1, x2 = -1, y2 = -1;
		//fscanf_s(file, "%f", &uc);
		while (true)
		{
			

			fscanf_s(file, "%d", &uc);
			fscanf_s(file, "%d", &fu);
			fscanf_s(file, "%lf", &ramal);
			fscanf_s(file, "%lf", &x);
			fscanf_s(file, "%lf", &y);
			fscanf_s(file, "%lf", &x2);
			fscanf_s(file, "%lf", &y2);

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



