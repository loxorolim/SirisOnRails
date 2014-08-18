
function metersToPoints(meters) {
    var points = [];
    for (var i = 0; i < meters.length ; i++) {
        if (!meters[i].connected) {
            var p = latLngToPoint(meters[i].getPosition());
            meters[i].X = p.x;
            meters[i].Y = p.y;
            points.push(p);
        }

     
    }    
    return points;   
}


function latLngToPoint(position) {
    var projection = map.getProjection();
    return projection.fromLatLngToPoint(position);
}
function pointToLatLng(point) {
    var projection = map.getProjection();
   return projection.fromPointToLatLng(point);
}