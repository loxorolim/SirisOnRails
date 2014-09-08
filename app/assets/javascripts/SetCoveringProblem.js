const AUTO_PLAN_FILE_ID = 0;
const PROPAGATION_FILE_ID = 1;
const METRIC_FILE_ID = 2;

function sendDrawRequisition(){
    sendDataToServer("http://localhost:3000/autoplan", 'POST', PROPAGATION_FILE_ID);
    
}

function sendSCPToServer() {

    //var data = prepareNetworkToSend();
   // var data = data.toString();
    //_ajax_request("http://localhost:3000/autoplan", data, 'POST');
   sendDataToServer("http://localhost:3000/autoplan", 'POST', AUTO_PLAN_FILE_ID);
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
        case PROPAGATION_FILE_ID:
            data = createPropagationFileModel();
            break;
        case METRIC_FILE_ID:
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
                    case PROPAGATION_FILE_ID:
                        readPropagationResponse(data);
                        break;
                    case METRIC_FILE_ID:
                        readMetricResponse();
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
                    newDap.place(latLng.lat(),latLng.lng());
                    
                }
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
function readMetricResponse(){

}
function printScpMatrixTeste(uncoveredMeters) {
  //   if(meshEnabled)
  //      Matrixes = createMeshScpMatrixes();
  //  else
  //      scpMatrix = createScpMatrix();

    var scpMatrix = createScpMatrix(uncoveredMeters);
    //var scpMatrix = Matrixes.scpMatrix;
    //var coverageMatrix = Matrixes.coverageMatrix;
    var Z = scpMatrix.length;
    var Y = poles.length;
    //TEM Q MUDAR ESSE NEGÓCIO AQUI!
    var ret = "set Z;\n set Y;\n param A{r in Z, m in Y}, binary;\n var Route{m in Y}, binary;\n minimize cost: sum{m in Y} Route[m];\n subject to covers{r in Z}: sum{m in Y} A[r,m]*Route[m]>=1;\n solve; \n printf {m in Y:  Route[m] == 1} \"%s \", m > \"Results.txt\";\n data;\n";
    ret += "set Z:= ";
    for (var i = 0; i < Z; i++)
        ret += "Z" + (i + 1) + " ";
    ret += ";\n"
    ret += "set Y:= ";
    for (var i = 0; i < Y; i++)
        ret += "Y" + (i + 1) + " ";
    ret += ";\n";

    ret += "param A : ";

    for (var i = 0; i < Y; i++)
        ret += "Y" + (i + 1) + " ";
    ret += ":= \n";
    for (var i = 0; i < Z; i++) {
        ret += "Z" + (i + 1) + " ";
        for (var j = 0; j < Y; j++) {
            var um = false;
            for(var k = 0; k < scpMatrix[i].length;k++)
                if(j == scpMatrix[i][k])
                    um = true;
            if (um)
                ret += "1 ";
            else
                ret += "0 ";

        }

    }
    ret += "\n";
    ret += ";";
    ret += "end;";
    
    return ret;
    

   // for(var i = 0; i < )
}

function covers2(pt1, pt2, r) {

    return (google.maps.geometry.spherical.computeDistanceBetween(pt1.getPosition(), pt2.getPosition()) <= r)
}
function propagationValuesToSend(){
    var s;
    if(scenario == "Urbano") 
        s = 0;
    else if (scenario == "Suburbano")
        s = 1;
    else
        s = 2;

    var t;
    if(technology == "802_15_4") 
        t = 0;
    else if (scenario == "802_11_a")
        t = 1;
    else
        t = 2;

    var srdv;
    if(SRD)
        srdv = 1;
    else
        srdv = 0;

    var ret = "";
    ret+= s + "\n";
    ret+= t + "\n";
    ret+= H_TX + "\n";
    ret+= H_RX + "\n";
    ret+= BIT_RATE + "\n";
    ret+= TRANSMITTER_POWER + "\n";
    ret+= srdv + "\n";
    return ret;

}
function createAutoPlanningFileModel(){
    //var uncoveredMeters = meters.filter(function (item) {
    //        return (item.connected != true);
    //});
    var uncoveredMeters = meters; //POR ENQUANTO VOU DEIXAR ISSO AQUI PRA NÃO CONFUNDIR AS POSIÇÕES DO SERVDOR COM OS MEDIODRES DAQUI 
    var ret = AUTO_PLAN_FILE_ID + '\n';
    ret += propagationValuesToSend();
    

    if(meshEnabled)
        ret+=meshMaxJumps;
    else
        ret+="0";
  //  ret += "\n" + getDapMaximumReach() + "\n";
    ret+= "\n";
    //ret += "\nMeters\n";
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
function createPropagationFileModel(){
    
       //var uncoveredMeters = meters.filter(function (item) {
    //        return (item.connected != true);
    //});
    
    var ret = PROPAGATION_FILE_ID + '\n';
    ret += propagationValuesToSend();
    
    if(meshEnabled)
        ret+=meshMaxJumps;
    else
        ret+="0";
  //  ret += "\n" + getDapMaximumReach() + "\n";
    ret+= "\n";
    //ret += "\nMeters\n";
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
function createPropagationFileModel2(distance){
    
    var ret = PROPAGATION_FILE_ID + '\n';
    ret += distance + '\n';
    ret += propagationValuesToSend();
    return ret;
}
/*function createScpMatrix(uncoveredMeters){

    var r = getDapMaximumReach();
    var sM = [];
    for (var i = 0 ; i < uncoveredMeters.length ; i++) {
        var polesThatCover = [];
        for (var j = 0; j < poles.length; j++)
            if (covers2(uncoveredMeters[i], poles[j], r)) {
                polesThatCover.push(j);
            }
        if(polesThatCover.length > 0)
            sM.push(polesThatCover);
    }
    //Essa função depende de como o Mesh está implementado, talvez devesse estar no script do mesh?
    if(meshEnabled){
        var sMCopy = [];
        for(var i = 0; i < sM.length; i++){
            var toAdd = [];
            for(var j = 0; j < sM[i].length; j++)
                toAdd.push(sM[i][j])
            sMCopy.push(toAdd);
        }

        var nM = createMeterNeighbourhoodMatrix2(uncoveredMeters);
        for(var i = 0; i < uncoveredMeters.length; i++){
            var neighbours = nM[i];
            for(var j = 0 ; j < meshMaxJumps; j++){
                var newNeighbours = [];
                for(var k = 0; k < neighbours.length; k++){
                    sM[i] = sM[i].concat(sMCopy[neighbours[k]])
                    newNeighbours = newNeighbours.concat(nM[neighbours[k]]);
                }
                sM[i] = sM[i].filter(function (elem, pos) {
                    return sM[i].indexOf(elem) == pos;
                });
                newNeighbours = newNeighbours.filter(function (elem, pos) {
                return newNeighbours.indexOf(elem) == pos;
                });
                neighbours = newNeighbours;

            }
        }
    }




    return sM;
}
function createMeterNeighbourhoodMatrix2(uncoveredMeters) {
   // var points = metersToPoints(meters);
    var r = getDapMaximumReach();
    var M = [];

    for (var i = 0 ; i < uncoveredMeters.length ; i++) {
        var pointsCovered = [];
        for (var j = 0; j < uncoveredMeters.length; j++)
            if (i != j && covers2(uncoveredMeters[i], uncoveredMeters[j], r)) {
                pointsCovered.push(j);

            }
        M.push(pointsCovered);
    }

    return M;
}
*/
function createScpMatrixes() {
    var points = metersToPoints(meters);
    var electricPoles = [];
    for (var i = 0 ; i < poles.length ; i++) {
        electricPoles.push(latLngToPoint(poles[i].position));
    }
    var r = getDapMaximumReach();
    var cM = [];
    var sM = [];
    for (var i = 0; i < points.length; i++) {
        var newArray = [];
        sM.push(newArray);
    }
    for (var i = 0 ; i < electricPoles.length ; i++) {
        var pointsCovered = [];
        for (var j = 0; j < points.length; j++)
            if (covers(electricPoles[i], points[j], r)){
                pointsCovered.push(j);
                sM[j].push(i);

            }
        cM.push(pointsCovered);
    }
    var scpMatrixes = ({
        scpMatrix: sM,
        coverageMatrix: cM 
    });
    return scpMatrixes;
}
function createCoverageMatrix(points) {
    
    var electricPoles = [];
    for (var i = 0 ; i < poles.length ; i++) {
        electricPoles.push(latLngToPoint(poles[i].position));
    }
    var r = getDapMaximumReach();
    var cM = [];
    for (var i = 0 ; i < electricPoles.length ; i++) {
        var pointsCovered = [];
        for (var j = 0; j < points.length; j++)
            if (covers(electricPoles[i], points[j], r)) {
                pointsCovered.push(j);
            }
        cM.push(pointsCovered);
    }

    return cM;
}
function createMeshScpMatrixes() {
 //   var Matrixes = createScpMatrixes();
    //   var scpMatrix = Matrixes.scpMatrix;
    var points = metersToPoints(meters);
    var sM = [];
    var cM = createCoverageMatrix(points);
    var neighbourMatrix = createMeterNeighbourhoodMatrix();

    for (var i = 0; i < points.length; i++) {
        var newArray = [];
        sM.push(newArray);
    }

  //  var added = [];
    //  var s = coverageMatrix[pos].length; //OS MEDIDORES QUE O DAP EM POS SATISFAZ
    for (var k = 0; k < cM.length; k++) {
        var added = [];
        var neighbours = cM[k];
        for (var i = 0; i < meshMaxJumps + 1; i++) {
            var newNeighbourhood = [];
            for (var j = 0 ; j < neighbours.length; j++) {
                added.push(neighbours[j]);
                newNeighbourhood = newNeighbourhood.concat(neighbourMatrix[neighbours[j]]);
            }
            neighbours = newNeighbourhood.filter(function (elem, pos) {
                return newNeighbourhood.indexOf(elem) == pos;
            });
        }
        added = added.filter(function (elem, pos) {
            return added.indexOf(elem) == pos; mes
        });
        cM[k] = cM[k].concat(added);
        cM[k] = cM[k].filter(function (elem, pos) {
            return cM[k].indexOf(elem) == pos;
        });
        for (var l = 0; l < cM[k].length; l++) {
            sM[cM[k][l]].push(k);
        }
    }
    var scpMatrixes = ({
        scpMatrix: sM,
        coverageMatrix: cM
    });
    return scpMatrixes;
}
function createMeshScpMatrixes2() {
    //   var Matrixes = createScpMatrixes();
    //   var scpMatrix = Matrixes.scpMatrix;
    var points = metersToPoints(meters);
    var sM = [];
    var cM = createCoverageMatrix(points);
    var neighbourMatrix = createMeterNeighbourhoodMatrix();

    for (var i = 0; i < points.length; i++) {
        var newArray = [];
        sM.push(newArray);
    }

    //  var added = [];
    //  var s = coverageMatrix[pos].length; //OS MEDIDORES QUE O DAP EM POS SATISFAZ
    for (var k = 0; k < cM.length; k++) {
        var added = [];
        var neighbours = cM[k];
        for (var i = 0; i < meshMaxJumps + 1; i++) {
            var newNeighbourhood = [];
            for (var j = 0 ; j < neighbours.length; j++) {
                added.push(neighbours[j]);
                newNeighbourhood = newNeighbourhood.concat(neighbourMatrix[neighbours[j]]);
            }
            neighbours = newNeighbourhood.filter(function (elem, pos) {
                return newNeighbourhood.indexOf(elem) == pos;
            });
        }
        added = added.filter(function (elem, pos) {
            return added.indexOf(elem) == pos; mes
        });
        cM[k] = cM[k].concat(added);
        cM[k] = cM[k].filter(function (elem, pos) {
            return cM[k].indexOf(elem) == pos;
        });
        for (var l = 0; l < cM[k].length; l++) {
            sM[cM[k][l]].push(k);
        }
    }
    var scpMatrixes = ({
        scpMatrix: sM,
        coverageMatrix: cM
    });
    return scpMatrixes;
}


//function createScpMatrixes() {
//    var cM = [];
//    var sM = [];
//    for (var i = 0; i < meters.length; i++) {
//        var aux = [];
//        sM.push(aux);
//    }
//    var dapsToRemove = [];
//    for (var j = 0; j < poles.length; j++) {
//        var dap = createDAP();
//       // dapsToRemove.push(dap);
//        dap.place(poles[j].getPosition().lat(), poles[j].getPosition().lng());
//       // dap.connectByDistance();
//       // dap.connectByDistanceMesh();
//        var covered = [];
//        for (var i = 0; i < dap.neighbours.length; i++) {
//            if (dap.neighbours[i].type == "Meter") {
//                var toAdd = positionInArray(dap.neighbours[i]);
//                covered.push(toAdd);
//                sM[toAdd].push(j);
//            }
            
            
//          //  covered.push(dap.neighbours[i]);
//        }
//        for (var i = 0; i < dap.meshMeters.length; i++) {
//            covered.push(positionInArray(dap.meshMeters[i].target));
//            sM[toAdd].push(j);
//          // covered.push(dap.meshMeters[i].target);
//        }
//        cM.push(covered);
//        dap.remove();
//    }
//   // for (var i = 0; i < dapsToRemove.length;i++)
//   //     dapsToRemove[i].remove();
//    var scpMatrixes = ({
//        scpMatrix: sM,
//        coverageMatrix: cM
//    });
//    return scpMatrixes;
//}
function positionInArray(marker) {

    if (marker.type == "Meter") {
        for (var i = 0; i < meters.length; i++) {
            if (marker.ID == meters[i].ID)
                return i;
        }
    }
}

function createMeterNeighbourhoodMatrix() {
    var points = metersToPoints(meters);
    var r = getDapMaximumReach();
    var M = [];

    for (var i = 0 ; i < points.length ; i++) {
        var pointsCovered = [];
        for (var j = 0; j < points.length; j++)
            if (i != j && covers(points[i], points[j], r)) {
                pointsCovered.push(j);

            }
        M.push(pointsCovered);
    }

    return M;
}
