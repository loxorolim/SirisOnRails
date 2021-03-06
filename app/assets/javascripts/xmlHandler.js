const PROCESS_FILE_ID = 8;
function sendKMLToBeProcessed(url, method, kml) {

    var obj = new Object();
    obj["KML"] = kml;
    obj["action_id"] = PROCESS_FILE_ID;

    $.ajax({
        url: url,
        type: method,
        data: {
            'data': JSON.stringify(obj),
        },
        dataType: "json",
        success: function (data) {
            setScenario(data.scenario);
            setPower(data.power);
            setTechnology(data.technology);
            setMeshHops(data.mesh_hops+1);
            setValidCellRadius(data.valid_cell_radius);
            var meters = data.meters, daps = data.daps, poles = data.poles, coverage_area = data.coverage_area;
            var aux = [];
            var sw_lat = -1, sw_lng = -1, ne_lat = -1, ne_lng = -1;
            var first = true;
            var arrays = [];
            arrays.push(meters);
            arrays.push(daps);
            arrays.push(poles);
            arrays.push(coverage_area);
            var current_array;
            var i = 0;
            for (a = 0; a < arrays.length; a++) {
                current_array = arrays[a];
                for (i = 0; i < current_array.length;i++) {
                    aux = current_array[i].split('/');
                    var longitude = parseFloat(aux[1]);
                    var latitude = parseFloat(aux[0]);
                    //Para centralizar a tela nas posições inseridas
                    if (latitude < sw_lat || first == true)
                        sw_lat = latitude;
                    if (latitude >= ne_lat || first == true)
                        ne_lat = latitude;
                    if (longitude < sw_lng || first == true)
                        sw_lng = longitude;
                    if (longitude >= ne_lng || first == true)
                        ne_lng = longitude;
                    first = false;
                    switch (current_array) {
                        case meters:
                            var meter = createMeter();
                            meter.placeOnMap(latitude, longitude);
                            break;
                        case daps:
                            var dap = createDAP();
                            dap.placeOnMap(latitude, longitude);
                            break;
                        case poles:
                            var pole = createPole();
                            pole.placeOnMap(latitude, longitude);
                            break;
                        case coverage_area:
                            var signal_info = [];
                            for (j = 2; j < aux.length; j++) {
                                signal_info.push(aux[j]);
                            }
                            var heatmapPoint = createHeatmapPoint();
                            heatmapPoint.placeOnMap(parseFloat(aux[0]), parseFloat(aux[1]), signal_info);
                            break;
                        default:
                            break;
                    }
                }
            }
            var pos1 = new google.maps.LatLng(sw_lat, sw_lng);
            var pos2 = new google.maps.LatLng(ne_lat, ne_lng);
            var bounds = new google.maps.LatLngBounds(pos1, pos2);
            map.fitBounds(bounds);
            sendDrawRequest();
            DrawHeatmap();
        }
    });
}
function loadMetersFromTxt(file)
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
	//sendDataToServer(serverAddress, 'POST', HEATGRID_FILE_ID);
	setHeatmap();
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
	
    sendKMLToBeProcessed(serverAddress, 'POST', kml);
    //var pos1 = new google.maps.LatLng(sw_lat, sw_lng);
    //var pos2 = new google.maps.LatLng(ne_lat, ne_lng);
    //var bounds = new google.maps.LatLngBounds(pos2, pos1);
    //map.fitBounds(bounds);
    //for (var i = 0; i < daps.length; i++) {
    //    daps[i].signalInfo = getSignalInfo(daps[i].position);
    //}
    return;
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
	mHops = parseInt(mHops)+1;
	setMeshHops(mHops);
	var mVCR = $(kml).find('ValidCellRadius').text();
	mVCR = parseInt(mVCR);
	setValidCellRadius(mVCR);
	var first = true;
	var sw_lat = -1,sw_lng=-1,ne_lat=-1,ne_lng=-1;

	$(kml).find('Placemark').each(function(){
		var type = $(this).find('name').text();
		var weight = parseFloat($(this).find('value').text()); // PARA O MAPA DE CALOR SOMENTE
		var operators = [];
		$(this).find('Operators').each(function(){
			$(this).find('id').each(function(){
				var operatorId = $(this).text();
				operators.push(operatorId);
			});
		});
		if($("#dapFormatRadio").is(':checked'))
			type="Agregador";
		if($("#meterFormatRadio").is(':checked'))
			type="Medidor";
		if($("#poleFormatRadio").is(':checked'))
			type="Poste";
		if($("#heatmapFormatRadio").is(':checked'))		
			type="Ponto de coleta";
				
		$(this).find('Point').each(function(){
			var coord =	$(this).find('coordinates').text();
			var coords = coord.split(",");
			var longitude = coords[0];
			var latitude = coords[1];
			//Para centralizar a tela nas posições inseridas
			if(latitude < sw_lat || first == true)
				sw_lat = latitude;
			if(latitude >= ne_lat || first == true)
				ne_lat = latitude;
			if(longitude < sw_lng || first == true)
				sw_lng = longitude;
			if(longitude >= ne_lng || first == true)	
				ne_lng = longitude;
			first = false;
			type = type.toUpperCase();
			switch(type){
				case "MEDIDOR":
					var meter = createMeter();
					meter.placeOnMap(latitude, longitude);
				break;
				case "POSTE":
					var pole = createPole();
					pole.placeOnMap(latitude, longitude);
				break;
				case "AGREGADOR":
					var dap = createDAP();
					dap.placeOnMap(latitude, longitude);
				break;
			    case "PONTO DE COLETA":
			        var hmPoint = createHeatmapPoint();
			        hmPoint.push(latitude, longitude, operators);
				break;
				default:
				break;
			}
		});
	})
	var pos1 = new google.maps.LatLng(sw_lat,sw_lng);
	var pos2 = new google.maps.LatLng(ne_lat,ne_lng);
	var bounds = new google.maps.LatLngBounds(pos2,pos1);
	map.fitBounds(bounds);
	for(var i = 0; i < daps.length; i++){
		daps[i].signalInfo = getSignalInfo(daps[i].position);
	}
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
		init+="<Placemark>\n<name>Enlace</name>\n<efficiency>"+lines[i].efficiency+"</efficiency>\n<delay>"+lines[i].delay+"</delay>\n<LineString>\n<coordinates>\n"+path.getAt(0).lng()+","+path.getAt(0).lat()+",0\n"+path.getAt(1).lng()+","+path.getAt(1).lat()+",0\n</coordinates>\n</LineString>\n</Placemark>\n";
	}
	init+="</Folder>\n";
	init+="</Document>\n";
	init+="</kml>";
	return init;
}
