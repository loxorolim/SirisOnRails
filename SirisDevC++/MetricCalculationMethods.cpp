#include "MetricCalculationMethods.h"
//Método para se calcular as métricas!
//Antes de começar leia os comentários do método statisticalList

double getNumberOfRetries(double quality)
{
	double compQuality = 1 - quality;
	return (quality*(1 + 2 * compQuality + 3 * pow(compQuality, 2)) + 4 * pow(compQuality, 3));

}

double calculateLinkDelay(double quality, int pckSize, double rate,int technology)
{
	double numOfRetries = getNumberOfRetries(quality);
	if (technology == t802_11_g)
		return ((pckSize / (rate*pow(2,20)))*(numOfRetries / round(numOfRetries)))*1000; //*1000 pra ser milisegundos
	if (technology == t802_15_4)
		return ((pckSize / (250*pow(2, 10)))*(numOfRetries / round(numOfRetries))) * 1000; //*1000 pra ser milisegundos
}


//Junta dois vetores igual ao do AutoPlanning, mas não lembro bem porque tenoh outro método igual aqui.
vector<int> MetricCalculation::concatVectors(vector<int> &v1, vector<int> &v2)
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
//A partir da lista de cobertura (Coverage List - cL) determina-se quantos medidores cada DAP cobre.
// A lista de cobertura é simplesmente um vetor de vetores, que relaciona cada DAP com os medidores que ele alcança.
// O método simplesmente percorre a lista e calcula a média e descobre o mínimo e o máximo.
vector<double> MetricCalculation::minMedMaxMetersPerDap(vector<vector<int> > cL)
{
	vector<double> ret; //Como tem min, mid e max, o formato aqui é um vetor com 3 posições, 0-min 1-mid 2-max
	ret.push_back(-1);ret.push_back(-1);ret.push_back(-1);
	double med = 0;
	if(cL.size() != 0)
	{
		for(int i = 0; i < cL.size();i++)
		{
			if(cL[i].size() < ret[0] || ret[0] == -1)
				ret[0] = cL[i].size();
			if(cL[i].size() > ret[2])
					ret[2] = cL[i].size();

			med += cL[i].size();
		}
		ret[1] = med/cL.size();
	}
	else
	{
		ret[0] = 0;
		ret[1] = 0;
		ret[2] = 0;
	}

	return ret;

}
//A partir da Coverage List esse método calcula a redundancia mínima, média e máxima.
//A redundância é por quantos agregadores um medidor é coberto.
//O método percorre a Coverage List e vai incrementando em 1 unidade os medidores que o DAP cobre.
vector<double> MetricCalculation::minMedMaxRedundancyPerMeter(vector<vector<int> > cL)
{
	vector<double> ret;
	ret.push_back(-1);ret.push_back(-1);ret.push_back(-1);
	double med = 0;
	vector<double> m;//Vetor que começa com tudo 0 e vai sendo incrementado de acordo com os DAPs que cobrem cada medidor.
	for(int i = 0; i < meters.size(); i++)
	{
		m.push_back(0);
	}
	for(int i = 0; i < cL.size(); i++)
	{
		for(int j = 0; j < cL[i].size();j++)
		{
			m[cL[i][j]]++;
		}

	}

	for(int i = 0; i < m.size(); i++)
	{
		if(m[i] < ret[0] || ret[0] == -1)
			ret[0] = m[i];
		if(m[i] > ret[2])
			ret[2] = m[i];
		med += m[i];
	}
	ret[1] = med/m.size();
	return ret;
}
//A partir da Statistical List esse método calcula a quantidade de medidores por salto.
//Cada sComponent tem informação de um enlace, inclusive a que salto pertence.
//Esse método simplesmente lê esses valores para cada salto.
vector<int> MetricCalculation::meterPerHop(vector<sComponent*> sL)
{
	vector<int> ret;
	for(int i = 0; i < meshEnabled+1; i++)
	{
		ret.push_back(0);
	}
	for(int i = 0; i < sL.size();i++)
	{
		ret[sL[i]->hop]++;
	}
	return ret;
}
//A partir da Statistical List esse método calcula a qualidade dos enlaces por salto.
//Um dos atributos do sComponent é um outro sComponent pai que é daonde o enlace surgiu. Por exemplo, se o enlace é de um único salto o pai dele é null, se o enlace
// é de dois saltos, o pai dele vai ser algum outro sComponent.
//Assim, calcula-se a qualidade dos enlaces de multiplo saltos multiplicando todo o caminho até que o pai seja null (ou seja, 1 salto).
vector<double> MetricCalculation::linkQualityPerHop(vector<sComponent*> sL)
{
	vector<double> ret;
	vector<int> hopQnt;
	for(int i = 0; i < meshEnabled+1; i++)
	{
		ret.push_back(0);
		hopQnt.push_back(0);
	}
	for(int i = 0; i < sL.size();i++)
	{
		int hop = sL[i]->hop;
		double eff = 1;
		sComponent* s = sL[i];
		while(hop >= 0)
		{
			//cout << s->efficiency << "\n";
			eff *= s->efficiency;

			hop--;
			s = s->meshFather;

		}
		ret[sL[i]->hop] += eff;
		hopQnt[sL[i]->hop]++;

	}
	for(int i = 0; i < meshEnabled+1; i++)
	{
		if(hopQnt[i] != 0)
			ret[i] = ret[i]/hopQnt[i];
	}
	return ret;
}

vector<double> MetricCalculation::linkDelayPerHop(vector<sComponent*> sL)
{
	vector<double> ret;
	vector<int> hopQnt;
	for (int i = 0; i < meshEnabled + 1; i++)
	{
		ret.push_back(0);
		hopQnt.push_back(0);
	}
	for (int i = 0; i < sL.size(); i++)
	{
		int hop = sL[i]->hop;
		double delay = 0;
		sComponent* s = sL[i];
		while (hop >= 0)
		{
			//cout << s->efficiency << "\n";
			delay += s->delay;
			if (hop > 0)
				delay += perHopDelay;
			hop--;
			s = s->meshFather;

		}
		ret[sL[i]->hop] += delay;
		hopQnt[sL[i]->hop]++;

	}
	for (int i = 0; i < meshEnabled + 1; i++)
	{
		if (hopQnt[i] != 0)
			ret[i] = ret[i] / hopQnt[i];
	}
	return ret;
}

//Essa statisticalList é um vetor de sComponent que é utilizado no cálculo de várias métricas.
//Ele funciona igual ao cálculo de enlace. Só que ao invés de objetos DrawInfo são objetos sComponent.
//Veja o MetricCalculationMethods.h pra ver o que o sComponent tem.
// A statisticalList contém, então, informações de todos os enlaces desenhados.
// OBS: Obviamente, não se desenha nem considera na métricas TODOS os enlaces possíveis! Antigamente eu considerava
// nas métricas, mas o processamento é absurdo porque são MUITOS enlaces "invisíveis". Portanto, as métricas são
// apenas dos enlaces que são vistos na interface.
vector<sComponent*> MetricCalculation::statisticalList()
{

	vector<sComponent*> statisticalComponents;
	vector<Position*> connectedDevices;
	connectedDevices = daps;
	vector<Position*> uncoveredMeters = meters;
	vector<Position*> aux = connectedDevices;
	Grid* g = new Grid(aux,uncoveredMeters, regionLimiter);
	g->putPositions(aux);
	for (int i = 0; i < meshEnabled+1; i++)
	{
		vector<Position*> newCovered;
		for (int j = 0; j < uncoveredMeters.size(); j++)
		{
			vector<Position*> toCheck = g->getCell(uncoveredMeters[j]);
			sComponent* toAdd = chooseDeviceToConnect(uncoveredMeters[j], aux, statisticalComponents, i);
			if (toAdd)
			{
				statisticalComponents.push_back(toAdd);
				newCovered.push_back(uncoveredMeters[j]);
				g->putPosition(uncoveredMeters[j]);
			}
		}
		aux = newCovered;
		uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
	}
	delete g;
	return statisticalComponents;
}
//A lista de cobertura é bem simples. É igual a matriz de cobertura do SCP. Só que relaciona os DAPs e os medidores que ele cobre.
// Esse método é igual ao createSCP() do AutoPlanning só que considera DAPs ao invés de postes!
vector<vector<int> > MetricCalculation::coverageList()
{
	Grid* g = new Grid(meters,daps, regionLimiter);
	//Grid* g = new Grid(regionLimiter);
	g->putPositions(meters);
	vector<int> aux;
	vector<vector<int> > sM;
	//sM.reserve(meters.size());

	for (int i = 0; i < daps.size(); i++)
	{
		vector<int> metersCovered;
		vector<Position*> metersReduced = g->getCell(daps[i]);
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(daps[i], metersReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}
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
		}

	}
	delete g;
	return sM;
}
//vector<vector<sComponent*> > MetricCalculation::createStatisticalMeterNeighbourhood(Grid *g)
//{
//	vector<vector<sComponent*> > M;
//
//	for (int i = 0; i < meters.size(); i++)
//	{
//		vector<sComponent*> pointsCovered;
//		//vector<Position*> meterRegion = getActiveRegion(meters, meters[i]);
//		vector<Position*> meterRegion = g->getCell(meters[i]);
//		for (int j = 0; j < meterRegion.size(); j++)
//		{
//			double dist = getDistance(meters[i], meterRegion[j]);
//			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD);
//			if (i != j && eff >= MARGIN_VALUE)
//			{
//				sComponent* component = new sComponent(meterRegion[j]->index, dist, eff, 0, NULL);
//				pointsCovered.push_back(component);
//			}
//
//		}
//		M.push_back(pointsCovered);
//	}
//
//	return M;
//}
//Mesmo coisa que no AutoPlanning
vector<vector<int> > MetricCalculation::createMeterNeighbourhood(Grid *g)
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
//Mesmo coisa que no LinkCalculation. Não está sendo usado! O chooseDeviceToConnect que é usado!
sComponent* MetricCalculation::chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters, vector<sComponent*> sC, int meshHop)
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
		double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
		if (eff >= MARGIN_VALUE)
		{
			sComponent* father = NULL;
			for(int i = 0; i < sC.size();i++){ if(sC[i]->index == meterToConnect->index) father = sC[i]; break; }
			double delay = calculateLinkDelay(eff, packetSize, BIT_RATE, technology);
			sComponent* ret = new sComponent(meter->index, dist, eff,delay, meshHop, father);
			return ret;
		}
	}
	return NULL;
}
//Mesmo coisa que no LinkCalculation
sComponent* MetricCalculation::chooseDeviceToConnect(Position* meter, vector<Position*> &devices, vector<sComponent*> sC, int hop)
{
	double minDist = -1;
	Position* deviceToConnect = NULL;
	for (int i = 0; i < devices.size(); i++)
	{
		double dist = getDistance(meter, devices[i]);
		if (dist < minDist || minDist == -1)
		{
			minDist = dist;
			deviceToConnect = devices[i];
		}
	}
	if (minDist != -1)
	{
		double dist = getDistance(meter, deviceToConnect);
		double eff = 0;
		if (hop == 0)
			eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
		else
			eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_TX, SRD);

		if (eff >= MARGIN_VALUE)
		{
			sComponent* ret;
			if(hop == 0)
			{
				double delay = calculateLinkDelay(eff, packetSize, BIT_RATE,technology);
				ret = new sComponent(meter->index, dist, eff,delay, hop, NULL);
			}
			else
			{
				sComponent* father = NULL;
				for(int i = 0; i < sC.size();i++)
				{
					if(sC[i]->index == deviceToConnect->index)
					{
						father = sC[i];
						break;
					}
				}
				double delay = calculateLinkDelay(eff, packetSize, BIT_RATE,technology);
				ret = new sComponent(meter->index, dist, eff,delay, hop, father);
			}
			return ret;
		}
	}
	return NULL;
}
string statisticFormat(string name, string val)
{
	string ret = "";
	ret += "<metric type = \"" + name + "\">" + val + "</metric>\n";
	return ret;
}
string statisticFormat(string name, string val, int hop)
{
	string ret = "";
	ret += "<metric type = \"" + name + "\" hop = \""+to_string(hop)+"\">" + val + "</metric>\n";
	return ret;
}
string statisticFormat(string name, vector<string> vals )
{
	string ret = "";
	ret += "<metric type = \"" + name + "\" range = \"min\">" + vals[0] + "</metric>\n";
	ret += "<metric type = \"" + name + "\" range = \"med\">" + vals[1] + "</metric>\n";
	ret += "<metric type = \"" + name + "\" range = \"max\">" + vals[2] + "</metric>\n";
	//ret += "<statistic type = \"" + name + "\">\n"
	//	+ "<min>" + vals[0] + "</min>\n"
	//	+ "<med>" + vals[1] + "</med>\n"
	//	+ "<max>" + vals[2] + "</max>\n" 
	//	+ "</statistic>\n";

	return ret;
}
//Esse é o método principal.Ele cria a Statistical List, a Coverage List, chama os métodos e cria uma String com todas essas informações que será lida pelo cliente
// e exibida. Os caracteres "<>" são apenas da formatação para separar os valores dos textos que esses valores representam.
string MetricCalculation::executeMetricCalculation()
{
	//Códigos:
	//DQ - Dap Quantity
	//MQ - Meter Quantity
	//UM - Uncovered Meters
	//QPHX - Quality Per Hop X=Number
	//DPHX - Delay Per Hop X=Number
	//MPHX - Meter Per Hop X=Number
	//MPDMin
	//MPDMed - Meter Per Dap
	//MPDMax
	//RMin
	//RMed - Redundancy
	//RMax



	vector<sComponent*> sL = statisticalList();
	vector<vector<int> > cL = coverageList();
	//for(int i = 0; i < sL.size();i++)
	//{
	//	cout << sL[i]->index << " "<< sL[i]->distance << " "<< sL[i]->efficiency << " " << sL[i]->hop << " \n";
	//}


	vector<double > v = linkQualityPerHop(sL);
	vector<int > v2 = meterPerHop(sL);
	vector<double> v3 = minMedMaxMetersPerDap(cL);
	vector<double> v4 = minMedMaxRedundancyPerMeter(cL);
	vector<double > v5 = linkDelayPerHop(sL);
	int coveredMeters = 0;
	for(int i = 0; i < meshEnabled+1; i++)
	{
		coveredMeters += v2[i];
	}
	string ret =  "<Statistics>\n";
	//ret+= "DAPs quantity: " + to_string(daps.size()) + "\n";
	//ret+= "Quantidade de agregadores<>" + to_string(daps.size()) + "\n";
	//ret += "DQ<>" + to_string(daps.size()) + "\n";
	ret += statisticFormat("DAPQnt", to_string(daps.size()));
	//ret+= "Meters quantity: " + to_string(meters.size()) + "\n";
	//ret+= "Quantidade de medidores<>" + to_string(meters.size()) + "\n";
	//ret += "MQ<>" + to_string(meters.size()) + "\n";
	ret += statisticFormat("MeterQnt", to_string(meters.size()));
	//ret+= "Uncovered meters quantity: " + to_string(meters.size() - coveredMeters) + "\n";
	//ret+= "Medidores descobertos<>" + to_string(meters.size() - coveredMeters) + "\n";
	//ret += "UM<>" + to_string(meters.size() - coveredMeters) + "\n";
	ret += statisticFormat("UncoveredMeters", to_string(meters.size() - coveredMeters));

	for(int i = 0; i < meshEnabled+1; i++)
	{
		//ret+=  "Mesh hop quality " + to_string(i+1) + ": " + to_string(v[i],3) + "\n";
		//ret+=  "Qualidade media do salto " + to_string(i+1) + "<>" + to_string(v[i],3) + "\n";
		//ret += "QPH" + to_string(i + 1) + "<>" + to_string(v[i], 3) + "\n";
		ret += statisticFormat("QualityPerHop", to_string(v[i], 3),i+1);
	}
	for (int i = 0; i < meshEnabled + 1; i++)
	{
		//ret+=  "Mesh hop quality " + to_string(i+1) + ": " + to_string(v[i],3) + "\n";
		//ret += "Atraso medio do salto " + to_string(i + 1) + "<>" + to_string(v5[i], 3) + "ms \n";
		//ret += "DPH" + to_string(i + 1) + "<>" + to_string(v5[i], 3) + "\n";
		ret += statisticFormat("DelayPerHop", to_string(v5[i], 3), i + 1);
	
	}

	for(int i = 0; i < meshEnabled+1; i++)
	{
		//ret += "Meter per hop " + to_string(i+1) + ": " + to_string(v2[i]) + "\n";
		//ret += "Medidores a " + to_string(i+1) + " salto<>" + to_string(v2[i]) + "\n";
		//ret += "MPH" + to_string(i + 1) + "<>" + to_string(v2[i]) + "\n";
		ret += statisticFormat("MetersPerHop", to_string(v2[i], 3), i + 1);	
	}

	//ret+= "Min Meters per DAP: " + to_string(v3[0]) + "\n";
	//ret+=  "Med Meters per DAP: " + to_string(v3[1],3) + "\n";
	//ret+=  "Max Meters per DAP: " + to_string(v3[2]) + "\n";
	//ret+= "Min Medidores por agregador<>" + to_string(v3[0]) + "\n";
	//ret+=  "Med Medidores por agregador<>" + to_string(v3[1],3) + "\n";
	//ret+=  "Max Medidores por agregador<>" + to_string(v3[2]) + "\n";
	//ret += "MPDMin<>" + to_string(v3[0]) + "\n";
	//ret += "MPDMed<>" + to_string(v3[1], 3) + "\n";
	//ret += "MPDMax<>" + to_string(v3[2]) + "\n";
	vector<string> v3_string;
	v3_string.push_back(to_string(v3[0]));	v3_string.push_back(to_string(v3[1]));	v3_string.push_back(to_string(v3[2]));
	ret += statisticFormat("MetersPerDap", v3_string);
	//ret+=  "Min redundancy per meter: " + to_string(v4[0]) + "\n";
	//ret+=  "Med redundancy per meter: " + to_string(v4[1],3) + "\n";
	//ret+=  "Max redundancy per meter: " + to_string(v4[2]) + "\n";
	//ret+=  "Min redundancia por medidor<>" + to_string(v4[0]) + "\n";
	//ret+=  "Med redundancia por medidor<>" + to_string(v4[1],3) + "\n";
	//ret+=  "Max redundancia por medidor<>" + to_string(v4[2]);
	//ret += "RMin<>" + to_string(v4[0]) + "\n";
	//ret += "RMed<>" + to_string(v4[1], 3) + "\n";
	//ret += "RMax<>" + to_string(v4[2]);
	vector<string> v4_string;
	v4_string.push_back(to_string(v4[0]));	v4_string.push_back(to_string(v4[1]));	v4_string.push_back(to_string(v4[2]));
	ret += statisticFormat("Redundancy", v4_string);
	ret += "</Statistics>";

	for(int i = 0; i < sL.size();i++)
	{
		delete sL[i];
	}

	return ret;
}

//string MetricCalculation::executeMetricCalculation()
//{
//
//
//	double mpdSum = 0, max = -1, min = -1;
//	vector<vector<int>> SCP = createScp();
//	for (int i = 0; i < SCP.size(); i++)
//	{
//
//		int toFind = i;
//		int pos = -1;
//		for (int i = 0; i < daps.size(); i++){ if (daps[i]->index == toFind){ pos = i; break; } }
//		if (pos != -1)
//		{
//			//vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
//			int nMeters = SCP[i].size();
//			mpdSum += nMeters;
//			if (max == -1 || nMeters > max)
//				max = nMeters;
//			if (min == -1 || nMeters < min)
//				min = nMeters;
//		}
//	}
//	string answer = "Number of DAPs: "+ to_string(daps.size()) +"\n";
//	answer += "Average Number of Meters per DAP: " + to_string(mpdSum/daps.size()) + "\n";
//	answer += "Maximum Number of Meters in a DAP: " + to_string(max) + "\n";
//	answer += "Minimum Number of Meters in a DAP: " + to_string(min) + "\n";
//	return  answer;
//
//
//
//	vector<vector<sComponent*>> sL = statisticalList();
//	//COLOCAR A FUNÇÃO DE ROBUSTEZ!
//	//ARMAZENAR ESSA STATISTICAL LIST
//
//	//string answer = "";
//	if (sL.size() > 0)
//	{
//
//		int numOfDaps = sL.size();
//
//	/*	answer += "Number of DAPs: " + to_string(numOfDaps) + "\n";
//
//		vector<double> alpd = averageLinksPerDap(sL);
//
//		double alpdmedia = alpd[0];
//		double alpdmax = alpd[1];
//		double alpdmin = alpd[2];
//
//		answer += "Average Links per DAP: " + to_string(alpdmedia) + "\n";
//		answer += "Maximum Links in a DAP: " + to_string(alpdmax) + "\n";
//		answer += "Minimum Links in a DAP: " + to_string(alpdmin) + "\n";*/
//
//		vector<double> ampd = averageMetersPerDap(sL);
//		double ampdmedia = ampd[0];
//		double ampdmax = ampd[1];
//		double ampdmin = ampd[2];
//
//		answer += "Average Number of Meters per DAP: " + to_string(ampdmedia) + "\n";
//		answer += "Maximum Number of Meters in a DAP: " + to_string(ampdmax) + "\n";
//		answer += "Minimum Number of Meters in a DAP: " + to_string(ampdmin) + "\n";
//
//		/*vector<vector<double>> ammpd = avaregeMeshMetersPerDap(sL, meshEnabled);
//		for (int i = 0; i < meshEnabled + 1; i++)
//		{
//			double ammpdmedia = ammpd[i][0];
//			double ammpdmax = ammpd[i][1];
//			double ammpdmin = ammpd[i][2];
//			double hop = ammpd[i][3];
//			if (ammpdmedia > 0)
//			{
//				answer += "Average " + to_string(i) + " mesh hops links: " + to_string(ammpdmedia) + "\n";
//				answer += "Maximum " + to_string(i) + " mesh hops links: " + to_string(ammpdmax) + "\n";
//				answer += "Minimum " + to_string(i) + " mesh hops links: " + to_string(ammpdmin) + "\n";
//			}
//		}
//		vector<vector<double>> avgHops = averageHops(sL, meshEnabled);
//		for (int i = 0; i < meshEnabled + 1; i++)
//		{
//			double avgHopsEff = avgHops[i][0];
//			double avgHopsQnt = avgHops[i][1];
//			if (avgHopsEff > 0)
//			{
//				answer += "Average " + to_string(i) + " mesh hops efficiency: " + to_string(avgHopsEff) + "\n";
//				answer += to_string(i) + " mesh hops links quantity: " + to_string(avgHopsQnt) + "\n";
//			}
//
//		}*/
//	}
//	else
//	{
//		answer = "Nao ha DAPs para se coletar estatasticas!";
//	}
//	return answer.c_str();
//}
