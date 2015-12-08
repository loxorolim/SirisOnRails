const heatmapPointColor = '#0000FF';

function createHeatmapPoint() {
    var marker = new google.maps.Circle({
        ID: null,
        type: "HeatmapPoint",
        position: null,
        opIds: [],
        map: null,
        zIndex: 2,
        draggable: true,
        ghost: null,
        radius: 1,
        center: null,
        strokeColor: heatmapPointColor,
        strokeOpacity: 1,
        strokeWeight: 2,
        fillColor: heatmapPointColor,
        fillOpacity: 1,
        getPosition: function () {
            return this.position;
        },
        placeOnMap: function (latitude, longitude, opIds) {
            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            this.center = latLng;
            this.opIds = opIds;
            heatmapPoints.push(this);
            this.ID = generateUUID();
            if (drawHeatmapPoints)
                this.setMap(map);
            setHeatmap();
        },
        remove: function () {
            var heatmapPoint = this;
            heatmapPoints = heatmapPoints.filter(function (item) {
                return (item.ID != heatmapPoint.ID);
            });
            this.setMap(null);
        },
        createGhost: function () {
            var pos = this.getPosition();
            this.ghost = new google.maps.Circle({
                position: pos,
                center: pos,
                map: map,
                zIndex: 1,
                draggable: false,
                strokeColor: heatmapPointColor,
                strokeOpacity: 0.6,
                strokeWeight: 2,
                fillColor: heatmapPointColor,
                fillOpacity: 0.6,
                radius: 1,
            });
        },
        removeGhost: function () {
            if (this.ghost != null)
                this.ghost.setMap(null);
        },

        displayInfoWindow: function () {
            var content = 'ID: ' + this.ID +
                '<br>Latitude: ' + this.position.lat() +
                '<br>Longitude: ' + this.position.lng();
            // '<br>Quantidade de vizinhos: ' + this.neighbours.length +
            // '<br>Carga: ' + this.load +
            // '<br>Conectado?: ' + this.connected;

            infowindow.setContent(content);
            infowindow.open(map, this);

        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal") {
            marker.remove();
            setHeatmap();
            DrawHeatmap();
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
        marker.removeGhost();
        this.position = this.center;
        setHeatmap();
        DrawHeatmap();
    });
    return marker;
}