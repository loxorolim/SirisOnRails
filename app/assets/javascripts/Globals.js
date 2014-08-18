//var enableMarkerClusterer = true;
var markerCluster;
var GREEN = "#00FF00";
var YELLOW = "#FFFF00";
var BLUE = "#0000FF";

var drawHeatmap = false;
var heatmap;
var taxiData = [];
var heatmapPoints = [];
var efficiencyRadio = 30; //30metros

var lines = [];
var dashedLines = [];

var ID = 0;

var meshMaxJumps = 3;

var map;

var elevator;
var insertListener;

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
var meshEnabled = false;
var markerPair = [];
var markerConnections = [];


var circles = [];
var request;



var currentIns = "DAP";
var table = [];

//DEFINE O FORMATO E AS IMAGENS UTILIZADAS PARA OS ÍCONES, TAMBÉM SERVE PARA CENTRALIZAR AS IMAGENS COM SUAS RESPECTIVAS POSIÇÕES
var dapOnIconImage =
       {
           url: '/assets/daprouter.png',
           size: new google.maps.Size(19, 19),
           anchor: new google.maps.Point(9.5, 9.5)
       };
var dapOffIconImage =
    {
        url: '/assets/daprouteroff.png',
        size: new google.maps.Size(19, 19),
        anchor: new google.maps.Point(9.5, 9.5)
    };
var meterOffIconImage =
    {
        url: '/assets/blackSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5,5)
    }
var meterBestIconImage =
    {
        url: '/assets/greenSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
var meterBetterIconImage =
    {
        url: '/assets/yellowSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
var meterGoodIconImage =
    {
        url: '/assets/redSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }
var meterMeshIconImage =
    {
        url: '/assets/meshSquare.png',
        size: new google.maps.Size(10, 10),
        anchor: new google.maps.Point(5, 5)
    }

var poleIcon = {
    url: '/assets/pole.png'
}
        
//////////

var mouseInsertionIcon = new google.maps.Marker({});