function createMeter() {
    var marker = new google.maps.Marker({
        ID: null,
        type: "Meter",
        position: null,
        map: map,
        zIndex: 2,
        connected: false,
        draggable: true,
        load: 0,

        //        meshConnectionLines: [],
        icon: meterOffIconImage,
        //        dapsConnected: [],
        neighbours: [],
        meshNeighbours: [],
        place: function (latitude, longitude) {

            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            meters.push(this);
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();
            resetMesh();
            this.removeConnections(this.getPosition());
            this.connectByDistance(this.getPosition());
            if (meshEnabled)
                connectViaMesh();
        },
        remove: function () {
            var meter = this;
            meters = meters.filter(function (item) {
                return (item.ID != meter.ID);
            });
            this.setMap(null);
            for (var i = 0; i < this.neighbours.length; i++) {
                this.neighbours[i].disconnectTarget(this);
            }

            markerCluster.removeMarker(this, true);
        },
        changeIcon: function (newIcon) {
            this.setIcon(newIcon);
        },
        changeMeterColor: function () {
            //CHAMAR DEPOIS DE ATUALIZAR A POSICAO
            var color;
            var dist = -1;
            for (i = 0; i < this.neighbours.length; i++) {
                var auxDist = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), this.neighbours[i].getPosition());
                if (dist == -1 || auxDist < dist)
                    dist = auxDist;

            }
            var values = getValuesFromTable(dist);
            if (values != -1 && dist != -1) {
                if (values.color == GREEN)
                    this.changeIcon(meterBestIconImage);
                if (values.color == YELLOW)
                    this.changeIcon(meterBetterIconImage);
                if (values.color == BLUE)
                    this.changeIcon(meterGoodIconImage);
                this.connected = true;
            }
            else {
                this.changeIcon(meterOffIconImage);
                this.connected = false;
            }

        },
        connectOneWay: function (target) {
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            this.neighbours.push(target);
            this.connected = true;
            this.changeMeterColor();

            //target.neighbours.push(this);
        },
        connect: function (target) {
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            //for (var i = 0; i < this.dapsConnected.length; i++) {
            //    if (this.dapsConnected[i].ID == target.ID) {
            //        this.dapsConnected.splice(i, 1);
            //        break;
            //    }
            //}
            this.neighbours.push(target);
            //this.dapsConnected.push(target);
            this.connected = true;
            this.changeMeterColor();

            //target.neighbours.push(this);
        },
        disconnectTarget: function (target) {
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            //for (var i = 0; i < this.dapsConnected.length; i++) {
            //    if (this.dapsConnected[i].ID == target.ID) {
            //        this.dapsConnected.splice(i, 1);
            //        break;
            //    }
            //}
            this.changeMeterColor();
        },


        connectByDistance: function (newDistance) {
            for (var i = 0; i < daps.length; i++) {
                var dist = google.maps.geometry.spherical.computeDistanceBetween(newDistance, daps[i].position);
                var values = getValuesFromTable(dist);
                if (values != -1) {

                    daps[i].refresh();
                    // this.connect(daps[i], values.color);
                    //daps[i].removeConnections(daps[i].getPosition());
                    //daps[i].connectByDistance(daps[i].getPosition());


                }
            }
        },
        removeConnections: function (newDistance) {
            for (var i = 0; i < this.neighbours.length; i++) {
                this.neighbours[i].disconnectTarget(this);
            }
            this.neighbours = [];
            this.changeMeterColor();

        },
        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng() +
                '<br>Quantidade de vizinhos: ' + this.neighbours.length +
                '<br>Carga: ' + this.load +
                '<br>Conectado?: ' + this.connected;

            infowindow.setContent(content);
            infowindow.open(map, this);

        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            if (meshEnabled)
                executeRFMesh()
        }
        else
            marker.displayInfoWindow();

    });
    google.maps.event.addListener(marker, 'dragstart', function (event) {
        marker.removeConnections(event.latLng);
        if (meshEnabled)
            resetMesh();
        infowindow.setMap(null);
        // removeMesh();
    });
    google.maps.event.addListener(marker, 'drag', function (event) {


    });

    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.removeConnections(event.latLng);
        marker.connectByDistance(event.latLng);
        marker.setPosition(event.latLng);
        if (meshEnabled)
            connectViaMesh();
    });
    return marker;
}

function createMeter2() {
    var marker = new google.maps.Marker({
        ID: null,
        type: "Meter",
        position: null,
        map: map,
        zIndex: 2,
        connected: false,
        draggable: true,
        load: 0,

        //        meshConnectionLines: [],
        icon: meterOffIconImage,
        //        dapsConnected: [],
        neighbours: [],
        meshNeighbours: [],
        place: function (latitude, longitude) {

            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            meters.push(this);
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();
            resetMesh();
            this.removeConnections(this.getPosition());
            this.connectByDistance(this.getPosition());
            if (meshEnabled)
                connectViaMesh();
        },
        remove: function () {
            var meter = this;
            meters = meters.filter(function (item) {
                return (item.ID != meter.ID);
            });
            this.setMap(null);
            for (var i = 0; i < this.neighbours.length; i++) {
                this.neighbours[i].disconnectTarget(this);
            }

            markerCluster.removeMarker(this, true);
        },
        changeIcon: function (newIcon) {
            this.setIcon(newIcon);
        },
        changeMeterColor: function () {
            //CHAMAR DEPOIS DE ATUALIZAR A POSICAO
            var color;
            var dist = -1;
            for (i = 0; i < this.neighbours.length; i++) {
                var auxDist = google.maps.geometry.spherical.computeDistanceBetween(this.getPosition(), this.neighbours[i].getPosition());
                if (dist == -1 || auxDist < dist)
                    dist = auxDist;

            }
            var values = getValuesFromTable(dist);
            if (values != -1 && dist != -1) {
                if (values.color == GREEN)
                    this.changeIcon(meterBestIconImage);
                if (values.color == YELLOW)
                    this.changeIcon(meterBetterIconImage);
                if (values.color == BLUE)
                    this.changeIcon(meterGoodIconImage);
                this.connected = true;
            }
            else {
                this.changeIcon(meterOffIconImage);
                this.connected = false;
            }

        },
        connect: function (target) {
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            //for (var i = 0; i < this.dapsConnected.length; i++) {
            //    if (this.dapsConnected[i].ID == target.ID) {
            //        this.dapsConnected.splice(i, 1);
            //        break;
            //    }
            //}
            this.neighbours.push(target);
            //this.dapsConnected.push(target);
            this.connected = true;
            this.changeMeterColor();

            //target.neighbours.push(this);
        },
        disconnectTarget: function (target) {
            for (var i = 0; i < this.neighbours.length; i++) {
                if (this.neighbours[i].ID == target.ID) {
                    this.neighbours.splice(i, 1);
                    break;
                }
            }
            //for (var i = 0; i < this.dapsConnected.length; i++) {
            //    if (this.dapsConnected[i].ID == target.ID) {
            //        this.dapsConnected.splice(i, 1);
            //        break;
            //    }
            //}
            this.changeMeterColor();
        },

        connectByDistance: function (newDistance) {
            for (var i = 0; i < daps.length; i++) {
                var dist = google.maps.geometry.spherical.computeDistanceBetween(newDistance, daps[i].position);
                var values = getValuesFromTable(dist);
                if (values != -1) {

                    daps[i].refresh();
                    // this.connect(daps[i], values.color);
                    //daps[i].removeConnections(daps[i].getPosition());
                    //daps[i].connectByDistance(daps[i].getPosition());


                }
            }
        },
        removeConnections: function (newDistance) {
            for (var i = 0; i < this.neighbours.length; i++) {
                this.neighbours[i].disconnectTarget(this);
            }
            this.neighbours = [];
            this.changeMeterColor();

        },
        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng() +
                '<br>Quantidade de vizinhos: ' + this.neighbours.length +
                '<br>Carga: ' + this.load +
                '<br>Conectado?: ' + this.connected;

            infowindow.setContent(content);
            infowindow.open(map, this);

        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            if (meshEnabled)
                executeRFMesh()
        }
        else
            marker.displayInfoWindow();

    });
    google.maps.event.addListener(marker, 'dragstart', function (event) {
        marker.removeConnections(event.latLng);
        if (meshEnabled)
            resetMesh();
        infowindow.setMap(null);
        // removeMesh();
    });
    google.maps.event.addListener(marker, 'drag', function (event) {


    });

    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.removeConnections(event.latLng);
        marker.connectByDistance(event.latLng);
        marker.setPosition(event.latLng);
        if (meshEnabled)
            connectViaMesh();
    });
    return marker;
}