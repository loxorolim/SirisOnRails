const GREEN_ID = 0;
const YELLOW_ID = 1;
const BLUE_ID = 2;
const GREEN = "#00FF00";
const YELLOW = "#FFFF00";
const BLUE = "#0000FF";
const Urbano = 0;
const Suburbano = 1;
const Rural = 2;
const t802_15_4 = 0;
const t802_11_a = 1;
const t802_11_g = 2;
const serverAddress = "/autoplan";

const REMOVE_ALL = 0;
const REMOVE_DAPS = 1;
const REMOVE_METERS = 2;
const REMOVE_POLES = 3;

var directionsService;
var markerCluster;
var scenario = Urbano;
var technology = t802_15_4;
var H_TX = 3;
var H_RX = 5;
var BIT_RATE;
var TRANSMITTER_POWER = -20;
var SRD = true;
var REDUNDANCY = 1;

var drawHeatmap = false;
var heatmap;
var taxiData = [];
var heatmapPoints = [];
var heatGrids = [];
var efficiencyRadio = 30; //30metros

var lines = [];
var dashedLines = [];

var ID = 0;

var meshMaxJumps = 3;

var map;

var elevator;
var insertListener;

var elementsGrid;
var visibleCells = [];


var poles = [];
var allMarkers = [];
var disconnectedMeters = [];
var oneHopMeters = [];
var meters = [];
var daps = [];
var routers = [];
var infowindow = new google.maps.InfoWindow();
var opMode = "Insertion";
var radioMode = "Line";
var dbm = "dbm0";
var meshEnabled = 0;
var markerPair = [];
var markerConnections = [];
var drawingManager;
var removeSelectionMode = 0;



var request;



var currentIns = "DAP";


//DEFINE O FORMATO E AS IMAGENS UTILIZADAS PARA OS ÍCONES, TAMBÉM SERVE PARA CENTRALIZAR AS IMAGENS COM SUAS RESPECTIVAS POSIÇÕES
const dapOnIconImage =
       {
           url: '/assets/daprouter.png',
           size: new google.maps.Size(19, 19),
           anchor: new google.maps.Point(9.5, 9.5)
       };
const dapOffIconImage =
    {
        url: '/assets/daprouteroff.png',
        size: new google.maps.Size(19, 19),
        anchor: new google.maps.Point(9.5, 9.5)
    };
const dapGhostIconImage =
   {
       url: '/assets/dapghost.png',
       size: new google.maps.Size(19, 19),
       anchor: new google.maps.Point(9.5, 9.5)
   };
const meterGhostIconImage =
{
    url: '/assets/meterghost.png',
    size: new google.maps.Size(10, 10),
    anchor: new google.maps.Point(5,5)
}
const meterOffIconImage =
    {
        url: '/assets/blackSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5,5),
        
    }
const meterBestIconImage =
    {
        url: '/assets/greenSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
const meterBetterIconImage =
    {
        url: '/assets/yellowSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
const meterGoodIconImage =
    {
        url: '/assets/redSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
const meterMeshIconImage =
    {
        url: '/assets/meshSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }

const poleIcon = {
    url: '/assets/pole.png'
}
        
//////////

var mouseInsertionIcon = new google.maps.Marker({});
