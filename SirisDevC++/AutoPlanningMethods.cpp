#include "AutoPlanningMethods.h"

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
vector<int> AutoPlanning::uncoverableMeters(vector<vector<int> > &SCP)
{
	
	vector<int> uncoverableMeters;
	for (int i = 0; i < AutoPlanning::meters.size(); i++)
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


vector<vector<int> > AutoPlanning::createMeterNeighbourhood(Grid *g)
{
	vector<vector<int> > M;

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

vector<vector<int> > AutoPlanning::createScp()
{
	Grid* g = new Grid(meters,poles, regionLimiter);
	vector<int> aux;
	vector<vector<int> > sM;
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

		vector<vector<int> > nM = createMeterNeighbourhood(g);
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
vector<vector<int> > AutoPlanning::createScpSemGrid()
{
	Grid* g = new Grid(meters, poles, 10);
	vector<int> aux;
	vector<vector<int> > sM;
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

		vector<vector<int> > nM = createMeterNeighbourhood(g);// ESSE MÉTODO NÃO PODERIA ESTAR NO INICIO DO MÉTODO PARA QUE NÃO FOSSE CHAMADO VÁRIAS VEZES???!?!?!?
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
void AutoPlanning::saveGLPKFile(vector<vector<int> > &SCP)
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

void AutoPlanning::saveGLPKFileReduced(vector<vector<int> > &SCP)
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
string AutoPlanning::executeAutoPlan()
{
	return "u mad m8?";
}
