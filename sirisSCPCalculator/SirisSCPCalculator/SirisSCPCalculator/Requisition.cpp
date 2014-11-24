#include "stdafx.h"
#include "Requisition.h"
#include "MetricsResponse.h"
#include "Grasp.h"
#include <time.h>
#include <sstream>


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

		vector<vector<int>> nM = createMeterNeighbourhood(g);// ESSE M�TODO N�O PODERIA ESTAR NO INICIO DO M�TODO PARA QUE N�O FOSSE CHAMADO V�RIAS VEZES???!?!?!?
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

			//TEM Q MUDAR ESSE NEG�CIO AQUI!
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
			resp += "end;";
		
			ofstream f("GlpkFile.txt");

			f << resp;
			f.close();

	
	
		}
	
}
void Requisition::saveGLPKFileReduced(vector<vector<int>> &SCP)
{
	FILE *file;
	{

		//TEM Q MUDAR ESSE NEG�CIO AQUI!
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
			uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
			if (!uncov)
				resp += "Z" + to_string(i + 1) + " ";
		}

		resp += ";\n";
		resp += "set Y:= ";

		for (int i = 0; i < poles.size(); i++)
			resp += "Y" + to_string(i + 1) + " ";

		resp += ";\n";

		resp += "param A := ";

		//for (int i = 0; i < poles.size(); i++)
		//	resp += "Y" + to_string(i + 1) + " ";

		resp += "\n";
		for (int i = 0; i < meters.size(); i++)
		{
			int uncov = -1;
			uncov = (find(uncMeters.begin(), uncMeters.end(), i) != uncMeters.end());
			if (!uncov)
			{
				//fprintf_s(file, "%s%d%s", "Z", (i + 1), " ");
				for (int j = 0; j < poles.size(); j++)
				{
					//bool um = false;
					//for (int k = 0; k < SCP.size(); k++)
					int cov = -1;
					cov = (find(SCP[j].begin(), SCP[j].end(), i) != SCP[j].end());
					if (cov)
						resp += "[Z" + to_string(i + 1) + ",Y" + to_string(j + 1)+"] 1";

				}
			}

		}
		resp += "\n";
		resp += ";";
		resp += "end;";

		ofstream f("GlpkFile.txt");

		f << resp;
		f.close();



	}

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
			cov = (find(SCP[j].begin(), SCP[j].end(), i) != SCP[j].end());
			if (cov)
			{
				coverable = 1;
				break;
			}
		}
		if (!coverable)
			uncoverableMeters.push_back(i);
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
			fclose(f);
		}

	}

}
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
string Requisition::getAutoPlanResponse()
{
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
	system("glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");
	//time(&timerend);
	seconds = (clock() - clo)/1000;
	FILE *fi;
	fopen_s(&fi, "ns3files\\AutoPlanningResults.txt", "w");
	if (fi)
	{
		//printf("\n-----%d------", v.size());
		fprintf_s(fi, "%f\n", seconds);
		fclose(fi);
	}

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
void Requisition::getTestResponse()
{

	FILE *fi;
	fopen_s(&fi, "ns3files\\AutoPlanningResults.txt", "w");
	if (fi)
	{
		//FAZ PELO M�TODO EXATO
		vector<vector<int>> SCP = createScp();
		//saveGLPKFile(SCP);
		saveGLPKFileReduced(SCP);
		double seconds;
		double clo = clock();
		system("glpk-4.54\\w64\\glpsol.exe --math GlpkFile.txt");
		seconds = (clock() - clo) / 1000;


		fprintf_s(fi, "Optimal solution time: %f\n", seconds);

		ifstream f("Results.txt");
		string str;
		getline(f, str);
		vector<string> x = split(str, ' ');
		for (int i = 0; i < x.size(); i++)
		{
			string snum = x[i].substr(1);
			daps.push_back(poles[stoi(snum) - 1]);
		}
		string result = getMetricResponse();
		fprintf(fi,result.c_str());
		//------------------------------------------------
		//FAZ PELO M�TODO GRASP
//		string result = "";

		int cSatisfied, nColumns, columnsSize = SCP.size();
		vector<vector<int>> graspscp;
		for (int i = 0; i < meters.size(); i++)
		{
			vector<int> aux;
			graspscp.push_back(aux);
		}
		for (int i = 0; i < SCP.size(); i++)
		{
			for (int j = 0; j < SCP[i].size(); j++)
			{
				graspscp[SCP[i][j]].push_back(i);
			}
		}
		//clo = clock();
		//double alpha = 0.8;
		//int* sol = metaheuristic(graspscp, columnsSize, &cSatisfied, &nColumns,alpha);
		//seconds = (clock() - clo) / 1000;
		//fprintf_s(fi, "Grasp solution time: %f\n", seconds);
		//

		//vector<Position*> dg;
		//for (int i = 0; i < columnsSize; i++)
		//{
		//	if (sol[i] == 1)
		//	{
		//		dg.push_back(poles[i]);
		//		
		//	}
		////	fprintf(fi, " %d ",sol[i]);
		//		
		//}
		//daps = dg;
		//result = getMetricResponse();
		//fprintf(fi, result.c_str());


		//------------------------------------------------
		//FAZ PELO M�TODO GULOSO
		
		clo = clock();
		double alpha = 1;
		int* sol = metaheuristic(graspscp, columnsSize, &cSatisfied, &nColumns,alpha);
		seconds = (clock() - clo) / 1000;
		fprintf_s(fi, "Greedy solution time: %f\n", seconds);

		vector<Position*> dguloso;
		for (int i = 0; i < columnsSize; i++)
		{
			if (sol[i] == 1)
			{
				dguloso.push_back(poles[i]);
			}
			//	fprintf(fi, " %d ",sol[i]);

		}
		//------------------------------------------------
		daps = dguloso;
		result = getMetricResponse();
		fprintf(fi, result.c_str());
		daps.clear();
		free(sol);
		fclose(fi);
	}


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
	vector<vector<sComponent*>> sL;
	for (int i = 0; i < daps.size(); i++)
	{
		vector<sComponent*> toAdd;
		for (int j = 0; j < meters.size(); j++)
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
			Grid* g = new Grid(meters,daps, regionLimiter);
			vector<vector<int>> nM = createMeterNeighbourhood(g);
			//vector<sComponent*> neighbours = toAdd;
			vector<int> neighbours;
			for (int h = 0; h < toAdd.size(); h++)
				neighbours.push_back(toAdd[h]->index);


			for (int k = 0; k < meshEnabled; k++){
				vector<int> newNeighbourhood;
				vector<sComponent*> meshToAdd;
				for (int j = 0; j < neighbours.size(); j++){

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
	
	vector<vector<sComponent*>> sL = statisticalList();
	//COLOCAR A FUN��O DE ROBUSTEZ!
	//ARMAZENAR ESSA STATISTICAL LIST

	string answer = "";
	if (sL.size() > 0)
	{
		
		int numOfDaps = sL.size();
		
		answer += "Number of DAPs: " + to_string(numOfDaps) + "\n";
	
		vector<double> alpd = averageLinksPerDap(sL);

		double alpdmedia = alpd[0];
		double alpdmax = alpd[1];
		double alpdmin = alpd[2];

		answer += "Average Links per DAP: " + to_string(alpdmedia) + "\n";
		answer += "Maximum Links in a DAP: " + to_string(alpdmax) + "\n";
		answer += "Minimum Links in a DAP: " + to_string(alpdmin) + "\n";

		vector<double> ampd = averageMetersPerDap(sL);
		double ampdmedia = ampd[0];
		double ampdmax = ampd[1];
		double ampdmin = ampd[2];

		answer += "Average Number of Meters per DAP: " + to_string(ampdmedia) + "\n";
		answer += "Maximum Number of Meters in a DAP: " + to_string(ampdmax) + "\n";
		answer += "Minimum Number of Meters in a DAP: " + to_string(ampdmin) + "\n";

		vector<vector<double>> ammpd = avaregeMeshMetersPerDap(sL, meshEnabled);
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

		}
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
		getTestResponse();

	return "";
}