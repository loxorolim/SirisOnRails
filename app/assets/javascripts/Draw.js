

function drawLine(marker1, marker2, colorname)
{
    var markerPositions = [marker1.getPosition(), marker2.getPosition()];
    var color;
    if (colorname == "GREEN") 
        color = "#00FF00";   
    else
        if (colorname == "YELLOW") 
            color = "#FFFF00"       
        else 
            color = "#0000FF"
        

    var routerPath = new google.maps.Polyline(
	{
	    path: markerPositions,
	    strokeColor: color,
	    strokeOpacity: 1.0,
	    strokeWeight: 2,
	    clickable: false
	});
    lines.push(routerPath);
    routerPath.setMap(map);
    //lines[lines.length - 1].setMap(map);

    if (radioMode == "Radius") 
        routerPath.setVisible(false);
    

}
function drawDashedLine(marker1, marker2, colorname)
{
    var lineSymbol =
    {
        path: 'M 0,-1 0,1',
        strokeOpacity: 1,
        scale: 3
    };
    var markerPositions = [marker1.getPosition(), marker2.getPosition()];
    var color;
    if (colorname == "GREEN") 
        color = "#00FF00";   
    else
        if (colorname == "YELLOW")
            color = "#FFFF00"        
        else 
            color = "#0000FF"       

    var routerPath = new google.maps.Polyline(
	{
	    path: markerPositions,
	    strokeColor: color,
	    strokeOpacity: 0,
	    icons: [{
	        icon: lineSymbol,
	        offset: '0',
	        repeat: '15px'
	    }],
	    clickable: false,
	    strokeWeight: 2
	});
    dashedLines.push(routerPath);
    dashedLines[dashedLines.length - 1].setMap(map);
    routerPath.setMap(map);


}
function getCircleColorPositions() {
    var medPos1;
    var medPos2;
    var endPos = table[table.length - 1].distance;
    for (var i = 0; i < table.length - 1 ; i++)
    {
        if (table[i].color == "GREEN" && table[i + 1].color == "YELLOW")
        {
            //medPos1 = (table[i].distance + table[i+1].distance)/2; 
            medPos1 = table[i].distance;
        }
        if (table[i].color == "YELLOW" && table[i + 1].color == "RED")
        {
            //medPos2 = (table[i].distance + table[i+1].distance)/2;
            medPos2 = table[i].distance;
        }
    }
    var positions =
	{
	    ini: 0,
	    med1: medPos1,
	    med2: medPos2,
	    end: endPos
	}
    return positions;
}
function drawCircle(marker)
{
    var positions = getCircleColorPositions();

    var greenCircle;
    var yellowCircle;
    var redCircle;
    redCircle = new google.maps.Circle(
	{
	    center: marker.getPosition(),
	    radius: positions.end,
	    strokeColor: "#0000FF",
	    strokeOpacity: 0.8,
	    strokeWeight: 0,
	    fillColor: "#0000FF",
	    fillOpacity: 0.35,
	    clickable: false,
	    map: map
	});
    yellowCircle = new google.maps.Circle(
	{
	    center: marker.getPosition(),
	    radius: positions.med2,
	    strokeColor: "#FFFF00",
	    strokeOpacity: 0.8,
	    strokeWeight: 0,
	    fillColor: "#FFFF00",
	    fillOpacity: 0.35,
	    clickable: false,
	    map: map
	});
    greenCircle = new google.maps.Circle(
	{
	    center: marker.getPosition(),
	    radius: positions.med1,
	    strokeColor: "#00FF00",
	    strokeOpacity: 0.8,
	    strokeWeight: 0,
	    fillColor: "#00FF00",
	    fillOpacity: 0.35,
	    clickable: false,
	    map: map
	});
    marker.reachCircles = [greenCircle, yellowCircle, redCircle];
    if (radioMode != "Radius")
    {
        greenCircle.setVisible(false);
        yellowCircle.setVisible(false);
        redCircle.setVisible(false);
    }
}
function removeMarkerCircles(marker)
{
    marker.reachCircles[0].setVisible(false);
    marker.reachCircles[1].setVisible(false);
    marker.reachCircles[2].setVisible(false);
}

function drawRefresh()
{
    infowindow.setMap(null);
    for (j = 0; j < allMarkers.length; j++)
    {
        removeMarkerConnections(allMarkers[j]);
        connectNodesByDistance(allMarkers[j]);
        if (allMarkers[j].type != "Meter")
        {
            removeMarkerCircles(allMarkers[j]);
            drawCircle(allMarkers[j]);
        }
    }
}
function setLinesInvisible()
{
    for (i = 0; i < lines.length; i++)
    {
        lines[i].setVisible(false);
    }
}
function setCirclesInvisible()
{
    for (i = 0; i < allMarkers.length; i++)
    {
        if (allMarkers[i].reachCircles != null)
        {
            allMarkers[i].reachCircles[2].setVisible(false);
            allMarkers[i].reachCircles[1].setVisible(false);
            allMarkers[i].reachCircles[0].setVisible(false);
        }
    }
}
function setLinesVisible()
{
    for (i = 0; i < lines.length; i++)
    {
        lines[i].setVisible(true);
    }
}
function setCirclesVisible()
{
    for (i = 0; i < allMarkers.length; i++)
    {
        if (allMarkers[i].reachCircles != null) {
            allMarkers[i].reachCircles[2].setVisible(true);
            allMarkers[i].reachCircles[1].setVisible(true);
            allMarkers[i].reachCircles[0].setVisible(true);
        }
    }
}