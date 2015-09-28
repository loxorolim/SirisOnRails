#include "Grid.h"


Grid::Grid(vector<Position*> v, vector<Position*> v2, double cS)
{
	//ESSES VETORES QUE SE PASSA NA CRIAÇÃO DO GRID SERVEM PARA A DIVISÃO DAS CÉLULAS, NÃO NECESSARIAMENTE OS VETORES VÃO SER INSERIDOS NO GRID
	//AS POSIÇÕES QUE SERÃO ADICIONADAS NO GRID DEVEM SER ADICIONADAS PELO MÉTODO putPositions();
	//POR QUE? PORQUE DURANTE O AUTO PLANEJAMENTO, POR EXEMPLO, EU TINHA UM GRID SÓ DE MEDIDORES E EU PRECISAVA PASSAR A POSIÇÃO DUM DAP
	//PARA PEGAR A CÉLULA, SÓ QUE ESSE DAP PODERIA ESTAR FORA DO GRID, PORTANTO, NESSE CASO AS CÉLULAS DEVIAM CONSIDERAR AS POSIÇÕES DOS
	//DAPS TAMBÉM, MAS SÓ HAVERÁ MEDIDORES NO GRID!

	

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
	//minX = -90;
	//minY = -180;

	cellSizeMeters = cS;


}
void Grid::putPosition(Position* p)
{
	//se for igual ao min fazer algo...

	int posX = 0, posY = 0;
	if (p->latitude != minX)
		posX = ceil((p->latitude - minX)/((180*cellSizeMeters)/(M_PI*EARTH_RADIUS)))-1;
	if (p->longitude != minY)
		posY = ceil((p->longitude -minY)/((180*cellSizeMeters)/(M_PI*EARTH_RADIUS*cos((minX + (posX)*((180*cellSizeMeters)/(M_PI*EARTH_RADIUS)))*(M_PI/180)))))-1;
	//cells[posX][posY].push_back(p);

	cells[make_pair(posX, posY)].push_back(p);
}
void Grid::putPositions(vector<Position*> p)
{
	if (p.size())
	{
		for (int i = 0; i < p.size(); i++)
			putPosition(p[i]);
	}
}
vector<Position*> Grid::getCell(Position* reference)
{
	vector<Position*> ret;
	int posX = -1, posY = -1;
	//int posX = 0, posY = 0;
	if (reference->latitude != minX)
		posX = ceil((reference->latitude - minX) / ((180 * cellSizeMeters) / (M_PI*EARTH_RADIUS))) - 1;
	if (reference->longitude != minY)
		posY = ceil((reference->longitude - minY) / ((180 * cellSizeMeters) / (M_PI*EARTH_RADIUS*cos((minX + (posX)*((180 * cellSizeMeters) / (M_PI*EARTH_RADIUS)))*(M_PI / 180))))) - 1;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			map<pair<int, int>, vector<Position*> >::iterator it = cells.find(make_pair(posX + i, posY + j));
			if (it != cells.end())
				ret.insert(ret.end(), it->second.begin(), it->second.end());
		}
	}
	return ret;
}
vector<Position*> Grid::getUniqueCell(Position* reference)
{
	vector<Position*> ret;
	int posX = -1, posY = -1;
	//int posX = 0, posY = 0;
	if (reference->latitude != minX)
		posX = ceil((reference->latitude - minX) /((180*cellSizeMeters)/(M_PI*EARTH_RADIUS)))-1;
	if (reference->longitude != minY)
		posY = ceil((reference->longitude - minY) / ((180*cellSizeMeters)/(M_PI*EARTH_RADIUS*cos((minX + (posX)*((180*cellSizeMeters)/(M_PI*EARTH_RADIUS)))*(M_PI/180)))))-1;
	if (posX == -1 || posY == -1)
		 return ret;

	return cells[make_pair(posX, posY)];

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

string Grid::getCellsTeste()
{
	string ret = "";
	for (map<pair<int, int>, vector<Position*> >::iterator it = cells.begin(); it != cells.end(); ++it)
	{
		pair<int,int> cellNum = it->first;
		int numLat = cellNum.first;
		int numLng = cellNum.second;
		double posX = minX + (numLat)*getLatOfDistance(cellSizeMeters);
		double posY = minY + (numLng)*getLongOfDistance(posX,cellSizeMeters);

		double posX2 = posX+getLatOfDistance(cellSizeMeters);
		double posY2 = posY + getLongOfDistance(posX2,cellSizeMeters);



		ret += to_string(posX)+";"+to_string(posY) + "<>" + to_string(posX2)+";"+to_string(posY2) + "/n";
	}
	return ret;


}
string Grid::getCellEdgePositions(pair<int, int> pos)
{
	string ret = "";
	int numLat = pos.first;
	int numLng = pos.second;
	double posX = minX + (numLat)*getLatOfDistance(cellSizeMeters);
	double posY = minY + (numLng)*getLongOfDistance(posX, cellSizeMeters);
	double posX2 = posX + getLatOfDistance(cellSizeMeters);
	double posY2 = posY + getLongOfDistance(posX2, cellSizeMeters);

	ret += to_string(posX) + ";" + to_string(posY) + "<>" + to_string(posX2) + ";" + to_string(posY2);
	
	return ret;


}