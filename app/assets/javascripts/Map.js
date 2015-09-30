
function initialize() {

    var MY_MAPTYPE_ID = 'custom_style';
    var mapOptions =
	{
	    zoom: 4,
	    center: new google.maps.LatLng(-20.30273, -40.28118),

	    mapTypeControl: true,
	    panControl: false,
	    zoomControl: true,    
	    
	    zoomControlOptions:
		{
		    position: google.maps.ControlPosition.TOP_RIGHT

		}

	}
	
    map = new google.maps.Map(document.getElementById('map_canvas'), mapOptions);
    var input = (document.getElementById('pac-input'));
    map.controls[google.maps.ControlPosition.TOP_LEFT].push(input);
    var searchBox = new google.maps.places.SearchBox((input));
	//$( document ).tooltip();

    // google.maps.event.addListener(map, 'idle', showMarkers);
    //google.maps.event.addListener(map,'dragend', drawGridElements);
   // elevator = new google.maps.ElevationService();  
   // directionsService = new google.maps.DirectionsService();

    drawingManager = new google.maps.drawing.DrawingManager({
    drawingMode: null,
    drawingControl: false,
    rectangleOptions: {
      fillColor: '#0000ff',
      fillOpacity: 0.1,
      strokeWeight: 1,
      clickable: false,
      editable: false,
      zIndex: 1,

    }

  });
  drawingManager.setMap(map);
  google.maps.event.addListener(drawingManager, 'rectanglecomplete', function (event) {
       var recBounds = event.getBounds();
       switch(removeSelectionMode){

            case REMOVE_ALL:
            case REMOVE_DAPS:
                for(var i = 0; i < daps.length; i++){
                    if(recBounds.contains(daps[i].getPosition())){
                        daps[i].remove();
                        i--;
                    }
                } 
                if(removeSelectionMode != REMOVE_ALL)
                    break;
            case REMOVE_METERS:
                for(var i = 0; i < meters.length; i++){
                    if(recBounds.contains(meters[i].getPosition())){
                        meters[i].remove();
                        i--;
                    }
                }
                if(removeSelectionMode != REMOVE_ALL)
                    break;
            case REMOVE_POLES:
                for(var i = 0; i < poles.length; i++){
                    if(recBounds.contains(poles[i].getPosition())){
                        poles[i].remove();
                        i--;
                    }
               }
            break;

            default:
            break;

       }
       markerCluster.repaint();
       event.setMap(null);
       sendDrawRequest();
  });
   
    map.controls[google.maps.ControlPosition.TOP_CENTER].push(
	document.getElementById('trapezoid'));
    markerCluster = new MarkerClusterer(map,[], {
        maxZoom: 30,
        minimumClusterSize: 50,
        gridSize: 30
    });
    
    markerCluster = new MarkerClusterer(map);
    //drawHeatMap();
    insertListener = google.maps.event.addListener(map, 'click', function (event) {
        //if (opMode == "Insertion") {
        //    placeDAP(event.latLng.lat(), event.latLng.lng(), currentTech);

        //}
    });
    google.maps.event.addListener(searchBox, 'places_changed', function() {
        var places = searchBox.getPlaces();

        if (places.length == 0) {
          return;
        }
        var bounds = new google.maps.LatLngBounds();
        bounds.extend(places[0].geometry.location);
        map.fitBounds(bounds);
        map.setZoom(13);
  });
    
    setButtons();
    setScenario(Suburbano);
    setPower(20);
    setTechnology(t802_11_g);
    setBitRate(6);
	setHTX(3);
	setHRX(5);
	setMeshHops(3);
	
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
