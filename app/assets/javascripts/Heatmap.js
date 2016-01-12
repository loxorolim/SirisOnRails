

function setHeatmap(){
	if(heatmapPolygon)
        heatmapPolygon.setMap(null);
    var circles = [];
	for(var i = 0; i < heatmapPoints.length; i++){
			circles.push(drawCircle(heatmapPoints[i].position,heatmapEffectRange/1609.344,1)) ;
	}
    heatmapPolygon = new google.maps.Polygon({
                 paths: circles,
                 strokeColor: "#00FF00",
                 strokeOpacity: 0.35,
                 strokeWeight: 0,
                 fillColor: "#00BED0",
                 fillOpacity: 0.35,
                 clickable: false
     });
     //heatmapPolygon.setMap(map);
}
function insertHeatmapPoint(lat, lng, weight, operators){
    //var loc = new google.maps.LatLng(lat, lng)
    var hmpoint = createHeatmapPoint();
    hmpoint.placeOnMap(lat, lng, operators);
    //var hmpoint = {
    //    position: loc,
    //    weight: weight,
    //    opIds: operators
    //}
    heatmapPoints.push(hmpoint);
}

