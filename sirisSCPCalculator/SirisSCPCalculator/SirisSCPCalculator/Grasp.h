
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
int clausesSatisfied(vector<vector<int>> &scp, int column);
int* initialSolution(int size);
int selectCandidate(vector<int> RCL);
vector<int> generateRCL(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int size, int* solution);
vector<vector<int>> copyScp(vector<vector<int>> &scp);
int* constructPhase(vector<vector<int>> scp, int size, int* solution);
void removeCovered(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int candidate, int* tam);
vector<vector<int>> coverageMatrix(vector<vector<int>> &scp, int size);
int* metaheuristic(vector<vector<int>> &scp, int size, int* cSatisfied, int* nColumns,double a);
vector<vector<int>> createSCPFromFile(const char* name, int* rowsSize, int* columnsSize);
void saveGLPKFile(vector<vector<int>> &SCP, int rs, int cs);