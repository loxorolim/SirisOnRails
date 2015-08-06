#ifndef _GRASP_H
#define _GRASP_H
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

typedef struct solut{
	int clausesSatisfied;
	int numberOfColumns;
	int* solution;

}sol;

typedef struct tuple
{
	int pos, cSatisfied;
}duple;

class Grasp
{
	private:
		int  iterations = 100;
		float alpha = 0.8;
		int matrixinte = 0;

	public:
		int clausesSatisfied(vector<vector<int>> &scp, int column);
		int* initialSolution(int size);
		int selectCandidate(vector<int> RCL);
		vector<int> generateRCL(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int size, int* solution);
		vector<vector<int>> copyScp(vector<vector<int>> &scp);
		int* constructPhase(vector<vector<int>> scp, int size, int* solution);
		void removeCovered(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int candidate, int* tam);
		vector<vector<int>> coverageMatrix(vector<vector<int>> &scp, int size);
		int* metaheuristic(vector<vector<int>> &scp, int size, int* cSatisfied, int* nColumns);
		vector<int> greedyheuristic(vector<vector<int>> &scp, int size);
}

#endif