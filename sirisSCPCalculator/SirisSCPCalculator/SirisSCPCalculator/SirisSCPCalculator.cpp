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

int main(int argc, char* argv[])
{
	double wow = getHataSRDSuccessRate(8, 0, 0, 0.25, 0, 3, 5, 1);
	double wow2 = 1 - wow;
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


