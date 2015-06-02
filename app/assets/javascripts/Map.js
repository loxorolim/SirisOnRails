
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
    var input = (document.getElementById('pac-input'));
    map.controls[google.maps.ControlPosition.TOP_LEFT].push(input);
    var searchBox = new google.maps.places.SearchBox((input));


    // google.maps.event.addListener(map, 'idle', showMarkers);
    //google.maps.event.addListener(map,'dragend', drawGridElements);
   // elevator = new google.maps.ElevationService();  
    directionsService = new google.maps.DirectionsService();

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

       event.setMap(null);
       sendDrawRequest();
  });
    //loadFromKML();

    //var overlay = new google.maps.OverlayView();
    //overlay.setMap(map);
    



    //loadCarDriveFromXml();
    //loadNodesFromXml();
    //loadDapPositionsFromXml();
   // loadMetersFromTxt("gridTeste3.txt");
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
	elementsGrid = createGrid();
	elementsGrid.startGrid(0.001);
	
    //init();


   // applyPlanning();
}
 function drawGridElements() {

	var x  = ("Num cells: " + elementsGrid.getNumberOfCells());
	//elementsGrid.drawCells();
    var inBounds = elementsGrid.getCellsInWindow(map);//CELULAS QUE ESTÃO VISIVEIS QUANDO O USUARIO TERMINA DE ARRASTAR

    for (key in inBounds) { 
        if (!visibleCells.hasOwnProperty(key)) {
            for(var j = 0; j < inBounds[key].length;j++){
                inBounds[key][j].setVisible(true);
            }
        }
        else{
            delete visibleCells[key];
        }
    }
    for (key in visibleCells) {        
        for(var j = 0; j < visibleCells[key].length;j++){
            visibleCells[key][j].setVisible(false);
        }
    
    }    
    visibleCells = inBounds;
	/*for(var i = 0; i < visibleCells.length; i++)
        for(var j = 0; j < visibleCells[i].length;j++)
            visibleCells[i][j].setVisible(false); 

	visibleCells = inBounds;
    for(var i = 0; i < inBounds.length; i++)
        for(var j = 0; j < inBounds[i].length;j++)
            inBounds[i][j].setVisible(true); 
			
	*/
    var y = (inBounds.length);
	
       

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
