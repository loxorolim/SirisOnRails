var overralQualitySum = 0;

function collectInfo(){

	var coveredMeters = meters.filter(function (item) {
            return (item.connected == true && connectedMeters.indexOf(item) < 0);
        });
	var coveredMetersNum = coveredMeters.length;
	var numOfDaps = daps.length;

	var metricInfo
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
/*function c(){

	var avgQualitySum = 0;
    var metersConnected = 0;
    var avgDistanceSum = 0;
    var nM = createNeighbourhoodMatrix();
    for(var i = 0; i < daps.length; i ++){
    	var directNeighbours = daps[i].neighbours;
    	var metersCovered = 0;
    	 for(var i = 0; i < directNeighbours.length; i ++){
	        var dist = getDistance(this, directNeighbours[i]);
	        var value = getValuesFromTable(dist);
	        avgQualitySum += value.efficiency;
	        metersCovered++;
   		 }
   		 if(meshEnabled){
   		 	
   		 	var nextHopNeighbours = function(neighbours){
   		 		var newNeighbours = [];
   		 		 	for(var k = 0; k < neighbours.length; k++){
                    newNeighbours = newNeighbours.concat(nM[neighbours[k]]);
                }
                newNeighbours = newNeighbours.filter(function (elem, pos) {
                return newNeighbours.indexOf(elem) == pos;
                });
                return newNeighbours;
   		 	}
   			
   		 	for(var j = 0; j < meshMaxJumps; j ++){
   		 		var newNeighbours = nextHopNeighbours(directNeighbours);
   		 	}
   		 }


    }

}
*/

function statisticalMatrix() {
	var sM = [];
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
					hop: 0
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
            				var meshComponent = {
	            				index: aux[l].index,
			            		distance: aux[l].distance,
			            		efficiency: aux[l].efficiency,
								hop: k+1
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
		sM.push(toAdd);
	}
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
