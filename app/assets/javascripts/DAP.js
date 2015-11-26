function generateUUID() {
    var d = new Date().getTime();
    var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        var r = (d + Math.random() * 16) % 16 | 0;
        d = Math.floor(d / 16);
        return (c == 'x' ? r : (r & 0x7 | 0x8)).toString(16);
    });
    return uuid;
}
function getSignalInfo(latLng){
    var ret = [];
    //var latLng = new google.maps.LatLng(lat, lng);
    for(var i = 0; i < heatmapPoints.length; i++){
        var hmLatLng = heatmapPoints[i].position;
        var dist = google.maps.geometry.spherical.computeDistanceBetween (latLng, hmLatLng);
        if(dist <= heatmapRadio)
        {
            for(k in heatmapPoints[i].opIds)
                ret.push(heatmapPoints[i].opIds[k]);
        }
    }
    ret = ret.filter(function(item, pos) {
        return ret.indexOf(item) == pos;
    })
    return ret;
    
    

}

function createDAP() {
    var marker = new google.maps.Marker({
        ID: null,
        type: "DAP",
        position: null,
        map: map,
        zIndex: 1,
        draggable: true,
        icon: dapOnIconImage,   
        ghost: null,
        ID: null,
        signalInfo: [],
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
			//elementsGrid.putPosition(this);
            this.ID = generateUUID();
            signalInfo = getSignalInfo(latLng);



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
                '<br>Longitude: ' + this.position.lng() +
                '<br>Coberto por: ';
            for(var i = 0; i < signalInfo.length;i++)
            {
                content+= signalInfo[i];
                if(i != signalInfo.length-1)
                    content += ", ";
            }


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
        signalInfo = getSignalInfo(event.latLng);
        sendDrawRequest();
        marker.removeGhost();

    });
    return marker;
}

