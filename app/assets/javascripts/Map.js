

function initialize() {

    var MY_MAPTYPE_ID = 'custom_style';
    var mapOptions =
	{
	    zoom: 3,
//	    minZoom: 3,
//        maxZoom: 18,
	    center: new google.maps.LatLng(-28.643387, 0.612224),
	    // mapTypeId: google.maps.MapTypeId.ROADMAP,
	  
	    mapTypeControl: true,
	    panControl: false,
	    zoomControl: true,    
	    //mapTypeControlOptions: {
	    //    mapTypeIds: [google.maps.MapTypeId.ROADMAP, MY_MAPTYPE_ID]
	    //},
	    //mapTypeId: MY_MAPTYPE_ID,
	    
	    zoomControlOptions:
		{
		    position: google.maps.ControlPosition.TOP_RIGHT

		},
	    //mapTypeControlOptions:
		//{
		//    style: google.maps.MapTypeControlStyle.HORIZONTAL_BAR,
		//    position: google.maps.ControlPosition.TOP_RIGHT

		//},

	}

    //var featureOpts = [
    //{
    //    stylers: [
    //   { "lightness": -70 },
    //   { "saturation": -67 }
    //    ]
    //},
    
    //];
    map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions);
   
    //var styledMapOptions = {
    //    name: 'Custom Style'
    //};

    //var customMapType = new google.maps.StyledMapType(featureOpts, styledMapOptions);

    //map.mapTypes.set(MY_MAPTYPE_ID, customMapType);
    
    // Create an ElevationService
    elevator = new google.maps.ElevationService();  
   // clusterMap();
    
 //   var ctaLayer = new google.maps.KmlLayer({
 //       url: 'http://www.midiacom.uff.br/~grolim/KmlExamples/agronomicakml.kml'
 //   });
 //   ctaLayer.setMap(map);
    //executeBER();
    loadCarDriveFromXml();
    loadNodesFromXml();
    loadDapPositionsFromXml();
    //var trapezoid = $('#trapezoid');
    //trapezoid.index = 1;
    map.controls[google.maps.ControlPosition.TOP_CENTER].push(
  document.getElementById('trapezoid'));
    markerCluster = new MarkerClusterer(map,[], {
        //maxZoom: 13,
        minimumClusterSize: 50,
        gridSize: 30
    });
    
    //markerCluster = new MarkerClusterer(map, allMarkers);
    drawHeatMap();
    insertListener = google.maps.event.addListener(map, 'click', function (event) {
        //if (opMode == "Insertion") {
        //    placeDAP(event.latLng.lat(), event.latLng.lng(), currentTech);

        //}
    });
    
    setButtons();
    createTableFromOptions();

   // applyPlanning();
}
function clusterMap() {
    for (var i = 0; i < allMarkers.length; i++) {
        allMarkers[i].setOptions({ map: map, visible: true });
    }
    markerCluster = new MarkerClusterer(map, allMarkers);
    // markerCluster.setMap(null);
    markerCluster.setGridSize(3);
    // markerCluster.refresh();
}

function unclusterMap() {
    markerCluster.clearMarkers();
    // markerCluster.refresh();
    for (var i = 0; i < allMarkers.length; i++) {
        allMarkers[i].setOptions({ map: map, visible: true });
    }
}
google.maps.event.addDomListener(window, 'load', initialize);
