

function resetDraw(){
    for(var i = 0; i < lines.length; i++){
        lines[i].setMap(null);
    }
    lines = [];

}

function drawLine(latlng1,latlng2,color,efficiency,distance,dashed){
    var markerPositions = [latlng1, latlng2];
    var c;
    switch(color){
        case GREEN_ID:
            c = GREEN;
            break;
        case YELLOW_ID:
            c = YELLOW;
            break;
        case BLUE_ID:
            c = BLUE;
            break;
        default:
            break;
    }
    var routerPath ;
    if(dashed){
        var lineSymbol =
        {
            path: 'M 0,-1 0,1',
            strokeOpacity: 1,
            scale: 3
        };
        routerPath = new google.maps.Polyline(
        {
            path: markerPositions,
            strokeColor: c,
            strokeOpacity: 0,
            icons: [{
                icon: lineSymbol,
                offset: '0',
                repeat: '15px'
            }],
            clickable: false,
            strokeWeight: 2,
            distance: distance,
            efficiency: efficiency
        });

    }
    else{
        routerPath = new google.maps.Polyline(
        {
        path: markerPositions,
        strokeColor: c,
        strokeOpacity: 1.0,
        strokeWeight: 2,
        clickable: false,
        distance: distance,
        efficiency: efficiency
        });
    }
    
    lines.push(routerPath);
    routerPath.setMap(map);
}

function drawLine2(marker1, marker2, colorname)
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
