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
	if(id == DAP)
		sort(daps.begin(), daps.end(), compareByLatitude);
	if (id == METER)
		sort(meters.begin(), meters.end(), compareByLatitude);
	if (id == POLE)
		sort(poles.begin(), poles.end(), compareByLatitude);
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
vector<vector<int>> Requisition::createScp2()
{
	vector<int> aux;
	vector<vector<int>> sM;
	sM.reserve(meters.size());

	for (int i = 0; i < meters.size(); i++)
	{
		vector<int> polesThatCover;
		vector<Position*> polesReduced = getActiveRegion(poles, meters[i]);
		for (int j = 0; j < polesReduced.size(); j++)
		{
			double dist = getDistance(meters[i], polesReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (eff >= MARGIN_VALUE)
				polesThatCover.push_back(polesReduced[j]->index);
		}

		//if (polesThatCover.length > 0)
		if (i % 1000 == 0)
			printf("%d", i);

		sM.push_back(polesThatCover);
	}
	//if (meshEnabled)
	//{
	//	vector<vector<int>> sMCopy;
	//	for (int i = 0; i < sM.size(); i++)
	//	{
	//		vector<int> toAdd;
	//		for (int j = 0; j < sM[i].size(); j++)
	//			toAdd.push_back(sM[i][j]);
	//		sMCopy.push_back(toAdd);
	//	}

	//	vector<vector<int>> nM = createMeterNeighbourhood();
	//	for (int i = 0; i < meters.size(); i++)
	//	{
	//		vector<int> neighbours = nM[i];
	//		for (int j = 0; j < meshEnabled; j++)
	//		{
	//			vector<int> newNeighbours;
	//			for (int k = 0; k < neighbours.size(); k++)
	//			{
	//				sM[i] = concatVectors(sM[i], sMCopy[neighbours[k]]);
	//				newNeighbours = concatVectors(newNeighbours, nM[neighbours[k]]);
	//			}
	//			//sM[i] = removeRepeated(sM[i]);
	//			//newNeighbours = removeRepeated(newNeighbours);
	//			neighbours = newNeighbours;

	//		}
	//		if (i % 1000 == 0)
	//			printf("MESH%d", i);
	//	}

	//}




	return sM;
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
	vector<int> aux;
	vector<vector<int>> sM;
	sM.reserve(meters.size());

	for (int i = 0; i < poles.size(); i++)
	{
		vector<int> metersCovered;
		vector<Position*> metersReduced = getActiveRegion(meters, poles[i]);
		for (int j = 0; j < metersReduced.size(); j++)
		{
			double dist = getDistance(poles[i], metersReduced[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, SRD);
			if (eff >= MARGIN_VALUE)
				metersCovered.push_back(metersReduced[j]->index);
		}

		//if (polesThatCover.length > 0)
		if (i % 1000 == 0)
			printf("%d", i);

		sM.push_back(metersCovered);
	}
	if (meshEnabled)
	{

		vector<vector<int>> nM = createMeterNeighbourhood();
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
			
				printf("MESH%d", i);
		}

	}




	return sM;
}
vector<vector<int>> Requisition::createMeterNeighbourhood()
{
	vector<vector<int>> M;

	for (int i = 0; i < meters.size(); i++) {
		vector<int> pointsCovered;
		vector<Position*> meterRegion = getActiveRegion(meters, meters[i]);
		for (int j = 0; j < meterRegion.size(); j++)
		{
			double dist = getDistance(meters[i], meterRegion[j]);
			double eff = getHataSRDSuccessRate(dist, scenario, technology, BIT_RATE, TRANSMITTER_POWER,H_TX, H_RX, SRD);
			if (i != j && eff >= MARGIN_VALUE)
				pointsCovered.push_back(j);

		}
		M.push_back(pointsCovered);
	}

	return M;
}