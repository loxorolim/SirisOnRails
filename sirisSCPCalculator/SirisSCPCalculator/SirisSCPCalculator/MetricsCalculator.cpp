#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include "auxiliars.h"
using namespace std;



//vector<vector<sComponent>> statisticalList(vector<Position> &daps, vector<Position> &meters, vector<Position> a, int meshEnabled)
//{
//	vector<vector<sComponent>> sL;
//	for (int i = 0; i < daps.size(); i++)
//	{
//		vector<sComponent> toAdd;
//		for (int j = 0; j < meters.size(); j++)
//		{
//			float dist = getDistance(daps[i], meters[j]);
//			Value *values = getValuesFromTable(dist);
//		
//			if (values != NULL){
//				sComponent component;
//				component.index = j;
//				component.distance;
//				component.efficiency = values->efficiency;
//				component.distance = values->distance;
//				component.hop = 0;
//				component.meshFather = NULL;
//
//				toAdd.push_back(component);
//			}
//		}
//		if (meshEnabled){
//			var nM = createNeighbourhoodMatrix();
//			var neighbours = toAdd;
//			for (var k = 0; k < meshMaxJumps; k++){
//				var newNeighbourhood = [];
//				var meshToAdd = [];
//				for (var j = 0; j < neighbours.length; j++){
//
//					var aux = nM[neighbours[j].index];
//					for (var l = 0; l < aux.length; l++){
//						var containsElement = function(array, elem){
//							for (var i = 0; i < array.length; i++)
//							if (array[i].index == elem)
//								return true;
//							return false;
//						};
//
//						if (!containsElement(toAdd, aux[l].index)){
//							var father;
//							for (var z = 0; z < toAdd.length; z++){
//								if (toAdd[z].index == neighbours[j].index){
//									father = toAdd[z];
//									break;
//								}
//							}
//
//
//							var meshComponent = {
//							index: aux[l].index,
//							   distance : aux[l].distance,
//									  efficiency : aux[l].efficiency,
//											   hop : k + 1,
//												 meshFather : father
//							};
//
//							meshToAdd = meshToAdd.concat(meshComponent);
//							newNeighbourhood = newNeighbourhood.concat(nM[neighbours[j].index]);
//						}
//
//					}
//
//
//				}
//				neighbours = newNeighbourhood.filter(function(elem, pos) {
//					return newNeighbourhood.indexOf(elem) == pos;
//				});
//				toAdd = toAdd.concat(meshToAdd);
//			}
//		}
//		sL.push(toAdd);
//	}
//	return sL;
//}
//function createNeighbourhoodMatrix() {
//	// var points = metersToPoints(meters);
//	//var r = getDapMaximumReach();
//	var M = [];
//
//	for (var i = 0; i < meters.length; i++) {
//		var pointsCovered = [];
//		for (var j = 0; j < meters.length; j++){
//			var values = getValuesFromTable(getDistance(meters[i].getPosition(), meters[j].getPosition()));
//			if (i != j && values != -1) {
//				var m = {
//				index: j,
//				   efficiency : values.efficiency,
//							distance : values.distance
//				};
//				//pointsCovered.push(j);
//				pointsCovered.push(m);
//			}
//		}
//
//		M.push(pointsCovered);
//	}
//
//	return M;
//}