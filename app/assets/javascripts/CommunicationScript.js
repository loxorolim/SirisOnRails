const AUTO_PLAN_FILE_ID = 0;
const DRAW_FILE_ID = 1;
const METRIC_FILE_ID = 2;
const TEST_COLLECTION_FILE_ID = 3;

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
    //var data = printScpMatrixTeste(uncoveredMeters);
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
                        readMetricResponse(data);
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
function readMetricResponse(data){
   // alert(data);
	var split = data.split("\n");
	var text = "";
	for(var i = 0; i < split.length;i++){
		text += split[i]+"<br>";
			
	}

    $(function() {
        $( "#statisticDialog" ).dialog({
            show: {
        effect: "blind",
        duration: 1000
      },
      hide: {
        effect: "explode",
        duration: 1000
      }
    });

$("#metricsTable tr").remove();
    $( "#metricsTable tbody" ).append( "<tr>" +
      "<td>" + "name.val()" + "</td>" +
      "<td>" + "email.val()" + "</td>" +
      "<td>" + "password.val()" + "</td>" +
    "</tr>" );
    //$( "#statisticDialog" ).html(text);
    });


}

function propagationValuesToSend(){
    var s = scenario;
    var t = technology;
    var srdv;
    if(SRD)
        srdv = 1;
    else
        srdv = 0;

	if(technology == t802_11_g)
		BIT_RATE = 6;
	if(technology == t802_15_4)
		BIT_RATE = 0.25;
    var ret = "";
    ret+= s + "\n";
    ret+= t + "\n";
    ret+= H_TX + "\n";
    ret+= H_RX + "\n";
    ret+= BIT_RATE + "\n";
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
    var toSave = formatKMLText();
    var blob = new Blob([toSave], {type: "text/plain;charset=utf-8"});
    saveAs(blob, "viz"+meters.length+"-"+poles.length+"-"+daps.length+".kml");
}

function upload(fileInput) {

    if (window.File && window.FileReader && window.FileList && window.Blob) {
        var file = fileInput.files[0];
        var reader = new FileReader();
        reader.onload = function(e) {
            var fileText = reader.result;
            loadFromKMLText(fileText);
            
        }
        reader.readAsText(file);    
    } else {
      alert('The File APIs are not fully supported by your browser.');
    }
}
