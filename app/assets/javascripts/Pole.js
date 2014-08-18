function createPole() {
    var marker = new google.maps.Marker({
        type: "Pole",
        position: null,
        map: map,
        draggable: true,
        icon: poleIcon,
        zIndex: 0,
        ID: null,
        place: function (latitude, longitude) {
            var latLng = new google.maps.LatLng(latitude, longitude);
            this.position = latLng;
            poles.push(this);
            markerCluster.addMarker(this, true);
            this.ID = generateUUID();
        },
        remove: function () {
            var pole = this;
            poles = poles.filter(function (item) {
                return (item.ID != pole.ID);
            });
            this.setMap(null);
            markerCluster.removeMarker(this, true);
        }

    });
    google.maps.event.addListener(marker, 'click', function (event) {
        if (opMode == "Removal")
            marker.remove();
    });
    google.maps.event.addListener(marker, 'dragend', function (event) {
        marker.setPosition(event.latLng);
    });
    return marker;
}