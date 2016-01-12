const AUTO_PLAN_FILE_ID = 0;
const DRAW_FILE_ID = 1;
const METRIC_FILE_ID = 2;
//const HEATGRID_FILE_ID = 3;
const TEST_COLLECTION_FILE_ID = 4;
const KML_CREATION_FILE_ID = 5;
const GET_RANGE_FILE_ID = 6;
const KML_FILE_ID = 7;

var newPolesAux = [];

function sendDrawRequest(){
    sendDataToServer(serverAddress, 'POST', DRAW_FILE_ID);
}

function sendSCPToServer() {

    //var data = prepareNetworkToSend();
   // var data = data.toString();
    //_ajax_request("http://localhost:3000/autoplan", data, 'POST');
   sendDataToServer(serverAddress, 'POST', AUTO_PLAN_FILE_ID);
}

function sendDataToServer(url,method,type) {
    var uncoveredMeters = meters.filter(function (item) {
            return (item.connected != true);
    });
    var data = createObjectToSend(type);

    $.ajax({
            url: url,
            type: method,
            data: {
                'data': JSON.stringify(data),
            },
            dataType: "json",
            success: function (data) { 
                switch(type){
                    case AUTO_PLAN_FILE_ID:
                        readAutoPlanResponse(data);
                        break;
                    case DRAW_FILE_ID:
                        readPropagationResponse(data);
                        break;
                    case METRIC_FILE_ID:
                        readMetricResponse(data,false);
                        break;
                    case GET_RANGE_FILE_ID:
                        readGetRangeResponse(data,true);
                        break;
					case KML_FILE_ID:
                        readKMLResponse(data);
                        break;
                    default:
                        break;
                }
            }
        });
}
function getLastLine(x){
    if(x.lastIndexOf("\n")>0) {
        return x.substring(0, x.lastIndexOf("\n"));
    } else {
        return x;
    }
}
function readGetRangeResponse(data){

    if(coveragePolygon)
        coveragePolygon.setMap(null);
    var dapRange = data.DAPtoMeterDistance;
    var meterRange = data.MeterToMeterDistance;
    var circles = [];
    for(var i = 0; i < daps.length; i++){
        circles.push(drawCircle(daps[i].getPosition(),dapRange/1609.344,1)) ;
    }
    for(var i = 0; i < meters.length; i++){
        if(meters[i].hop != null && meters[i].hop < meshMaxJumps  )
            circles.push(drawCircle(meters[i].getPosition(),meterRange/1609.344,1)) ;
    }
    coveragePolygon = new google.maps.Polygon({
                 paths: circles,
                 strokeColor: "#00FF00",
                 strokeOpacity: 0.35,
                 strokeWeight: 0,
                 fillColor: "#00FF00",
                 fillOpacity: 0.35,
                 clickable: false
     });
    DrawRangeView();
    
}
function readKMLResponse(data){

	var kml = data.KML;
	var blob = new Blob([kml], {type: "text/plain;charset=utf-8"});
	saveAs(blob, "viz"+meters.length+"-"+poles.length+"-"+daps.length+".kml");
    
}
function readAutoPlanResponse(data){
    if (autoPlanOverwrite) {
        while (daps.length > 0)
            daps[0].remove();
    }
	for(var i = 0 ; i < data.ChosenDAPs.length; i++){
	    var toAdd = data.ChosenDAPs[i];
	    var latLng;
	    if (!autoPlanHeatmap) 
	        latLng = poles[toAdd].position;
	    else
	        latLng = newPolesAux[toAdd].position;
		var newDap = createDAP();
		newDap.placeOnMap(latLng.lat(),latLng.lng());
	}
    sendDrawRequest();
	
}
function readPropagationResponse(data){
    resetDraw();
    //var drawInfo = data.split(" ");
   /* for(var i = 0; i < drawInfo.length-1; i++){
        var split = drawInfo[i].split("/");
        var latlng1 = split[0].split("<>");
        var lat1 = parseFloat(latlng1[0]);
        var lng1 = parseFloat(latlng1[1]);
        var pos1 = new google.maps.LatLng(lat1,lng1);
        var latlng2 = split[1].split("<>");
        var lat2 = parseFloat(latlng2[0]);
        var lng2 = parseFloat(latlng2[1]);
        var pos2 = new google.maps.LatLng(lat2,lng2);
        var color = parseInt(split[2]);
        var efficiency = parseFloat(split[3]);
        var distance = parseFloat(split[4]);
        var dashed = parseInt(split[5]);
        drawLine(pos1,pos2,color,efficiency,distance,dashed);

    }*/
    var circOptions = { strokeColor: connectedColor};
    var circOptions2 = { strokeColor: disconnectedColor};
    for(var i = 0; i < meters.length; i++)
        meters[i].setOptions(circOptions2);
    var connectedMeters = [];
	
    for(var i = 0; i < data.DrawInfos.length; i++){
		var split = data.DrawInfos[i].split('/');
        var a = split[0];
		var b = split[1];
		var hopnumber = parseInt(split[2]);
        var latlng1 = meters[a].getPosition();
        
        meters[a].setOptions(circOptions);
        var latlng2;     
        meters[a].hop = hopnumber+1;//+1 pq ta vindo do servidor que 0 = 1 salto
        if(hopnumber==0)
            latlng2 = daps[b].getPosition();
        else
            latlng2 = meters[b].getPosition();
        var efficiency = parseFloat(split[3]);
        var delay = parseFloat(split[4]);
        var distance = parseInt(split[5]);
        var dashed = parseInt(split[6]);
        drawLine(latlng1,latlng2,efficiency,delay,distance,dashed);

    }
    if(drawRangeView)
        sendDataToServer(serverAddress, 'POST', GET_RANGE_FILE_ID);  
   

}
//Função que lê o código recebido e retorna o que se deve escrever para o usuário na tabela de estatísticas.
function statisticsDecode(type,val){
    var ret = "";
	switch(type){
		case "DAPQnt":
			return "Quantidade de DAPs";
		case "MeterQnt":
			return "Quantidade de medidores";
		case "UncoveredMeters":
			return "Medidores decobertos";
        case "QualityPerHop":
            return "Qualidade do salto "+val;
        case "DelayPerHop":
            return "Delay do salto "+val;
        case "MetersPerHop":
            return "Min Medidores a "+val+" salto(s)";
		case "MinMetersPerDAP":
			return "Medidores por DAP (Min)";
        case "MedMetersPerDAP":
            return "Medidores por DAP (Med)";
        case "MaxMetersPerDAP":
            return "Medidores por DAP (Max)";
		case "MinRedundancy":
			return "Redundância (Min)";
        case "MedRedundancy":
            return "Redundância (Med)";
        case "MaxRedundancy":
            return "Redundância (Max)";
		default:		
		break;
	}
	return "Código não definido";
}

function readMetricResponse(data,kml){
 if(!kml){
    $("#metricsTable tr").remove();

    if(data != ""){
            for(var property in data){
                var type = property;
                if(data[type] instanceof Array)
                    aux = data[type].length;
                else
                    aux = 1;
                for(var i = 0; i < aux; i++){
                    var value;
                    if(data[type] instanceof Array)
                        value = data[type][i];
                    else
                        value = data[type];
                    $( "#metricsTable tbody" ).append( 
                        "<tr>" +
                        "<th class=\"ui-widget-header \" style=\"width: 170px;\">" + statisticsDecode(type,i) + "</th>" +
                        "<td>" + value + "</td>" +
                        +"</tr>" ); 
                }
            }
    }
    else{
			$( "#metricsTable tbody" ).append( 
			"<tr>" +
			  "<th class=\"ui-widget-header \">" + "Nao há medidores e/ou agregadores" + "</th>" 
			   +"</tr>" );
        }       
        $(function() {
            $( "#statisticDialog" ).dialog({
                show: {
                    effect: "drop",
                    duration: 500
                },
                hide: {
                    effect: "drop",
                    duration: 500
                },
                resizable: false,
            });
        });
	}
	else{
		var kml = formatKMLText(data);
		var blob = new Blob([kml], {type: "text/plain;charset=utf-8"});
		saveAs(blob, "viz"+meters.length+"-"+poles.length+"-"+daps.length+".kml");
	}
}
function propagationValuesToSendObject(){
    
    var rate;
    if(technology == t802_11_g)
        rate = BIT_RATE;
    if(technology == t802_15_4)
        rate = 0.25;

    var ret = { "scenario": scenario,
                "technology": technology,
                "power": TRANSMITTER_POWER,
                "mesh_hops": meshMaxJumps-1, 
                "rate": rate,
                "h_tx": H_TX,
                "h_rx": H_RX,
                "redundancy": REDUNDANCY };

    return ret;
}
function getLatLngObject(array){
    var aux = [];
    for(var i = 0; i < array.length; i++){
        var aux_obj = { "lat": array[i].getPosition().lat(),
                        "lng": array[i].getPosition().lng(),
                        "signal_info": array[i].opIds
                   };
        aux.push(aux_obj);
    }
    return aux;
}
function selectPolesWithSignal() {
    var newpoles = [];
    for (var i = 0; i < poles.length; i++) {
        var consider = false;
        for (var j = 0; j < heatmapPoints.length; j++) {
            var dist = google.maps.geometry.spherical.computeDistanceBetween(poles[i].getPosition(), heatmapPoints[j].getPosition());
            if (dist <= heatmapEffectRange) {
                newpoles.push(poles[i]);
                consider = true;
                break;
            }
        }
    }
    newPolesAux = newpoles;
}
function createObjectToSend(type){
    var obj_to_send = propagationValuesToSendObject();
    obj_to_send["action_id"] = type;
    switch(type){
        case KML_FILE_ID:
            obj_to_send["signal_info"] = getLatLngObject(heatmapPoints);
            obj_to_send["meters"] = getLatLngObject(meters);
            obj_to_send["poles"] = getLatLngObject(poles);
            obj_to_send["DAPs"] = getLatLngObject(daps);
               
            break;
        case AUTO_PLAN_FILE_ID:
            obj_to_send["meters"] = getLatLngObject(meters);
            //obj_to_send["poles"] = getLatLngObject(poles);
            if (autoPlanHeatmap) {
                selectPolesWithSignal();
                obj_to_send["poles"] = getLatLngObject(newPolesAux);
            } else {
                obj_to_send["poles"] = getLatLngObject(poles);
            }
            obj_to_send["overwrite"] = true;
            if (!autoPlanOverwrite) {
                obj_to_send["DAPs"] = getLatLngObject(daps);
                obj_to_send["overwrite"] = false;
            }
            break;
        case DRAW_FILE_ID:
        case METRIC_FILE_ID:
            obj_to_send["meters"] = getLatLngObject(meters);
            obj_to_send["DAPs"] = getLatLngObject(daps);
            break;
        case GET_RANGE_FILE_ID:
            break;
        default:
            break;
    }
    return obj_to_send;
}
function download() {
	sendDataToServer(serverAddress, 'POST', KML_FILE_ID);
}

function upload(fileInput) {

    if (window.File && window.FileReader && window.FileList && window.Blob) {
        var file = fileInput.files[0];
         var extension = fileInput.files[0].name.split('.').pop().toLowerCase();
        var reader = new FileReader();
        reader.onload = function(e) {
            var fileText = reader.result;
            if(extension == "txt")
            {
                if($("#heatmapFormatRadio").is(':checked'))
                    loadHeatmap(fileText);
                if($("#meterFormatRadio").is(':checked'))
                    loadMetersTeste(fileText);
                if($("#poleFormatRadio").is(':checked'))
                    loadPolesTeste(fileText);
            }
            else                   
			    loadFromKMLText(fileText);
            $("#uploadDialog").dialog("close");
			$("#resetOnUpload").prop('checked', false);
            
        }
        if(file)
            reader.readAsText(file);    
    } else {
      alert('The File APIs are not fully supported by your browser.');
    }
}
