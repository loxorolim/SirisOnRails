
var DAPLIMIT = 20;
var METERLIMIT = 5;

function generateUUID() {
    var d = new Date().getTime();
    var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        var r = (d + Math.random() * 16) % 16 | 0;
        d = Math.floor(d / 16);
        return (c == 'x' ? r : (r & 0x7 | 0x8)).toString(16);
    });
    return uuid;
};
function teste() {
    for (var i = 0; i < daps.length; i++)
        daps[i].connectByDistanceMesh();
    
}
function connectViaMeshTeste() {
    resetMesh();
    //for (var i = 0; i < meters.length; i++) {
    //    if (!meters[i].connected)
    //        meters[i].connectViaMesh();
    //}
    var connectedMeters = meters.filter(function (item) {
        return (item.connected == true);
    });

    for (var i = 0; i < meshMaxJumps; i++) {
        for (var j = 0 ; j < meters.length; j++) {
            if(!meters[j].connected)
                meters[j].connectViaMesh(connectedMeters);
        }
        connectedMeters = meters.filter(function (item) {
            return (item.connected == true && connectedMeters.indexOf(item) < 0);
        });

    }


}







