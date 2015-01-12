
function initialize() {

    var MY_MAPTYPE_ID = 'custom_style';
    var mapOptions =
	{
	    zoom: 3,
	    center: new google.maps.LatLng(-28.643387, 0.612224),

	    mapTypeControl: true,
	    panControl: false,
	    zoomControl: true,    
	    
	    zoomControlOptions:
		{
		    position: google.maps.ControlPosition.TOP_RIGHT

		}

	}

    map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions);
    // google.maps.event.addListener(map, 'idle', showMarkers);
    google.maps.event.addListener(map,'bounds_changed', removeMarkers);
    elevator = new google.maps.ElevationService();  
    directionsService = new google.maps.DirectionsService();

    //var overlay = new google.maps.OverlayView();
    //overlay.setMap(map);
    
//             var ctaLayer = new google.maps.KmlLayer({
//    url: 'http://www.midiacom.uff.br/~grolim/DadosSiris.kml',
//    
//  });


  //          ctaLayer.setMap(map);


    //loadCarDriveFromXml();
    //loadNodesFromXml();
    //loadDapPositionsFromXml();
    //loadMetersFromTxt("gridTeste3.txt");
    //loadMetersFromTxt("filemeters9999999.txt");
    //loadPolesFromTxt("filepoles9999999.txt");
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
    //init();


   // applyPlanning();
}
function removeMarkers() {

   var bounds = map.getBounds();

  // var sw = bounds.getSouthWest();
  // var ne = bounds.getNorthEast();
  // var bounds = new google.maps.LatLngBounds(sw, ne);
    for (var i = 0 ; i < meters.length; i++){
        if (!bounds.contains(new google.maps.LatLng(meters[i].getPosition().lat(), meters[i].getPosition().lng()))) {
            meters[i].setVisible(false);     
        }
        else{
            meters[i].setVisible(true);
        }   
    }
    for (var i = 0 ; i < poles.length; i++){
        if (!bounds.contains(new google.maps.LatLng(poles[i].getPosition().lat(), poles[i].getPosition().lng()))) {
            poles[i].setVisible(false);     
        }
        else{
            poles[i].setVisible(true);
        }   
    }
    for (var i = 0 ; i < daps.length; i++){
        if (!bounds.contains(new google.maps.LatLng(daps[i].getPosition().lat(),daps[i].getPosition().lng()))) {
            daps[i].setVisible(false);     
        }
        else{
            daps[i].setVisible(true);
        }   
    }
       

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
var pi_180 = Math.PI / 180.0;
var pi_4 = Math.PI * 4;

function LatLongToPixelXY(latitude, longitude) {

    var sinLatitude = Math.sin(latitude * pi_180);
    var pixelY = (0.5 - Math.log((1 + sinLatitude) / (1 - sinLatitude)) /(pi_4)) * 256;
    var pixelX = ((longitude + 180) / 360) * 256;

    //var pixel = new Object();
    //pixel.x = (0.5 + pixelX) | 0;
    //pixel.y = (0.5 + pixelY) | 0;

    var pixel =  { x: pixelX, y: pixelY};
    
    return pixel;
}
google.maps.event.addDomListener(window, 'load', initialize);
