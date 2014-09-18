function applyPlanning() {
    //var solution = autoPlanningGrasp();
    //applySolution(solution);
    sendSCPToServer();
    //printScpMatrixTeste();
    //var solution = [];
    //for (var i = 0; i < 140; i++)
    //    if (i == 40 || i == 57 || i == 59)
    //        solution[i] = 1;
    //    else
    //        solution[i] = 0;
    //applySolution(solution);
    $.unblockUI();
}
function applySolution(solution) {
    for (var i = 0 ; i < solution.length; i++) {
        if (solution[i] == 1) {
            var latLng = poles[i].position;
            var newDap = createDAP();
            newDap.placeOnMap(latLng.lat(),latLng.lng());
            //placeDAP(latLng.lat(), latLng.lng(), "bla");
        }
            
    }

}

function covers(c, pt, r) {
    var latlng = pointToLatLng(pt, meters);
    var latlng2 = pointToLatLng(c, meters);
    return (google.maps.geometry.spherical.computeDistanceBetween(latlng, latlng2) <= r)
}

//function containsPoint(elem, vet) {
//    for (var i = 0; i < vet.length; i++) {
//        if (vet[i].x == elem.x && vet[i].y == elem.y)
//            return i;
//    }
//    return -1;
//}
//function findBestCoverage(points, coverage) {
   
//    var coveragePos = -1;
//    var numContained = 0;
//    for (var i = 0; i < coverage.length; i++) {
//        if (coverage[i].points.length >= numContained) {
//            var contained = 0;
//            for (var j = 0; j < points.length ; j++) {
//                if (containsPoint(points[j], coverage[i].points) >= 0)
//                    contained++;
//            }
//            if (contained > numContained) {
//                coveragePos = i;
//                numContained = contained;
//            }
//        }
        
            
//    }
//    return coveragePos;
//}

//function addDapAtPoints(circles,meters) {
//    for (var i = 0; i < circles.length; i++) {
//        var latLng = pointToLatLng(circles[i], meters);
//        placeDAP(latLng.lat(), latLng.lng(), "bla");
//    }
//}
function getDapMaximumReach() {
    return table[table.length - 1].distance;
}
//function applyPlanning() {
//    var points = metersToPoints(meters);
//    var r = getDapMaximumReach();
//    //var CirclesBetween2Points = [];   
//    //for(var i = 0; i < points.length; i ++)
//    //    for( var j = 0; j < points.length;j++)
//    //        if (i != j) {
//    //            var circle = circlesFromP1p2r(points[i], points[j], r);
//    //            if (circle != null)
//    //                CirclesBetween2Points.push(circlesFromP1p2r(points[i], points[j], r));
//    //        }
//    var electricPoles = [];
//    for (var i = 0 ; i < poles.length ; i++) {
//        electricPoles.push(latLngToPoint(poles[i].position));
//    }
//    var coverage = [];
//    for(var i = 0 ; i < electricPoles.length ; i++){
//        var obj = ({
//            c: electricPoles[i],
//            points: []});
//        for(var j = 0; j < points.length; j ++)
//            if(covers(electricPoles[i],points[j],r)){
//                obj.points.push(points[j]);
                
//            }   
//        coverage.push(obj);
//    }
//    coverage.sort(function(a, b) {
//        if (a.points.length > b.points.length) return -1;
//        if (a.points.length < b.points.length) return 1;
//        return 0;
//    });
//    var circlesChosen = [];
   
//    while (points.length > 0) {
//        var bestCoveragePos = findBestCoverage(points, coverage);
//        if (bestCoveragePos == -1)
//            break;
//        var bestCoverage = coverage[bestCoveragePos];

//        var listaPontos = printPoints(points);

//        var listaCoberta = printPoints(coverage[bestCoveragePos].points);

//        for (var k = 0; k < coverage[bestCoveragePos].points.length; k++) {
//            var pointPos = containsPoint(coverage[bestCoveragePos].points[k], points);
//            if(pointPos >= 0)
//                points.splice(pointPos, 1);
//            listaPontos = printPoints(points);
//            listaCoberta = printPoints(coverage[bestCoveragePos].points);
//        }
//        circlesChosen.push(coverage[bestCoveragePos].c);
//        coverage.splice(bestCoveragePos, 1);

  
//    }
    
//    addDapAtPoints(circlesChosen, meters);

//}
//function printPoints(points) {
//    var listaPontos = "";
//    points.sort(function (a, b) {
//        if (a.x < b.x) return -1;
//        if (a.x > b.x) return 1;
//        return 0;
//    });
//    for (var ls = 0 ; ls < points.length; ls++) {
//        listaPontos += "Ponto " + ls + " : (" + points[ls].x + ", " + points[ls].y + ")\n";
//    }
//    return listaPontos;

//}

