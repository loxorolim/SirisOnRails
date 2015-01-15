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
#include "Grasp.h"
#include <time.h>
#include <chrono>
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
		int z = 0;
		while (z<mSize)
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
			


			z++;

		}
		fclose(file);
		fclose(filemeters);
		fclose(filepoles);
	}

	
	

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

int checkFeasibleTest(vector<vector<int>> &scp, int mSize)
{
	int ret = 0;
	int* v = (int*)malloc(sizeof(int)*mSize);
	for (int i = 0; i < scp.size(); i++)
	{
		for (int j = 0; j < scp[i].size(); j++)
		{
			v[scp[i][j]] = 1;
		}
	}
	int notCovered = 0;
	for (int i = 0; i < mSize; i++)
	{
		if (v[i] != 1)
			notCovered++;
	}
	free(v);
	return notCovered;
}
void createSCPTeste(string fm, string fp)
{
	string arqm = "arqsTeste//"+fm+".txt";
	string arqp = "arqsTeste//" + fp + ".txt";
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
//	Grid* g = new Grid(meters, 0.005);
//	vector<Position*> vp = g->getCell(meters[100]);
	Requisition* req = new Requisition();
	req->setConfig(0, 1, 0, 0, 15, 3, 5, 1);
	req->setMeters(meters);
	req->setPoles(poles);
	req->getTestResponse(fm);
	//vector<vector<int>> scp = req->createScp();
	//cout << checkFeasibleTest(scp,meters.size());

	//const clock_t begin_time = clock();
	//req->createScp();
	//float dif = float(clock() - begin_time) / CLOCKS_PER_SEC;

	//const clock_t begin_time2 = clock();
	//req->createScpSemGrid();
	//float dif2 = float(clock() - begin_time2) / CLOCKS_PER_SEC;
	//printf("%d %d\n", meters.size(), poles.size());
	//printf("%f\n", dif);
	//printf("%f\n", dif2);

	//printf((req->getAutoPlanResponse()).c_str());
	//vector<vector<int>> SCP =  req->createScp();
	//vector<vector<int>> scp2 = createScpMatrix22(meters, poles, 0, 1, 0, 0, 15, 3, 5, 1);
	//saveGLPKFile2(scp2,poles,"glpk2.txt");
	//req->getAutoPlanResponse();
	//int r = checkFeasibleTest(SCP,meters.size());
//	printf("\n%d", r);
	
	
	//req = new Requisition();
	//req->setConfig(0, 1, 0, 0, 15, 3, 5, 1);
	//req->setMeters(meters);
	//req->setPoles(poles);
	//SCP = req->createScp();
	//r = checkFeasibleTest(SCP, meters.size());
	//printf("\n%d", r);
	//req->saveGLPKFile(SCP);
	//system("C:\\Users\\Guilherme\\Downloads\\glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");
//	delete req;


	
}
void createSCPTeste2(string fm, string fp)
{
	string arqm = "arqsTeste//" + fm + ".txt";
	string arqp = "arqsTeste//" + fp + ".txt";
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
		double lat = -1,a,b;
		double lng = -1;
		fscanf_s(file2, "%lf %lf", &lat, &lng);
		fscanf_s(file2, "%lf %lf",&a,&b);
		fscanf_s(file2, "%lf %lf", &a, &b);

		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}
	//	Grid* g = new Grid(meters, 0.005);
	//	vector<Position*> vp = g->getCell(meters[100]);
	Requisition* req = new Requisition();
	req->setConfig(3, 0, 0, 0, 15, 3, 5, 1);
	req->setMeters(meters);
	req->setPoles(poles);
	//req->getTestResponse(fm);
	req->getAutoPlanResponse();
	//delete req;



}
void createSCPTesteDeCobertura(string fm, string fp)
{
	string arqm = "arqsTeste//" + fm + ".txt";
	string arqp = "arqsTeste//" + fp + ".txt";
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
		double lat = -1, a, b;
		double lng = -1;
		fscanf_s(file2, "%lf %lf", &lat, &lng);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}
	//	Grid* g = new Grid(meters, 0.005);
	//	vector<Position*> vp = g->getCell(meters[100]);
	Requisition* req = new Requisition();
	req->setConfig(0, 0, 0, 0, 15, 3, 5, 1);
	req->setMeters(meters);
	req->setPoles(poles);

	FILE *fi;
	string n = "ns3files\\AutoPlanningResults" + fm + "MultiHopTest.txt";
	fopen_s(&fi, n.c_str(), "w");
	if (fi)
	{
		for (int i = 0; i < 6; i++)
		{
			req->setConfig(i, 0, 0, 0, 15, 3, 5, 1);
			vector<vector<int>> scp = req->createScp();
			vector<int> unc = req->uncoverableMeters(scp);
			fprintf_s(fi, "Meters sem cobertura: %d\n", unc.size());
		}
		
	}
	fclose(fi);

	//delete req;



}
void createSCPTesteDeCobertura2(string fm, string fp)
{
	string arqm = "arqsTeste//" + fm + ".txt";
	string arqp = "arqsTeste//" + fp + ".txt";
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
		double lat = -1, a, b;
		double lng = -1;
		fscanf_s(file2, "%lf %lf", &lat, &lng);
		fscanf_s(file2, "%lf %lf", &a, &b);
		fscanf_s(file2, "%lf %lf", &a, &b);
		if (lat == -1)
			break;
		Position *toAdd = new Position(lat, lng, poles.size());
		poles.push_back(toAdd);
	}
	//	Grid* g = new Grid(meters, 0.005);
	//	vector<Position*> vp = g->getCell(meters[100]);
	Requisition* req = new Requisition();
	req->setConfig(0, 0, 0, 0, 15, 3, 5, 1);
	req->setMeters(meters);
	req->setPoles(poles);

	FILE *fi;
	string n = "ns3files\\AutoPlanningResults" + fm + "MultiHopTest.txt";
	fopen_s(&fi, n.c_str(), "w");
	if (fi)
	{
		for (int i = 0; i < 6; i++)
		{
			req->setConfig(i, 0, 0, 0, 15, 3, 5, 1);
			vector<vector<int>> scp = req->createScp();
			vector<int> unc = req->uncoverableMeters(scp);
			fprintf_s(fi, "Meters sem cobertura: %d\n", unc.size());
		}

	}
	fclose(fi);

	//delete req;



}
int main(int argc, char* argv[])
{

	//int rs=0, cs=0;
	//vector<vector<int>> SCP = createSCPFromFile("scp410.txt", &rs, &cs);
	//saveGLPKFile2(SCP, cs, "SCP41.txt");
	//string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math SCP41.txt";
	//const clock_t begin_time = clock();
	//system(access.c_str());
	//double secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
//	vector<vector<int>> SCPfeasible;
//	for (int i = 0; i < SCP.size(); i++)
//	{
//		vector<int> toAdd;
//		for (int j = 0; j < SCP[i].size(); j++)
//		{
//			toAdd.push_back(SCP[i][j]);
//		}
//		if (SCP[i].size() > 0)
//		{
//			SCPfeasible.push_back(toAdd);
//		}
//}
	propagationTable();
	//createSCPTesteDeCobertura("filemeters1000", "filepoles1000");
	//createSCPTesteDeCobertura("filemeters2000", "filepoles2000");
	//createSCPTesteDeCobertura("filemeters3000", "filepoles3000");
	//createSCPTesteDeCobertura("filemeters4000", "filepoles4000");
	//createSCPTesteDeCobertura("filemeters5000", "filepoles5000");
	//createSCPTesteDeCobertura("filemeters10000", "filepoles10000");
	//createSCPTesteDeCobertura("filemeters15000", "filepoles15000");
	//createSCPTesteDeCobertura("filemeters9999999", "filepoles9999999");
//	createSCPTeste("filemeters5000", "filepoles5000");

	//createSCPTeste("filemeters4000", "filepoles4000");

	/*createSCPTeste("filemeters3000", "filepoles3000");
	createSCPTeste("filemeters2000", "filepoles2000");
	createSCPTeste("filemeters1000", "filepoles1000");
	createSCPTeste("filemeters10000", "filepoles10000");*/
	//createSCPTeste("filemeters15000", "filepoles15000");
	createSCPTeste("filemeters9999999", "filepoles9999999");
	//scanf_s("%d");
	return 0;
	//createSCPTesteDeCobertura2("metersInstanciaPequena1576", "polesInstanciaPequena340");
	//createSCPTesteDeCobertura2("metersInstanciaMédia3666", "polesInstanciaMédia773");

//	createSCPTeste("filemeters4000", "filepoles4000");
//	createSCPTeste("filemeters3000", "filepoles3000");
//	createSCPTeste("filemeters2000", "filepoles2000");
	//createSCPTeste("filemeters1000", "filepoles1000");
	//createSCPTeste("filemeters9999999", "filepoles9999999");
	//createSCPTeste("metersInstanciaMédia3666", "polesInstanciaMédia773");


	//createSCPTeste2("metersInstanciaPequena1576", "polesInstanciaPequena340");
	//createSCPTeste2("metersInstanciaMédia3666", "polesInstanciaMédia773");
	//createSCPTeste("arqsTeste//TestezinhoMeters.txt", "arqsTeste//TestezinhoPoles.txt");
	//vector<vector<int>> cMatrix = coverageMatrix(SCP, cs);
	//saveGLPKFile(cMatrix, rs, cs);
	//vector<vector<int>> scp = { { 0, 1 }, { 0, 1 }, { 0, 1, 2 }, { 1, 2 }, { 0, 1, 2 }, { 0, 1 }, {} };
	//int nc, ns;
	//metaheuristic(scp, 3, &nc, &ns);

	//funcConversaoDadosHomma("dadoshommaconvertidos.csv",1000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 2000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 3000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 4000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 5000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 10000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 15000);
	//funcConversaoDadosHomma("dadoshommaconvertidos.csv", 9999999);
//	double wow = getHataSRDSuccessRate(50, 0, 0, 0.25, 0, 3, 5, 1);
//	double wow2 = 1 - wow;
	{ //essas chaves tao aki por causa do teste do memory leak

		
		//createSCPTeste("arqsTeste//filemeters1000.txt", "arqsTeste//filepoles1000.txt");
//		glp_print_mip(problem, "broken_solution.txt");
//	glp_write_prob(problem, 0, "broken_glp.mod");
		//createSCPTeste("arqsTeste//filemeters9999999.txt", "arqsTeste//filepoles9999999.txt");

		//createSCPTeste();
		//functeste();
//		for (int i = 0; i < 200; i++)
//			double wow = getHataSRDSuccessRate(i, 0, 0, 0.25, 0, 3, 5, 1);

		//string answer = "";
		//Requisition *req = new Requisition();

		//answer = req->getResponse();
		//printf_s("%s",answer.c_str());
		//delete req;
	}

	FILE *pFile;
	freopen_s(&pFile, "MeMLeakTest.txt", "w", stdout);
	if (pFile)
	{

		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtDumpMemoryLeaks();
		
	}
	fclose(pFile);




	
	return 0;
}



