#include "Grid.h"


Grid::Grid(vector<Position*> v, vector<Position*> v2, double cS)
{
	//ESSES VETORES QUE SE PASSA NA CRIAÇÃO DO GRID SERVEM PARA A DIVISÃO DAS CÉLULAS, NÃO NECESSARIAMENTE OS VETORES VÃO SER INSERIDOS NO GRID
	//AS POSIÇÕES QUE SERÃO ADICIONADAS NO GRID DEVEM SER ADICIONADAS PELO MÉTODO putPositions();
	//POR QUE? PORQUE DURANTE O AUTO PLANEJAMENTO, POR EXEMPLO, EU TINHA UM GRID SÓ DE MEDIDORES E EU PRECISAVA PASSAR A POSIÇÃO DUM DAP
	//PARA PEGAR A CÉLULA, SÓ QUE ESSE DAP PODERIA ESTAR FORA DO GRID, PORTANTO, NESSE CASO AS CÉLULAS DEVIAM CONSIDERAR AS POSIÇÕES DOS
	//DAPS TAMBÉM, MAS SÓ HAVERÁ MEDIDORES NO GRID!

	//OBS: ACHO QUE VOU MANDER O PADRÃO DO MINX SEMPRE -90 E O MINY SEMPRE -180 QUE SÃO OS VALORES MINIMOS DA LATITUDE LONGITUDE! ASSIM JÁ PADRONIZA SEMPRE.


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
	minX = -90;
	minY = -180;



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
	minX = -90;
	minY = -180;

	cellSize = cS;
	for (int i = 0; i < v.size(); i++)
		putPosition(v[i]);

}
Grid::Grid(double cS)
{
	if (cS <= 0)
		return;

	minX = -90;
	minY = -180;

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
	for(int i = 0; i < p.size();i++)
		putPosition(p[i]);
}
vector<Position*> Grid::getCell(Position* reference)
{
	cout<< "Pegando celulas da referencia: " << reference->latitude << " ; " << reference->longitude;
	vector<Position*> ret;

	vector<Position*> aux;
	//PARTE CENTRAL
	Position* auxRef = new Position(reference->latitude,reference->longitude);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude + getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude - getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	//PARTE DO TOPO
	auxRef->latitude = reference->latitude + getLatOfDistance(cellSizeMeters);
	auxRef->longitude = reference->longitude;

	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude + getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude - getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;
	//PARTE DE BAIXO

	auxRef->latitude = reference->latitude - getLatOfDistance(cellSizeMeters);
	auxRef->longitude = reference->longitude;

	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude + getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	auxRef->longitude = reference->longitude - getLongOfDistance(auxRef->latitude,cellSizeMeters);
	aux = getUniqueCell(auxRef);
	ret.insert(ret.end(), aux.begin(), aux.end());
	cout<< "Pegando celula da AuxRef: " << auxRef->latitude << " ; " << auxRef->longitude;

	delete auxRef;

	return ret;
	//return cells[posX][posY];
//	return cells[make_pair(posX, posY)];
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
double getLongOfDistance(double lat, double distance)
{
	return ((180*distance)/(M_PI*EARTH_RADIUS*cos(lat*(M_PI/180))));
}
double getLatOfDistance(double distance)
{
	return ((180*distance)/(M_PI*EARTH_RADIUS));
}

