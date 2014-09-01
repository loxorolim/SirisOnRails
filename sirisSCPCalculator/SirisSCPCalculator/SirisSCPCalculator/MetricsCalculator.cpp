#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
using namespace std;


bool containsElement (vector<int>, elem){
	for (var i = 0; i < array.length; i++)
	if (array[i].index == elem)
		return true;
	return false;
};

vector<vector<sComponent*>> statisticalList(vector<Position*> &daps, vector<Position*> &meters, vector<Position*> a, int meshEnabled, int meshMaxJumps, int env, int technology, double bit_rate, double transmitter_power, double h_tx, double h_rx, bool SRD)
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
			vector<sComponent*> neighbours = toAdd;
			for (int k = 0; k < meshMaxJumps; k++){
				vector<sComponent*> newNeighbourhood;
				vector<sComponent*> meshToAdd ;
				for (int j = 0; j < neighbours.size(); j++){

					vector<int> aux = nM[neighbours[j]->index];
					for (int l = 0; l < aux.size(); l++){
						
						
						if (!(find(toAdd.begin(), toAdd.end(), aux[l]) != toAdd.end()))
						{
							sComponent* father;
							for (int z = 0; z < toAdd.size(); z++){
								if (toAdd[z]->index == neighbours[j]->index){
									father = toAdd[z];
									break;
								}
							}
							 
							double distBetweenMeters = getDistance(meters[father->index],meters[aux[l]]);
							double effBetweenMeters = getHataSRDSuccessRate(distBetweenMeters, env, technology, bit_rate, transmitter_power, h_tx, h_rx, SRD);
							sComponent* meshComponent = new sComponent(aux[l], distBetweenMeters, effBetweenMeters, k + 1, father);

							meshToAdd.push_back(meshComponent);
							//meshToAdd = meshToAdd.concat(meshComponent);
							newNeighbourhood.insert(newNeighbourhood.end(), nM[neighbours[j]->index].begin(), nM[neighbours[j]->index].end());
							//newNeighbourhood = newNeighbourhood.concat(nM[neighbours[j].index]);
						}

					}


				}
				neighbours = newNeighbourhood.filter(function(elem, pos) {
					return newNeighbourhood.indexOf(elem) == pos;
				});
				toAdd = toAdd.concat(meshToAdd);
			}
		}
		sL.push(toAdd);
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

		M.push(pointsCovered);
	}

	return M;
}