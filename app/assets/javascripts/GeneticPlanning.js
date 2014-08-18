var geneticRadius;
var scpMatrix;
var geneticPoles;
var geneticPoints;

function geneticPlanning() {
    geneticRadius = getDapMaximumReach();
    geneticPoints = metersToPoints(meters);
    for (var i = 0 ; i < dapPositions.length ; i++) {
        geneticPoles.push(latLngToPoint(dapPositions[i]));
    }
    scpMatrix = createSCPMatrix(r,electricPoles,points);
    generateFeasibleIndividual(scpMatrix[i].length);
}
function generatePopulation(size) {

}
function generateFeasibleIndividual(size) {
    var individual = [];
    for (var i = 0; i < size ; i++) {
        var random = Math.random();
        if (random >= 0.5)
            individual[i] = 1;
        else
            individual[i] = 0;
    }
    //checkIfFeasible(individual);
    return individual;
}
function makeFeasible(individual) {


}
function createSCPMatrix(r,electricPoles,points) {
    var scpMatrix = [];
    for (var i = 0 ; i < points.length; i++) {
        scpMatrix[i] = [];
        for (var j = 0; j < electricPoles.length; j++) {
            if (covers(electricPoles[j], points[i], r))
                scpMatrix[i][j] = 1;
            else
                scpMatrix[i][j] = 0;
        }
    }
    return scpMatrix;
}