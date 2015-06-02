var TILE_SIZE = 256;

//Mercator --BEGIN--
function bound(value, opt_min, opt_max) {
  if (opt_min !== null) value = Math.max(value, opt_min);
  if (opt_max !== null) value = Math.min(value, opt_max);
  return value;
}

function degreesToRadians(deg) {
  return deg * (Math.PI / 180);
}

function radiansToDegrees(rad) {
  return rad / (Math.PI / 180);
}

function MercatorProjection() {
  this.pixelOrigin_ = new google.maps.Point(TILE_SIZE / 2,
  TILE_SIZE / 2);
  this.pixelsPerLonDegree_ = TILE_SIZE / 360;
  this.pixelsPerLonRadian_ = TILE_SIZE / (2 * Math.PI);
}

MercatorProjection.prototype.fromLatLngToPoint = function (latLng,
opt_point) {
  var me = this;
  var point = opt_point || new google.maps.Point(0, 0);
  var origin = me.pixelOrigin_;

  point.x = origin.x + latLng.lng() * me.pixelsPerLonDegree_;

  // NOTE(appleton): Truncating to 0.9999 effectively limits latitude to
  // 89.189.  This is about a third of a tile past the edge of the world
  // tile.
  var siny = bound(Math.sin(degreesToRadians(latLng.lat())), - 0.9999,
  0.9999);
  point.y = origin.y + 0.5 * Math.log((1 + siny) / (1 - siny)) * -me.pixelsPerLonRadian_;
  return point;
};

MercatorProjection.prototype.fromPointToLatLng = function (point) {
  var me = this;
  var origin = me.pixelOrigin_;
  var lng = (point.x - origin.x) / me.pixelsPerLonDegree_;
  var latRadians = (point.y - origin.y) / -me.pixelsPerLonRadian_;
  var lat = radiansToDegrees(2 * Math.atan(Math.exp(latRadians)) - Math.PI / 2);
  return new google.maps.LatLng(lat, lng);
};

var desiredRadiusPerPointInMeters = 10;
function getNewRadius() {
  
  
  var numTiles = 1 << map.getZoom();
  var center = map.getCenter();
  var moved = google.maps.geometry.spherical.computeOffset(center, 10000, 90); /*1000 meters to the right*/
  var projection = new MercatorProjection();
  var initCoord = projection.fromLatLngToPoint(center);
  var endCoord = projection.fromLatLngToPoint(moved);
  var initPoint = new google.maps.Point(
	initCoord.x * numTiles,
	initCoord.y * numTiles);
   var endPoint = new google.maps.Point(
	endCoord.x * numTiles,
	endCoord.y * numTiles);
var pixelsPerMeter = (Math.abs(initPoint.x-endPoint.x))/10000.0;
var totalPixelSize = Math.floor(desiredRadiusPerPointInMeters*pixelsPerMeter);
console.log(totalPixelSize);
return totalPixelSize;
 
}
var pontosteste = [
  { location: new google.maps.LatLng(37.782, -122.447), weight: 0.9 },
  { location: new google.maps.LatLng(37.782, -122.443), weight: 0.8 },
  { location: new google.maps.LatLng(37.782, -122.441), weight: 0.7 },
  { location: new google.maps.LatLng(37.782, -122.439), weight: 1.0 },
  { location: new google.maps.LatLng(37.782, -122.435), weight: 0.5 },
  { location: new google.maps.LatLng(37.785, -122.447), weight: 0.3 },
  { location: new google.maps.LatLng(37.785, -122.445), weight: 0.6 },
  { location: new google.maps.LatLng(37.785, -122.441), weight: 0.9 },
  { location: new google.maps.LatLng(37.785, -122.437), weight: 1.0 },
  { location: new google.maps.LatLng(37.785, -122.435), weight: 0.5 }
];

function createHeatMap()
{

    var pointArray = new google.maps.MVCArray(heatmapPoints);

    heatmap = new google.maps.visualization.HeatmapLayer({
        data: pointArray,
        radius: getNewRadius()
		//dissipating: false,
        //maxIntensity: 10000

    });
   // heatmap.set('radius', heatmap.get('radius') ? null : 40);
    var gradient = [
     'rgba(0, 0,  255, 0)',
     'rgba(16, 16, 240, 1)',
     'rgba(32, 32, 224, 1)',
     'rgba(48, 48, 208, 1)',
     'rgba(64, 64, 192, 1)',
     'rgba(80, 80, 176, 1)',
     'rgba(96, 96, 160, 1)',
     'rgba(112, 112, 144, 1)',
     'rgba(128, 128, 128, 1)',
     'rgba(144, 144, 112, 1)',
     'rgba(160, 160, 96, 1)',
     'rgba(176, 176, 80, 1)',
     'rgba(192, 192, 64, 1)',
     'rgba(208, 208, 48, 1)',
     'rgba(224, 224, 32, 1)',
     'rgba(240, 240, 16, 1)',
     'rgba(255, 255, 0, 1)',
     'rgba( 255,240, 0, 1)',
     'rgba( 255,224, 0, 1)',
     'rgba( 255,208, 0, 1)',
     'rgba( 255,192, 0, 1)',
     'rgba( 255,176, 0, 1)',
     'rgba( 255,160, 0, 1)',
     'rgba( 255,144, 0, 1)',
     'rgba( 255,128, 0, 1)',
     'rgba( 255,112, 0, 1)',
     'rgba( 255,96, 0, 1)',
     'rgba( 255,80, 0, 1)',
     'rgba( 255,64, 0, 1)',
     'rgba( 255,48, 0, 1)',
     'rgba( 255,32, 0, 1)',
     'rgba( 255,16, 0, 1)'
     

    ]
   // heatmap.set('gradient', heatmap.get('gradient') ? null : gradient); 
   	  
	google.maps.event.addListener(map, 'zoom_changed', function () {
	  heatmap.setOptions({radius:getNewRadius()});
	});
   // heatmap.setMap(map);
   

}

