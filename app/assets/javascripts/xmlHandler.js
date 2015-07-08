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
function loadHeatmap(file) {
	var heatmap = [];
	var data = file.split(",");
	for(var i = 0; i < data.length;i++){
		var info = data[i].split(" ");
			var loc = new google.maps.LatLng(info[0], info[1])
			var hmpoint = {
				position: loc,
				weight: info[2]
			}
			heatmap.push(hmpoint);
	}
	heatmapPoints = heatmap;
	sendDataToServer(serverAddress, 'POST', HEATGRID_FILE_ID);
	//createHeatMap();
}
function loadMetersTeste(file) {
	var heatmap = [];
	var data = file.split("\n");
	for(var i = 0; i < data.length;i++){
		if(data[i] != ""){
			var info = data[i].split(" ");
		    var meter = createMeter();
	        meter.placeOnMap(info[0], info[1]);
   		}
	}
}
function loadPolesTeste(file) {
	var heatmap = [];
	var data = file.split("\n");
	for(var i = 0; i < data.length;i++){
		if(data[i] != ""){
			var info = data[i].split(" ");
	   		var pole = createPole();
        	pole.placeOnMap(info[0], info[1]);
        }
	}
}
function loadFromKMLText(kml){
	
	//var teste = kml.split("\n");
	//for(var i = 0; i < teste.length; i++)
	//{
	//	var wow = teste[i].split(" ");
	//	var meter = createMeter();
	//	meter.placeOnMap(wow[1], wow[0]);
	//}
	
	var sce = $(kml).find('Scenario').text();
	sce = parseInt(sce);
	setScenario(sce);
	var pow = $(kml).find('Power').text();	
	pow = parseInt(pow);
	setPower(pow);
	var tech = $(kml).find('Technology').text();
	tech = parseInt(tech);
	setTechnology(tech);	
	var mHops = $(kml).find('MeshHops').text();
	mHops = parseInt(mHops);
	setMeshHops(mHops);
	var mEnabled = $(kml).find('MeshEnabled').text();
	mEnabled = parseInt(mEnabled);
	setMeshActivation(mEnabled);


	$(kml).find('Placemark').each(function(){
		var type = $(this).find('name').text();
		if($("#dapFormatRadio").is(':checked'))
			type="Agregador";
		if($("#meterFormatRadio").is(':checked'))
			type="Medidor";
		if($("#poleFormatRadio").is(':checked'))
			type="Poste";
		$(this).find('Point').each(function(){
			var coord =	$(this).find('coordinates').text();
			var coords = coord.split(",");
			var longitude = coords[0];
			var latitude = coords[1];
			switch(type){
				case "Medidor":
					var meter = createMeter();
					meter.placeOnMap(latitude, longitude);
				break;
				case "Poste":
					var pole = createPole();
					pole.placeOnMap(latitude, longitude);
				break;
				case "Agregador":
					var dap = createDAP();
					dap.placeOnMap(latitude, longitude);
				break;
				default:
				break;
			}
		});
		
	})
	
	sendDrawRequest();

}
function formatKMLText(metrics) {


	var init = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.2\">\n";
	init += "<Document>\n";
	init += "<Scenario>"+scenario+"</Scenario>\n";
	init += "<Technology>"+technology+"</Technology>\n";
	init += "<Power>"+TRANSMITTER_POWER+"</Power>\n";
	init += "<MeshHops>"+meshMaxJumps+"</MeshHops>\n";
	init += "<MeshEnabled>"+meshEnabled+"</MeshEnabled>\n";
	//init += "<Metrics>"+metrics+"</Metrics>\n";
	init += metrics+"\n";
	
	init += "<Folder><name>Medidores</name>\n"
	for(var i = 0;i<meters.length;i++){
		init+="<Placemark>\n<name>Medidor</name>\n<Point>\n<coordinates>"+
		meters[i].getPosition().lng()
		+","+meters[i].getPosition().lat()
		+",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init+="</Folder>\n";
	init+="<Folder><name>Postes</name>\n";
	for(var i = 0;i<poles.length;i++){
		init+="<Placemark>\n<name>Poste</name>\n<Point>\n<coordinates>"+poles[i].getPosition().lng()+","+poles[i].getPosition().lat()+",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init+="</Folder>\n";
	init+="<Folder><name>Agregadores</name>\n";
	for(var i = 0;i<daps.length;i++){
		init+="<Placemark>\n<name>Agregador</name>\n<Point>\n<coordinates>"+daps[i].getPosition().lng()+","+daps[i].getPosition().lat()+",0</coordinates>\n</Point>\n</Placemark>\n";
	}
	init+="</Folder>\n";
	init+="<Folder><name>Enlaces</name>\n";
	for(var i = 0;i<lines.length;i++){
		var path = lines[i].getPath();
		init+="<Placemark>\n<name>Enlace</name>\n<description>"+lines[i].efficiency+"</description>\n<LineString>\n<coordinates>\n"+path.getAt(0).lng()+","+path.getAt(0).lat()+",0\n"+path.getAt(1).lng()+","+path.getAt(1).lat()+",0\n</coordinates>\n</LineString>\n</Placemark>\n";
	}
	init+="</Folder>\n";
	init+="</Document>\n";
	init+="</kml>";
	return init;
}
