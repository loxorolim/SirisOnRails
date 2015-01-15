#include "stdafx.h"
#include "Requisition.h"
#include "MetricsResponse.h"
#include "Grasp.h"
#include <time.h>
#include <sstream>
#include <windows.h>

double memTestCount = -1;
double timeTestCount = -1;

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool compareByLongitude(Position* a, Position *b)
{
	return a->longitude < b->longitude;
}
bool compareByLatitude(Position* a, Position *b)
{
	return a->latitude < b->latitude;
}
void executeGlpk(string filename)
{
	//string access = "C:\\Sites\\first_app\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
	string access = "C:\\Users\\Guilherme\\Documents\\GitHub\\SirisOnRails\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe  --math " + filename + " --memlim " + to_string(memlimit) + " > wow.txt";
	system(access.c_str());
}
float getMemUsageFromGlpkFile(string fname)
{
	ifstream f(fname.c_str());
	string str;
	//int cont = 1;
	float mem = -1;
	while (getline(f, str))
	{
		
		int c = str.find("Memory used: ");
		if (c >= 0)
		{
			//cont = 0;
			vector<string> s = split(str, ' ');
			int pos = -1;
			for (int i = 0; i < s.size(); i++)
			{
				int c2 = s[i].find("Mb");
				if (c2 >= 0)
				{
					pos = i;
					break;
				}
			}
			mem = stof(s[pos-1]);
		}
	}
	return mem;

}
float getTimeUsageFromGlpkFile(string fname)
{
	ifstream f(fname.c_str());
	string str;
	//int cont = 1;
	float time = -1;
	while (getline(f, str))
	{
		//getline(f, str);
		int c = str.find("Time used: ");
		if (c >= 0)
		{
		//	cont = 0;
			
			vector<string> s = split(str, ' ');
			int pos = -1;
			for (int i = 0; i < s.size(); i++)
			{
				int c2 = s[i].find("secs");
				if (c2 >= 0)
				{
					pos = i;
					break;
				}
			}
			time = stof(s[pos-1]);
		}
	}
	return time;

}
vector<Position*> Requisition::getActiveRegion(vector<Position*> &vSorted, Position* ref)
{

	int ind1 = binary_search(vSorted, ref->latitude - regionLimiter, 0);
	int ind2 = binary_search(vSorted, ref->latitude + regionLimiter, 0);
	vector<Position*>::const_iterator first = vSorted.begin() + ind1;
	vector<Position*>::const_iterator last = vSorted.begin() + ind2;
	vector<Position*> newVec(first, last);
	sort(newVec.begin(), newVec.end(), compareByLongitude);
	ind1 = binary_search(newVec, ref->longitude - regionLimiter, 1);
	ind2 = binary_search(newVec, ref->longitude + regionLimiter, 1);
	vector<Position*>::const_iterator first2 = newVec.begin() + ind1;
	vector<Position*>::const_iterator last2 = newVec.begin() + ind2;
	vector<Position*> newVec2(first2, last2);

	return newVec;
}
void Requisition::readPositions(int id)
{
	int pLength;
	scanf_s("%d", &pLength);
	for (int i = 0; i < pLength; i++)
	{
		double lat;
		double lng;
		scanf_s("%lf %lf", &lat, &lng);
		Position *toAdd = new Position(lat, lng,i);
		if (id == DAP)
			daps.push_back(toAdd);
		if (id == METER)
			meters.push_back(toAdd);
		if (id == POLE)
			poles.push_back(toAdd);	
	}
	//if(id == DAP)
	//	sort(daps.begin(), daps.end(), compareByLatitude);
	//if (id == METER)
	//	sort(meters.begin(), meters.end(), compareByLatitude);
	//if (id == POLE)
	//	sort(poles.begin(), poles.end(), compareByLatitude);
}

void Requisition::readConfiguration()
{
	
	scanf_s("%d", &option);
	scanf_s("%d", &scenario);
	scanf_s("%d", &technology);
	scanf_s("%lf", &H_TX);
	scanf_s("%lf", &H_RX);
	scanf_s("%lf", &BIT_RATE);
	scanf_s("%lf", &TRANSMITTER_POWER);
	scanf_s("%d", &SRD);
	scanf_s("%d", &meshEnabled);
	switch (option)
	{
	case AUTOPLAN:
		readPositions(METER);
		readPositions(POLE);
		break;
	case DRAW:
		readPositions(METER);
		readPositions(DAP);
		break;
	case METRIC:
		readPositions(METER);
		readPositions(DAP);
		break;
	case TEST:
		readPositions(METER);
		readPositions(POLE);		
		break;
	default:
		break;
	}
}

vector<int> concatVectors(vector<int> &v1, vector<int> &v2)
{
	vector<int> ret;
	for (int i = 0; i < v1.size(); i++)
		ret.push_back(v1[i]);
	for (int i = 0; i < v2.size(); i++)
	{
		bool add = true;
		for (int j = 0; j < v1.size(); j++)
		if (v1[j] == v2[i])
		{
			add = false;
			break;
		}
		if (add)
			ret.push_back(v2[i]);

	}
	return ret;

}
vector<vector<int>> Requisition::createScp()
{
	Grid* g = new Grid(meters,poles, regionLimiter);
	vector<int> aux;
	vector<vector<int>> sM;
	//sM.reserve(meters.size());

	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		//vector<Position*> metersReduced = getActiveRegion(meters, poles[i]);
		vector<Position*> metersReduced = g->getCell(poles[i]);
		//printf("%d - ", i);
		//for (int z = 0; z < metersReduced.size(); z++)
		//	printf("%d ", metersReduced[z]->index);
		//printf("\n");
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(poles[i], metersReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}

		//if (polesThatCover.length > 0)
		//if (i % 1000 == 0)
		//	printf("%d", i);

		sM.push_back(metersCovered);
	}
	if (meshEnabled)
	{

		vector<vector<int>> nM = createMeterNeighbourhood(g);
		int firstPos = meters[0]->index;
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> neighbours = sM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{

					int toFind = neighbours[k];
					int pos = 0;
					for (int i = 0; i < meters.size(); i++){ if (meters[i]->index == toFind){ pos = i; break; } }
					
					sM[i] = concatVectors(sM[i], nM[pos]);
					newNeighbours = concatVectors(newNeighbours, nM[pos]); //ESSA PARTE AQUI É PASSÍVEL DE OTIMIZAÇÃO! DIMINUIR NEWNEIGHBOURS DE SM[I]!!!
				}
				neighbours = newNeighbours;

			}
			
			//	printf("MESH%d", i);
		}

	}
	delete g;



	return sM;
}
vector<vector<int>> Requisition::createScpSemGrid()
{
	Grid* g = new Grid(meters, poles, 10);
	vector<int> aux;
	vector<vector<int>> sM;
	//sM.reserve(meters.size());

	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		//vector<Position*> metersReduced = getActiveRegion(meters, poles[i]);
		vector<Position*> metersReduced = g->getCell(poles[i]);
		//printf("%d - ", i);
		//for (int z = 0; z < metersReduced.size(); z++)
		//	printf("%d ", metersReduced[z]->index);
		//printf("\n");
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(poles[i], metersReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}

		//if (polesThatCover.length > 0)
		//if (i % 1000 == 0)
		//	printf("%d", i);

		sM.push_back(metersCovered);
	}
	if (meshEnabled)
	{

		vector<vector<int>> nM = createMeterNeighbourhood(g);// ESSE MÉTODO NÃO PODERIA ESTAR NO INICIO DO MÉTODO PARA QUE NÃO FOSSE CHAMADO VÁRIAS VEZES???!?!?!?
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> neighbours = sM[i];
			for (int j = 0; j < meshEnabled; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{
					sM[i] = concatVectors(sM[i], nM[neighbours[k]]);
					newNeighbours = concatVectors(newNeighbours, nM[neighbours[k]]);
				}
				neighbours = newNeighbours;

			}

			//	printf("MESH%d", i);
		}

	}
	delete g;



	return sM;
}
vector<vector<int>> Requisition::createMeterNeighbourhood(Grid *g)
{
	vector<vector<int>> M;

	for (int i = 0; i < meters.size(); i++) 
	{
		vector<int> pointsCovered;
		//vector<Position*> meterRegion = getActiveRegion(meters, meters[i]);
		vector<Position*> meterRegion = g->getCell(meters[i]);
		for (int j = 0; j < meterRegion.size(); j++)
		{
			double dist = getDistance(meters[i], meterRegion[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD);
			if (i != j && eff >= MARGIN_VALUE)
				pointsCovered.push_back(meterRegion[j]->index);

		}
		M.push_back(pointsCovered);
	}

	return M;
}
void Requisition::saveGLPKFile(vector<vector<int>> &SCP)
{
	FILE *file;
		//fopen_s(&file, filename.c_str(), "w");
	//	if (file == 0)
	//	{
	//		printf("Could not open file\n");
	//	}
	//	else
		{

			//TEM Q MUDAR ESSE NEGÓCIO AQUI!
		    vector<int> uncMeters = uncoverableMeters(SCP);
			string resp;
			resp += "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n";
			//fprintf_s(file, "%s", "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
			//ret += "set Z:= ";
			//fprintf_s(file, "set Z:= ");
			resp += "set Z:= ";
			for (int i = 0; i < meters.size(); i++)
			{
				int uncov = -1;
				uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
				if (!uncov)
					resp += "Z" + to_string(i + 1) + " ";
			}

			resp += ";\n";
			resp += "set Y:= ";

			for (int i = 0; i < poles.size(); i++)
				resp += "Y" + to_string(i + 1) + " ";

			resp += ";\n";

			resp += "param A : ";
	
			for (int i = 0; i < poles.size(); i++)
				resp += "Y" + to_string(i + 1) + " ";

			resp += ":= \n";
			for (int i = 0; i < meters.size(); i++) 
			{
				int uncov = -1;
				uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
				if (!uncov)
				{
					resp += "Z" + to_string(i + 1) + " ";
					//fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
					for (int j = 0; j < poles.size(); j++)
					{
						//bool um = false;
						//for (int k = 0; k < SCP.size(); k++)
						int cov = -1;
						cov = (find(SCP[j].begin(), SCP[j].end(), i) != SCP[j].end());
						if (cov)
							resp += "1 ";
						else
							resp += "0 ";

					}
				}
	
			}
			resp += "\n";
			resp += ";";
			resp += "end;\n";
		
			ofstream f("GlpkFile.txt");

			f << resp;
			f.close();

	
	
		}
	
}

void Requisition::saveGLPKFileReduced(vector<vector<int>> &SCP)
{

	     
		//TEM Q MUDAR ESSE NEGÓCIO AQUI!
		vector<int> uncMeters = uncoverableMeters(SCP);

		
		string resp;
		resp += "set Z;\n set Y;\n param A{r in Z, m in Y} default 0, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n";
		//fprintf_s(file, "%s", "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
		//ret += "set Z:= ";
		//fprintf_s(file, "set Z:= ");
		resp += "set Z:= ";
		for (int i = 0; i < meters.size(); i++)
		{
			int uncov = -1;
			uncov = (find(uncMeters.begin(), uncMeters.end(), meters[i]->index) != uncMeters.end());
			if (!uncov)
				resp += "Z" + to_string(meters[i]->index + 1) + " ";
		}
		
		resp += ";\n";
		resp += "set Y:= ";

		for (int i = 0; i < poles.size(); i++)
			resp += "Y" + to_string(poles[i]->index + 1) + " ";

		resp += ";\n";

		resp += "param A := ";

		//for (int i = 0; i < poles.size(); i++)
		//	resp += "Y" + to_string(i + 1) + " ";

		resp += "\n";
		for (int i = 0; i < meters.size(); i++)
		{
			int uncov = -1;
			uncov = (find(uncMeters.begin(), uncMeters.end(), meters[i]->index) != uncMeters.end());
			if (!uncov)
			{
				//fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
				for (int j = 0; j < poles.size(); j++)
				{
					//bool um = false;
					//for (int k = 0; k < SCP.size(); k++)
					int cov = -1;
					cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
					if (cov)
						resp += "[Z" + to_string(meters[i]->index + 1) + ",Y" + to_string(poles[j]->index + 1)+"] 1";

				}
			}

		}
		
		resp += "\n";
		resp += ";";
		resp += "end;\n";
		
		ofstream f("GlpkFile.txt");

		f << resp;
		f.close();

}
vector<int> Requisition::uncoverableMeters(vector<vector<int>> &SCP)
{
	vector<int> uncoverableMeters;
	for (int i = 0; i < meters.size(); i++)
	{
		int coverable = 0;
		for (int j = 0; j < poles.size(); j++)
		{
			int cov = -1;
			cov = (find(SCP[j].begin(), SCP[j].end(), meters[i]->index) != SCP[j].end());
			if (cov)
			{
				coverable = 1;
				break;
			}
		}
		if (!coverable)
			uncoverableMeters.push_back(meters[i]->index);
	}
	return uncoverableMeters;
}


void Requisition::dapsToNs3File(vector<vector<int>> &scp, vector<int> &chosenDaps)
{
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		Position* d = poles[chosenDaps[i]];
		vector<Position*> m;
		for (int j = 0; j < scp[chosenDaps[i]].size(); j++)
		{
			m.push_back(meters[scp[chosenDaps[i]][j]]);
		}
		vector<Position*> v;
		v.push_back(d);
		for (int z = 0; z < m.size(); z++)
			v.push_back(m[z]);

		double dx = v[0]->latitude;
		for (int z = 1; z < v.size(); z++)
		{
			if (v[z]->latitude < dx)
				dx = v[z]->latitude;
		}
		double dy = v[0]->longitude;
		for (int z = 1; z < v.size(); z++)
		{
			if (v[z]->longitude < dy)
				dy = v[z]->longitude;
		}
		FILE *f;
		
		string fname = "ns3files\\dapns3-" + to_string(i) + ".txt";
		//printf(fname.c_str());
		//printf("\n-----%lf %lf------", dx,dy);
		fopen_s(&f, fname.c_str(), "w");
		if (f)
		{
			//printf("\n-----%d------", v.size());
			fprintf_s(f, "%d\n", v.size());
			Position *ref = new Position(dx, dy);
			for (int z = 0; z < v.size(); z++)
			{
				Position *aux = new Position(v[z]->latitude, dy);
				Position *aux2 = new Position(dx, v[z]->longitude);
				//printf("\n-----%lf %lf------", getDistance(ref, aux), getDistance(ref, aux2));
				fprintf_s(f, "%lf %lf\n", getDistance(ref,aux),getDistance(ref,aux2));
				delete(aux);
				delete(aux2);
			}
			delete(ref);
			
		}
		fclose(f);

	}

}


string Requisition::gridAutoPlanning()
{
	Grid* metergrid = new Grid(meters, poles, regionLimiter);
	Grid* polegrid = new Grid(poles, meters, regionLimiter);
	vector<Position*> metersAux = meters, polesAux = poles;
	map<pair<int, int>, vector<Position*>> meterCells = metergrid->getCells();
	vector<string> chosenDaps;
	//string str;
	int i = 1;
	float maximummemusage = -1;
	float maximumtimeusage = -1;
	for (map<pair<int, int>, vector<Position*>>::iterator it = meterCells.begin(); it != meterCells.end(); ++it)
	{
	
		vector<Position*> cellsMeters;
		vector<Position*> cellsPoles;
		
		cellsMeters = it->second; //Recebe todas as posições dos medidores que estão na célula
		cellsPoles = polegrid->getCell(cellsMeters[0]);//Pega a posição de um desses medidores e usa como referência pra pegar os postes da mesma célula e das células vizinhas.
		meters = cellsMeters;
		poles = cellsPoles;

		//printf("\nGrid %d : %d medidores e %d postes", i++, cellsMeters.size(), cellsPoles.size());
		
		vector<vector<int>> cellSCP = createScp();
		saveGLPKFileReduced(cellSCP);
		//string access = "C:\\Sites\\first_app\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt --memlim " + to_string(memlimit);
		//system(access.c_str());
		executeGlpk("GlpkFile.txt");
		ifstream f("Results.txt");
		string gridAnswer;
		getline(f, gridAnswer);
		//str += gridAnswer;

		vector<string> x = split(gridAnswer, ' ');
		for (int i = 0; i < x.size(); i++)
		{
			//string snum = x[i].substr(1);
			chosenDaps.push_back(x[i]);
		}
		float memusage = getMemUsageFromGlpkFile("wow.txt");
		float timeusage = getTimeUsageFromGlpkFile("wow.txt");
		if (memusage > maximummemusage)
		{
			maximummemusage = memusage;
		}
		if (timeusage > maximumtimeusage)
		{
			maximumtimeusage = timeusage;
		}

	}
	string str = "";
	//Remove redundâncias
	sort(chosenDaps.begin(), chosenDaps.end());
	chosenDaps.erase(unique(chosenDaps.begin(), chosenDaps.end()), chosenDaps.end());
	for (int i = 0; i < chosenDaps.size(); i++)
	{
		str += chosenDaps[i] + " ";
	}

	delete metergrid;
	delete polegrid;
	meters = metersAux;
	poles = polesAux;
	memTestCount = maximummemusage;
	timeTestCount = maximumtimeusage;
	return str;



	
}
string Requisition::getAutoPlanResponse()
{
	
	//return gridAutoPlanning();
	vector<vector<int>> SCP = createScp();
	//for (int i = 0; i < SCP.size(); i++)
	//{
	//	printf("%d - ", i);
	//	for (int j = 0; j < SCP[i].size(); j++)
	//		printf("%d", SCP[i][j]);
	//	printf("\n");
	//}
	//saveGLPKFile(SCP);
	saveGLPKFileReduced(SCP);
	
	double seconds;
	double clo = clock();
	//time_t timerini, timerend;
	//time(&timerini);
	//system("\\glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");
	//system("C:\\Sites\\first_app\\sirisSCPCalculator\\SirisSCPCalculator\\SirisSCPCalculator\\glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");

	executeGlpk("GlpkFile.txt");
	getMemUsageFromGlpkFile("wow.txt");
	//time(&timerend);
	seconds = (clock() - clo)/1000;
	FILE *fi;
	fopen_s(&fi, "ns3files\\AutoPlanningResults.txt", "w");
	if (fi)
	{
		//printf("\n-----%d------", v.size());
		fprintf_s(fi, "%f\n", seconds);
		
	}
	fclose(fi);
	ifstream f("Results.txt");
	string str;
	getline(f, str);
	vector<string> x = split(str, ' ');
	vector<int> chosenDaps;
	for (int i = 0; i < x.size(); i++)
	{
		string snum = x[i].substr(1);
		
		chosenDaps.push_back(stoi(snum)-1);
	}
	dapsToNs3File(SCP, chosenDaps);


	
	//string file_contents;
	//while (getline(f, str))
	//{
	//	file_contents += str;
	//	file_contents.push_back('\n');
	//}
	return str;
}

string Requisition::exactAutoPlanning()
{
	vector<vector<int>> SCP = createScp();
	//vector<vector<int>> SCP2 = createScpSemGrid();
	saveGLPKFileReduced(SCP);
	//system("glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");
	executeGlpk("GlpkFile.txt");
	float memusage =  getMemUsageFromGlpkFile("wow.txt");
	float timeusage = getTimeUsageFromGlpkFile("wow.txt");
	ifstream f("Results.txt");
	string str;
	getline(f, str);
	memTestCount = memusage;
	timeTestCount = timeusage;
	return str;

	
}

void doGridTest(Requisition *req,double gridsize, FILE *fi)
{

	memTestCount = -1;
	timeTestCount = -1;
	//regionLimiter *= 10;

	req->setRegionLimiter(gridsize);
	Position* aux = new Position(0, 0);
	Position* aux2 = new Position(0 + gridsize, 0);
	Position* aux3 = new Position(0, 0 + gridsize);
	double secondsgp = -1;
	//double clogp = clock();
	const clock_t begin_time = clock();
	string strgp = req->gridAutoPlanning();
	secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
	//secondsgp = clock() - clogp;
	fprintf_s(fi, "Grid height: %f \n Grid width: %f \n\nGrid planning solution time: %f\n\n", getDistance(aux, aux2), getDistance(aux, aux3), secondsgp);
	fprintf_s(fi, "Maximum Memory Used: %f\n\n", memTestCount);
	fprintf_s(fi, "Maximum Time Used: %f\n\n", timeTestCount);
	vector<Position*> daps;
	vector<Position*> poles = req->getPoles();
	vector<string> xgp = split(strgp, ' ');
	for (int i = 0; i < xgp.size(); i++)
	{
		string snum = xgp[i].substr(1);
		daps.push_back(poles[stoi(snum) - 1]);
	}
	req->setDAPs(daps);
	string result = "Número de daps: " + to_string(daps.size());
	fprintf(fi, result.c_str());
	//string resultgp = req->getMetricResponse();
	//fprintf(fi, resultgp.c_str());

	

	fprintf(fi, "------------------------------------------------------------------\n");
}

void Requisition::getTestResponse(string fname)
{

	FILE *fi;
	string n = "ns3files\\AutoPlanningResults" + fname + ".txt";
	fopen_s(&fi, n.c_str(), "w");
	if (fi)
	{

		fprintf_s(fi, "Meters Number: %d\n", meters.size());
		fprintf_s(fi, "Poles Number: %d\n",poles.size());
//
//		doGridTest(this, 0.001, fi);
//		doGridTest(this, 0.005, fi);
		meshEnabled = 0;
		scenario = 0;
		doGridTest(this, 0.01, fi);
		scenario = 1;
		doGridTest(this, 0.01, fi);
		scenario = 2;
		doGridTest(this, 0.01, fi);
		meshEnabled = 3;
		scenario = 0;
		doGridTest(this, 0.01, fi);
		scenario = 1;
		doGridTest(this, 0.01, fi);
		scenario = 2;
		doGridTest(this, 0.01, fi);

//		doGridTest(this, 0.1, fi);
//		//FAZ PELO MÉTODO GRID PLANNING
//		double regionLimiter = 0.005;
////		while (regionLimiter <= 0.01)
////		{
//			daps.clear();
//			memTestCount = -1;
//			timeTestCount = -1;
//			//regionLimiter *= 10;
//			setRegionLimiter(regionLimiter);
//			Position* aux = new Position(0, 0);
//			Position* aux2 = new Position(0 + regionLimiter, 0);
//			Position* aux3 = new Position(0, 0+regionLimiter);
//			double secondsgp = -1;
//			//double clogp = clock();
//			const clock_t begin_time = clock();
//			string strgp = gridAutoPlanning();
//			secondsgp = float(clock() - begin_time) / CLOCKS_PER_SEC;
//			//secondsgp = clock() - clogp;
//			fprintf_s(fi, "Grid height: %f \n Grid width: %f \n\nGrid planning solution time: %f\n\n",getDistance(aux,aux2),getDistance(aux,aux3), secondsgp);
//			fprintf_s(fi, "Maximum Memory Used: %f\n\n", memTestCount);
//			fprintf_s(fi, "Maximum Time Used: %f\n\n", timeTestCount);
//
//			vector<string> xgp = split(strgp, ' ');
//			for (int i = 0; i < xgp.size(); i++)
//			{
//				string snum = xgp[i].substr(1);
//				daps.push_back(poles[stoi(snum) - 1]);
//			}
//			string resultgp = getMetricResponse();
//			fprintf(fi, resultgp.c_str());
//			fprintf(fi, "------------------------------------------------------------------\n");


//		}
		////FAZ PELO MÉTODO EXATO
		//daps.clear();
		//double seconds = -1;
		////double clo = clock();


		//memTestCount = -1;
		//timeTestCount = -1;
		//setRegionLimiter(0.001);
		//const clock_t begin_time2 = clock();
		//string str = exactAutoPlanning();
		//seconds = float(clock() - begin_time2) / CLOCKS_PER_SEC;

		////seconds = clock() - clo;
		//fprintf_s(fi, "Optimal solution time: %f\n\n", seconds);
		//fprintf_s(fi, "Maximum Memory Used: %f\n\n", memTestCount);
		//fprintf_s(fi, "Maximum Time Used: %f\n\n", timeTestCount);

		//vector<string> x = split(str, ' ');
		//for (int i = 0; i < x.size(); i++)
		//{
		//	string snum = x[i].substr(1);
		//	daps.push_back(poles[stoi(snum) - 1]);
		//}
		//string result = getMetricResponse();
		//fprintf(fi, result.c_str());
		//fprintf(fi, "------------------------------------------------------------------\n");
		
	}
	fclose(fi);
	//vector<vector<int>> SCP = createScp();
	//getExactSol(SCP);
	//getGraspSol(SCP);
	//getGreedySol(SCP);

	//dapsToNs3File(SCP, chosenDaps);




}
vector<Position*> removeVectorFromAnother(vector<Position*> &v1, vector<Position*> &v2)
{
	//remove v2 do v1
	vector<Position*> ret;
	for (int i = 0; i < v1.size(); i++)
	{
		auto it = find(v2.begin(), v2.end(), v1[i]);
		if (it == v2.end())
		{
			ret.push_back(v1[i]);
		}
	}
	return ret;
}
vector<vector<sComponent*>> Requisition::statisticalList()
{
	Grid* g = new Grid(meters, poles, regionLimiter);
	vector<vector<sComponent*>> sL;
	for (int i = 0; i < daps.size(); i++)
	{
		vector<sComponent*> toAdd;
		vector<Position*> toCompare = g->getCell(daps[i]);
		for (int j = 0; j < toCompare.size(); j++)
		{
			double dist = getDistance(daps[i], meters[j]);
			double efficiency = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);

			if (efficiency >= MARGIN_VALUE){
				sComponent* component = new sComponent(j, dist, efficiency, 0, NULL);
				toAdd.push_back(component);
			}
		}
		if (meshEnabled)
		{
			//Grid* g = new Grid(meters,daps, regionLimiter);
			vector<vector<int>> nM = createMeterNeighbourhood(g);
			//vector<sComponent*> neighbours = toAdd;
			vector<int> neighbours;
			for (int h = 0; h < toAdd.size(); h++)
				neighbours.push_back(toAdd[h]->index);


			for (int k = 0; k < meshEnabled; k++)
			{
				vector<int> newNeighbourhood;
				vector<sComponent*> meshToAdd;
				for (int j = 0; j < neighbours.size(); j++)
				{

					vector<int> aux = nM[neighbours[j]];
					for (int l = 0; l < aux.size(); l++)
					{

						if (!(contains(toAdd, aux[l])))
						{
							sComponent* father = NULL;
							for (int z = 0; z < toAdd.size(); z++){
								if (toAdd[z]->index == neighbours[j]){
									father = toAdd[z];
									break;
								}
							}

							double distBetweenMeters = getDistance(meters[father->index], meters[aux[l]]);
							double effBetweenMeters = getHataSRDSuccessRate(distBetweenMeters, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
							sComponent* meshComponent = new sComponent(aux[l], distBetweenMeters, effBetweenMeters, k + 1, father);

							meshToAdd.push_back(meshComponent);
							//meshToAdd = meshToAdd.concat(meshComponent);
							newNeighbourhood.insert(newNeighbourhood.end(), nM[neighbours[j]].begin(), nM[neighbours[j]].end());
							//newNeighbourhood = newNeighbourhood.concat(nM[neighbours[j].index]);
						}

					}


				}
				/*	neighbours = newNeighbourhood.filter(function(elem, pos) {
				return newNeighbourhood.indexOf(elem) == pos;
				});*/
				neighbours = removeRepeated(newNeighbourhood);
				//				deleteVector(neighbours);
				//	neighbours = newNeighbourhood;
				toAdd.insert(toAdd.end(), meshToAdd.begin(), meshToAdd.end());
				//toAdd = toAdd.concat(meshToAdd);
				
			}
			delete g;
		}
		sL.push_back(toAdd);
	}
	return sL;
}

DrawInfo* chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, int scenario, int technology, double BIT_RATE, double TRANSMITTER_POWER, double H_TX, double H_RX, bool SRD)
{
	double minDist = -1;
	Position* meterToConnect = NULL;
	for (int i = 0; i < connectedMeters.size(); i++)
	{
		double dist = getDistance(meter, connectedMeters[i]);
		if (dist < minDist || minDist == -1)
		{
			minDist = dist;
			meterToConnect = connectedMeters[i];
		}
	}
	if (minDist != -1)
	{
		double dist = getDistance(meter, meterToConnect);
		double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
		if (effs >= MARGIN_VALUE) {
			DrawInfo* ret = new DrawInfo(meter, meterToConnect, effs, dist, 1);
			return ret;
		}
	}
	return NULL;
}
vector<DrawInfo*> Requisition::calculateDrawingInfo()
{


	vector<DrawInfo*> toCover;
	vector<Position*> coveredMeters;
	vector<Position*> allMarkers;
	allMarkers.insert(allMarkers.end(), meters.begin(), meters.end());

	for (int d = 0; d < daps.size(); d++)
	{
		for (int i = 0; i < allMarkers.size(); i++) {
			double dist = getDistance(daps[d], allMarkers[i]);
			double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (effs >= MARGIN_VALUE)
			{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO

				DrawInfo* toAdd = new DrawInfo(daps[d], allMarkers[i], effs, dist, 0);
				toCover.push_back(toAdd);
				coveredMeters.push_back(allMarkers[i]);
			}
		}
		//toCover = toCover.sort(function(a, b) { return a.value.distance - b.value.distance });
		//for (int i = 0; i < toCover.size(); i++)
		//	this.connect(toCover[i].marker, toCover[i].value);
	}
	if (meshEnabled)
	{
		sort(coveredMeters.begin(), coveredMeters.end());
		coveredMeters.erase(unique(coveredMeters.begin(), coveredMeters.end()), coveredMeters.end());
		vector<Position*> uncoveredMeters = removeVectorFromAnother(allMarkers, coveredMeters);
		vector<Position*> aux = coveredMeters;
		for (int i = 0; i < meshEnabled; i++)
		{
			vector<Position*> newCovered;
			for (int j = 0; j < uncoveredMeters.size(); j++)
			{
				DrawInfo* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
				if (toAdd)
				{
					toCover.push_back(toAdd);
					newCovered.push_back(uncoveredMeters[j]);
				}
			}
			aux = newCovered;
			uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
		}

	}
	return toCover;
}
string Requisition::getDrawResponse()
{
	vector<DrawInfo*> drawInfos = calculateDrawingInfo();
	string ret = "";
	for (int i = 0; i < drawInfos.size(); i++)
		ret += drawInfos[i]->toString() + " ";

	for (int i = 0; i < drawInfos.size(); i++)
		delete drawInfos[i];
	

	return ret;
}
string Requisition::getMetricResponse()
{


	double mpdSum = 0, max = -1, min = -1;
	vector<vector<int>> SCP = createScp();
	for (int i = 0; i < SCP.size(); i++)
	{
		
		int toFind = i;
		int pos = -1;
		for (int i = 0; i < daps.size(); i++){ if (daps[i]->index == toFind){ pos = i; break; } }
		if (pos != -1)
		{
			//vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
			int nMeters = SCP[i].size();
			mpdSum += nMeters;
			if (max == -1 || nMeters > max)
				max = nMeters;
			if (min == -1 || nMeters < min)
				min = nMeters;
		}
	}
	string answer = "Number of DAPs: "+ to_string(daps.size()) +"\n";
	answer += "Average Number of Meters per DAP: " + to_string(mpdSum/daps.size()) + "\n";
	answer += "Maximum Number of Meters in a DAP: " + to_string(max) + "\n";
	answer += "Minimum Number of Meters in a DAP: " + to_string(min) + "\n";
	return  answer;

	
	
	vector<vector<sComponent*>> sL = statisticalList();
	//COLOCAR A FUNÇÃO DE ROBUSTEZ!
	//ARMAZENAR ESSA STATISTICAL LIST

	//string answer = "";
	if (sL.size() > 0)
	{
		
		int numOfDaps = sL.size();
		
	/*	answer += "Number of DAPs: " + to_string(numOfDaps) + "\n";
	
		vector<double> alpd = averageLinksPerDap(sL);

		double alpdmedia = alpd[0];
		double alpdmax = alpd[1];
		double alpdmin = alpd[2];

		answer += "Average Links per DAP: " + to_string(alpdmedia) + "\n";
		answer += "Maximum Links in a DAP: " + to_string(alpdmax) + "\n";
		answer += "Minimum Links in a DAP: " + to_string(alpdmin) + "\n";*/

		vector<double> ampd = averageMetersPerDap(sL);
		double ampdmedia = ampd[0];
		double ampdmax = ampd[1];
		double ampdmin = ampd[2];

		answer += "Average Number of Meters per DAP: " + to_string(ampdmedia) + "\n";
		answer += "Maximum Number of Meters in a DAP: " + to_string(ampdmax) + "\n";
		answer += "Minimum Number of Meters in a DAP: " + to_string(ampdmin) + "\n";

		/*vector<vector<double>> ammpd = avaregeMeshMetersPerDap(sL, meshEnabled);
		for (int i = 0; i < meshEnabled + 1; i++)
		{
			double ammpdmedia = ammpd[i][0];
			double ammpdmax = ammpd[i][1];
			double ammpdmin = ammpd[i][2];
			double hop = ammpd[i][3];
			if (ammpdmedia > 0)
			{
				answer += "Average " + to_string(i) + " mesh hops links: " + to_string(ammpdmedia) + "\n";
				answer += "Maximum " + to_string(i) + " mesh hops links: " + to_string(ammpdmax) + "\n";
				answer += "Minimum " + to_string(i) + " mesh hops links: " + to_string(ammpdmin) + "\n";
			}
		}
		vector<vector<double>> avgHops = averageHops(sL, meshEnabled);
		for (int i = 0; i < meshEnabled + 1; i++)
		{
			double avgHopsEff = avgHops[i][0];
			double avgHopsQnt = avgHops[i][1];
			if (avgHopsEff > 0)
			{
				answer += "Average " + to_string(i) + " mesh hops efficiency: " + to_string(avgHopsEff) + "\n";
				answer += to_string(i) + " mesh hops links quantity: " + to_string(avgHopsQnt) + "\n";
			}

		}*/
	}
	else
	{
		answer = "Nao ha DAPs para se coletar estatasticas!";
	}
	return answer.c_str();
}
string Requisition::getResponse()
{
	if (option == AUTOPLAN)
		return getAutoPlanResponse();
	if (option == DRAW)
		return getDrawResponse();
	if (option == METRIC)
		return getMetricResponse();
	if (option == TEST)
		getTestResponse("Default");

	return "";
}