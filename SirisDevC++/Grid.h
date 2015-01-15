#ifndef _GRID_H
#define _GRID_H
#include <vector>
#include <map>
#include "auxiliars.h"
#include <math.h>
using namespace std;

class Grid
{
	private:
		map<pair<int, int>, vector<Position*> > cells;
		//vector<vector<vector<Position*>>> cells;
		double cellSize,minX,minY;
		

	public:
		Grid(vector<Position*> v,vector<Position*> v2, double size);
		Grid(vector<Position*> v, double size);
		void putPosition(Position* p);
		void putPositions(vector<Position*> ps);
		vector<Position*> getCell(Position* reference);
		map<pair<int, int>, vector<Position*> > getCells();
};
double getMinX(vector<Position*> v);
double getMinY(vector<Position*> v);
double getMaxX(vector<Position*> v);
double getMaxY(vector<Position*> v);





#endif