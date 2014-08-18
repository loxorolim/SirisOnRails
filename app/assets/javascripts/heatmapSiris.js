
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

function drawHeatMap()
{
    //for (var i = 0; i < pontosteste.length ; i++) {
    //    var hmpoint = {
    //        position: pontosteste[i].location,
    //        efficiency: 0.9
    //    }
    //    heatmapPoints.push(hmpoint);
   // heatmapPoints = pontosteste;

    //}
    var pointArray = new google.maps.MVCArray(heatmapPoints);

    heatmap = new google.maps.visualization.HeatmapLayer({
        data: pointArray,
        radius: 35,
        opacity: 0.7,
        maxIntensity: 10000

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
    heatmap.set('gradient', heatmap.get('gradient') ? null : gradient);
  
   // heatmap.setMap(map);
   

}


//function changeRadius() {
//    heatmap.set('radius', heatmap.get('radius') ? null : 40);
//}
