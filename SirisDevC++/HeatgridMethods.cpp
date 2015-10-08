#include "HeatgridMethods.h"

string Heatgrid::calculateHeatgridInfo()
{
	string ret = "";
	int size = 20; //20metros
	Grid *g = new Grid(points,points,size); 
	g->putPositions(points);
	map<pair<int, int>, vector<Position*> > cells = g->getCells();
	for (map<pair<int, int>, vector<Position*> >::iterator it = cells.begin(); it != cells.end(); ++it)
	{
		vector<Position*> points;
		points = it->second; //Recebe todas as posições dos pontos
		double bestVal=0;
		for (int i = 0; i < points.size(); i++)
		{
			if (points[i]->weight> bestVal)
				bestVal = points[i]->weight;
		}
		string edges = g->getCellEdgePositions(it->first);
		edges += "<>" + to_string(bestVal) + "/n";	
		ret += edges;
	}
	return ret;
}
string Heatgrid::executeHeatgrid()
{
	return calculateHeatgridInfo();
}
