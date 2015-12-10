#include "MetricCalculationMethods.h"
//Método para se calcular as métricas!
//Antes de começar leia os comentários do método statisticalList




////Junta dois vetores igual ao do AutoPlanning, mas não lembro bem porque tenoh outro método igual aqui.
//vector<int> MetricCalculation::concatVectors(vector<int> &v1, vector<int> &v2)
//{
//	vector<int> ret;
//	for (int i = 0; i < v1.size(); i++)
//		ret.push_back(v1[i]);
//	for (int i = 0; i < v2.size(); i++)
//	{
//		bool add = true;
//		for (int j = 0; j < v1.size(); j++)
//		if (v1[j] == v2[i])
//		{
//			add = false;
//			break;
//		}
//		if (add)
//			ret.push_back(v2[i]);
//
//	}
//	return ret;
//
//}
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
	for(int i = 0; i < mesh+1; i++)
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
	for(int i = 0; i < mesh+1; i++)
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
			//CALCULO DA EFICIENCIA DO CAMINHO PODE SER CALCULDA DE UMA FORMA MAIS EFICIENTE
			eff *= s->efficiency;

			hop--;
			s = s->meshFather;

		}
		ret[sL[i]->hop] += eff;
		hopQnt[sL[i]->hop]++;

	}
	for(int i = 0; i < mesh+1; i++)
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
	for (int i = 0; i < mesh + 1; i++)
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
				delay += PER_HOP_DELAY;
			hop--;
			s = s->meshFather;

		}
		ret[sL[i]->hop] += delay;
		hopQnt[sL[i]->hop]++;

	}
	for (int i = 0; i < mesh + 1; i++)
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
	for (int i = 0; i < mesh+1; i++)
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
		//uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
		for (int j = 0; j < newCovered.size(); j++)
		{
			vector<Position*>::iterator it;
			it = find(uncoveredMeters.begin(), uncoveredMeters.end(), newCovered[j]);
			if (it != uncoveredMeters.end())
				uncoveredMeters.erase(it);
		}
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
			double eff = getLinkQuality(dist);

			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}
		sM.push_back(metersCovered);
	}
	if (mesh)
	{

		vector<vector<int> > nM = createMeterNeighbourhood(g);
		for (int i = 0; i < sM.size(); i++)
		{
			vector<int> neighbours = sM[i];
			for (int j = 0; j < mesh; j++)
			{
				vector<int> newNeighbours;
				for (int k = 0; k < neighbours.size(); k++)
				{

					int toFind = neighbours[k];
					int pos = 0;
					for (int l = 0; l < meters.size(); l++){ if (meters[l]->index == toFind){ pos = l; break; } }

					//sM[i] = concatVectors(sM[i], nM[pos]);
					sM[i].insert(sM[i].end(), nM[pos].begin(), nM[pos].end());
					sort(sM[i].begin(), sM[i].end());
					sM[i].erase(unique(sM[i].begin(), sM[i].end()), sM[i].end());

					//newNeighbours = concatVectors(newNeighbours, nM[pos]); //ESSA PARTE AQUI É PASSÍVEL DE OTIMIZAÇÃO! DIMINUIR NEWNEIGHBOURS DE SM[I]!!!
					newNeighbours.insert(newNeighbours.end(), nM[pos].begin(), nM[pos].end());
					sort(newNeighbours.begin(), newNeighbours.end());
					newNeighbours.erase(unique(newNeighbours.begin(), newNeighbours.end()), newNeighbours.end());
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
			double eff = getLinkQualityBetweenMeters(dist);
			if (meters[i]->index != meterRegion[j]->index && eff >= MARGIN_VALUE)
				pointsCovered.push_back(meterRegion[j]->index);

		}
		M.push_back(pointsCovered);
	}

	return M;
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
			eff = getLinkQuality(dist);
		else
			eff = getLinkQualityBetweenMeters(dist);

		if (eff >= MARGIN_VALUE)
		{
			sComponent* ret;
			if(hop == 0)
			{
				double delay = calculateLinkDelay(eff, bit_rate,technology);
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
				double delay = calculateLinkDelay(eff, bit_rate,technology);
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
	vector<sComponent*> sL = statisticalList();
	vector<vector<int> > cL = coverageList();

	vector<double > v = linkQualityPerHop(sL);
	vector<int > v2 = meterPerHop(sL);
	vector<double> v3 = minMedMaxMetersPerDap(cL);
	vector<double> v4 = minMedMaxRedundancyPerMeter(cL);
	vector<double > v5 = linkDelayPerHop(sL);
	int coveredMeters = 0;
	for(int i = 0; i < mesh+1; i++)
	{
		coveredMeters += v2[i];
	}
	Document document;
	document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();

	Value vAux; 
	vAux.SetString(to_string(daps.size()).c_str(), allocator);
	document.AddMember("DAPQnt", vAux, allocator);
	vAux.SetString(to_string(meters.size()).c_str(), allocator);
	document.AddMember("MeterQnt", vAux, allocator);
	vAux.SetString(to_string(meters.size() - coveredMeters).c_str(), allocator);
	document.AddMember("UncoveredMeters", vAux, allocator);
	Value array1(rapidjson::kArrayType), array2(rapidjson::kArrayType), array3(rapidjson::kArrayType) ;
	for (int i = 0; i <	mesh + 1; i++)
	{
		Value val1,val2,val3;
		val1.SetString(to_string(v[i], 3).c_str(), allocator);
		val2.SetString(to_string(v2[i], 3).c_str(), allocator);
		val3.SetString(to_string(v5[i], 3).c_str(), allocator);
		array1.PushBack(val1, allocator);
		array2.PushBack(val2, allocator);
		array3.PushBack(val3, allocator);
	}
	document.AddMember("QualityPerHop", array1, allocator);
	document.AddMember("MetersPerHop", array2, allocator);
	document.AddMember("DelayPerHop", array3, allocator);
	vAux.SetString(to_string(v3[0]).c_str(), allocator);
	document.AddMember("MinMetersPerDAP", vAux, allocator);
	vAux.SetString(to_string(v3[1]).c_str(), allocator);
	document.AddMember("MedMetersPerDAP", vAux, allocator);
	vAux.SetString(to_string(v3[2]).c_str(), allocator);
	document.AddMember("MaxMetersPerDAP", vAux, allocator);
	vAux.SetString(to_string(v4[0]).c_str(), allocator);
	document.AddMember("MinRedundancy", vAux, allocator);
	vAux.SetString(to_string(v4[1]).c_str(), allocator);
	document.AddMember("MedRedundancy", vAux, allocator);
	vAux.SetString(to_string(v4[2]).c_str(), allocator);
	document.AddMember("MaxRedundancy", vAux, allocator);

	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);


	for(int i = 0; i < sL.size();i++)
	{
		delete sL[i];
	}

	return strbuf.GetString();
}
string MetricCalculation::getMetricCalculationString()
{
	vector<sComponent*> sL = statisticalList();
	vector<vector<int> > cL = coverageList();
	vector<double > v = linkQualityPerHop(sL);
	vector<int > v2 = meterPerHop(sL);
	vector<double> v3 = minMedMaxMetersPerDap(cL);
	vector<double> v4 = minMedMaxRedundancyPerMeter(cL);
	vector<double > v5 = linkDelayPerHop(sL);
	int coveredMeters = 0;
	for (int i = 0; i < mesh + 1; i++)
	{
		coveredMeters += v2[i];
	}
	string ret = "<Statistics>\n";
	ret += statisticFormat("DAPQnt", to_string(daps.size()));
	ret += statisticFormat("MeterQnt", to_string(meters.size()));
	ret += statisticFormat("UncoveredMeters", to_string(meters.size() - coveredMeters));

	for (int i = 0; i < mesh + 1; i++){
		ret += statisticFormat("QualityPerHop", to_string(v[i], 3), i + 1);
		ret += statisticFormat("MetersPerHop", to_string(v2[i], 3), i + 1);
	}
	vector<string> v3_string;
	v3_string.push_back(to_string(v3[0]));	v3_string.push_back(to_string(v3[1]));	v3_string.push_back(to_string(v3[2]));
	ret += statisticFormat("MetersPerDap", v3_string);

	vector<string> v4_string;
	v4_string.push_back(to_string(v4[0]));	v4_string.push_back(to_string(v4[1]));	v4_string.push_back(to_string(v4[2]));
	ret += statisticFormat("Redundancy", v4_string);
	ret += "</Statistics>";

	for (int i = 0; i < sL.size(); i++)	
		delete sL[i];

	return ret;
}
MetricResult* MetricCalculation::executeMetricCalculationTest()
{
	vector<sComponent*> sL = statisticalList();
	vector<vector<int> > cL = coverageList();

	MetricResult* result = new MetricResult();
	result->linkQualityPerHop = linkQualityPerHop(sL);
	result->meterPerHop = meterPerHop(sL);
	result->minMedMaxMetersPerDap = minMedMaxMetersPerDap(cL);
	result->minMedMaxRedundancyPerMeter = minMedMaxRedundancyPerMeter(cL);
	result->linkDelayPerHop = linkDelayPerHop(sL);
	result->numOfDaps = daps.size();
	result->numOfMeters = meters.size();
	int coveredMeters = 0;
	for (int i = 0; i < mesh + 1; i++)
	{
		coveredMeters += result->meterPerHop[i];
	}
	result->uncoveredMeters = meters.size() - coveredMeters;
	for (int i = 0; i < sL.size(); i++)
		delete sL[i];

	return result;
}
