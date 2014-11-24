function showNodesXml()
{
	var init = "&lt?xml version=\"1.0\" encoding=\"utf-8\"?&gt" + "<br>&ltNodes&gt";
	var meters = "";
	var daps = "";
	var fin = "";
	var markers = getAllMarkers();
	for ( i = 0; i < markers.length; i++)
	{	
		if(markers[i].type == "Meter")
		{
			meters += "<br>&nbsp&ltmeter&gt" 
			+ "<br>&nbsp&nbsp&ltLatitude&gt" + markers[i].position.lat() + "&lt\/Latitude&gt"
			+ "<br>&nbsp&nbsp&ltLongitude&gt" + markers[i].position.lng() + "&lt\/Longitude&gt" 
			+ "<br>&nbsp&lt\/meter&gt";
		}

		else 
		if(markers[i].type == "DAP")
		{
			daps += "<br>&nbsp&ltDAP&gt" 
			+ "<br>&nbsp&nbsp&ltLatitude&gt" + markers[i].position.lat() + "&lt\/Latitude&gt" 
			+ "<br>&nbsp&nbsp&ltLongitude&gt" + markers[i].position.lng() + "&lt\/Longitude&gt" 
			+ "<br>&nbsp&nbsp&ltTechnology&gt" + markers[i].teleTech + "&lt\/Technology&gt" 
			+ "<br>&nbsp&nbsp&ltsetInsertionOptions&gt" + markers[i].setInsertionOptions + "&lt\/setInsertionOptions&gt" 
			+ "<br>&nbsp&lt\/DAP&gt";
		}
	}
	fin += "<br>&lt\/Nodes&gt";
	$("#xmltextnodes").html(init+meters+daps+fin);
}
function loadMetersFromTxt( file)
{
	$(document).ready(function()
	{
		$.ajax(
		{
			type : "GET",
			url : "/assets/"+file,
			dataType : "text",
			success : function(text)
			{
				var split = text.split("\n");
				for(var i = 0; i < split.length; i++)
				{
					var pos = split[i].split(" ");
					var meter = createMeter();
					meter.placeOnMap(pos[1], pos[0]);
				}
			}
		});
	});
}
function loadPolesFromTxt( file)
{
	$(document).ready(function()
	{
		$.ajax(
		{
			type : "GET",
			url : "/assets/"+file,
			dataType : "text",
			success : function(text)
			{
				var split = text.split("\n");
				for(var i = 0; i < split.length; i++)
				{
					var pos = split[i].split(" ");
					var pole = createPole();
					pole.placeOnMap(pos[1], pos[0]);
				}
			}
		});
	});
}
function showNodesKml() {
    var init = "&lt?xml version=\"1.0\" encoding=\"UTF-8\"?&gt" + "<br>&ltkml xmlns=\"http://earth.google.com/kml/2.2\"&gt<br>&ltDocument&gt";
    var style = "<br>&ltStyle id=\"greenLine\"&gt<br>&ltLineStyle&gt<br>&ltcolor&gtFF00D214&lt\/color&gt<br>&ltwidth&gt5&lt\/width&gt<br>&lt\/LineStyle&gt<br>&lt\/Style&gt<br>&ltStyle id=\"blueLine\"&gt<br>&ltLineStyle&gt<br>&ltcolor&gtFFB40014&lt\/color&gt<br>&ltwidth&gt5&lt\/width&gt<br>&lt\/LineStyle&gt<br>&lt\/Style&gt<br>&ltStyle id=\"yellowLine\"&gt<br>&ltLineStyle&gt<br>&ltcolor&gtFF14F0FF&lt\/color&gt<br>&ltwidth&gt5&lt\/width&gt<br>&lt\/LineStyle&gt<br>&lt\/Style&gt";
    var meters = "";
    var daps = "";
    var linestext = "";
    var fin = "";
    var markers = getAllMarkers();
    for (i = 0; i < markers.length; i++) {
        if (markers[i].type == "Meter") {
            meters += "<br>&ltPlacemark&gt"
			+ "<br>&ltname&gt" + "Medidor" + "&lt\/name&gt"
			+ "<br>&ltdescription&gt" + "Um medidor inteligente" + "&lt\/description&gt"
            + "<br>&ltPoint&gt"
            + "<br>&ltcoordinates&gt" + markers[i].position.lng() + "," + markers[i].position.lat() + "&lt\/coordinates&gt"
            + "<br>&lt\/Point&gt"
			+ "<br>&lt\/Placemark&gt";
        }

        else
            if (markers[i].type == "DAP") {
                daps += "<br>&ltPlacemark&gt"
			    + "<br>&ltname&gt" + "DAP" + "&lt\/name&gt"
			    + "<br>&ltdescription&gt" + "Um DAP com tecnologia " + markers[i].teleTech + "&lt\/description&gt"
                + "<br>&ltPoint&gt"
                + "<br>&ltcoordinates&gt" + markers[i].position.lng() + "," + markers[i].position.lat() + "&lt\/coordinates&gt"
                + "<br>&lt\/Point&gt"
                + "<br>&lt\/Placemark&gt";
            }
    }
    for (i = 0; i < lines.length; i++) {
        var path = lines[i].getPath();
        var color = lines[i].strokeColor;
        linestext += "<br>&ltPlacemark&gt"
			    + "<br>&ltname&gt" + "Linha reta" + "&lt\/name&gt";
        if (color == "#00FF00")
            linestext += "<br>&ltstyleUrl&gt" + "#greenLine" + "&lt\/styleUrl&gt";
        if (color == "#FFFF00")
            linestext += "<br>&ltstyleUrl&gt" + "#yellowLine" + "&lt\/styleUrl&gt";
        if (color == "#0000FF")
            linestext += "<br>&ltstyleUrl&gt" + "#blueLine" + "&lt\/styleUrl&gt";
        linestext +=
			     "<br>&ltdescription&gt" + "A linha reta de uma conexao "  + "&lt\/description&gt"
                + "<br>&ltLineString&gt"
                + "<br>&ltcoordinates&gt" + path.j[0].lng() + "," + path.j[0].lat() + "," + "0 " + path.j[1].lng() + ","  + path.j[1].lat() + "," + "0 " + "&lt\/coordinates&gt"
                + "<br>&lt\/LineString&gt"
                + "<br>&lt\/Placemark&gt";

    }
    fin += "<br>&lt\/Document&gt<br>&lt\/kml&gt";
    $("#xmltextnodes").html(init + style + meters + daps + linestext+ fin);
}
function showPolesXml() {
    var init = "&lt?xml version=\"1.0\" encoding=\"utf-8\"?&gt" + "<br>&ltPositions&gt";
    var meters = "";
    var fin = "";
    var markers = poles;
    for (i = 0; i < markers.length; i++) {

            meters += "<br>&nbsp&ltpoint&gt"
			+ "<br>&nbsp&nbsp&ltLatitude&gt" + markers[i].position.lat() + "&lt\/Latitude&gt"
			+ "<br>&nbsp&nbsp&ltLongitude&gt" + markers[i].position.lng() + "&lt\/Longitude&gt"
			+ "<br>&nbsp&lt\/point&gt";
        
    }
    fin += "<br>&lt\/Positions&gt";
    $("#xmltextpoles").html(init + meters + fin);
}
function loadNodesFromXml()
{
	
	$(document).ready(function()
	{
		$.ajax(
		{
			type : "GET",
			url : "/assets/test2.xml",
			dataType : "xml",
			success : function(xml)
			{
				//$("node[name='x']");
				$(xml).find('meter').each(function()
				{
					var latitude = $(this).find('Latitude').text();
					var longitude = $(this).find('Longitude').text();
				    //placeMeter(latitude, longitude);
					var meter = createMeter();
					meter.placeOnMap(latitude, longitude);
				})
				$(xml).find('DAP').each(function()
				{
					var latitude = $(this).find('Latitude').text();
					var longitude = $(this).find('Longitude').text();
					var technology = $(this).find('Technology').text();
					var setInsertionOptions = parseInt($(this).find('setInsertionOptions').text(),10);
					
					// loadMarker(latitude,longitude);
				    //placeDAP(latitude, longitude, technology, setInsertionOptions);
					var dap = createDap();
					//dap.placeOnMap(latitude, longitude);
				});
				sendDrawRequest();
			}
		});
	});
}
function loadCarDriveFromXml() {
    heatmapPoints = [];
    $(document).ready(function () {
        $.ajax(
		{
		    type: "GET",
		    url: "/assets/cardrive.xml",
		    dataType: "xml",
		    success: function (xml) {
		        //$("node[name='x']");
		        $(xml).find('point').each(function () {
		            var latitude = parseFloat($(this).find('Latitude').text());
		            var longitude = parseFloat($(this).find('Longitude').text());
		            var efficiency = parseFloat($(this).find('Efficiency').text());
		            var point = { location: new google.maps.LatLng(latitude + 0.0000001, longitude + 0.0000001), weight: Math.pow(efficiency,4), efficiency: efficiency/10 }
		            //for (var i = 0; i < efficiency * 10 ; i++) {
		            //    var point = { location: new google.maps.LatLng(latitude + 0.0000001, longitude + 0.0000001) }
		            //    
		            //}
		            heatmapPoints.push(point);
		              
		        })		
		    }
		});
    });
}
function loadDapPositionsFromXml() {
    dapPositions = [];
    $(document).ready(function () {
        $.ajax(
		{
		    type: "GET",
		    url: "/assets/dappositions.xml",
		    dataType: "xml",
		    success: function (xml) {
		        //$("node[name='x']");
		        $(xml).find('point').each(function () {
		            var latitude = parseFloat($(this).find('Latitude').text());
		            var longitude = parseFloat($(this).find('Longitude').text());
		            //placePole(latitude, longitude);
		            var pole = createPole();
		            pole.placeOnMap(latitude, longitude);
		            //var point = new google.maps.LatLng(latitude, longitude);
		            //dapPositions.push(point);
		            //var marker = new google.maps.Marker(
                    //{
                    //    position: point,
                    //    map: map,
                    //    draggable: false,
                    //    icon: poleIcon,

                    //});

		        })
		        sendDrawRequest();
		    }
		});
    });
}
function loadReachFromTable(tech,scenario, dbm)
{
	var ret;
	$(document).ready(function()
	{
		$.ajax(
		{
			async: false,
			type : "GET",
			url : "table.xml",
			dataType : "xml",
			success : function(xml)
			{
				$(xml).find(tech).each(function()
				{   				
					$(this).find(scenario).each(function()
					{
						$(this).find(dbm).each(function()
						{
							ret = parseInt($(this).find('Reach').text(),10);
							
						})
						
					})
				})
			}
		});
	});
	return ret;

}
function loadInfoFromTable(tech,scenario,power,dst)
{
	var ret = [];
	$(document).ready(function()
	{
		$.ajax(
		{
			async: false,
			type : "GET",
			url : "modeloTabela.xml",
			dataType : "xml",
			success : function(xml)
			{
				
			
				$(xml).find("Technology[type = " +tech+ "]").each(function()
				{   				

					$(this).find("Scenario[type = "+scenario+"]").each(function()
					{
						$(this).find("Power[dbm = "+power+"]").each(function()
						{
							$(this).find("Distance").each(function()
							{
								var split = $(this).attr("range").split("-");
								var val1 = parseFloat(split[0],10);
								var val2 = parseFloat(split[1],10);
								if(val1 <= dst && dst < val2)
								{
									//alert(parseFloat($(this).find('TS').text(),10));
									//alert($(this).find('Color').text());

									ret.push(parseFloat($(this).find('TS').text(),10));
									ret.push($(this).find('Color').text());
									return ret;
									
								}
								//alert(parseFloat($(this).find('TS').text(),10));
								//alert($(this).find('Color').text());
							
							})
							
						})
						
					})
				})
			}
		});
	});
	return ret;

}
function loadTable()
{
	var ret = [];
	var table =[]
	
	
	$(document).ready(function()
	{
		$.ajax(
		{
			async: false,
			type : "GET",
			url : "modeloTabela.xml",
			dataType : "xml",
			success : function(xml)
			{
				
				var tech = [];
				$(xml).find("Technology").each(function()
				{   				
					//var sce = [];
					//sce.push($(this).attr("type"));
					//tech.push(sce);
					var x = {
						name: $(this).attr("type"),
						children: []
					}
					
					
					$(this).find("Scenario").each(function()
					{
					
						var y = {
							name: $(this).attr("type"),
							children: []
						}
						x.children.push(y);
						
						$(this).find("Power").each(function()
						{
							var z = {
								name: $(this).attr("dbm"),
								children: []
							}
							y.children.push(z);
								
							$(this).find("Distance").each(function()
							{
								var a = {
								name: $(this).attr("range"),
								children: []
								}
								var split = $(this).attr("range").split("-");
								var val1 = parseFloat($(this).find('TS').text(),10);
								var val2 = $(this).find('Color').text();
								a.children.push(val1);
								a.children.push(val2);							
								z.children.push(a);
								
								

							
							})
							
						})
						
					})
					table.push(x);
				})
				
			}
		});
	});
	return table;

}

