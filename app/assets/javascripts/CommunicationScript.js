const AUTO_PLAN_FILE_ID = 0;
const DRAW_FILE_ID = 1;
const METRIC_FILE_ID = 2;
const TEST_COLLECTION_FILE_ID = 3;
const KML_CREATION_FILE_ID = 5;

function sendDrawRequest(){
    sendDataToServer(serverAddress, 'POST', DRAW_FILE_ID);
}

function sendSCPToServer() {

    //var data = prepareNetworkToSend();
   // var data = data.toString();
    //_ajax_request("http://localhost:3000/autoplan", data, 'POST');
   sendDataToServer(serverAddress, 'POST', AUTO_PLAN_FILE_ID);
}

function prepareNetworkToSend(){
    var reach = getDapMaximumReach();
    var uncMeters = meters.filter(function (item) {
            return (item.connected == false);
        });
    var toSend ={
        "uncoveredMeters": uncMeters,
        "polesPositions": poles,
        "reach": reach 
    };
    return toSend;

}
function sendDataToServer(url,method,type) {
    var uncoveredMeters = meters.filter(function (item) {
            return (item.connected != true);
    });
    var data ;
    switch(type){
        case AUTO_PLAN_FILE_ID:
            data = createAutoPlanningFileModel();
            break;
        case DRAW_FILE_ID:
            data = createDrawFileModel();
            break;
        case METRIC_FILE_ID:
            data = createMetricsFileModel();
            break;
        case TEST_COLLECTION_FILE_ID:
            data = createTestFileModel();
            break;
        case 4: //SÓ PRO TESTE DO GRID, PODE IGNORAR
            data = createGridTestFileModel();
            break;
		case KML_CREATION_FILE_ID:
            data = createMetricsFileModel();
            break;

        default:
            data = -1;
            break;

    }

    $.ajax({
            url: url,
            type: method,
            data: {
                'data': data,
            },
            dataType: "text",
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
                    case 4: //SÓ UM TESTE DO GRID, PODE IGNORAR
                        readGridTestResponse(data);
                        break;
					case KML_CREATION_FILE_ID:
						readMetricResponse(data,true);
						break;
                    default:
                        break;
                }
                

                
                
                //var d2 = new Date();
                //alert(d2-d);
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
function readAutoPlanResponse(data){
	if(data == "")
		return;
    data = data.substring(data.lastIndexOf("\n")+1, data.length );
    var split = data.split(" ");
	
	for(var i = 0; i < daps.length; i++){ //REMOVER DEPOIS! ESTÁ AQUI PARA NÃO BUGAR O PLANEJAMENTO!
		daps[i].remove();
		i--;
	}
	for(var i = 0 ; i < split.length-1; i ++){
		var toAdd = parseInt(split[i].slice(1));
		var latLng = poles[toAdd-1].position;
		var newDap = createDAP();
		newDap.placeOnMap(latLng.lat(),latLng.lng());
		
	}
	
    sendDrawRequest();
	
}
function readPropagationResponse(data){
    resetDraw();
    var drawInfo = data.split(" ");
    for(var i = 0; i < drawInfo.length-1; i++){
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

    }
   

}
function readGridTestResponse(data){
    
   var geoms = data.split("/n");
   for(var i = 0; i < geoms.length-1; i++){
        var poss = geoms[i].split("<>");

        var pos1 = poss[0].split(";");
        var pos1lat = pos1[0];
        var pos1lng = pos1[1];

        var pos2 = poss[1].split(";");
        var pos2lat = pos2[0];
        var pos2lng = pos2[1];

        var rectangle = new google.maps.Rectangle({
            strokeColor: '#FF0000',
            strokeOpacity: 0.8,
            strokeWeight: 0.1,
            fillColor: '#FF0000',
            fillOpacity: 0.35,
            clickable:false,
            map: map,
            geodesic: true,
            bounds: new google.maps.LatLngBounds(
              new google.maps.LatLng(pos1lat, pos1lng),
              new google.maps.LatLng(pos2lat,pos2lng))
      });

   }

}
//Função que lê o código recebido e retorna o que se deve escrever para o usuário na tabela de estatísticas.
function statisticsDecode(code,aux){
    var ret = "";
	switch(code){
		case "DapQnt":
			return "Quantidade de DAPs";
		case "MeterQnt":
			return "Quantidade de medidores";
		case "UncoveredMeters":
			return "Medidores decobertos";
        case "QualityPerHop":

            return "Qualidade do salto "+aux;
        case "DelayPerHop":
            return "Delay do salto "+aux;
        case "MetersPerHop":
            return "Medidores a "+aux+" salto(s)";
		case "MetersPerDap":
			return aux+" Medidores por DAP";
		case "Redundancy":
			return aux+" Redundância";
		default:		
		break;
	}
	return "Código não definido";
}
function formatStatisticsToKML(stats){
	var ret;
	if(data != ""){
		var split = data.split("\n");	
		for(var i = 0; i < split.length;i++){	
			var aux = split[i].split("<>");
			var text;
			//text += aux[0]+": "+aux[1] + "\n";
	
			switch(aux[0]){
				case "DQ":
					return "<DAPQnt> "+val+" </DAPQnt>";
				case "MQ":
					return "<MeterQnt> "+val+" </MeterQnt>";
				case "UM":
					return "<UncoveredMeters> "+val+" </UncoveredMeters>";
				case "MPDMin":
					return "Mínimo de medidores por DAP";
				case "MPDMed":
					return "<MetersPerDap>\n";
				case "MPDMax":
					return "Máximo de medidores por DAP";
				case "RMin":
					return "Redundância Mínima";
				case "RMed":
					return "Redundância Média";
				case "RMax":
					return "Redundância Máxima";
				default:
					var aux = code.substring(0, 3);
					var num = code.substring(3,4);
					if(aux=="QPH")
						return "Qualidade média do salto "+num;		
					if(aux =="DPH")
						return "Atraso médio do salto "+num;	
					if(aux == "MPH")
						return "Medidores a "+num+" salto(s)";			
				break;
			}
			return "Código não definido";
			
		}
	}
	
}
function readMetricResponse(data,kml){
 

 if(!kml){
    $("#metricsTable tr").remove();
    if(data != ""){
        var split = data.split("\n");
        var text = "";
        $(data).find('statistic').each(function(){
            var type = $(this).attr('type');
            var hop = $(this).attr('hop');
            var value = $(this).find('value').text();
            var value = $(this).find('min').text();
            var value = $(this).find('med').text();
            var value = $(this).find('max').text();

            $( "#metricsTable tbody" ).append( 
            "<tr>" +
              "<th class=\"ui-widget-header \">" + type + "</th>" +
              "<td>" + value + "</td>" +
               +"</tr>" );  

        });
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

}

  /* if(!kml){
		$("#metricsTable tr").remove();
		if(data != ""){
			var split = data.split("\n");
			var text = "";		
			for(var i = 0; i < split.length;i++){
				var aux = split[i].split("<>");
				$( "#metricsTable tbody" ).append( 
				"<tr>" +
				  "<th class=\"ui-widget-header \">" + statisticsDecode(aux[0]) + "</th>" +
				  "<td>" + aux[1] + "</td>" +
				   +"</tr>" );	
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
		var text = "\n";
		if(data != ""){
			var split = data.split("\n");	
			for(var i = 0; i < split.length;i++){
				var aux = split[i].split("<>");
				text += aux[0]+": "+aux[1] + "\n";
			}
		}
		var kml = formatKMLText(text);
		var blob = new Blob([kml], {type: "text/plain;charset=utf-8"});
		saveAs(blob, "viz"+meters.length+"-"+poles.length+"-"+daps.length+".kml");
		
	}
    */
}

function propagationValuesToSend(){
    var s = scenario;
    var t = technology;
    var srdv;
    if(SRD)
        srdv = 1;
    else
        srdv = 0;

    var rate;
	if(technology == t802_11_g)
		rate = BIT_RATE;
	if(technology == t802_15_4)
		rate = 0.25;
    var ret = "";
    ret+= s + "\n";
    ret+= t + "\n";
    ret+= H_TX + "\n";
    ret+= H_RX + "\n";
    ret+= rate + "\n";
    ret+= TRANSMITTER_POWER + "\n";
    ret+= srdv + "\n";
    if(meshEnabled)
        ret+=meshMaxJumps+"\n";
    else
        ret+="0"+"\n";
    return ret;

}
function createAutoPlanningFileModel(){
    //var uncoveredMeters = meters.filter(function (item) {
    //        return (item.connected != true);
    //});
    var uncoveredMeters = meters; //POR ENQUANTO VOU DEIXAR ISSO AQUI PRA NÃO CONFUNDIR AS POSIÇÕES DO SERVDOR COM OS MEDIODRES DAQUI 
    var ret = AUTO_PLAN_FILE_ID + '\n';
    ret += propagationValuesToSend();

    ret+= uncoveredMeters.length+"\n";
    for(var i = 0; i <uncoveredMeters.length; i++){
        ret += uncoveredMeters[i].getPosition().lat() + " " + uncoveredMeters[i].getPosition().lng();
        ret += "\n";
    }
    ret += poles.length;
    ret += "\n";
    for(var i = 0; i <poles.length; i++){
        ret += poles[i].getPosition().lat() + " " + poles[i].getPosition().lng();
        ret += "\n";
    }
    return ret;
}
function createDrawFileModel(){
    
    
    var ret = DRAW_FILE_ID + '\n';
    ret += propagationValuesToSend();
    
    ret+= meters.length+"\n";
    for(var i = 0; i <meters.length; i++){
        ret += meters[i].getPosition().lat() + " " + meters[i].getPosition().lng();
        ret += "\n";
    }
    ret += daps.length;
    ret += "\n";
    for(var i = 0; i <daps.length; i++){
        ret += daps[i].getPosition().lat() + " " + daps[i].getPosition().lng();
        ret += "\n";
    }
    return ret;
}
function createGridTestFileModel(){
    
    
    var ret = 4 + '\n';
    ret += propagationValuesToSend();
    
    ret+= meters.length+"\n";
    for(var i = 0; i <meters.length; i++){
        ret += meters[i].getPosition().lat() + " " + meters[i].getPosition().lng();
        ret += "\n";
    }

    return ret;
}
function createMetricsFileModel(){
    
    var ret = METRIC_FILE_ID + '\n';
    ret += propagationValuesToSend();
    
    ret+= meters.length+"\n";
    for(var i = 0; i <meters.length; i++){
        ret += meters[i].getPosition().lat() + " " + meters[i].getPosition().lng();
        ret += "\n";
    }
    ret += daps.length;
    ret += "\n";
    for(var i = 0; i <daps.length; i++){
        ret += daps[i].getPosition().lat() + " " + daps[i].getPosition().lng();
        ret += "\n";
    }
    return ret;
}
function createTestFileModel(){
    
    var uncoveredMeters = meters; //POR ENQUANTO VOU DEIXAR ISSO AQUI PRA NÃO CONFUNDIR AS POSIÇÕES DO SERVDOR COM OS MEDIODRES DAQUI 
    var ret = TEST_COLLECTION_FILE_ID + '\n';
    ret += propagationValuesToSend();

    ret+= uncoveredMeters.length+"\n";
    for(var i = 0; i <uncoveredMeters.length; i++){
        ret += uncoveredMeters[i].getPosition().lat() + " " + uncoveredMeters[i].getPosition().lng();
        ret += "\n";
    }
    ret += poles.length;
    ret += "\n";
    for(var i = 0; i <poles.length; i++){
        ret += poles[i].getPosition().lat() + " " + poles[i].getPosition().lng();
        ret += "\n";
    }
    return ret;
}
function download() {
	sendDataToServer(serverAddress, 'POST', KML_CREATION_FILE_ID);
   // var toSave = formatKMLText();
   // var blob = new Blob([toSave], {type: "text/plain;charset=utf-8"});
   // saveAs(blob, "viz"+meters.length+"-"+poles.length+"-"+daps.length+".kml");
}

function upload(fileInput) {

    if (window.File && window.FileReader && window.FileList && window.Blob) {
        var file = fileInput.files[0];
        var reader = new FileReader();
        reader.onload = function(e) {
            var fileText = reader.result;
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
