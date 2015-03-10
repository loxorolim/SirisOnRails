
function initialize() {

    var MY_MAPTYPE_ID = 'custom_style';
    var mapOptions =
	{
	    zoom: 17,
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

/*    var grid = createGrid();
    var positions = []
    for(var i = 0; i < 10; i++){
        var latLng = new google.maps.LatLng(i, i);
        var marker = new google.maps.Marker();
        marker.position = latLng;
        positions.push(marker);

        var latLng2 = new google.maps.LatLng(i+1, i-1);
        var marker2 = new google.maps.Marker();
        marker2.position = latLng2;
        positions.push(marker2)

    }
    grid.startGrid(positions,2);
    grid.putPositions(positions);
    var teste = grid.getAllElements();
    var latLng = new google.maps.LatLng(-33, -33);
    var marker3 = new google.maps.Marker();
    marker3.position = latLng;
    //var retorno = grid.getCell(latLng);
    grid.putPosition(marker3);
    grid.getCellsInWindow(map);
    */



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
    setScenario(Suburbano);
    setPower(20);
    setTechnology(t802_11_g);
    //init();


   // applyPlanning();
}
 function removeMarkers() {


    var grid = createGrid();
    var positions = [];
    for(var i = 0; i < meters.length; i++){
         positions.push(meters[i]);
    }
    grid.startGrid(positions,0.01);
    grid.putPositions(positions);
    var inBounds = grid.getCellsInWindow(map);
    for(var i = 0; i < inBounds.length; i++)
        for(var j = 0; j < inBounds[i].length;j++)
            inBounds[i][j].setVisible(false);
       

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
