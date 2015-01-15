#include "Grid.h"


Grid::Grid(vector<Position*> v, vector<Position*> v2, double cS)
{
	//ESSES VETORES QUE SE PASSA NA CRIA��O DO GRID SERVEM PARA A DIVIS�O DAS C�LULAS, N�O NECESSARIAMENTE OS VETORES V�O SER INSERIDOS NO GRID
	//AS POSI��ES QUE SER�O ADICIONADAS NO GRID DEVEM SER ADICIONADAS PELO M�TODO putPositions();
	//POR QUE? PORQUE DURANTE O AUTO PLANEJAMENTO, POR EXEMPLO, EU TINHA UM GRID S� DE MEDIDORES E EU PRECISAVA PASSAR A POSI��O DUM DAP
	//PARA PEGAR A C�LULA, S� QUE ESSE DAP PODERIA ESTAR FORA DO GRID, PORTANTO, NESSE CASO AS C�LULAS DEVIAM CONSIDERAR AS POSI��ES DOS
	//DAPS TAMB�M, MAS S� HAVER� MEDIDORES NO GRID!


	if (cS <= 0)
		return;
	vector<Position*> vUnion;
	for (int i = 0; i < v.size(); i++)
		vUnion.push_back(v[i]);
	for (int i = 0; i < v2.size(); i++)
		vUnion.push_back(v2[i]);

	double mindx = getMinX(vUnion);
	double mindy = getMinY(vUnion);
	double maxdx = getMaxX(vUnion);
	double maxdy = getMaxY(vUnion);
	int nx = ceil((maxdx - mindx) / cS);
	int ny = ceil((maxdy - mindy) / cS);
	minX = mindx;
	minY = mindy;



	//cells = vector<vector<vector<Position*>>>(nx);
	//for (int i = 0; i < nx; i++)
	//	cells[i] = vector<vector<Position*>>(ny);
	cellSize = cS;
	//putPositions(v);
	//for (int i = 0; i < v.size(); i++)
	//	putPosition(v[i]);

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

	cellSize = cS;
	for (int i = 0; i < v.size(); i++)
		putPosition(v[i]);

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
void Grid::putPositions(vector<Position*> p)
{
	for(int i = 0; i < p.size();i++)
		putPosition(p[i]);
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
map< pair < int, int >, vector < Position* > > Grid::getCells()
{
	return cells;
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

