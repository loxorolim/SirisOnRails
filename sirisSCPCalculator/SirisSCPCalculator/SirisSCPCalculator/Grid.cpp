#include "stdafx.h"
#include "Grid.h"


Grid::Grid(vector<Position*> v, double cS)
{
	if (cS <= 0)
		return;
	double mindx = getMinX(v);
	double mindy = getMinY(v);
	double maxdx = getMaxX(v);
	double maxdy = getMaxY(v);
	int nx = ceil((maxdx - mindx) / cS);
	int ny = ceil((maxdy - mindy) / cS);
	minX = mindx;
	minY = mindy;



	//cells = vector<vector<vector<Position*>>>(nx);
	//for (int i = 0; i < nx; i++)
	//	cells[i] = vector<vector<Position*>>(ny);
	cellSize = cS;
	for (int i = 0; i < v.size(); i++)
		putPosition(v[i]);

	//FILE* f;
	//fopen_s(&f, "gridTeste.txt", "w");
	//if (f)
	//{
	//	for (map<pair<int, int>, vector<Position*>>::iterator it = cells.begin(); it != cells.end(); ++it)
	//	{

	//		fprintf_s(f, "%lf %lf\n", mindx + it->first.first*cS, mindy + it->first.second*cS);
	//		fprintf_s(f, "%lf %lf\n", mindx + it->first.first*cS + cS, mindy + it->first.second*cS + cS);
	//		fprintf_s(f, "%lf %lf\n", mindx + it->first.first*cS + cS, mindy + it->first.second*cS);
	//		fprintf_s(f, "%lf %lf\n", mindx + it->first.first*cS, mindy + it->first.second*cS+ cS);
	//	}
	//}
	//fclose(f);

}
void Grid::putPosition(Position* p)
{
	//se for igual ao min fazer algo...

	int posX = 0, posY = 0;
	if (p->latitude != minX)
		posX = ceil((p->latitude - minX)/cellSize)-1;
	if (p->longitude != minY)
		posY = ceil((p->longitude -minY)/cellSize)-1;
	//cells[posX][posY].push_back(p);

	cells[make_pair(posX, posY)].push_back(p);
}
vector<Position*> Grid::getCell(Position* reference)
{
	vector<Position*> ret;
	int posX = 0, posY = 0;
	if (reference->latitude != minX)
		posX = ceil((reference->latitude - minX) / cellSize) - 1;
	if (reference->longitude != minY)
		posY = ceil((reference->longitude - minY) / cellSize) - 1;
	if (posX == -1 || posY == -1)
		 return ret;
	
	vector<Position*> aux;
	aux = cells[make_pair(posX - 1, posY-1)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX - 1, posY)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX - 1, posY + 1)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX, posY - 1)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX, posY)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX, posY + 1)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX + 1, posY - 1)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX + 1, posY)];
	ret.insert(ret.end(), aux.begin(), aux.end());
	aux = cells[make_pair(posX + 1, posY + 1)];
	ret.insert(ret.end(), aux.begin(), aux.end());

	return ret;
	//return cells[posX][posY];
//	return cells[make_pair(posX, posY)];
}
double getMinX(vector<Position*> v)
{
	double dx = v[0]->latitude;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i]->latitude < dx)
			dx = v[i]->latitude;
	}
	return dx;

}
double getMinY(vector<Position*> v)
{
	double dy = v[0]->longitude;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i]->longitude < dy)
			dy = v[i]->longitude;
	}
	return dy;
}
double getMaxX(vector<Position*> v)
{
	double dx = v[0]->latitude;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i]->latitude > dx)
			dx = v[i]->latitude;
	}
	return dx;

}
double getMaxY(vector<Position*> v)
{
	double dy = v[0]->longitude;
	for (int i = 1; i < v.size(); i++)
	{
		if (v[i]->longitude > dy)
			dy = v[i]->longitude;
	}
	return dy;
}