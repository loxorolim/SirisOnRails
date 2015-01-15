#include "MetricCalculationMethods.h"

vector<double> MetricCalculation::minMedMaxMetersPerDap()
{
//	Grid* g = new Grid(meters,daps, regionLimiter);
//	g->putPositions(meters);
//	vector<int> aux;
//	vector<vector<int> > sM;
//	double min,med,max;
//	//sM.reserve(meters.size());
//
//	for (int i = 0; i < daps.size(); i++)
//	{
//		vector<int> metersCovered;
//		vector<Position*> metersReduced = g->getCell(daps[i]);
//		for (int j = 0; j < metersReduced.size(); j++)
//		{
//			double dist = getDistance(daps[i], metersReduced[j]);
//			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//			if (eff >= MARGIN_VALUE)
//				metersCovered.push_back(metersReduced[j]->index);
//		}
//
//		sM.push_back(metersCovered);
//	}
//	if (meshEnabled)
//	{
//
//		vector<vector<int> > nM = createMeterNeighbourhood(g);
//		int firstPos = meters[0]->index;
//		for (int i = 0; i < sM.size(); i++)
//		{
//			vector<int> neighbours = sM[i];
//			for (int j = 0; j < meshEnabled; j++)
//			{
//				vector<int> newNeighbours;
//				for (int k = 0; k < neighbours.size(); k++)
//				{
//
//					int toFind = neighbours[k];
//					int pos = 0;
//					for (int i = 0; i < meters.size(); i++){ if (meters[i]->index == toFind){ pos = i; break; } }
//
//					sM[i] = concatVectors(sM[i], nM[pos]);
//					newNeighbours = concatVectors(newNeighbours, nM[pos]); //ESSA PARTE AQUI É PASSÍVEL DE OTIMIZAÇÃO! DIMINUIR NEWNEIGHBOURS DE SM[I]!!!
//				}
//				neighbours = newNeighbours;
//			}
//		}
//
//	}
//	delete g;
}
vector<int> MetricCalculation::minMedMaxRedundancyPerMeter()
{

}
vector<int> MetricCalculation::meterPerHop(vector<sComponent*> sL)
{
	vector<int> ret;
	for(int i = 0; i < meshEnabled; i++)
	{
		ret.push_back(0);
	}
	for(int i = 0; i < sL.size();i++)
	{
		ret[sL[i]->hop]++;
	}
	return ret;
}
vector<double> MetricCalculation::linkQualityPerHop(vector<sComponent*> sL)
{
	vector<double> ret;
	vector<int> hopQnt;
	for(int i = 0; i < meshEnabled; i++)
	{
		ret.push_back(0);
	}
	for(int i = 0; i < sL.size();i++)
	{
		int hop = sL[i]->hop;
		double eff = 1;
		sComponent* s = sL[i];
		while(hop  >= 0)
		{
			eff *= s->efficiency;
			hop--;
			s = s->meshFather;
		}
	}
	return ret;
}

//bool contains(vector<sComponent*> v, int elem)
//{
//	for (int i = 0; i < v.size(); i++)
//	{
//		if (v[i]->index == elem)
//			return true;
//	}
//	return false;
//}
//
//vector<vector<sComponent*>> MetricCalculation::statisticalList()
//{
//	Grid* g = new Grid(meters,daps, regionLimiter);
//	g->putPositions(meters);
//	vector<int> aux;
//	vector<vector<sComponent*> > sM;
//
//
//	for (int i = 0; i < daps.size(); i++)
//	{
//		vector<sComponent*> metersCovered;
//		vector<Position*> metersReduced = g->getCell(daps[i]);
//		for (int j = 0; j < metersReduced.size(); j++)
//		{
//			double dist = getDistance(daps[i], metersReduced[j]);
//			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
//			if (eff >= MARGIN_VALUE)
//			{
//				sComponent* component = new sComponent(metersReduced[j]->index, dist, eff, 0, NULL);
//				metersCovered.push_back(component);
//			}
//		}
//		sM.push_back(metersCovered);
//	}
//	if (meshEnabled)
//	{
//
//		vector<vector<sComponent*> > nM = createMeterNeighbourhood(g);
//		for (int i = 0; i < sM.size(); i++)
//		{
//			vector<sComponent*> neighbours = sM[i];
//			for (int j = 0; j < meshEnabled; j++)
//			{
//				vector<sComponent*> newNeighbours;
//				for (int k = 0; k < neighbours.size(); k++)
//				{
//					int toFind = neighbours[k]->index;
//					int pos = 0;
//					for (int l = 0; l < meters.size(); l++){ if (meters[l]->index == toFind){ pos = i; break; } }
//
//					for(int l = 0; l < nM[pos]; l++)
//					{
//						sComponent* component = new sComponent(nM[pos][l]->index, sM[i][k]->distance + nM[pos][l]->distance, sM[i][k]->efficiency*nM[pos][l]->efficiency, meshEnabled+1, sM[i][k]);
//						sM[i].push_back(component);
//					}
//					//sM[i] = concatVectors(sM[i], nM[pos]);
//					newNeighbours = concatVectors(newNeighbours, nM[pos]); //ESSA PARTE AQUI É PASSÍVEL DE OTIMIZAÇÃO! DIMINUIR NEWNEIGHBOURS DE SM[I]!!!
//				}
//				neighbours = newNeighbours;
//			}
//		}
//
//	}
//	delete g;
//}
vector<sComponent*> MetricCalculation::statisticalList()
{


	vector<sComponent*> statisticalComponents;
	vector<Position*> coveredMeters;
	vector<Position*> allMarkers;

	//allMarkers.insert(allMarkers.end(), meters.begin(), meters.end());
	for(int i = 0; i < meters.size();i++)
	{
		allMarkers.push_back(meters[i]);
	}

	Grid* g = new Grid(allMarkers,daps, regionLimiter);
	g->putPositions(allMarkers);
	g->putPositions(daps);

	for (int d = 0; d < daps.size(); d++)
	{
		vector<Position*> markersReduced = g->getCell(daps[d]);
		for (int i = 0; i < markersReduced.size(); i++)
		{
			if(markersReduced[i]->index != daps[i]->index)
			{
				double dist = getDistance(daps[d], markersReduced[i]);
				double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
				if (eff >= MARGIN_VALUE)
				{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO

					//sComponent* toAdd = new DrawInfo(daps[d], markersReduced[i], eff, dist, 0);
					sComponent* toAdd = new sComponent(markersReduced[i]->index, dist, eff, 0, NULL);
					statisticalComponents.push_back(toAdd);
					coveredMeters.push_back(markersReduced[i]);
				}
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

		Grid* g2 = new Grid(aux,uncoveredMeters, regionLimiter);
		g2->putPositions(aux);
		for (int i = 0; i < meshEnabled; i++)
		{
			vector<Position*> newCovered;
			for (int j = 0; j < uncoveredMeters.size(); j++)
			{
				vector<Position*> toCheck = g2->getCell(uncoveredMeters[j]);
				sComponent* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux, statisticalComponents, i+1);
				if (toAdd)
				{
					statisticalComponents.push_back(toAdd);
					newCovered.push_back(uncoveredMeters[j]);
					g2->putPosition(uncoveredMeters[j]);
				}
			}
			aux = newCovered;
			uncoveredMeters = removeVectorFromAnother(uncoveredMeters, newCovered);
		}
		delete g2;

	}
	delete g;
	return statisticalComponents;
}
vector<vector<sComponent*> > MetricCalculation::createMeterNeighbourhood(Grid *g)
{
	vector<vector<sComponent*> > M;

	for (int i = 0; i < meters.size(); i++)
	{
		vector<sComponent*> pointsCovered;
		//vector<Position*> meterRegion = getActiveRegion(meters, meters[i]);
		vector<Position*> meterRegion = g->getCell(meters[i]);
		for (int j = 0; j < meterRegion.size(); j++)
		{
			double dist = getDistance(meters[i], meterRegion[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD);
			if (i != j && eff >= MARGIN_VALUE)
			{
				sComponent* component = new sComponent(meterRegion[j]->index, dist, eff, 0, NULL);
				pointsCovered.push_back(component);
			}

		}
		M.push_back(pointsCovered);
	}

	return M;
}
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
		if (eff >= MARGIN_VALUE) {
			sComponent* father;
			for(int i = 0; i < sC.size();i++){ if(sC[i]->index == meterToConnect->index) father = sC[i]; break; }
			sComponent* ret = new sComponent(meter->index, dist, eff, meshHop, father);
			return ret;
		}
	}
	return NULL;
}
string MetricCalculation::executeMetricCalculation()
{
	vector<sComponent*> sL = statisticalList();
	for(int i = 0; i < sL.size();i++)
	{
		cout << sL[i]->index << " "<< sL[i]->distance << " "<< sL[i]->efficiency << " " << sL[i]->hop << " \n";
	}
	for(int i = 0; i < sL.size();i++)
	{
		delete sL[i];
	}
	return "fim";
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
