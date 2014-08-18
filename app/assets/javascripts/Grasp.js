var alpha = 0.8;
var iterations = 500;
var p = 0.75;

function autoPlanningGrasp() {
    var Matrixes;
    //var start = new Date().getTime();

    //createMeshCoverageMatrix(poles);

    //Matrixes = createScpMatrixes();
    if(meshEnabled)
        Matrixes = createMeshScpMatrixes();
    else
        Matrixes = createScpMatrixes();
   // var end = new Date().getTime();
    //var time = end - start;
    //alert('Execution time: ' + time);
//    var meterNeighbourhood = createMeterNeighbourhoodMatrix();
    var scpMatrix = Matrixes.scpMatrix;
    var coverageMatrix = Matrixes.coverageMatrix;
    var bestSolution;
    var bestFitness = -1;
  //  alpha = 1;
  //  for (var a = 0; a < 10; a++) {
        for (var i = 0; i < iterations; i++) {
            var scpCopy = matrixCopy(scpMatrix);
            var coverageCopy = matrixCopy(coverageMatrix);
//            var neighbourCopy = matrixCopy(meterNeighbourhood);
            var solution = constructPhase(scpCopy, coverageCopy);
//            var solution = meshConstructPhase(scpCopy, coverageCopy, neighbourCopy);
              //      solution = LocalSearch(solution, scpMatrix, coverageMatrix);
            var vals = solutionEvaluation(scpMatrix, coverageMatrix, solution);
            if (bestFitness == -1 || solution.fitness < bestFitness) {
                bestSolution = solution.solution;
                bestFitness = solution.fitness;
            }
           
        }
 //       alpha = alpha - 0.10;
 //   }
   
    return bestSolution;
}
function matrixCopy(matrix) {
    var ret = [];
    for (var i = 0; i < matrix.length; i++) {
        var line = [];
        for (var j = 0; j < matrix[i].length; j++) 
            line.push(matrix[i][j]);       
        ret.push(line);
    }
    return ret;
}
function arrayCopy(array) {
    var ret = [];
    for (var i = 0; i < array.length; i++) {     
        ret.push(array[i]);
    }
    return ret;
}
function constructPhase(scpMatrix, coverageMatrix) {

    var solution = initialSolution(coverageMatrix.length);
    
//    var scpMatrix = [[0, 1, 4], [5, 6], [0, 1, 2], [1, 5]];
//    var coverageMatrix = [[0,2], [0,2,3], [2],[],[0],[1,3],[1]];
    //TALVEZ TENHA Q COPIAR AS MATRIZES ANTES
    var tam = scpMatrix.length;
    var solutionFitness = 0;
    while(tam > 0){ 
        var rcl = generateRCL(scpMatrix, coverageMatrix, solution);
        if (rcl.length == 0) //caso haja medidor impossivel de ser coberto
            break;
        var chosen = Math.floor((Math.random() * rcl.length));
        chosen = rcl[chosen].position;
        solution[chosen] = 1;
        solutionFitness++;
        //        var scpCopy = scp

        tam = removeCovered(scpMatrix, coverageMatrix, chosen, tam);

    }
    var ret = ({
        solution: solution,
        fitness: solutionFitness
    });
    return ret;
}
//function meshConstructPhase(scpMatrix, coverageMatrix, neighbourMatrix) {

//    var solution = initialSolution(coverageMatrix.length);

//    //    var scpMatrix = [[0, 1, 4], [5, 6], [0, 1, 2], [1, 5]];
//    //    var coverageMatrix = [[0,2], [0,2,3], [2],[],[0],[1,3],[1]];
//    //TALVEZ TENHA Q COPIAR AS MATRIZES ANTES
//    var tam = scpMatrix.length;
//    var solutionFitness = 0;
//    while (tam > 0) {
//        var rcl = meshGenerateRCL(scpMatrix, coverageMatrix, neighbourMatrix, solution);
//        if (rcl.length == 0) //caso haja medidor impossivel de ser coberto
//            break;
//        var chosen = Math.floor((Math.random() * rcl.length));
//       // chosen = rcl[chosen].position;
//        solution[rcl[chosen].position] = 1;
//        solutionFitness++;
//        //        var scpCopy = scp

//        tam = meshRemoveCovered(scpMatrix, coverageMatrix, neighbourMatrix, rcl[chosen], tam);

//    }
//    var ret = ({
//        solution: solution,
//        fitness: solutionFitness
//    });
//    return ret;
//}
function initialSolution(size) {
    var solution = [];
    for (var i = 0; i < size; i++)
        solution.push(0);
    return solution;
}
function removeCovered(scpMatrix, coverageMatrix, chosen, tam) {

    var newTam = tam - coverageMatrix[chosen].length;
    var scpRemoved = [];
    for (var i = 0; i < coverageMatrix[chosen].length; i++) {
        var pos = coverageMatrix[chosen][i];
        var length = scpMatrix[pos].length;
        var copy = scpMatrix[pos].slice(0, length);
        scpMatrix[pos].splice(0, length);
        var obj = ({
            position: pos,
            posCopy: copy
        });
        scpRemoved.push(obj);
    }
    for (var i = 0; i < scpRemoved.length; i++)
        for (var j = 0; j < scpRemoved[i].posCopy.length; j++)
            for (var k = 0; k < coverageMatrix[scpRemoved[i].posCopy[j]].length; k++)
                if (coverageMatrix[scpRemoved[i].posCopy[j]][k] == scpRemoved[i].position) {
                    coverageMatrix[scpRemoved[i].posCopy[j]].splice(k, 1);
                    break;
                }


    return newTam;

}
//function meshRemoveCovered(scpMatrix,coverageMatrix,neighbourMatrix, chosen, tam) {

//    var newTam = tam - chosen.covered.length;
//    var scpRemoved = [];
//    for (var i = 0; i < chosen.covered.length; i++) {
//        var pos = chosen.covered[i];
//        var length = scpMatrix[pos].length;
//        var copy = scpMatrix[pos].slice(0, length);
//        scpMatrix[pos].splice(0, length);
//        var obj = ({
//            position: pos,
//            posCopy: copy 
//        });
//        scpRemoved.push(obj);
//    }
//    for (var i = 0; i < scpRemoved.length; i++)
//        for (var j = 0; j < scpRemoved[i].posCopy.length; j++)
//            for (var k = 0; k < coverageMatrix[scpRemoved[i].posCopy[j]].length; k++)
//                if (coverageMatrix[scpRemoved[i].posCopy[j]][k] == scpRemoved[i].position) {
//                    coverageMatrix[scpRemoved[i].posCopy[j]].splice(k, 1);
//                    break;
//                }

//    for (var i = 0; i < chosen.covered.length; i++) {
//        for (var j = 0; j < neighbourMatrix.length; j++) {
//            for (var k = 0; k < neighbourMatrix[j].length; k++) {
//                if (neighbourMatrix[j][k] == chosen.covered[i]) {
//                    neighbourMatrix[j].splice(k, 1);
//                    break;
//                }
//            }
//        }
//    }

//   return newTam;

//}
function generateRCL(scpMatrix, coverageMatrix, solution) {
    
    var max = 0;
    var rcl = [];
    for (var i = 0; i < coverageMatrix.length; i++) {
        if (solution[i] != 1) {
            var s = coverageMatrix[i].length;
            if (s >= alpha * max && s != 0) {
                var duple = ({
                    position: i,
                    satisfied: s
                });
                rcl.push(duple);
                if (s > max)
                    max = s;
            }
        }
    }
    var limit = alpha * max;
    rcl = rcl.filter(function (item) {
        return (item.satisfied >= limit);
    });
    return rcl;

}
//function meshSatisfability(coverageMatrix, neighbourMatrix, pos) {
//    // var meshMaxJumps = 3;
//    var added = [];
//  //  var s = coverageMatrix[pos].length; //OS MEDIDORES QUE O DAP EM POS SATISFAZ
//    var neighbours = coverageMatrix[pos];
//    for (var i = 0; i < meshMaxJumps+1; i++) {       
//        var newNeighbourhood = [];
//        for (var j = 0 ; j < neighbours.length; j++) {
//            added.push(neighbours[j]);
//            newNeighbourhood = newNeighbourhood.concat(neighbourMatrix[neighbours[j]]);
//        }
//        neighbours = newNeighbourhood.filter(function (elem, pos) {
//            return newNeighbourhood.indexOf(elem) == pos;
//        });

        
//    }
//    added = added.filter(function (elem, pos) {
//        return added.indexOf(elem) == pos;mes
//    });

//    return added;
//}
//function meshGenerateRCL(scpMatrix, coverageMatrix, neighbourMatrix, solution) {

//    var max = 0;
//    var rcl = [];
//    for (var i = 0; i < coverageMatrix.length; i++) {
//        if (solution[i] != 1) {
//            var added = meshSatisfability(coverageMatrix, neighbourMatrix, i);
//            var s = added.length;
//            if (s >= alpha * max && s != 0) {
//                var triple = ({
//                    position: i,
//                    covered: added,
//                    satisfied: s
//                });
//                rcl.push(triple);
//                if (s > max)
//                    max = s;
//            }
//        }
//    }
//    var limit = alpha * max;
//    rcl = rcl.filter(function (item) {
//        return (item.satisfied >= limit);
//    });
//    return rcl;

//}
//function LocalSearch(solution, scpMatrix, coverageMatrix) {

//    var bestSolution = arrayCopy(solution);
//    var vals = solutionEvaluation(scpMatrix, coverageMatrix, bestSolution);
//    var bestSatisfied = vals.rowsSatisfied;
//    var bestColumnsNumber = vals.columnsNumber;
//    for (var i = 0; i < coverageMatrix.length; i++) {
//        var r = Math.random();
//        if (r < p)
//            bestFlip(scpMatrix, coverageMatrix, solution);
//        else
//            randomFlip(solution);

//        var newVals = solutionEvaluation(scpMatrix, coverageMatrix, solution);
//        if (newVals.rowsSatisfied == bestSatisfied)
//            if (newVals.columnsNumber < bestColumnsNumber) {
//                bestColumnsNumber = newVals.columnsNumber;
//                bestSolution = arrayCopy(solution);
//            }
//    }
//    return bestSolution;
//}

//function randomFlip(solution) {
//    var r = Math.random() * solution.length;
//    if (solution[r] == 1)
//        solution[r] = 0;
//    else
//        solution[r] = 1;
//}
//function bestFlip(scpMatrix,coverageMatrix,solution) {
//    var bestFlips = [];
//    var ret = solutionEvaluation(scpMatrix, coverageMatrix, solution);
//    var bestSatisfied = ret.rowsSatisfied;
//    var bestColumnsNumber = ret.columnsNumber;
//    var cArray = createCounterArray(coverageMatrix, scpMatrix.length, solution);

//    for (var i = 0; i < coverageMatrix.length ; i++) {
//        var analysed = solution[i];
//        if (analysed == 0) { //É 0
//            var sat = ret.rowsSatisfied;
//            for (var j = 0; j < coverageMatrix[i].length; j++)
//                if (cArray[coverageMatrix[i][j]] == 0)
//                    sat++;
//            if (sat > bestSatisfied) {
//                while (bestFlips.length > 0) {
//                    bestFlips.pop();
//                }
//                bestFlips.push(i);
//                bestSatisfied = sat;
//                bestColumnsNumber = ret.columnsNumber + 1;
//            }
//            else if ( sat = bestSatisfied) {
//                if (ret.columnsNumber + 1 == bestColumnsNumber)
//                    bestFlips.push(i);
//            }
                
//        }
//        else {  //É 1
//            var sat = ret.rowsSatisfied;
//            for (var j = 0; j < coverageMatrix[i].length; j++)
//                if (cArray[coverageMatrix[i][j]] == 1)
//                    sat--;
//            if (sat == bestSatisfied) {
//                if (ret.columnsNumber - 1 < bestColumnsNumber) {
//                    while (bestFlips.length > 0) {
//                        bestFlips.pop();
//                    }
//                    bestFlips.push(i);
//                    bestColumnsNumber = ret.columnsNumber - 1;
//                }
//                else if (ret.columnsNumber - 1 == bestColumnsNumber)
//                    bestFlips.push(i);
//            }
//        }
//    }
//    var pos = Math.random() * bestFlips.length;
//    if (bestFlips.length != 0) {
//        if (solution[pos] == 1)
//            solution[pos] = 0;
//        else
//            solution[pos] = 1;
//    }
        
//}
function solutionEvaluation(scpMatrix, coverageMatrix, solution) {
    var rowsSatisfied = 0;
    var columnsNumber = 0;

    for (var i = 0; i < scpMatrix.length; i++) 
        for (var j = 0; j < scpMatrix[i].length; j++)
            if(solution[scpMatrix[i][j]] == 1){
                rowsSatisfied++;
                break;
            }
    for (var i = 0; i < solution.length; i++)
        if (solution[i] == 1)
            columnsNumber++;
    var ret = ({
        rowsSatisfied: rowsSatisfied,
        columnsNumber: columnsNumber
    });
    return ret;
}
function createCounterArray(coverageMatrix, rowSize, solution) {
    var cArray = [];
    for (var i = 0; i < rowSize; i++)
        cArray.push(0);
    for (var i = 0; i < coverageMatrix.length; i++) {
        if (solution[i] == 1)
            for (var j = 0; j < coverageMatrix[i].length; j++)
                cArray[coverageMatrix[i][j]]++;
    }
    return cArray;
}