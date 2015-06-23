

function resetDraw(){
    for(var i = 0; i < meters.length; i++)
        meters[i].hop = null;
    for(var i = 0; i < lines.length; i++){
        lines[i].setMap(null);
    }
    lines = [];

}
function colorInterpolation(efficiency)
{

	var yellowvalR = 0x00;	var yellowvalG = 0xff;	var yellowvalB = 0x00;
	var bluevalR = 0xff; var bluevalG = 0xff; var bluevalB = 0x00;

	var val = (yellowvalR - bluevalR)/0.1;
	var cDec =  Math.floor(bluevalR + (efficiency - 0.90)*val);
	var cR = cDec.toString(16);	
	if(cDec < 16)
		cR = "0" + cR;;

	var val = (yellowvalG - bluevalG)/0.1;
	var cDec =  Math.floor(bluevalG + (efficiency - 0.90)*val);
	var cG = cDec.toString(16);	
	if(cDec < 16)
		cG = "0" + cG;;
	var val = (yellowvalB - bluevalB)/0.1;
	var cDec =  Math.floor(bluevalB + (efficiency - 0.90)*val);
	var cB = cDec.toString(16);	
	if(cDec < 16)
		cB = "0" + cB;;	
	return "#"+cR+cG+cB;
	
}
function drawLine(latlng1,latlng2,efficiency,distance,dashed){
    var markerPositions = [latlng1, latlng2];
    var c = colorInterpolation(efficiency);


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
            clickable: true,
            strokeWeight: 2,
            distance: distance,
            efficiency: efficiency,
            
        });
 

    }
    else{
        routerPath = new google.maps.Polyline(
        {
        path: markerPositions,
        strokeColor: c,
        strokeOpacity: 1.0,
        strokeWeight: 2,
        clickable: true,
        distance: distance,
        efficiency: efficiency
        });
    }
           google.maps.event.addListener(routerPath, 'click', function (event) {
            alert(routerPath.distance + " " + routerPath.efficiency);

            });
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
function drawCircle(point, radius, dir)
{ 
    var d2r = Math.PI / 180;   // degrees to radians 
    var r2d = 180 / Math.PI;   // radians to degrees 
    var earthsradius = 3963; // 3963 is the radius of the earth in miles
    var points = 32; 

    // find the raidus in lat/lon 
    var rlat = (radius / earthsradius) * r2d; 
    var rlng = rlat / Math.cos(point.lat() * d2r); 

    var extp = new Array(); 
    if (dir==1) {var start=0;var end=points+1} // one extra here makes sure we connect the
    else{var start=points+1;var end=0}
    for (var i=start; (dir==1 ? i < end : i > end); i=i+dir)  
    {
        var theta = Math.PI * (i / (points/2)); 
        ey = point.lng() + (rlng * Math.cos(theta)); // center a + radius x * cos(theta) 
        ex = point.lat() + (rlat * Math.sin(theta)); // center b + radius y * sin(theta) 
        extp.push(new google.maps.LatLng(ex, ey));
    }
    return extp;
}
