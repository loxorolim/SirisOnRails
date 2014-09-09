#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
#include "MetricsResponse.h"

using namespace std;


vector<int> removeRepeated(vector<int> &v)
{
	vector<int> ret;
	for (int i = 0; i < v.size(); i++)
	{
		bool insert = true;
		for (int j = 0; j < ret.size(); j++)
		{
			if (v[i] == ret[j])
			{
				insert = false;
				break;
			}
				
		}
		if (insert)
			ret.push_back(v[i]);
	}
	return ret;

}
bool contains(vector<sComponent*> v, int elem)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->index == elem)
			return true;
	}
	return false;
}


vector<vector<sComponent*>> statisticalList(vector<Position*> &daps, vector<Position*> &meters, int meshEnabled, int meshMaxJumps, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{
	vector<vector<sComponent*>> sL;
	for (int i = 0; i < daps.size(); i++)
	{
		vector<sComponent*> toAdd;
		for (int j = 0; j < meters.size(); j++)
		{
			double dist = getDistance(daps[i], meters[j]);
			double efficiency = getHataSRDSuccessRate(dist, env, technology,  bit_rate, transmitter_power, h_tx, h_rx, SRD);
		
			if (efficiency >= MARGIN_VALUE){
				sComponent* component = new sComponent(j,dist,efficiency,0,NULL);
				toAdd.push_back(component);
			}
		}
		if (meshEnabled){
			vector<vector<int>> nM = createNeighbourhoodMatrix(meters, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
			//vector<sComponent*> neighbours = toAdd;
			vector<int> neighbours;
			for (int h = 0; h < toAdd.size(); h++)
				neighbours.push_back(toAdd[h]->index);
				
			
			for (int k = 0; k < meshMaxJumps; k++){
				vector<int> newNeighbourhood;
				vector<sComponent*> meshToAdd ;
				for (int j = 0; j < neighbours.size(); j++){

					vector<int> aux = nM[neighbours[j]];
					for (int l = 0; l < aux.size(); l++){
						
						
						if (!(contains(toAdd,aux[l])))
						{
							sComponent* father = NULL;
							for (int z = 0; z < toAdd.size(); z++){
								if (toAdd[z]->index == neighbours[j]){
									father = toAdd[z];
									break;
								}
							}
							 
							double distBetweenMeters = getDistance(meters[father->index],meters[aux[l]]);
							double effBetweenMeters = getHataSRDSuccessRate(distBetweenMeters, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
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
		}
		sL.push_back(toAdd);
	}
	return sL;
}
vector<vector<int>> createNeighbourhoodMatrix(vector<Position*> &meters, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
{
	// var points = metersToPoints(meters);
	//var r = getDapfmaximumReach();
	vector<vector<int>> M;

	for ( int i = 0; i < meters.size(); i++) {
		vector<int> pointsCovered;
		for (int j = 0; j < meters.size(); j++){
			double dist = getDistance(meters[i], meters[j]);
			double efficiency = getHataSRDSuccessRate(dist, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
			if (i != j && efficiency >= MARGIN_VALUE) {
				
				pointsCovered.push_back(j);
				//pointsCovered.push(m);
			}
		}

		M.push_back(pointsCovered);
	}

	return M;
}
double averageDirectLinkQuality(vector<vector<sComponent*>> statisticalList)
{
	double avgQuality = 0, numOfLinks = 0;
	for (int i = 0; i < statisticalList.size(); i++)
	{
		for (int j = 0; j < statisticalList[i].size(); j++)
		{
			if (statisticalList[i][j]->hop == 0)
			{
				avgQuality += statisticalList[i][j]->efficiency;
				numOfLinks++;
			}
		}

	}
	return avgQuality / statisticalList.size();

}
double calcEfficiency(sComponent *meter)
{
	double eff = 1;
	int hops = meter->hop;
	sComponent* m = meter;
	for (int i = 0; i < hops + 1; i++){
		eff *= m->efficiency;
		m = m->meshFather;
	}
	return eff;
};
vector<vector<double>> averageHops(vector<vector<sComponent*>> statisticalList, int meshMaxJumps)
{
	vector<vector<double>> avgHop;
	for (int i = 0; i < meshMaxJumps + 1; i++){
		double sum = 0;
		double qnt = 0;
		for (int j = 0; j < statisticalList.size(); j++)
		{
			for (int k = 0; k < statisticalList[j].size(); k++){
				if (statisticalList[j][k]->hop == i)
				{
					sum += calcEfficiency(statisticalList[j][k]);
					qnt++;
				}
			}
		}
		vector<double> toAdd;
		toAdd.push_back(sum / qnt);//AVG HOP EFFICIENCY
		toAdd.push_back(qnt); //HOP QNT
		avgHop.push_back(toAdd);
	}
	return avgHop;
}
vector<double> averageLinksPerDap(vector<vector<sComponent*>> statisticalList)
{
	double lpdSum = 0, max = -1, min = -1;
	for (int i = 0; i < statisticalList.size(); i++)
	{
		int nLinks = statisticalList[i].size();
		lpdSum += nLinks;
		if (max == -1 || nLinks > max)
			max = nLinks;
		if (min == -1 || nLinks < min)
			min = nLinks;
	}
	vector<double> alpdInfo;
	alpdInfo.push_back(lpdSum / statisticalList.size());//MEDIA
	alpdInfo.push_back(max);//MAX LINKS POR DAP
	alpdInfo.push_back(min);//MIN LINKS POR DAP

	return alpdInfo;
	//calcular desvio padrão
}
vector<sComponent*> noRepeat(vector<sComponent*> list)
{
	vector<sComponent*> ret;
	for (int i = 0; i < list.size(); i++)
	{
		bool add = true;
		for (int j = 0; j < ret.size(); j++)
		if (list[i]->index == ret[j]->index){
			add = false;
			break;
		}
		if (add)
			ret.push_back(list[i]);
	}
	return ret;

}
vector<vector<double>> avaregeMeshMetersPerDap(vector<vector<sComponent*>> statisticalList, int meshMaxJumps)
{
	vector<vector<double>> mmpdSum;
	for (int z = 0; z < meshMaxJumps + 1; z++)
	{
		int sum = 0, max = -1, min = -1;;
		for (int i = 0; i < statisticalList.size(); i++){

			vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
			int nMeters = 0;
			for (int j = 0; j< uniqueArray.size(); j++)
			if (uniqueArray[j]->hop == z)
				nMeters++;

			sum += nMeters;
			if (max == -1 || nMeters > max)
				max = nMeters;
			if (min == -1 || nMeters < min)
				min = nMeters;
		}
		vector<double> ammpdInfo;
		ammpdInfo.push_back(sum / statisticalList.size());
		ammpdInfo.push_back(max);
		ammpdInfo.push_back(min);
		ammpdInfo.push_back(z);
		//var ammpdInfo = {
		//avgMMPD: sum / statisticalList.length,
		//	 maxMMPD : max,
		//		   minMMPD : min,
		//				 hop : z
		//};
		mmpdSum.push_back(ammpdInfo);
	}

	return mmpdSum;
}
vector<double> averageMetersPerDap(vector<vector<sComponent*>> statisticalList)
{
	double mpdSum = 0, max = -1, min = -1;

	for (int i = 0; i < statisticalList.size(); i++){
		vector<sComponent*> uniqueArray = noRepeat(statisticalList[i]);
		int nMeters = uniqueArray.size();
		mpdSum += nMeters;
		if (max == -1 || nMeters > max)
			max = nMeters;
		if (min == -1 || nMeters < min)
			min = nMeters;
	}
	vector<double> ampdInfo;
	ampdInfo.push_back(mpdSum / statisticalList.size());
	ampdInfo.push_back(max);
	ampdInfo.push_back(min);
	//var ampdInfo = {
	//avgMPD: mpdSum / statisticalList.length,
	//	maxMPD : max,
	//		 minMPD : min
	//};
	return ampdInfo;
	//calcular desvio padrão
}
string executeMetricsOption()
{
	FILE *file;
	fopen_s(&file, "C:\\Sites\\first_app\\teste.txt", "r");
	int wow = 0;
	if (file)
	for (int i = 0; i < 9; i ++)
		fscanf_s(file, "%d",&wow);
	int scenario = 0;
	int technology = 0;
	double H_TX = 3;
	double H_RX = 5;
	double BIT_RATE = 6;
	double TRANSMITTER_POWER = -6;
	int SRD = 1;
	int meshEnabled = 0;

	//readConfiguration(&scenario, &technology, &H_TX, &H_RX, &BIT_RATE, &TRANSMITTER_POWER, &SRD, &meshEnabled);

	int metersLength;
	//scanf_s("%d", &metersLength);
	fscanf_s(file, "%d", &metersLength);
	vector<Position*> meters;
	vector<Position*> daps;
	for (int i = 0; i < metersLength; i++)
	{
		double lat;
		double lng;
		//scanf_s("%lf %lf", &lat, &lng);
		fscanf_s(file, "%lf %lf", &lat, &lng);
		Position *toAdd = new Position(lat, lng);
		meters.push_back(toAdd);

	}
	int dapsLength = 0;
	//scanf_s("%d", &dapsLength);
	fscanf_s(file, "%d", &dapsLength);
	for (int i = 0; i < dapsLength; i++)
	{
		double lat;
		double lng;
		//scanf_s("%lf %lf", &lat, &lng);
		fscanf_s(file, "%lf %lf", &lat, &lng);
		Position *toAdd = new Position(lat, lng);
		daps.push_back(toAdd);
	}

	vector<vector<sComponent*>> sL = statisticalList( daps,  meters,  meshEnabled,  meshEnabled, scenario, technology,  BIT_RATE,TRANSMITTER_POWER,  H_TX,  H_RX, SRD);
	
	string answer = "";
	int numOfDaps = sL.size();
	answer += "Number of DAPs: " + to_string(numOfDaps) +"\n";
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

	vector<vector<double>> ammpd = avaregeMeshMetersPerDap(sL,meshEnabled);
	for (int i = 0; i < meshEnabled; i++)
	{
		double ammpdmedia = ammpd[i][0];
		double ammpdmax = ammpd[i][1];
		double ammpdmin = ammpd[i][2];
		double hop = ammpd[i][4];

		answer += "Average " + to_string(i) + " mesh hops meters: " + to_string(ammpdmedia) + "\n";
		answer += "Maximum "+ to_string(i) + " mesh hops meters: " + to_string(ammpdmax) + "\n";
		answer += "Minimum " + to_string(i) + " mesh hops meters: " + to_string(ammpdmin) + "\n";
	}
	vector<vector<double>> avgHops = averageHops(sL, meshEnabled);
	for (int i = 0; i < meshEnabled; i++)
	{
		double avgHopsEff = ammpd[i][0];
		double avgHopsQnt = ammpd[i][1];
		answer += "Average " + to_string(i) + " mesh hops efficiency: " + to_string(avgHopsEff) + "\n";
		answer += "Average " + to_string(i) + " mesh hops meters quantity: " + to_string(avgHopsQnt) + "\n";

	}
	return "banana";
	//	var coveredMetersNum = coveredMeters.length;
	//	var numOfDaps = daps.length;

	//	var metricInfo

}