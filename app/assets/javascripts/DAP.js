function generateUUID() {
    var d = new Date().getTime();
    var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        var r = (d + Math.random() * 16) % 16 | 0;
        d = Math.floor(d / 16);
        return (c == 'x' ? r : (r & 0x7 | 0x8)).toString(16);
    });
    return uuid;
};

function createDAP() {
    var marker = new MarkerWithLabel({
        ID: null,
        type: "DAP",
        position: null,
        map: map,
        zIndex: 1,
        draggable: true,
        icon: dapOnIconImage,   
        ghost: null,
        ID: null,
        //meshMeters: [],
        labelContent: "0",
        labelAnchor: new google.maps.Point(22, 25),
        labelClass: "labels", // the CSS class for the label
        labelStyle: { opacity: 0.75 },
        placeOnMap: function (latitude, longitude) {
            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            daps.push(this);
          //  this.map = map;
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();



        },
        remove: function () {
            var dap = this;
            daps = daps.filter(function (item) {
                return (item.ID != dap.ID);
            });
            this.setMap(null);
            markerCluster.removeMarker(this, true);  
        //    sendDrawRequest();                  
        },
        changeIcon: function (newIcon) {
            this.icon = newIcon;
        },

        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng() ;

            infowindow.setContent(content);
            infowindow.open(map, this);
        },
        createGhost: function () {
            var pos = this.getPosition();
            this.ghost =  new google.maps.Marker({
                position: pos,
                map: map,
                zIndex: 1,
                draggable: false,
                icon: dapGhostIconImage,
             });
        },
        removeGhost: function () {
            if(this.ghost != null)
                this.ghost.setMap(null);
        },

        

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            sendDrawRequest();
        }
        else
            marker.displayInfoWindow();

    });
    google.maps.event.addListener(marker, 'dragstart', function (event) {
        infowindow.setMap(null);
        marker.createGhost();
    });
    google.maps.event.addListener(marker, 'drag', function (event) {
    });

    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.setPosition(event.latLng);
        sendDrawRequest()
        marker.removeGhost();

    });
    return marker;
}

// function createDAP2() {
//     var marker = new MarkerWithLabel({
//         ID: null,
//         type: "DAP",
//         position: null,
//         map: map,
//         zIndex: 1,
//         draggable: true,
//         icon: dapOnIconImage,
//         coveredMeters: 0,
//         neighbours: [],
//         connectionLines: [],
//         ghost: null,
//         //meshMeters: [],
//         labelContent: "0",
//         labelAnchor: new google.maps.Point(22, 25),
//         labelClass: "labels", // the CSS class for the label
//         labelStyle: { opacity: 0.75 },
//         place: function (latitude, longitude) {
//             var latLng = new google.maps.LatLng(latitude, longitude);
//             this.position = latLng;
//             daps.push(this);
//             markerCluster.addMarker(this, true);
//             this.ID = generateUUID();
//             this.refresh();
//             calculateEfficiency(marker);

//         },
//         remove: function () {
//             var dap = this;
//             daps = daps.filter(function (item) {
//                 return (item.ID != dap.ID);
//             });
//             this.setMap(null);
//             markerCluster.removeMarker(this, true);
//             if (meshEnabled) 
//                 resetMesh();
//             this.removeConnections();  
//             this.refresh();                   
//         },
//         changeIcon: function (newIcon) {
//             this.icon = newIcon;
//         },
//         connect: function (target, tableValue) {
//             this.connectOneWay(target, tableValue);
//             target.connectOneWay(this, tableValue);
//         },
//         connectOneWay: function (target, tableValue) {
//             this.coveredMeters++;
//             for (var i = 0; i < this.neighbours.length; i++) {
//                 if (this.neighbours[i].ID == target.ID) {
//                     this.neighbours.splice(i, 1);
//                     break;
//                 }
//             }
//             for (var i = 0; i < this.connectionLines.length; i++) {
//                 if (this.connectionLines[i].targetID == target.ID) {
//                     this.connectionLines[i].setMap(null);
//                     this.connectionLines.splice(i, 1);
//                     break;
//                 }
//             }
//             this.neighbours.push(target);
//             var markerPositions = [this.getPosition(), target.getPosition()];
            
//             var routerPath = new google.maps.Polyline(
// 	        {
// 	            targetID: target.ID,
// 	            path: markerPositions,
// 	            strokeColor: tableValue.color,
// 	            strokeOpacity: 1.0,
// 	            strokeWeight: 2,
// 	            clickable: false,
//                 distance: tableValue.distance,
//                 efficiency: tableValue.efficiency                
// 	        });
//             this.connectionLines.push(routerPath);
//             routerPath.setMap(map);

//         },
//         disconnectTarget: function (target) {
//             if (target.type == "Meter")
//                 this.coveredMeters--;
//             for (var i = 0; i < this.neighbours.length; i++) {
//                 if (this.neighbours[i].ID == target.ID) {
//                     this.neighbours.splice(i, 1);
//                     break;
//                 }
//             }
//             for (var i = 0; i < this.connectionLines.length; i++) {

//                 if (this.connectionLines[i].targetID == target.ID) {
//                     this.connectionLines[i].setMap(null);
//                     this.connectionLines.splice(i, 1);
//                     break;
//                 }
//             }
//         },

//         connectByDistance: function () {

//             var toCover = [];
//             var allMarkers = meters.concat(daps);
//             for (var i = 0; i < allMarkers.length; i++) {
//                 var dist = getDistance(this.getPosition(), allMarkers[i].position);
                
//                 var values = getValuesFromTable(dist);
//                 if (values != -1 && this.ID != allMarkers[i].ID) {
//                     var toAdd = {
//                         marker: allMarkers[i],                       
//                         value: values
//                     };
//                     toCover.push(toAdd);
//                 }
//             }
//             toCover = toCover.sort(function (a, b) { return a.value.distance - b.value.distance });          
//             for (var i = 0; i < toCover.length; i++) 
//                 this.connect(toCover[i].marker, toCover[i].value);

            
//         },

//         removeConnections: function () {
//             for (var i = 0; i < this.neighbours.length; i++) 
//                 this.neighbours[i].disconnectTarget(this);                       
//             for (var i = 0; i < this.connectionLines.length; i++) 
//                 this.connectionLines[i].setMap(null);
            
//             this.connectionLines = [];
//             this.neighbours = [];
//             this.coveredMeters = 0;
//         },
//         calculateEfficiency: function () {

//         },
//         refresh: function () {
//             sendDataToServer("http://localhost:3000/autoplan", 'POST', PROPAGATION_FILE_ID);
//             /*if (meshEnabled) {
//                 resetMesh();
//             }
//             this.removeConnections();
//             this.connectByDistance();
//             calculateEfficiency(this);//mudar
//             if (meshEnabled) {
//                 connectViaMesh();
//             }
//             */
//         },
//         displayInfoWindow: function () {
//             var content = 'ID: ' + this.ID +
//                 '<br>Latitude: ' + this.position.lat() +
//                 '<br>Longitude: ' + this.position.lng() +
//                 '<br>Vizinhos: ' + this.neighbours.length;

//             infowindow.setContent(content);
//             infowindow.open(map, this);
//         },
//         createGhost: function () {
//             var pos = this.getPosition();
//             this.ghost =  new google.maps.Marker({
//                 position: pos,
//                 map: map,
//                 zIndex: 1,
//                 draggable: false,
//                 icon: dapGhostIconImage,
//              });
//         },
//         removeGhost: function () {
//             if(this.ghost != null)
//                 this.ghost.setMap(null);
//         },
//         statisticalData: function () {
//            /* var avgQualitySum = 0;
//             var metersConnected = 0;
//             var avgDistanceSum = 0;
//             var directNeighbours = this.neighbours;
//             for(var i = 0; i < directNeighbours.length; i ++){
//                 var dist = getDistance(this, directNeighbours[i]);
//                 var value = getValuesFromTable(dist);
//                 avgQualitySum += value.efficiency;

//             }
//             metersConnected += directNeighbours.length;

//             if(meshEnabled){

//                 var neighbours ;
//                 var getNewNeighbours = function(neighbours){
//                     var ret = [];
//                     for(var i = 0; i < meters.length; i++){
//                         var dist = 
//                     }


//                 }
//                 for(var j = 0 ; j < meshMaxJumps; j++){
//                     var newNeighbours = [];
//                     for(var k = 0; k < neighbours.length; k++){
                        
//                     }

//                     newNeighbours = newNeighbours.filter(function (elem, pos) {
//                     return newNeighbours.indexOf(elem) == pos;
//                     });
//                     neighbours = newNeighbours;
//                 }
//             }
//         */   
//         }
        

//     });
//     google.maps.event.addListener(marker, 'click', function (event) {
//         if (opMode == "Removal") {
//             marker.remove();
//             if (meshEnabled)
//                 connectViaMesh()
//         }
//         else
//             marker.displayInfoWindow();

//     });
//     google.maps.event.addListener(marker, 'dragstart', function (event) {
//         infowindow.setMap(null);
//         if (meshEnabled)
//             resetMesh();
//         marker.removeConnections(event.latLng);
//         marker.createGhost();
//     });
//     google.maps.event.addListener(marker, 'drag', function (event) {
//     });

//     google.maps.event.addListener(marker, 'dragend', function (event) {
//         marker.setPosition(event.latLng);
//         marker.refresh();
//         marker.removeGhost();
//     });
//     return marker;
// }
