#include "stdafx.h"
#include "Requisition.h"

bool compareByLongitude(Position* a, Position *b)
{
	return a->longitude < b->longitude;
}
bool compareByLatitude(Position* a, Position *b)
{
	return a->latitude < b->latitude;
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
	Grid* g = new Grid(meters, 0.001);
	vector<int> aux;
	vector<vector<int>> sM;
	sM.reserve(meters.size());

	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		//vector<Position*> metersReduced = getActiveRegion(meters, poles[i]);
		vector<Position*> metersReduced = g->getCell(poles[i]);

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
void Requisition::saveGLPKFile(vector<vector<int>> SCP)
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
			string resp;
			resp += "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"C:\\Sites\\first_app\\Results.txt\";\n data;\n";
			//fprintf_s(file, "%s", "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n");
			//ret += "set Z:= ";
			//fprintf_s(file, "set Z:= ");
			resp += "set Z:= ";
			for (int i = 0; i < meters.size(); i++)
				resp += "Z" + to_string(i + 1) + " ";

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
			resp += "\n";
			resp += ";";
			resp += "end;";
		
			ofstream f("C:\\Sites\\first_app\\GlpkFile.txt");

			f << resp;
			f.close();

	
	
		}
	
}
string Requisition::getAutoPlanResponse()
{
	vector<vector<int>> SCP = createScp();
	saveGLPKFile(SCP);
	system("C:\\Users\\Guilherme\\Downloads\\glpk-4.54\\w64\\glpsol.exe --math C:\\Sites\\first_app\\GlpkFile.txt");
	

	ifstream f("C:\\Sites\\first_app\\Results.txt");
	string str;
	getline(f, str);
	//string file_contents;
	//while (getline(f, str))
	//{
	//	file_contents += str;
	//	file_contents.push_back('\n');
	//}
	return str;
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
	return "";
}
string Requisition::getResponse()
{
	if (option == AUTOPLAN)
		return getAutoPlanResponse();
	if (option == DRAW)
		return getDrawResponse();
	if (option == METRIC)
		return getMetricResponse();
	return "";
}