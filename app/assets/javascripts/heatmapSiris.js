const heatmapEffectRange = 20; //20 metros
const heatmapEfficiencyLimit = 0.8; //90%

function setHeatmap(){
	if(heatmapPolygon)
        heatmapPolygon.setMap(null);
    var circles = [];
	for(var i = 0; i < heatmapPoints.length; i++){
		if(heatmapPoints[i].weight >= heatmapEfficiencyLimit)
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