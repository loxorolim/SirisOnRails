
function setInfoWindowNull()
{
    infowindow.setMap(null);
}
function setOpMode(mode)
{
    opMode = mode;
}
function setInsertionOptions(type)
{
    currentIns = type;
    insertListener.remove();
    google.maps.event.removeListener(insertListener);
    insertListener = google.maps.event.addListener(map, 'click', function (event) {
        if (opMode == "Insertion")
        {
            if (type == "DAP") {
                var dap = createDAP();
                dap.placeOnMap(event.latLng.lat(), event.latLng.lng());
                sendDrawRequest();
            }
              //  placeDAP(event.latLng.lat(), event.latLng.lng(), currentTech);           
            if (type == "Meter") {
                var meter = createMeter();
                meter.placeOnMap(event.latLng.lat(), event.latLng.lng());
                sendDrawRequest();
            }
                //placeMeter(event.latLng.lat(), event.latLng.lng());
            if (type == "Pole") {
                var pole = createPole();
                pole.placeOnMap(event.latLng.lat(), event.latLng.lng());
                sendDrawRequest();
            }
              //  placePole(event.latLng.lat(), event.latLng.lng());

        }
        if (type == "Generator") {
                if(generatorPositions.length < 2)
                  generatorPositions.push(event.latLng);
                if(generatorPositions.length == 2){
                  generateScenario();
                  generatorPositions = [];
                }
            }

    });
}
function setRadio() {
    $("#radio").buttonset();
    $("#Insertion").click(function () {

    });
    $("#eraserRadio").button({
        icons: {
            primary: "eraser",
            secondary: null
        },
        text: false
    });
    $("#eraserRadio").click(function (event) {



        setOpMode("Removal");
        map.setOptions({ draggableCursor: "url(/assets/cursors/removecursor.cur), default" });
        setInfoWindowNull();
        //$('#configMessage').html(getConfigurations());

    });
    $("#viewRadio").button({
        icons: {
            primary: "view",
            secondary: null
        },
        text: false
    });
    $("#viewRadio").click(function () {
        setOpMode("View");
        map.setOptions({ draggableCursor: "url(https://maps.gstatic.com/mapfiles/openhand_8_8.cur), auto" });

    });
    $("#dapRadio").button({
        icons: {
            primary: "dap",
            secondary: null
        },
        text: false
    });
    $("#dapRadio").click(function () {
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("DAP")
        map.setOptions({ draggableCursor: "url(/assets/cursors/dapcursor.cur), auto" });

    });
    $("#meterRadio").button({
        icons: {
            primary: "meter",
            secondary: null
        },
        text: false
    });
    $("#meterRadio").click(function () {
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("Meter");
        map.setOptions({ draggableCursor: "url(/assets/cursors/metercursor.cur), default" })

    });
    $("#pole").button({
        icons: {
            primary: "pole",
            secondary: null
        },
        text: false
    });
    $("#pole").click(function () {
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("Pole");
        map.setOptions({ draggableCursor: "url(/assets/cursors/polecursor.cur), default" })

    });
}
function setButtons()
{

    setRadio();
    $( document ).tooltip({
      items: '*:not(.ui-dialog-titlebar-close)'
    });


	   $("#scenario")        
         .next()
           .button({
               text: false,              
               icons: {
                   primary: "ui-icon-triangle-1-s"
               }
           })
           .click(function () {
               var menu = $(this).parent().next().show().position({
                   my: "left top",
                   at: "left bottom",
                   of: this
               });
               $(document).one("click", function () {
                   menu.hide();
               });
               return false;
           })
           .parent()
             .buttonset()
             .next()
               .hide()
               .menu();
	   $("#technology")
            .next()
              .button({
                  text: false,
                  icons: {
                      primary: "ui-icon-triangle-1-s"
                  }
              })
              .click(function () {
                  var menu = $(this).parent().next().show().position({
                      my: "left top",
                      at: "left bottom",
                      of: this
                  });
                  $(document).one("click", function () {
                      menu.hide();
                  });
                  return false;
              })
              .parent()
                .buttonset()
                .next()
                  .hide()
                  .menu();
	   $("#slider").slider({
	       value: -20,
	       min: -20,
	       max: 30,
	       step: 1,
	       slide: function (event, ui) {
	           $("#power").text(ui.value + "dBm");
	       }
	   });
	  // $("#amount").val("$" + $("#slider").slider("value"));
	  
	   $("#slider").css({ opacity: 0.5 });
	   $("#slider").slider({
	       stop: function (event, ui) {
	           TRANSMITTER_POWER = ui.value;
	           sendDrawRequest();
	       }
	   });
	   $("#choosePower").hover(function () {
	       $("#slider").css({ opacity: 1.0 });
	   }, function () {
	       $("#slider").css({ opacity: 0.5 });

	   });
	   $("#selectScenario").css({ opacity: 0.5 });
	   $("#chooseScenario").hover(function () {
	       $("#selectScenario").css({ opacity: 1.0 });
	   }, function () {
	       $("#selectScenario").css({ opacity: 0.5 });
	       var menu = $("#scenario").parent().next().hide().position({
	           my: "left top",
	           at: "left bottom",
	           of: this
	       });


	       
	   });
	   $("#selectTechnology").css({ opacity: 0.5 });
	   $("#chooseTechnology").hover(function () {
	       $("#selectTechnology").css({ opacity: 1.0 });
	   }, function () {
	       $("#selectTechnology").css({ opacity: 0.5 });
	       var menu = $("#technology").parent().next().hide().position({
	           my: "left top",
	           at: "left bottom",
	           of: this
	       });
	   });

    $('#checkHeatmap').button({
        icons: {
            primary: "heatmap"
        },
        text: false

    }).click(function () {
	      
        $(this).blur();
        drawHeatmap = !drawHeatmap;
	       if (drawHeatmap)
	           heatmap.setMap(map);                             
	       else 
	           heatmap.setMap(null);

	      
	           
	       //$('#check').button.removeClass("ui-state-focus ui-state-hover");


	       
    });

    $('#autoPlanning').button().click(function () {
        
        //createMeshCoverageMatrix(poles);
        $(this).blur();
        //$.blockUI({  message: '<h1><img src="/assets/siri2.gif" /> Enviando ao servidor... </h1>' });
        // autoPlanningGrasp();
        //d = new Date();
        //setTimeout('applyPlanning()', 1000);
        //var sp = sendDataToServer("http://localhost:3000/autoplan", 'POST', PROPAGATION_FILE_ID);
        //sendDataToServer(serverAddress, 'POST', TEST_COLLECTION_FILE_ID);
        sendDataToServer(serverAddress, 'POST', AUTO_PLAN_FILE_ID);
        
        var s ="";
        for(var i = 0; i < meters.length;i++)
        {
            var latlng = meters[i].getPosition();
            s+= latlng.lat().toString() + " " + latlng.lng().toString() + "\n";

        }
        var s2 = "";
        for(var i = 0; i < poles.length;i++)
        {
            var latlng = poles[i].getPosition();
            s2+= latlng.lat().toString() + " " + latlng.lng().toString() + "\n";

        }

        var x = "wow";



            
            

       // teste();
        //collectInfo();
        //statisticalMatrix();
        //applyPlanning();
        //if (meshEnabled) {
        //   connectViaMesh();
           // meshEnabled = true;
            //markerCluster.clearMarkers();
            //$('#checkRFMesh').button({
            //    icons: {
            //        primary: "ui-icon-check"
            //    }
            //})
        //}



    });
    
    $('#checkRFMesh').button({
        icons: {
            primary: "mesh"
        },
 
        text: false

    }).click(function () {
        $(this).blur();
//        $.blockUI({ fadeIn: 0, message: '<h1><img src="siri2.gif" /> Carregando </h1>' });
        meshEnabled = !meshEnabled;
		sendDrawRequest();
        //$('#check').button.removeClass("ui-state-focus ui-state-hover");
//        $.unblockUI();


    });
	$('#statistic').button({
	icons: {
		primary: "statistic"
	},

	text: false
    }).click(function () {
        $(this).blur();
        sendDataToServer(serverAddress, 'POST', METRIC_FILE_ID);
    });
	$('#upload').button({
	icons: {
		primary: "upload"
	},

	text: false
    }).click(function () {
			$(this).blur();
				$(function() {
					$( "#uploadDialog" ).dialog({
						show: {
						effect: "blind",
						duration: 1000
						},
						hide: {
						effect: "explode",
						duration: 1000
						}
					});
				});     
		});
  $('#submitButton').button({
    text: "Submit"
    
  }).click(function () {
      upload($("#uploadFile").get(0));
    });
    $('#uploadFile').button();



	$('#download').button({
	icons: {
		primary: "download"
	},

	text: false
    }).click(function () {
			$(this).blur();
				$(function() {
					download();
				});     
		});
	
    $('#generator').button({

        text: false

    }).click(function () {
        $(this).blur();
        setInsertionOptions("Generator");
        //generateScenario();
    });
	   
	$("#ZigBee").click(function () {
	    setTechnology(t802_15_4)
	    sendDrawRequest();
	});
	$("#80211a").click(function () {
	    setTechnology(t802_11_a)
	    sendDrawRequest();
	});
	$("#80211g").click(function () {
	    setTechnology(t802_11_g)
	    sendDrawRequest();
	});
	$("#urbanRadio").click(function () {
		  setScenario(Urbano);
	    sendDrawRequest();
	});
	$("#suburbanRadio").click(function () {
	   setScenario(Suburbano);
	    sendDrawRequest();
	});
	$("#ruralRadio").click(function () {
	    setScenario(Rural);
	    sendDrawRequest();
	});
}
function setScenario(value){
  switch(value){
    case Urbano:
       $("#scenario").text("Urban");
       scenario = Urbano;
    break;
    case Suburbano:
     $("#scenario").text("Suburban");
       scenario = Suburban;
    break;
    case Rural:
      $("#scenario").text("Rural");
      scenario = Rural;
    break;
    default:
  }

}
function setTechnology(value){
  switch(value){
    case  t802_15_4:
       $("#technology").text("ZigBee");
       technology = Urbano;
    break;
    case  t802_11_a:
     $("#technology").text("802.11a");
       technology = Suburban;
    break;
    case  t802_11_g:
      $("#technology").text("802.11g");
      technology = Rural;
    break;
    default:
  }
}
function setPower(value){
  TRANSMITTER_POWER = value;
  $("#power").text(value + "dBm");
  $("#slider").slider('value', value);

}   