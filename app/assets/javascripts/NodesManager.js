/// <reference path="/js/jquery-1.10.2.min.js" />




function connectNodesByDistance(marker) 
{

    for (var i = 0; i < allMarkers.length; i++)
    {
        // var dis = distance(marker.position.lat(), marker.position.lng(), allMarkers[i].position.lat(), allMarkers[i].position.lng(), "K");
        if (!(marker.type == "Meter" && allMarkers[i].type == "Meter")) {
            var dis = distance(marker, allMarkers[i]);
            //dis = dis*1000;
            var values = getValuesFromTable(dis);

            if (values != -1) {
                //Se o marker é um DAP ele pode se conectar a DAPs e a medidores
                if (marker.type == "DAP")
                    connectMarkers(marker, allMarkers[i], values.color);
                else
                    //Se o marker é um medidor ele só pode conectar a DAPs
                    if (allMarkers[i].type == "DAP")
                        connectMarkers(allMarkers[i], marker, values.color);
            }
        }
        
    }

    if (marker.type == "DAP")
    {
        if (marker.neighbours.length == 0) 
            marker.setIcon(marker.offIcon);       
        else 
            marker.setIcon(marker.onIcon);      
    }
    else 
        marker.setIcon(getMeterColor(marker));
}
function distance(marker1, marker2)
{
    /*	var radlat1 = Math.PI * lat1 / 180
        var radlat2 = Math.PI * lat2 / 180
        var radlon1 = Math.PI * lon1 / 180
        var radlon2 = Math.PI * lon2 / 180
        var theta = lon1 - lon2
        var radtheta = Math.PI * theta / 180
        var dist = Math.sin(radlat1) * Math.sin(radlat2) + Math.cos(radlat1) * Math.cos(radlat2) * Math.cos(radtheta);
        dist = Math.acos(dist)
        dist = dist * 180 / Math.PI
        dist = dist * 60 * 1.1515
        if (unit == "K")
        {
            dist = dist * 1.609344
        }
        if (unit == "N")
        {
            dist = dist * 0.8684
        }
        */
    
    var latLngA = new google.maps.LatLng(marker1.position.lat(), marker1.position.lng());
    var latLngB = new google.maps.LatLng(marker2.position.lat(), marker2.position.lng());
    var dist =  google.maps.geometry.spherical.computeDistanceBetween(latLngA, latLngB);
    return dist
}
function removeFromList(list, obj) {
    for (var i = 0; i < list.length; i++) {
        if (list[i] == obj) {
            list.splice(i, 1);
        }
    }
}
function connectMarkers(marker, marker2, color)
{
    if (checkIfConnectionIsPossible(marker, marker2))
    {
        marker.neighbours.push(marker2);
        marker2.neighbours.push(marker);
        // Troca o ícone do marker2 para LIGADO já que acabou de acontecer uma ligação
        marker2.connected = true;
        marker.connected = true;
        if (marker2.type == "Meter") {
            marker2.setIcon(getMeterColor(marker2));
            removeFromList(disconnectedMeters, marker2);

        }
        else
            marker2.setIcon(marker2.onIcon);
        marker.setIcon(marker.onIcon);
        //
        var markers = [marker, marker2];
        markerConnections.push(markers);

        drawLine(marker, marker2, color);
    }
}
function checkIfConnectionIsPossible(marker1, marker2)
{
    if (marker1.ID == marker2.ID)
        return false
    return true;
}


function removeMarkerConnections(marker) 
{
    for (var i = 0; i < markerConnections.length; i++) 
    {
        if (markerConnections[i][0].ID == marker.ID || markerConnections[i][1].ID == marker.ID)
        {
            //Remove a conexão entre os nós
            clearConnection(markerConnections[i][0], markerConnections[i][1], i);
            // i-- é necessário pois voce remove uma posição e deve retornar uma unidade, para continuar da posição correta!
            i--;
        }
            
            
    }

}
function clearConnection(marker1,marker2,i)
{
    //Apaga a linha da conexão
    lines[i].setVisible(false);
    //Remove a conexão das listas de linhas para desenhar
    lines.splice(i, 1);
    //Ambos os nós deixam de ser vizinhos
    marker1.neighbours.splice(getMarkerPositionFromNeighbour(marker1,marker2), 1);
    marker2.neighbours.splice(getMarkerPositionFromNeighbour(marker2,marker1), 1);

    //Se não possuem vizinhos não estão conectados
    //Se for um DAP e não tiver vizinho 
    if (marker1.neighbours.length == 0)
    {
        marker1.connected = false;
        marker1.setIcon(marker1.offIcon);
        if (marker1.type == "Meter") 
            disconnectedMeters.push(marker1);       
    }
    if (marker2.neighbours.length == 0)
    {
        marker2.connected = false;
        if(marker2.type == "DAP")
            marker2.setIcon(marker2.offIcon);
        if (marker2.type == "Meter")
            disconnectedMeters.push(marker2);
                        
    }
    if (marker2.type == "Meter")   
        marker2.setIcon(getMeterColor(marker2));
    markerConnections.splice(i, 1);
}
function getMarkerPositionFromNeighbour(marker, marker2)
{
    for (i = 0; i < marker.neighbours.length; i++)
    {
        if (marker2.ID == marker.neighbours[i].ID)
            return i;
    }
    return -1;
}
function calculateEfficiency(marker)
{
    var avg = 0;
    var sumDist = 0;
    var values = [];
    for(var i = 0; i < heatmapPoints.length; i++)
    {
        var latLng = new google.maps.LatLng(marker.position.lat(), marker.position.lng());
        var dist = google.maps.geometry.spherical.computeDistanceBetween(latLng, heatmapPoints[i].location);
        if(dist < efficiencyRadio)
        {
            var value = heatmapPoints[i].efficiency * (1 - dist / efficiencyRadio);        
            if(value > avg)
                avg = value;
           //values.push(value);
           //sumDist += dist;
        }
    }
  //  for (var j = 0; j < values.length ; j++)
  //      avg += values[j].efficiency * (1 - values[j].distance/sumDist);
    marker.efficiency = avg;
    marker.labelContent = String(Math.floor(avg * 100)) + "%";
    if(avg >= 0.80)
        marker.labelStyle = { color: "#00c200" };
    else if (avg <= 0.40)
        marker.labelStyle = { color: "#FF0000" };
    else
        marker.labelStyle = { color: "#b0b200" };
    marker.label.draw();
}
function removeMarker(marker)
{
    removeMarkerConnections(marker);
    infowindow.setMap(null);
    for (var i = 0; i < allMarkers.length; i++)
    {
        if (allMarkers[i].ID == marker.ID)
        {

            markerCluster.removeMarker(allMarkers[i]);
            allMarkers[i].setMap(null);
            allMarkers.splice(i, 1);

        }
    }
    if (marker.type == "DAP")
    {
        removeMarkerCircles(marker);

    }
     
}
function removePole(pole) {
    for (var i = 0; i < poles.length; i++) {
        if (pole == poles[i]) {
            poles[i].setMap(null);
            poles.splice(i, 1);

        }
    }
}


function displayInfoWindow(marker)
{

    var neighboursIDs = "";
    for (var i = 0; i < marker.neighbours.length; i++) {
        neighboursIDs += marker.neighbours[i].ID + ", ";
    }
    var content = 'ID: ' + marker.ID + '<br>Latitude: ' + marker.position.lat() + '<br>Longitude: ' + marker.position.lng() + '<br>Elevation: ' + marker.elevation + '<br>Neighbours IDs: ' + neighboursIDs + '<br>Connected: ' + marker.connected;
    if (marker.type == "Meter")
        content += '<br>Hop: ' + marker.meshHop + '<br>X: ' + marker.X + '<br>Y: ' + marker.Y ;
    if (marker.type == "DAP")
        content += '<br>Efficiency: ' + marker.efficiency;
    if (marker.teleTech != null)
        content += '<br>Technology: ' + marker.teleTech + '<br>Reach: ' + marker.reach + ' meters';
    infowindow.setContent(content);
    infowindow.open(map, marker);

}
function getMeterColor(meter)
{
    //ESTA FUNÇÃO NÃO ESTÁ OTIMIZADA!!!!!!!!!!!!!!!!!!!
    //ESTA FUNÇÃO SÓ IRÁ FUNCIONAR SE TODOS OS DAPS USAREM A MESMA TECNOLOGIA!!!! CASO SEJA USADO UM AMBIENTE EM QUE EXISTEM VÁRIOS DAPS 
    //UTILIZANDO VÁRIAS TECNOLOGIAS NÃO FUNCIONARÁ!
    
    var color = -1;
    var dist = -1;
    for (i = 0; i < meter.neighbours.length; i++)
    {
        var auxDist = distance(meter, meter.neighbours[i]);
        if (dist == -1 || auxDist < dist)
            dist = auxDist;
        
    }
    var value = getValuesFromTable(dist);
    if (value.color == "GREEN")
        return meterBestIconImage;
    else
        if (value.color == "YELLOW")
            return meterBetterIconImage;
        else
            if (value.color == "RED")
                return meterGoodIconImage;
            else
                return meterOffIconImage;
    
}

//GETTERS AND SETTERS-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
function getValuesFromTable(dist)
{
    //var rounddist = Math.round(dist);
    if (dist > table[table.length - 1])
    {
        return -1;
    }
    for (var i = 0; i < table.length - 1 ; i++) {
        if (table[i].distance <= dist && dist < table[i + 1].distance) {
            return table[i + 1];
        }
    }
    return -1;
}

function createTableFromOptions()
{
  //  if (currentTech == "w80211")
  //  {
        var wifitable = [];
    //de 0 a 100 metros
        var i = 0;
        while(true)
        {
            var dist = i;
            //var sp = shadowingPropagation(i);
            var sp = getHataSRDSuccessRate(dist/1000);
            
            var c;
            if (sp >= 0.98)
                //  c = "GREEN";
                c = GREEN;
            else if (0.95 <= sp && sp < 0.98)
                //  c = "YELLOW";
                c = YELLOW;
            else
                c = BLUE;
             //   c = "RED";

            if (sp >= 0.9)
            {
                var value =
				{
				    distance: dist,
				    efficiency: sp,
				    color: c

				}
                wifitable.push(value);
            }
            else
            {
                break;
            }
            i += 5;
        }
        table = wifitable;
   // }

}



function setOpMode(mode)
{
    opMode = mode;
}
function getOpMode()
{
    return opMode;
}
function setRadioMode(mode)
{
    radioMode = mode;
}


function setInsertionOptions(type)
{
    currentIns = type;
    insertListener.remove();
    google.maps.event.removeListener(insertListener);
    insertListener = google.maps.event.addListener(map, 'click', function (event) {
        if (getOpMode() == "Insertion")
        {
            if (type == "DAP") {
                var dap = createDAP();
                dap.place(event.latLng.lat(), event.latLng.lng());
            }
              //  placeDAP(event.latLng.lat(), event.latLng.lng(), currentTech);           
            if (type == "Meter") {
                var meter = createMeter();
                meter.place(event.latLng.lat(), event.latLng.lng());
            }
                //placeMeter(event.latLng.lat(), event.latLng.lng());
            if (type == "Pole") {
                var pole = createPole();
                pole.place(event.latLng.lat(), event.latLng.lng());
            }
              //  placePole(event.latLng.lat(), event.latLng.lng());
        }

    });
}
function fetchReach(tech, scenario, dbm) {
    return loadReachFromTable(tech, scenario, dbm)


}
function executeRFMesh() {
    removeMesh();
    connectViaMesh();
}
function setRFMesh() {
    meshEnabled = !meshEnabled;
    if (meshEnabled)
        executeRFMesh();
    else
        removeMesh();
}
function removeMesh() {
    disconnectedMeters = [];
    for (var i = 0; i < dashedLines.length; i++)
    {
        dashedLines[i].setMap(null);
    }
    for (var i = 0; i < allMarkers.length; i++)
    {
        if (allMarkers[i].type == "Meter")
            allMarkers[i].meshHop = 0;
        if (!allMarkers[i].connected)
            disconnectedMeters.push(allMarkers[i]);
    }
    dashedLines = [];
}
function getConnectedMeters()
{
    var ret = [];
    for(var i = 0; i < allMarkers.length; i ++)
    {
        if (allMarkers[i].type == "Meter" && allMarkers[i].connected == true)
            ret.push(allMarkers[i]);
    }
    return ret;

}
//function connectViaMesh()
//{

//    //ALTERAR PARA 3 'FOR' UM DENTRO DO OUTRO!!!
//    //PARA CADA METER DESCONECTADO
//    var disMeters = disconnectedMeters.slice();
//    var metersToMesh = getConnectedMeters();
//    var hopMeters = [];
//    for (var n = 0 ; n < meshMaxJumps ; n++)
//    {
//        var disMeters = disconnectedMeters.slice();
        
//        for (var i = 0; i < disMeters.length ; i++)
//        {
//            //PEGA O METER MAIS PRÓXIMO QUE ESTÁ CONECTADO A UM DAP E FAZ UMA LIGAÇÃO MESH SE POSSÍVEL
//            var meterToConnect;
//            var finalDis = -1;
//            for (var j = 0; j < metersToMesh.length ; j++)
//            {
//                if (finalDis == -1)
//                {
//                    finalDis = distance(disMeters[i], metersToMesh[j]);
//                    meterToConnect = metersToMesh[j];
//                }
//                else
//                {
//                    var dist = distance(disMeters[i], metersToMesh[j]);
//                    if (dist < finalDis)
//                    {
//                        finalDis = dist;
//                        meterToConnect = metersToMesh[j];
//                    }

//                }
                
                
//            }
//            var values = getValuesFromTable(finalDis);
//            if (values != -1)
//            {
//                drawDashedLine(disMeters[i], meterToConnect, values.color)
//                removeFromList(disconnectedMeters, disMeters[i]);
//                disMeters[i].meshHop = n+1;
//                hopMeters.push(disMeters[i]);
//            }
            
//        }
//        metersToMesh = hopMeters;
//        hopMeters = [];
//    }
   
//}

function setInfoWindowNull()
{
    infowindow.setMap(null);
}
function getAllMarkers() {
    return allMarkers;
}
function setDapsToTechnology()
{
    for (i = 0 ; i < daps.length; i++)
    {
        daps[i].teleTech = currentTech;
        daps[i].reach = fetchReach(currentTech, scenario, dbm);
    }
    drawRefresh();
}

function setScenario(sce) {
    scenario = sce;
    setDapsToTechnology();
}


function getCurrentTech() {
    return currentTech;
}
function setCurrentTech(tech) {
    currentTech = tech;
}
function setdbm(d) {
    dbm = d;
    setDapsToTechnology();

}
//function getConfigurations() {
//    if (opMode == "Removal")
//        var mode = "Mode: " + opMode;
//    else
//        var mode = "Mode: " + currentIns + " " + opMode;
//    if (currentTech == "w80211")
//        var tech = "<br>Technology: " + 802.11;
//    else
//        var tech = "<br>Technology: " + currentTech;
//    var power;
//    switch (dbm) {
//        case "dbm0":
//            {
//                power = "0 dbm";
//                break;
//            }
//        case "dbm6":
//            {
//                power = "6 dbm";
//                break;
//            }
//        case "dbm12":
//            {
//                power = "12 dbm";
//                break;
//            }
//        case "dbm18":
//            {
//                power = "18 dbm";
//                break;
//            }
//        case "dbm24":
//            {
//                power = "24 dbm";
//                break;
//            }
//        case "dbm30":
//            {
//                power = "30 dbm";
//                break;
//            }
//        default:
//            break;
//    }
//    var power = "<br>Power: " + power;
//    var sce = "<br>Scenario: " + scenario;

//    return mode + tech + power + sce;
//}
