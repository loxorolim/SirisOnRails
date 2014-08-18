function createDAP() {
    var marker = new MarkerWithLabel({
        ID: null,
        type: "DAP",
        position: null,
        map: map,
        zIndex: 1,
        draggable: true,
        icon: dapOnIconImage,
        coveredMeters: 0,
        neighbours: [],
        connectionLines: [],
        //meshMeters: [],
        labelContent: "0",
        labelAnchor: new google.maps.Point(22, 25),
        labelClass: "labels", // the CSS class for the label
        labelStyle: { opacity: 0.75 },
        place: function (latitude, longitude) {
            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            daps.push(this);
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();
            this.refresh();
            calculateEfficiency(marker);

        },
        remove: function () {
            var dap = this;
            daps = daps.filter(function (item) {
                return (item.ID != dap.ID);
            });
            this.setMap(null);
            markerCluster.removeMarker(this, true);
            if (meshEnabled) 
                resetMesh();
            this.removeConnections();                     
        },
        changeIcon: function (newIcon) {
            this.icon = newIcon;
        },
        connect: function (target, color) {
            this.connectOneWay(target, color);
            target.connectOneWay(this, color);
        },
        connectOneWay: function (target, color) {
            this.coveredMeters++;
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            for (var i = 0; i < this.connectionLines.length; i++) {
                if (this.connectionLines[i].targetID == target.ID) {
                    this.connectionLines[i].setMap(null);
                    this.connectionLines.splice(i, 1);
                    break;
                }
            }
            this.neighbours.push(target);
            var markerPositions = [this.getPosition(), target.getPosition()];
            var routerPath = new google.maps.Polyline(
	        {
	            targetID: target.ID,
	            path: markerPositions,
	            strokeColor: color,
	            strokeOpacity: 1.0,
	            strokeWeight: 2,
	            clickable: false
	        });
            this.connectionLines.push(routerPath);
            routerPath.setMap(map);

        },
        disconnectTarget: function (target) {
            if (target.type == "Meter")
                this.coveredMeters--;
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            for (var i = 0; i < this.connectionLines.length; i++) {

                if (this.connectionLines[i].targetID == target.ID) {
                    this.connectionLines[i].setMap(null);
                    this.connectionLines.splice(i, 1);
                    break;
                }
            }
        },

        connectByDistance: function () {

            var toCover = [];
            var allMarkers = meters.concat(daps);
            for (var i = 0; i < allMarkers.length; i++) {
                var dist = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), allMarkers[i].position);
                var values = getValuesFromTable(dist);
                if (values != -1 && this.ID != allMarkers[i].ID) {
                    var toAdd = {
                        marker: allMarkers[i],
                        distance: dist,
                        value: values
                    };
                    toCover.push(toAdd);
                }
            }
            toCover = toCover.sort(function (a, b) { return a.distance - b.distance });          
            for (var i = 0; i < toCover.length; i++) 
                this.connect(toCover[i].marker, toCover[i].value.color);

            
        },

        removeConnections: function () {
            for (var i = 0; i < this.neighbours.length; i++) 
                this.neighbours[i].disconnectTarget(this);                       
            for (var i = 0; i < this.connectionLines.length; i++) 
                this.connectionLines[i].setMap(null);
            
            this.connectionLines = [];
            this.neighbours = [];
            this.coveredMeters = 0;
        },
        calculateEfficiency: function () {

        },
        refresh: function () {

            if (meshEnabled) {
                resetMesh();
            }
            this.removeConnections();
            this.connectByDistance();
            calculateEfficiency(this);//mudar
            if (meshEnabled) {
                connectViaMesh();
            }
        },
        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng() +
                '<br>Vizinhos: ' + this.neighbours.length;

            infowindow.setContent(content);
            infowindow.open(map, this);
        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            if (meshEnabled)
                connectViaMesh()
        }
        else
            marker.displayInfoWindow();

    });
    google.maps.event.addListener(marker, 'dragstart', function (event) {
        infowindow.setMap(null);
        if (meshEnabled)
            resetMesh();
        marker.removeConnections(event.latLng);
    });
    google.maps.event.addListener(marker, 'drag', function (event) {
    });

    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.setPosition(event.latLng);
        marker.refresh();
    });
    return marker;
}
function createDAP2() {
    var marker = new MarkerWithLabel({
        ID: null,
        type: "DAP",
        position: null,
        map: map,
        zIndex: 1,
        draggable: true,
        icon: dapOnIconImage,
        coveredMeters: 0,
        neighbours: [],
        connectionLines: [],
        meshMeters: [],
        labelContent: "0",
        labelAnchor: new google.maps.Point(22, 25),
        labelClass: "labels", // the CSS class for the label
        labelStyle: { opacity: 0.75 },
        place: function (latitude, longitude) {
            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            daps.push(this);
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();
            resetMesh();
            this.removeConnections(this.getPosition());
            this.connectByDistance(this.getPosition());
            if (meshEnabled)
                connectViaMesh();
            calculateEfficiency(marker);

        },
        remove: function () {
            var dap = this;
            daps = daps.filter(function (item) {
                return (item.ID != dap.ID);
            });
            this.setMap(null);
            markerCluster.removeMarker(this, true);
            //for (var i = 0; i < this.connectionLines.length; i++) {
            //    this.connectionLines[i].setMap(null);
            //}
            //for (var i = 0; i < this.neighbours.length; i++) {
            //    this.neighbours[i].disconnectTarget(this);
            //}
            this.removeConnections();
            this.removeMeshConnections();
            //     this.connectionLines = [];
        },
        changeIcon: function (newIcon) {
            this.icon = newIcon;
        },

        connect: function (target, color) {

            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            for (var i = 0; i < this.connectionLines.length; i++) {
                if (this.connectionLines[i].targetID == target.ID) {
                    this.connectionLines[i].setMap(null);
                    this.connectionLines.splice(i, 1);
                    break;
                }
            }
            this.neighbours.push(target);
            var markerPositions = [this.getPosition(), target.getPosition()];
            var routerPath = new google.maps.Polyline(
	        {
	            targetID: target.ID,
	            path: markerPositions,
	            strokeColor: color,
	            strokeOpacity: 1.0,
	            strokeWeight: 2,
	            clickable: false
	        });
            this.connectionLines.push(routerPath);
            routerPath.setMap(map);

        },
        disconnectTarget: function (target) {
            if (target.type == "Meter")
                this.coveredMeters--;
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            for (var i = 0; i < this.connectionLines.length; i++) {

                if (this.connectionLines[i].targetID == target.ID) {
                    this.connectionLines[i].setMap(null);
                    this.connectionLines.splice(i, 1);
                    break;
                }
            }
        },
        connectByDistance: function () {

            //var toCover = [];
            //var allMarkers = meters.concat(daps);
            //for (var i = 0; i < allMarkers.length; i++) {
            //    var dist = google.maps.geometry.spherical.computeDistanceBetween(newDistance, allMarkers[i].position);
            //    var values = getValuesFromTable(dist);
            //    if (values != -1 && this.ID != allMarkers[i].ID) {
            //        var toAdd = {
            //            marker: allMarkers[i],
            //            distance: dist,
            //            value: values
            //        };
            //        toCover.push(toAdd);
            //    }
            //}

            //toCover = toCover.sort(function (a, b) { return a.distance - b.distance });
            var toCover = this.normalCoverage();
            for (var i = 0; i < toCover.length; i++) {
                // var dist = google.maps.geometry.spherical.computeDistanceBetween(newDistance, allMarkers[i].position);
                // var values = getValuesFromTable(dist);
                //  if (values != -1 && this.ID != allMarkers[i].ID) {


                if (toCover[i].marker.type == "DAP") {
                    this.connect(toCover[i].marker, toCover[i].value.color);
                    toCover[i].marker.connect(this, toCover[i].value.color);
                }
                else {
                    if (this.coveredMeters < DAPLIMIT) {
                        this.connect(toCover[i].marker, toCover[i].value.color);
                        toCover[i].marker.connect(this);
                        this.coveredMeters++;
                    }
                }
                //  }
            }
        },
        meshConnect: function (mesh, target, color) {
            //     if (source) {
            var lineSymbol = {
                path: 'M 0,-1 0,1',
                strokeOpacity: 1,
                scale: 3
            };
            var meterPositions = [mesh.getPosition(), target.getPosition()];
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
            var meshToAdd = {
                mesh: mesh,
                target: target,
                meshLine: routerPath
            }

            this.meshMeters.push(meshToAdd);



            routerPath.setMap(map);
            this.propagateMeshLoad(target);
            //this.meshConnectionLines.push(routerPath);

            //    }
            //  else {
            target.changeIcon(meterMeshIconImage);
            //   }
            //this.meshNeighbours.push(target);
            this.coveredMeters++;
            target.connected = true;
        },
        propagateMeshLoad: function (meter) {
            var m = meter;
            var parent = this.meshMeters.filter(function (item) {
                return item.target.ID == m.ID;
            });
            while (parent.length != 0) {
                parent[0].mesh.load++;
                m = parent[0].mesh;
                var parent = this.meshMeters.filter(function (item) {
                    return item.target.ID == m.ID;
                });
            }


        },
        meterIsOverloaded: function (meter) {
            if (meter.load >= METERLIMIT)
                return true;
            else {
                var m = meter;
                var parent = this.meshMeters.filter(function (item) {
                    return item.target.ID == m.ID;
                });
                while (parent.length != 0) {
                    //parent[0].mesh.load++;
                    if (parent[0].mesh.load >= METERLIMIT)
                        return true;
                    m = parent[0].mesh;
                    var parent = this.meshMeters.filter(function (item) {
                        return item.target.ID == m.ID;
                    });
                }
                return false;
            }
            return false;
        },
        normalCoverage: function () {
            var toCover = [];
            var allMarkers = meters.concat(daps);
            for (var i = 0; i < allMarkers.length; i++) {
                var dist = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), allMarkers[i].position);
                var values = getValuesFromTable(dist);
                if (values != -1 && this.ID != allMarkers[i].ID) {
                    var toAdd = {
                        marker: allMarkers[i],
                        distance: dist,
                        value: values
                    };
                    toCover.push(toAdd);
                }
            }

            toCover = toCover.sort(function (a, b) { return a.distance - b.distance });
            var ret = [];
            var coveredMeters = 0;
            for (var i = 0; i < toCover.length; i++) {
                if (toCover[i].marker.type == "DAP") {
                    ret.push(toCover[i]);
                    //      this.connect(toCover[i].marker, toCover[i].value.color);
                    //      toCover[i].marker.connect(this, values.color);
                }
                else {
                    if (coveredMeters < DAPLIMIT) {
                        //   this.connect(toCover[i].marker, toCover[i].value.color);
                        //   toCover[i].marker.connect(this);
                        ret.push(toCover[i]);
                        coveredMeters++;
                    }
                }
            }
            return ret;
        },
        meshCoverage: function () {
            var aux = this.neighbours;
            var ret = [];

            for (var k = 0; k < meshMaxJumps; k++) {

                var disconnectedMeters = meters.filter(function (item) {
                    return (item.connected != true);
                });
                var mesh = [];
                for (var i = 0; i < aux.length; i++) {
                    for (var j = 0; j < disconnectedMeters.length; j++) {
                        var dist = google.maps.geometry.spherical.computeDistanceBetween(aux[i].getPosition(), disconnectedMeters[j].getPosition());
                        var distToDap = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), disconnectedMeters[j].position);
                        var values = getValuesFromTable(dist);
                        if (values != -1) {
                            var toAdd = {
                                marker: disconnectedMeters[j],
                                mesh: aux[i],
                                distance: dist,
                                distanceToDap: distToDap,
                                value: values
                            };
                            var found = false;
                            for (var z = 0; z < mesh.length; z++)
                                if (mesh[z].marker == toAdd.marker) {
                                    found = true;
                                    if (mesh[z].distance > toAdd.distance)
                                        mesh[z] = toAdd;
                                }
                            if (!found)
                                mesh.push(toAdd);
                        }
                    }
                }
                mesh.sort(function (a, b) { return a.distanceToDap - b.distanceToDap });
                aux = [];
                for (var i = 0; i < mesh.length && this.coveredMeters < DAPLIMIT; i++) {
                    //this.coveredMeters++;
                    if (!this.meterIsOverloaded(mesh[i].mesh)) {
                        ret.push(mesh[i]);
                        //this.meshConnect(mesh[i].mesh, mesh[i].marker, mesh[i].value.color)
                        //mesh[i].marker.meshConnect(mesh[i].mesh, mesh[i].value.color);
                        aux.push(mesh[i].marker);
                    }


                }
            }
            return ret;
        },
        calculateCoverage: function () {
            if (meshEnabled) {

            }
            else {

            }
        },
        connectByDistanceMesh: function () {

            var aux = this.neighbours;


            for (var k = 0; k < meshMaxJumps; k++) {

                var disconnectedMeters = meters.filter(function (item) {
                    return (item.connected != true);
                });
                var mesh = [];
                for (var i = 0; i < aux.length; i++) {
                    for (var j = 0; j < disconnectedMeters.length; j++) {
                        var dist = google.maps.geometry.spherical.computeDistanceBetween(aux[i].getPosition(), disconnectedMeters[j].getPosition());
                        var distToDap = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), disconnectedMeters[j].position);
                        var values = getValuesFromTable(dist);
                        if (values != -1) {
                            var toAdd = {
                                marker: disconnectedMeters[j],
                                mesh: aux[i],
                                distance: dist,
                                distanceToDap: distToDap,
                                value: values
                            };
                            var found = false;
                            for (var z = 0; z < mesh.length; z++)
                                if (mesh[z].marker == toAdd.marker) {
                                    found = true;
                                    if (mesh[z].distance > toAdd.distance)
                                        mesh[z] = toAdd;
                                }
                            if (!found)
                                mesh.push(toAdd);
                        }
                    }
                }
                mesh.sort(function (a, b) { return a.distanceToDap - b.distanceToDap });
                aux = [];
                // COM LIMITE DE CARGA
                //for (var i = 0; i < mesh.length && this.coveredMeters < DAPLIMIT; i++) {
                //    if (!this.meterIsOverloaded(mesh[i].mesh)) {
                //        this.meshConnect(mesh[i].mesh, mesh[i].marker, mesh[i].value.color)
                //        aux.push(mesh[i].marker);
                //    }
                //}
                //SEM LIMITE DE CARGA
                for (var i = 0; i < mesh.length; i++) {
                    this.meshConnect(mesh[i].mesh, mesh[i].marker, mesh[i].value.color)
                    aux.push(mesh[i].marker);
                }
            }
        },
        removeMeshConnections: function () {
            this.coveredMeters -= this.meshMeters.length;
            for (var i = 0; i < this.meshMeters.length; i++) {
                this.meshMeters[i].meshLine.setMap(null);
                this.meshMeters[i].mesh.meshNeighbours = [];
                this.meshMeters[i].mesh.load = 0;
                this.meshMeters[i].target.changeIcon(meterOffIconImage);
                this.meshMeters[i].target.connected = false;

            }
            this.meshMeters = [];

        },
        removeConnections: function (newDistance) {
            for (var i = 0; i < this.neighbours.length; i++) {
                //  var dist = google.maps.geometry.spherical.computeDistanceBetween(newDistance, this.neighbours[i].position);
                // var values = getValuesFromTable(dist);
                //if (values == -1) {
                this.neighbours[i].disconnectTarget(this);
                //}
            }

            for (var i = 0; i < this.connectionLines.length; i++) {

                this.connectionLines[i].setMap(null);
            }
            this.connectionLines = [];
            this.neighbours = [];
            this.coveredMeters = 0;
        },
        calculateEfficiency: function () {

        },
        refresh: function () {
            if (meshEnabled)
                resetMesh();
            this.removeConnections(this.getPosition());
            this.connectByDistance(this.getPosition());
            if (meshEnabled)
                this.connectByDistanceMesh();
        },
        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng() +
                '<br>Medidores cobertos: ' + this.neighbours.length +
                '<br>Medidores em mesh: ' + this.meshMeters.length;

            infowindow.setContent(content);
            infowindow.open(map, this);
        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            if (meshEnabled)
                connectViaMesh()
        }
        else
            marker.displayInfoWindow();

    });
    google.maps.event.addListener(marker, 'dragstart', function (event) {
        infowindow.setMap(null);
        marker.removeConnections(event.latLng);
        if (meshEnabled)
            resetMesh();
        //      removeMesh();
    });
    google.maps.event.addListener(marker, 'drag', function (event) {

        // marker.removeConnections(event.latLng);
        // marker.connectByDistance(event.latLng);
        //  connectViaMesh();

    });

    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.setPosition(event.latLng);
        marker.removeConnections(event.latLng);
        marker.connectByDistance();
        calculateEfficiency(marker);
        if (meshEnabled)
            connectViaMesh();


    });
    return marker;
}