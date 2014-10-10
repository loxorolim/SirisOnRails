var generatorPositions = [];
var offset = 5;
var offsetMin = 3;
var offsetMax = 8; // metros para colocar os postes
var meterOffset = 2;
var meterOffsetMin = 0.5;
var meterOffsetMax = 2;
var poleOffset = 10;
var poleOffsetMin = 5;
var poleOffsetMax = 15;


function generateScenario (){
  var start = generatorPositions[0];
  var end = generatorPositions[1];
  var request = {
      origin: start,
      destination: end,
      travelMode: google.maps.TravelMode.WALKING,
      provideRouteAlternatives: true
  };

  // Route the directions and pass the response to a
  // function to create markers for each step.
  directionsService.route(request, function(response, status) {
    if (status == google.maps.DirectionsStatus.OK) {
      //var warnings = document.getElementById('warnings_panel');
      //warnings.innerHTML = '<b>' + response.routes[0].warnings + '</b>';
      //directionsDisplay.setDirections(response);
      putMeters(response);
      putPoles(response);
    }
  });

}
function putMeters(directionResult) {
  // For each step, place a marker, and add the text to the marker's
  // info window. Also attach the marker to an array so we
  // can keep track of it and remove it when calculating new
  // routes.
   var geo=google.maps.geometry.spherical;
	   for(var j = 0; j < directionResult.routes.length; j++){
	   var myRoute = directionResult.routes[j].overview_path;
	   var i = 1
	   var ini = myRoute[0];
	   var seq ;
	   var distToSeq;
	   while(i < myRoute.length){
	   		seq = myRoute[i];
	   		distToSeq = geo.computeDistanceBetween(ini,seq);
	   		offset = randomBetween(offsetMin,offsetMax);
		   	if(distToSeq > offset){
		   		pos=geo.computeOffset(ini,offset,geo.computeHeading(ini,seq));
		   		generateMeters(pos,geo.computeHeading(ini,seq));
		   		//var marker = new google.maps.Marker({
		     	// position: pos,
		     	// map: map
		    	//});
		    	ini = pos;
		   	}
		   	else{
		   		i++;
		   	}
	   }
   }
   
}
function putPoles(directionResult) {
  // For each step, place a marker, and add the text to the marker's
  // info window. Also attach the marker to an array so we
  // can keep track of it and remove it when calculating new
  // routes.
   var geo=google.maps.geometry.spherical;
   for(var j = 0; j < directionResult.routes.length; j++){
   		var myRoute = directionResult.routes[j].overview_path;
	   var i = 1
	   var ini = myRoute[0];
	   var seq ;
	   var distToSeq;
	   while(i < myRoute.length){
	   		seq = myRoute[i];
	   		distToSeq = geo.computeDistanceBetween(ini,seq);
	   		poleOffset = randomBetween(poleOffsetMin,poleOffsetMax);
		   	if(distToSeq > poleOffset){
		   		
		   		pos=geo.computeOffset(ini,poleOffset,geo.computeHeading(ini,seq));
		   		var pole = createPole();
		   		pole.placeOnMap(pos.lat(),pos.lng());
		   		//var latLng = new google.maps.LatLng(latitude, longitude);
				//	var meter = createMeter();
				//	meter.placeOnMap(latitude, longitude);
				/*
					var populationOptions = {
				      strokeColor: '#FF0000',
				      strokeOpacity: 0.8,
				      strokeWeight: 2,
				      fillColor: '#FF0000',
				      fillOpacity: 0.35,
				      map: map,
				      center: pos,
				      radius: 1
				    };
    // Add the circle for this city to the map.
    			var cityCircle = new google.maps.Circle(populationOptions);
    			*/
		    	ini = pos;
		   	}
		   	else{
		   		i++;
		   	}
	   }
   }
  
}
function generateMeters(latlng, direction){
	var geo=google.maps.geometry.spherical;
	direction+= 90;
	var meterOffset = randomBetween(meterOffsetMin,meterOffsetMax);
	pos1=geo.computeOffset(latlng,-meterOffset,direction);
	meterOffset = randomBetween(meterOffsetMin,meterOffsetMax);
	pos2=geo.computeOffset(latlng,meterOffset,direction);
	var meter = createMeter();
	meter.placeOnMap(pos1.lat(), pos1.lng());
	var meter2 = createMeter();
	meter2.placeOnMap(pos2.lat(), pos2.lng());
	var populationOptions = {
				      strokeColor: '#FF0000',
				      strokeOpacity: 0.8,
				      strokeWeight: 2,
				      fillColor: '#FF0000',
				      fillOpacity: 0.35,
				      map: map,
				      center: pos2,
				      radius: 1
				    };
    // Add the circle for this city to the map.
   /* 			var cityCircle = new google.maps.Circle(populationOptions);
    			var populationOptions = {
				      strokeColor: '#FF0000',
				      strokeOpacity: 0.8,
				      strokeWeight: 2,
				      fillColor: '#FF0000',
				      fillOpacity: 0.35,
				      map: map,
				      center: pos1,
				      radius: 1
				    };
    // Add the circle for this city to the map.
    			var cityCircle = new google.maps.Circle(populationOptions);
    */
}
function randomBetween(num1,num2){
	return Math.floor(Math.random() * num2) + num1
}
//  for (var i = 0; i < myRoute.length; i++) {
//    var marker = new google.maps.Marker({
//      position: myRoute[i],
//      map: map
//    });
    //attachInstructionText(marker, myRoute.steps[i].instructions);
    //markerArray[i] = marker;
//  }

