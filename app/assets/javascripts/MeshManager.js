var mesh = [];

function connectViaMesh() {

    var connectedMeters = meters.filter(function (item) {
        return (item.connected == true);
    });

    for (var i = 0; i < meshMaxJumps; i++) {
        for (var j = 0 ; j < meters.length; j++) {
            if (!meters[j].connected) {
                chooseMeterToConnect(meters[j], connectedMeters);
               
            }
                
        }
        connectedMeters = meters.filter(function (item) {
            return (item.connected == true && connectedMeters.indexOf(item) < 0);
        });

    }

}
function chooseMeterToConnect(meter, connectedMeters) {
    var minDist = -1;
    var meterToConnect = -1;
    for (var i = 0; i < connectedMeters.length; i++) {
        var dist = google.maps.geometry.spherical.computeDistanceBetween(meter.getPosition(), connectedMeters[i].getPosition());
        if (dist < minDist || minDist == -1) {
            minDist = dist;
            meterToConnect = connectedMeters[i];
        }     
    }
    if (minDist != -1) {
        var values = getValuesFromTable(minDist);
        if (values != -1) {
            meshConnect(meter,meterToConnect,values.color);
        }
    }
   
}
function meshConnect(meter, meterToConnect, color) {

    var lineSymbol = {
        path: 'M 0,-1 0,1',
        strokeOpacity: 1,
        scale: 3
    };
    var meterPositions = [meter.getPosition(), meterToConnect.getPosition()];
    var routerPath = new google.maps.Polyline(
    {
        path: meterPositions,
        strokeColor: color,
        strokeOpacity: 0,
        icons: [{
            icon: lineSymbol,
            offset: '0',
            repeat: '15px'
        }],
        clickable: false,
        strokeWeight: 1
    });
    routerPath.setMap(map);
    meter.changeIcon(meterMeshIconImage);
    meter.connected = true;
    var meshToAdd = {
        meter: meter,
        target: meterToConnect,
        line: routerPath
    }

    mesh.push(meshToAdd);  
    
   // meter.connected = true;

}
function resetMesh() {

    for (var i = 0; i < mesh.length; i++) {
        mesh[i].line.setMap(null);
        mesh[i].meter.changeIcon(meterOffIconImage);
        mesh[i].meter.connected = false;
    }
    mesh = [];
}
function refreshMesh() {
    resetMesh();
    connectViaMesh();
}