//#include "stdafx.h"
//#include "Grasp.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <vector>
//
//using namespace std;
//
//int teste = 0;
//
//void evaluateSolution(vector<vector<int>> &scp, int * solution, int size, int* cSatisfied, int* numColumns)
//{
//	int num = 0;
//	int numC = 0;
//	for (int i = 0; i < scp.size(); i++)
//	{
//		for (int j = 0; j < scp[i].size(); j++)
//		{
//
//			for (int k = 0; k < size; k++)
//			{
//				if (solution[k] == 1)
//				{
//
//					if (scp[i][j] == k)
//					{
//						j = scp[i].size();
//						num++;
//
//						break;
//					}
//				}
//			}
//
//		}
//
//	}
//	for (int i = 0; i < size; i++)
//	if (solution[i])
//		numC++;
//
//	*numColumns = numC;
//	*cSatisfied = num;
//}
//
//int* counterArray(vector<vector<int>> &cMatrix, sol* solution, int rowSize, int size)
//{
//	int* cArray = (int*)malloc(sizeof(int)*rowSize);
//	for (int i = 0; i < rowSize; i++)
//	{
//		cArray[i] = 0;
//	}
//	for (int i = 0; i < size; i++)
//	{
//
//		if (solution->solution[i])
//		{
//			solution->numberOfColumns++;
//			for (int j = 0; j < cMatrix[i].size(); j++)
//			{
//				if (cArray[cMatrix[i][j]] == 0)
//					solution->clausesSatisfied++;
//				cArray[cMatrix[i][j]]++;
//
//			}
//		}
//	}
//	return cArray;
//}
//void BestFlip(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int* solut, int size, int* csat, int* ncol)
//{
//	sol* solution = (sol*)malloc(sizeof(sol));
//	solution->solution = solut;
//	solution->clausesSatisfied = 0;
//	solution->numberOfColumns = 0;
//	int * cArray = counterArray(cMatrix, solution, scp.size(), size);
//
//	vector<int> bestFlips;
//	bestFlips.push_back(-1);
//	int bestClausesSatisfied = solution->clausesSatisfied;
//	int bestNumberOfColumns = solution->numberOfColumns;
//	//	int first = 1;
//	for (int i = 0; i < size; i++)
//	{
//
//
//		solution->solution[i] = !(solution->solution[i]);
//		//		int isBetter = 0;
//
//		if (solution->solution[i]) //era 0
//		{
//
//			int satisfied = solution->clausesSatisfied;
//			for (int j = 0; j < cMatrix[i].size(); j++)
//			{
//				if (cArray[cMatrix[i][j]] == 0)
//				{
//					satisfied++;
//				}
//			}
//			if (satisfied > bestClausesSatisfied)
//			{
//				bestFlips.clear();
//				bestFlips.push_back(i);
//				bestClausesSatisfied = satisfied;
//				bestNumberOfColumns = solution->numberOfColumns + 1;
//			}
//			else if (satisfied == bestClausesSatisfied)
//			{
//				if (solution->numberOfColumns + 1 == bestNumberOfColumns)
//					bestFlips.push_back(i);
//			}
//
//		}
//		else //era 1
//		{
//			int satisfied = solution->clausesSatisfied;
//			for (int j = 0; j < cMatrix[i].size(); j++)
//			{
//				if (cArray[cMatrix[i][j]] == 1)
//				{
//					satisfied--;
//				}
//			}
//			if (satisfied == bestClausesSatisfied)
//			{
//				if (solution->numberOfColumns - 1 < bestNumberOfColumns)
//				{
//					bestFlips.clear();
//					bestFlips.push_back(i);
//					bestClausesSatisfied = satisfied;
//					bestNumberOfColumns = solution->numberOfColumns - 1;
//				}
//				if (solution->numberOfColumns - 1 == bestNumberOfColumns)
//					bestFlips.push_back(i);
//			}
//		}
//		solution->solution[i] = !(solution->solution[i]);
//	}
//
//	int pos = rand() % bestFlips.size();
//	if (bestFlips[pos] != -1)
//		solution->solution[bestFlips[pos]] = !solution->solution[bestFlips[pos]];
//	*csat = bestClausesSatisfied;
//	*ncol = bestNumberOfColumns;
//	free(solution);
//	free(cArray);
//
//}
//
//
//void RandomFlip(int * solution, int size)
//{
//	int pos = rand() % size;
//	solution[pos] = !solution[pos];
//}
//
//int numOfColumnsInSolution(int * solution, int size)
//{
//	int ret = 0;
//	for (int i = 0; i < size; i++)
//	if (solution[i] == 1)
//		ret++;
//	return ret;
//}
//int clausesSatisfiedBySolution(vector<vector<int>> &scp, int * solution, int size)
//{
//	int num = 0;
//	for (int i = 0; i < scp.size(); i++)
//	{
//		for (int j = 0; j < scp[i].size(); j++)
//		{
//
//			for (int k = 0; k < size; k++)
//			{
//				if (solution[k] == 1)
//				{
//					if (scp[i][j] == k)
//					{
//						j = scp[i].size();
//						num++;
//
//						break;
//					}
//				}
//				teste++;
//
//			}
//
//		}
//
//	}
//	return num;
//}
//int isBetterSolution(vector<vector<int>> &scp, int * newSolution, int * solution, int size)
//{
//	//2 se são iguais
//	//1 se é melhor
//	//0 se é pior
//	int numNewSat = clausesSatisfiedBySolution(scp, newSolution, size);
//	int numSat = clausesSatisfiedBySolution(scp, solution, size);
//	if (numNewSat > numSat)
//		return 1;
//	else if (numNewSat < numSat)
//		return 0;
//	else if (numNewSat == numSat)
//	{
//		int a = numOfColumnsInSolution(newSolution, size);
//		int b = numOfColumnsInSolution(solution, size);
//		if (a > b)
//			return 0;
//		if (a < b)
//			return 1;
//		if (a == b)
//			return 2;
//	}
//}
//
//
//
//void WalkSat(vector<vector<int>> &scp, int * solution, int size)
//{
//	//	int* newSolution;
//	vector<vector<int>> cMatrix = coverageMatrix(scp, size);
//	int bestClausesSatisfied;
//	int bestNumberOfColumns;
//	evaluateSolution(scp, solution, size, &bestClausesSatisfied, &bestNumberOfColumns);
//	int* solCopy = (int*)malloc(sizeof(int)*size);
//	for (int i = 0; i < size; i++)
//		solCopy[i] = solution[i];
//	for (int i = 0; i < 10 * size; i++)
//	{
//
//		float r = rand() % 100 + 1;
//		r = r / 100;
//		if (r < p)
//		{
//			int cSatisfied;
//			int nColumns;
//			BestFlip(scp, cMatrix, solution, size, &cSatisfied, &nColumns);
//			if (cSatisfied > bestClausesSatisfied)
//			{
//				bestClausesSatisfied = cSatisfied;
//				bestNumberOfColumns = nColumns;
//				for (int i = 0; i < size; i++)
//					solCopy[i] = solution[i];
//			}
//			else if (cSatisfied == bestClausesSatisfied)
//			{
//				if (nColumns < bestNumberOfColumns)
//				for (int i = 0; i < size; i++)
//					solCopy[i] = solution[i];
//			}
//
//		}
//		else
//			RandomFlip(solution, size);
//
//
//
//		//		free(solution);
//		//		solution = newSolution;
//
//		//		printf("\n");
//
//	}
//	for (int i = 0; i < size; i++)
//		solution[i] = solCopy[i];
//	free(solCopy);
//	for (int i = 0; i < cMatrix.size(); i++)
//	{
//		cMatrix[i].clear();
//	}
//	cMatrix.clear();
//	//	return solution;
//}
//
//int* initialSolution(int size)
//{
//	int* ret = (int*)malloc(sizeof(int)*size);
//	int i;
//	for (i = 0; i < size; i++)
//		ret[i] = 0;
//	return ret;
//}
//
//vector<vector<int>> coverageMatrix(vector<vector<int>> &scp, int size)
//{
//	//int* cArray = (int*)malloc(sizeof(int)*size);
//	vector<vector<int>> cMatrix;
//	cMatrix.resize(size);
//
//	for (int i = 0; i < scp.size(); i++)
//	{
//		for (int j = 0; j < scp[i].size(); j++)
//		{
//			cMatrix[scp[i][j]].push_back(i);
//		}
//	}
//	return cMatrix;
//
//}
//void removeCovered(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int candidate, int* tam)
//{
//
//	//	int r = 0;
//	int newTam = *tam;
//	for (int i = 0; i < cMatrix[candidate].size(); i++)
//	{
//		int x = cMatrix[candidate][i];
//		scp[cMatrix[candidate][i]].clear();
//		newTam--;
//		//	scp.erase(scp.begin() + (cMatrix[candidate][i]-r));
//		//	r++;
//	}
//	*tam = newTam;
//}
//void eraseFromMatrix(vector<vector<int>> &cMatrix, int pos, int elem)
//{
//	//	printf(" %d",cMatrix[pos].size());
//	for (int i = 0; i < cMatrix[pos].size(); i++)
//	{
//		if (cMatrix[pos][i] == elem)
//		{
//			cMatrix[pos].erase(cMatrix[pos].begin() + i);
//			matrixinte++;
//			break;
//		}
//	}
//
//}
//void updateMatrix(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int candidate)
//{
//	//	printf("%d", cMatrix[candidate].size());
//	while (cMatrix[candidate].size())
//	{
//		int x = cMatrix[candidate][0];
//		//printf(" %d", scp[x].size());
//		for (int j = 0; j < scp[x].size(); j++)
//		{
//
//			eraseFromMatrix(cMatrix, scp[x][j], x);
//		}
//	}
//
//}
//
//int* constructPhase(vector<vector<int>> scp, int size, int* solution)
//{
//	//vector<vector<int>> scpCopy = copyScp(scp);
//	vector<vector<int>> cMatrix = coverageMatrix(scp, size);
//	int tam = scp.size();
//	while (tam > 0)
//	{
//		matrixinte = 0;
//		vector<int> RCL = generateRCL(scp, cMatrix, size, solution);
//		if (RCL.size() == 0)
//			break;
//		int cand = selectCandidate(RCL);
//		solution[cand] = 1;
//		vector<vector<int>> scpCopy = scp;
//		removeCovered(scp, cMatrix, cand, &tam);
//		updateMatrix(scpCopy, cMatrix, cand);
//		//printf("MATRIX INTER: %d",matrixinte);
//		//	printf("%d",tam);
//	}
//
//	return solution;
//}
//
//int selectCandidate(vector<int> RCL)
//{
//	int size = RCL.size();
//	return RCL[rand() % size];
//
//}
//vector<int> generateRCL(vector<vector<int>> &scp, vector<vector<int>> &cMatrix, int size, int* solution)
//{
//	//int * numSatisfied = (int*)malloc(sizeof(int)*size);
//	vector<duple> numSatisfied;
//	int max = 0;
//	for (int i = 0; i < size; i++)
//	{
//		if (solution[i] != 1)
//		{
//			//int s = clausesSatisfied(scp,i);
//			int s = cMatrix[i].size();
//			if (s > alpha*max && s != 0)
//			{
//				duple d;
//				d.pos = i;
//				d.cSatisfied = s;
//				numSatisfied.push_back(d);
//				if (s > max)
//					max = s;
//			}
//		}
//	}
//	float L = alpha*max;
//	vector<int> RCL;
//	for (int i = 0; i < numSatisfied.size(); i++)
//	{
//		if (numSatisfied[i].cSatisfied >= L)
//			RCL.push_back(numSatisfied[i].pos);
//	}
//	return RCL;
//}
//int clausesSatisfied(vector<vector<int>> scp, int column)
//{
//	int num = 0;
//	for (int i = 0; i < scp.size(); i++)
//	{
//		for (int j = 0; j < scp[i].size(); j++)
//		{
//			if (scp[i][j] == column)
//			{
//				num++;
//				break;
//			}
//		}
//	}
//	return num;
//}
//int* metaheuristic(vector<vector<int>> &scp, int size, int* cSatisfied, int* nColumns)
//{
//
//	int* solution = initialSolution(size);
//	int* newSolution;
//	double bestSolutionTime;
//	double totalTime;
//	for (int i = 0; i < iterations; i++)
//	{
//		newSolution = initialSolution(size);
//		newSolution = constructPhase(scp, size, newSolution);
//	    WalkSat(scp, newSolution, size);
//
//
//
//		if (isBetterSolution(scp, newSolution, solution, size) == 1) //MUDAAAAAAAR
//		{
//			free(solution);
//			solution = newSolution;
//			int cS = 0, nC = 0;
//			evaluateSolution(scp, solution, size, &cS, &nC);
//			//printf("%f \n", bestSolutionTime);
//			//printf("%d \n", cS);
//			//printf("%d \n", nC);
//		}
//		else
//		{
//			free(newSolution);
//		}
//
//
//	}
//	evaluateSolution(scp, solution, size, cSatisfied, nColumns);
//	return solution;
//}
//
//vector<int> greedyheuristic(vector<vector<int>> &scp, int size)
//{
//	vector<int> solution;
//	vector<vector<int>> cMatrix = coverageMatrix(scp, size);
//	int tam = scp.size();
//	while (tam > 0)
//	{
//		int max = 0;
//		int pos = 0;
//		for (int i = 0; i < cMatrix.size(); i++)
//		{
//			if (cMatrix[i].size() > max)
//			{
//				pos = i;
//				max = cMatrix[i].size();
//			}
//		}
//		vector<vector<int>> scpCopy = scp;
//		solution.push_back(pos);
//		removeCovered(scp, cMatrix, pos, &tam);
//		updateMatrix(scpCopy, cMatrix, pos);
//		//printf("MATRIX INTER: %d",matrixinte);
//		//	printf("%d",tam);
//	}
//	return solution;
//}
//
