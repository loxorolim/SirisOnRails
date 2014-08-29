var overralQualitySum = 0;

function collectInfo(){
	var sL = statisticalList();
	var numOfDaps = sL.length;
//	var coveredMeters = meters.filter(function (item) {
//            return (item.connected == true && connectedMeters.indexOf(item) < 0);
//        });
	var alpd = averageLinksPerDap(sL);
	var ampd = averageMetersPerDap(sL);
	var ammpd = avaregeMeshMetersPerDap(sL);
	var avgHops = averageHops(sL);

//	var coveredMetersNum = coveredMeters.length;
//	var numOfDaps = daps.length;

//	var metricInfo
	
}
function averageDirectLinkQuality(){
	var avgQuality = 0 , numOfLinks=0;
	for(var i = 0; i < daps.length; i++){		
		for(var j = 0; j < daps[i].connectionLines.length; j++){
			avgQuality += daps[i].connectionLines[j].efficiency;
			numOfLinks++;
		}
		
	}
	return avgQuality/numOfLinks;

}
function averageHops(statisticalList){
	var avgHop = [];
	for(var i = 0; i < meshMaxJumps+1; i++){
		var sum = 0;
		var qnt = 0;
		for(var j = 0; j < statisticalList.length; j++){
			for(var k = 0; k < statisticalList[j].length; k++){
				if(statisticalList[j][k].hop == i){
					var calcEfficiency = function(meter){
						var eff = 1;
						var hops = meter.hop;
						var m = meter;
						for(var i = 0; i < hops+1;i++){
							eff*= m.efficiency;
							m = m.meshFather;

						}
						return eff;
					};

					sum+= calcEfficiency(statisticalList[j][k]);
					qnt++;
				}
			}
		}
		var toAdd = {
			hop: i,
			avgHopEfficiency: sum/qnt,
			hopQnt: qnt
		};
		avgHop.push(toAdd);
	}
	return avgHop;
}
function averageLinksPerDap(statisticalList){
	var lpdSum = 0, max=-1, min =-1;
	for(var i = 0; i < statisticalList.length; i++){
		var nLinks = statisticalList[i].length;
		lpdSum+= nLinks;
		if(max == -1 || nLinks > max)
			max = nLinks;
		if(min == -1 || nLinks < min)
			min = nLinks;
	}
	var alpdInfo = {
		avgLPD : lpdSum/statisticalList.length,
		maxLPD : max,
		minLPD : min
	};
	return alpdInfo;
	//calcular desvio padrão
}
function avaregeMeshMetersPerDap(statisticalList){
	var mmpdSum = [];
	for(var z = 0; z < meshMaxJumps+1; z++){
		var sum =0 , max=-1, min =-1;;
		for(var i = 0; i < statisticalList.length; i++){
		var noRepeat = function(list){
			var ret = [];
			for(var i = 0; i < list.length; i++){
				var add = true;
				for(var j = 0; j < ret.length;j++)
					if(list[i].index == ret[j].index){
						add = false;
						break;
					}
				if(add)
					ret.push(list[i]);
			}
			return ret;
		};
		var uniqueArray = noRepeat(statisticalList[i]);	
		var nMeters =0;
		for(var j =0; j< uniqueArray.length; j++)
			if(uniqueArray[j].hop==z)
				nMeters++;

		sum += nMeters;
		if(max == -1 || nMeters > max)
			max = nMeters;
		if(min == -1 || nMeters < min)
			min = nMeters;
		}
		var ammpdInfo = {
			avgMMPD : sum/statisticalList.length,
			maxMMPD : max,
			minMMPD : min,
			hop: z
		};
		mmpdSum.push(ammpdInfo);
	}
	
	return mmpdSum;
}
function averageMetersPerDap(statisticalList){
	var mpdSum = 0, max=-1, min =-1;

	for(var i = 0; i < statisticalList.length; i++){
		var noRepeat = function(list){
			var ret = [];
			for(var i = 0; i < list.length; i++){
				var add = true;
				for(var j = 0; j < ret.length;j++)
					if(list[i].index == ret[j].index){
						add = false;
						break;
					}
				if(add)
					ret.push(list[i]);
			}
			return ret;
		};
		var uniqueArray = noRepeat(statisticalList[i]);	
		var nMeters = uniqueArray.length;
		mpdSum+= nMeters;
		if(max == -1 || nMeters > max)
			max = nMeters;
		if(min == -1 || nMeters < min)
			min = nMeters;
	}
	var ampdInfo = {
		avgMPD : mpdSum/statisticalList.length,
		maxMPD : max,
		minMPD : min
	};
	return ampdInfo;
	//calcular desvio padrão
}


function statisticalList() {
	var sL = [];
	for(var i = 0 ; i < daps.length; i++){
		var toAdd = [];
		for(var j = 0; j < meters.length; j++){
			var dist = getDistance(daps[i].getPosition(), meters[j].getPosition());
			var values = getValuesFromTable(dist);
			if( values != -1){
				var component = {
					index: j,
					distance: values.distance,
					efficiency: values.efficiency,
					hop: 0,
					meshFather: null 
				};
				toAdd.push(component);
			}
		}
		if(meshEnabled){
			var nM = createNeighbourhoodMatrix();
			var neighbours = toAdd;
			for(var k = 0; k < meshMaxJumps ; k++){
				 var newNeighbourhood = [];
				 var meshToAdd = [];
	            for (var j = 0 ; j < neighbours.length; j++){

	            	var aux = nM[neighbours[j].index];
	            	for(var l = 0; l < aux.length; l++){
	            		var containsElement = function (array, elem){
									for(var i = 0; i < array.length; i++)
										if(array[i].index == elem)
											return true;									
									return false;
								};
									
	            		if(!containsElement(toAdd,aux[l].index)){
	            			var father;
	            			for(var z = 0; z < toAdd.length;z++){
	            				if(toAdd[z].index == neighbours[j].index){
	            					father = toAdd[z];
	            					break;
	            				}
	            			}
	            			

            				var meshComponent = {
	            				index: aux[l].index,
			            		distance: aux[l].distance,
			            		efficiency: aux[l].efficiency,
								hop: k+1,
								meshFather:father
		            		};

			                meshToAdd = meshToAdd.concat(meshComponent);
			                newNeighbourhood = newNeighbourhood.concat(nM[neighbours[j].index]);
			            }
	            	
	            	}
	            	
	                
	            }
	            neighbours = newNeighbourhood.filter(function (elem, pos) {
	                return newNeighbourhood.indexOf(elem) == pos;
	            });
	            toAdd = toAdd.concat(meshToAdd);
			}
		}
		sL.push(toAdd);
	}
	return sL;
}
function createNeighbourhoodMatrix() {
   // var points = metersToPoints(meters);
    //var r = getDapMaximumReach();
    var M = [];

    for (var i = 0 ; i < meters.length ; i++) {
        var pointsCovered = [];
        for (var j = 0; j < meters.length; j++){
        	var values = getValuesFromTable(getDistance(meters[i].getPosition(), meters[j].getPosition()));
        	 if (i != j && values != -1) {
        	 	var m = {
        	 		index: j,
        	 		efficiency: values.efficiency,
        	 		distance: values.distance
        	 	};
                //pointsCovered.push(j);
                pointsCovered.push(m);
            }
        }
           
        M.push(pointsCovered);
    }

    return M;
}
