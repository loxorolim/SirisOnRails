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
const serverAddress = document.URL+"/autoplan";

const REMOVE_ALL = 0;
const REMOVE_DAPS = 1;
const REMOVE_METERS = 2;
const REMOVE_POLES = 3;

const heatmapRadio =  10; //10metros
const heatmapLimit = 0.8;
const VIVO_ID = 0;
const CLARO_ID= 1;
const OI_ID = 2;
const TIM_ID = 3;

var directionsService;
var markerCluster;
var scenario = Urbano;
var technology = t802_15_4;
var H_TX = 3;
var H_RX = 5;
var BIT_RATE;
var TRANSMITTER_POWER = 20;
var SRD = true;
var REDUNDANCY = 1;
var LIMIT = 100;

var drawHeatmap = false;
var drawRangeView = false;
var autoPlanOverwrite = false;
var heatmap = [];

var taxiData = [];
var heatmapPoints = [];
var drawHeatmapPoints = false;
var heatGrids = [];
var efficiencyRadio = 30; //30metros

var lines = [];
var dashedLines = [];
var coveragePolygon;
var heatmapPolygon;

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
           url: document.URL+'/assets/daprouter.png',
           size: new google.maps.Size(19, 19),
           anchor: new google.maps.Point(9.5, 9.5)
       };
const dapOffIconImage =
    {
        url: document.URL+'/assets/daprouteroff.png',
        size: new google.maps.Size(19, 19),
        anchor: new google.maps.Point(9.5, 9.5)
    };
const dapGhostIconImage =
   {
       url: document.URL+'/assets/dapghost.png',
       size: new google.maps.Size(19, 19),
       anchor: new google.maps.Point(9.5, 9.5)
   };
const poleIcon = {
    url: document.URL+'/assets/pole.png'
}
var mouseInsertionIcon = new google.maps.Marker({});
