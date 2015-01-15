#include "LinkCalculationMethods.h"

vector<DrawInfo*> LinkCalculation::calculateDrawingInfo()
{


	vector<DrawInfo*> toCover;
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
			double dist = getDistance(daps[d], markersReduced[i]);
			double effs = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (effs >= MARGIN_VALUE)
			{ //SE CONSIDERAR DAPS TEM Q ALTERA AKI PRA NAO CRIAR UMA LINHA COM ELE MESMO

				DrawInfo* toAdd = new DrawInfo(daps[d], markersReduced[i], effs, dist, 0);
				toCover.push_back(toAdd);
				coveredMeters.push_back(markersReduced[i]);
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
				DrawInfo* toAdd = chooseMeterToConnect(uncoveredMeters[j], aux);
				if (toAdd)
				{
					toCover.push_back(toAdd);
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
	return toCover;
}
string LinkCalculation::executeLinkCalculation()
{
	vector<DrawInfo*> drawInfos = calculateDrawingInfo();
	string ret = "";
	for (int i = 0; i < drawInfos.size(); i++)
		ret += drawInfos[i]->toString() + " ";

	for (int i = 0; i < drawInfos.size(); i++)
		delete drawInfos[i];


	return ret;
}
DrawInfo* LinkCalculation::chooseMeterToConnect(Position* meter, vector<Position*> &connectedMeters)
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
