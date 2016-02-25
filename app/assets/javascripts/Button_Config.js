//teste
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
            var toInsert;
            if (type == "DAP")
                toInsert = createDAP();        
            if (type == "Meter")
                toInsert = createMeter();
            if (type == "Pole") 
                toInsert  = createPole();
            toInsert .placeOnMap(event.latLng.lat(), event.latLng.lng());
            sendDrawRequest();
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
function setOption(opt){
   switch(opt){
    case "view":
      setOpMode("View");
      map.setOptions({ draggableCursor: "url(https://maps.gstatic.com/mapfiles/openhand_8_8.cur), auto" });
      drawingManager.setDrawingMode(null);
      break;
    case "dap":
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("DAP")
        map.setOptions({ draggableCursor: "url("+document.URL+"/assets/cursors/dapcursor.cur), auto" });
        drawingManager.setDrawingMode(null);
        break;
    case "meter":
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("Meter");
        map.setOptions({ draggableCursor: "url("+document.URL+"/assets/cursors/metercursor.cur), default" });
        drawingManager.setDrawingMode(null);
        break;
    case "pole":
        setOpMode("Insertion");
        setInfoWindowNull();
        setInsertionOptions("Pole");
        map.setOptions({ draggableCursor: "url("+document.URL+"/assets/cursors/polecursor.cur), default" });
        drawingManager.setDrawingMode(null);
        break;
    case "eraser":
        setOpMode("Removal");
        map.setOptions({ draggableCursor: "url("+document.URL+"/assets/cursors/removecursor.cur), default" });
        setInfoWindowNull();
        drawingManager.setDrawingMode(null);
        break;
    case "eraserSelection":
        drawingManager.setDrawingMode(google.maps.drawing.OverlayType.RECTANGLE);
        removeSelectionMode = REMOVE_ALL;
        setInfoWindowNull();
        break;
    case "dapEraserSelection":
        drawingManager.setDrawingMode(google.maps.drawing.OverlayType.RECTANGLE);
        removeSelectionMode = REMOVE_DAPS;
        setInfoWindowNull();
        break;
    case "meterEraserSelection":
        drawingManager.setDrawingMode(google.maps.drawing.OverlayType.RECTANGLE);
        removeSelectionMode = REMOVE_METERS;
        setInfoWindowNull();
        break;
    case "poleEraserSelection":
        drawingManager.setDrawingMode(google.maps.drawing.OverlayType.RECTANGLE);
        removeSelectionMode = REMOVE_POLES;
        setInfoWindowNull();
        break;
    default:
    break;

  }
}

function setButtons()
{

  $('#helpmenu').attr('src', document.URL+'/assets/helpmenu.png');
	$('option[value=view]').attr('data-style', 'background-image: url('+document.URL+'/assets/viewicon.png);');
	$('option[value=dap]').attr('data-style', 'background-image: url('+document.URL+'/assets/dapicon.png);');
	$('option[value=meter]').attr('data-style', 'background-image: url('+document.URL+'/assets/metericon.png);');
	$('option[value=pole]').attr('data-style', 'background-image: url('+document.URL+'/assets/poleicon.png);');
	$('option[value=eraser]').attr('data-style', 'background-image: url('+document.URL+'/assets/removecursor.png);');
	$('option[value=eraserSelection]').attr('data-style', 'background-image: url('+document.URL+'/assets/removeselectionicon.png);');
	$('option[value=dapEraserSelection]').attr('data-style', 'background-image: url('+document.URL+'/assets/dapremoveselectionicon.png);');
	$('option[value=meterEraserSelection]').attr('data-style', 'background-image: url('+document.URL+'/assets/meterremoveselectionicon.png);');
	$('option[value=poleEraserSelection]').attr('data-style', 'background-image: url('+document.URL+'/assets/poleremoveselectionicon.png);');
    $.widget("custom.TFOiconSelectImg", $.ui.selectmenu, {
        _renderItem: function (ul, item) {
            var li = $("<li>", { html: item.element.html() });
            var attr = item.element.attr("data-style");
            if (typeof attr !== typeof undefined && attr !== false) {
                $("<span>", {
                    style: item.element.attr("data-style"),
                    "class": "ui-icon TFOOptlstFiltreImg"
                }).appendTo(li);
            }
            return li.appendTo(ul);
        }
        });

        $("#mouseOptions")
          .TFOiconSelectImg({
              create: function (event, ui) {
                  var widget = $(this).TFOiconSelectImg("widget");
                  $span = $('<span id="' + this.id + 'ImgSelected" class="TFOSizeImgSelected"> ').html("&nbsp;").appendTo(widget);
                  $span.attr("style", $(this).children(":first").data("style"));
              },
              change: function (event, ui) {
                  $("#" + this.id + 'ImgSelected').attr("style", ui.item.element.data("style"));
                  var opt = ui.item.value
                  setOption(opt);
              }
          }).TFOiconSelectImg("menuWidget").addClass("ui-menu-icons customicons");



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
	       value: -0,
	       min: -0,
	       max: 30,
	       step: 1,
	       slide: function (event, ui) {
	           $("#power").text(ui.value + "dBm");
	       }
	   });
	   $("#slider").css({ opacity: 0.5 });
	   $("#slider").slider({
	       stop: function (event, ui) {
	           TRANSMITTER_POWER = ui.value;
	           sendDrawRequest();
	       }
	   });
     $("#RedundancySlider").slider({
         value: 1,
         min: 1,
         max: 5,
         step: 1,
         slide: function (event, ui) {
             $("#Redundancy").text(ui.value);
         }
     });
     $("#limitSlider").slider({
         stop: function (event, ui) {
             LIMIT = ui.value;
         }
     });
	 $("#limitSlider").slider({
         value: 100,
         min: 1,
         max: 100,
         step: 1,
         slide: function (event, ui) {
             $("#limit").text(ui.value);
         }
     });
     $("#RedundancySlider").slider({
         stop: function (event, ui) {
             REDUNDANCY = ui.value;
         }
     });

	
	 //	$("#meshSliderDiv").slider({
     //    value: 0,
     //    min: 1,
     //    max: 5,
     //    step: 1,
     //    slide: function (event, ui) {
     //         $("#mesh").text(ui.value + " Saltos");
     //    }
     //}); 
     //$("#meshSliderDiv").slider({
     //    stop: function (event, ui) {
     //       setMeshHops(ui.value);
     //       sendDrawRequest();
     //    }
     //});

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

   


    $('#settings').button({
        icons: {
            primary: "settings"
        },
        text: false

    }).click(function () {       
        $("this").blur(); 

         $(function() {
          $( "#settingsDialog" ).dialog({
            show: {
            effect: "drop",
            duration: 500
            },
            hide: {
            effect: "drop",
            duration: 500
            },
            resizable: false,
            width: 454
            
          });
        });            
    });
    $('#help').button({
        icons: {
            primary: "help"
        },
        text: false

    }).click(function () {       
        $("this").blur(); 

         $(function() {
          $( "#helpDialog" ).dialog({
            show: {
            effect: "drop",
            duration: 500
            },
            hide: {
            effect: "drop",
            duration: 500
            },
            resizable: false,
            width: 600
            
          });
        });            
    });
     $('#rangeview').button({
        icons: {
            primary: "rangeview"
        },
        text: false

    }).click(function () {       
        $("this").blur();    
        toggleRangeView();
         

    });
     setAutoplanningButton();
     setMeshButton();
     setHeatmapButton();
   

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
					effect: "drop",
					duration: 500
					},
					hide: {
					effect: "drop",
					duration: 500
					},
					resizable: false,
					width: 460,
                    height:120,
					
				});
			});     
		});
    $('#submitButton').button({
    text: "Submit"
    }).click(function () {
		if($("#resetOnUpload").is(':checked')){
			while(meters.length > 0){
				meters[0].remove();
			}
			while(daps.length > 0){
				daps[0].remove();
			}
			while(poles.length > 0){
				poles[0].remove();
			}		
        while(heatmapPoints.length > 0)
            heatmapPoints[0].remove();       
        setHeatmap();
		}
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
	if(!isNaN(value)){
	  switch(value){
		case Urbano:
		   $("#scenario").text("Urbano");
		   scenario = Urbano;
		break;
		case Suburbano:
		 $("#scenario").text("Suburbano");
		   scenario = Suburbano;
		break;
		case Rural:
		  $("#scenario").text("Rural");
		  scenario = Rural;
		break;
		default:
	  }
	}
}
function setTechnology(value){
	if(!isNaN(value)){
	  switch(value){
		case  t802_15_4:
		   $("#technology").text("ZigBee");
		   technology = t802_15_4;
       $( "#slider" ).slider( "option", "min", -3 );
       $( "#slider" ).slider( "option", "max",  3 );
       setPower(0);
		break;
		case  t802_11_a:
		 $("#technology").text("802.11a");
		   technology = t802_11_a;
		break;
		case  t802_11_g:
		  $("#technology").text("802.11g");
		  technology = t802_11_g;
      $( "#slider" ).slider( "option", "min", 18 );
      $( "#slider" ).slider( "option", "max",  25 );
      setPower(20);

		break;
		default:
	  }
  }
}
function setPower(value){
	if(!isNaN(value) && value >= 0 && value <= 30){
	  TRANSMITTER_POWER = value;
	  $("#power").text(value + "dBm");
	  $("#slider").slider('value', value);
	}
}  
function setHRX(value){
	if(!isNaN(value) && value >= 0 && value <= 10){
	  H_RX = value;
	  $("#hrx").text(value + " Metros");
	  $("#hrxSlider").slider('value', value);
	}
}  
function setHTX(value){
	if(!isNaN(value) && value >= 0 && value <= 30){
	  H_TX = value;
	  $("#htx").text(value + " Metros");
	  $("#htxSlider").slider('value', value);
	}
}  
function setMeshHops(value){
	if(!isNaN(value) && value >= 1 && value <= 5){
	  meshMaxJumps = value;
	  $("#mesh").text(value + " Saltos");
	  $("#meshSlider").slider('value', value);
	}
}   
function setMeshActivation(value){
	if(value == 1){
		meshEnabled = 1;
		$('#meshButton').prop('checked', true);
	}
	if(value == 0){
		$('#meshButton').prop('checked', false);
		meshEnabled = 0;
	}
	$('#meshButton').button("refresh");
}
var bitRates = [1,2,5.5,6,9,11,12,18,24,36,54 ];
function setBitRate(value){
  if(!isNaN(value)){
    BIT_RATE = value;
    var pos;
    for(var i = 0;i<bitRates.length;i++){
      if(bitRates[i]==value){
        pos = i;
        break;
      }
    }
    $("#rate").text(bitRates[pos] + "Mbps");
    $("#rateSlider").slider('value', pos);
  }
}
function toggleHeatgrid(){
	drawHeatmap = !drawHeatmap;		
	if (drawHeatmap) {
	    $("#rangeview").removeAttr('checked');
	    $("#rangeview").button("refresh");
	    DrawHeatmap();
	    EraseRangeView();
	    drawRangeView = false;
	    DrawLines();
	    makeHeatmapPointsVisible();

	}
	else {
	    EraseHeatmap();
	    makeHeatmapPointsInvisible();
	}
	    
}
function toggleAutoPlanningOverwrite()
{
    autoPlanOverwrite = !autoPlanOverwrite;
}
function toggleAutoPlanningHeatmapOverwrite() {
    autoPlanHeatmap = !autoPlanHeatmap;
}
function toggleRangeView(){
  drawRangeView = !drawRangeView;

  if(drawRangeView) {
  	$("#checkHeatmap").removeAttr('checked');
  	$("#checkHeatmap").button("refresh");
    sendDataToServer(serverAddress, 'POST', GET_RANGE_FILE_ID);  
    EraseHeatmap();
    EraseLines();
  	drawHeatmap = false; 
  }
  else{
      DrawLines();
      EraseRangeView();
  }
}
var drawEnabled = true;
function setDrawOption(option) {
    drawEnabled = option;
    if (!option) {
        EraseLines();
        EraseHeatmap();
        EraseRangeView();
        makeHeatmapPointsInvisible();
    }
    else {
        DrawLines();
        DrawHeatmap();
        DrawRangeView();
        makeHeatmapPointsVisible()
    }

}
function DrawLines(){
    if (drawEnabled && !drawRangeView) {
        for (var i = 0; i < lines.length; i++)
            lines[i].setMap(map);
    }
}
function EraseLines(){
    for (var i = 0; i < lines.length; i++)
        lines[i].setMap(null);
}
function DrawHeatmap() {
    if (drawEnabled && drawHeatmap) {
        if (heatmapPolygon != null)
            heatmapPolygon.setMap(map);
    }
}
function EraseHeatmap() {
    if (heatmapPolygon != null)
        heatmapPolygon.setMap(null);
}
function DrawRangeView() {
    if (drawEnabled && drawRangeView) {
        if (coveragePolygon != null)
            coveragePolygon.setMap(map);
    }
}
function EraseRangeView() {
    if (coveragePolygon != null)
        coveragePolygon.setMap(null);
}
function makeHeatmapPointsVisible() {
    if (drawEnabled && drawHeatmapPoints && drawHeatmap) {
        for (var i = 0; i < heatmapPoints.length; i++) {
            heatmapPoints[i].setMap(map);
        }
    }
}
function makeHeatmapPointsInvisible() {
        for (var i = 0; i < heatmapPoints.length; i++) {
            heatmapPoints[i].setMap(null);
        }
}
function toggleHeatmapPointVisibility(){
    drawHeatmapPoints = !drawHeatmapPoints;
    if (drawHeatmapPoints)
        makeHeatmapPointsVisible();
    else
        makeHeatmapPointsInvisible();
}
function setSignalRadius(val){
    validCellRadius = val;
    setHeatmap();
    DrawHeatmap();
}
function setAutoplanningButton() {
    $('#autoPlanning').button().click(function () {
        $(this).blur();
        sendDataToServer(serverAddress, 'POST', AUTO_PLAN_FILE_ID);
    });
    $('#autoPlanning').mouseenter(function () {
        clearTimeout($(this).data('timeoutId'));
        $('#autoPlanningCheckboxDiv').show();
        $('#autoPlanningHeatmapCheckboxDiv').show();
        $('#RedundancySliderDiv').show();
    }).mouseleave(function () {
        var someElement = $(this),
        timeoutId = setTimeout(function () {
            $('#autoPlanningCheckboxDiv').hide();
            $('#autoPlanningHeatmapCheckboxDiv').hide();
            $('#RedundancySliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#autoPlanningCheckboxDiv').mouseenter(function () {
        clearTimeout($('#autoPlanning').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#autoPlanning'),
        timeoutId = setTimeout(function () {
            $('#autoPlanningCheckboxDiv').hide();
            $('#autoPlanningHeatmapCheckboxDiv').hide();
            $('#RedundancySliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#autoPlanningCheckbox').click(function () {
        toggleAutoPlanningOverwrite();
    });

    $('#autoPlanningHeatmapCheckboxDiv').mouseenter(function () {
        clearTimeout($('#autoPlanning').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#autoPlanning'),
        timeoutId = setTimeout(function () {
            $('#autoPlanningCheckboxDiv').hide();
            $('#autoPlanningHeatmapCheckboxDiv').hide();
            $('#RedundancySliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#autoPlanningHeatmapCheckbox').click(function () {
        toggleAutoPlanningHeatmapOverwrite();
    });

    $('#RedundancySliderDiv').mouseenter(function () {
        clearTimeout($('#autoPlanning').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#autoPlanning'),
        timeoutId = setTimeout(function () {
            $('#autoPlanningCheckboxDiv').hide();
            $('#autoPlanningHeatmapCheckboxDiv').hide();
            $('#RedundancySliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });

}
function setMeshButton() {
    $('#meshButton').button({
        icons: {
            primary: "mesh"
        },
        text: true
    });
    $('#meshButton').mouseenter(function () {
        clearTimeout($(this).data('timeoutId'));
        $('#meshSliderDiv').show();
    }).mouseleave(function () {
        var someElement = $(this),
        timeoutId = setTimeout(function () {
            $('#meshSliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#meshSliderDiv').mouseenter(function () {
        clearTimeout($('#meshButton').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#meshButton'),
        timeoutId = setTimeout(function () {
            $('#meshSliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
        //set the timeoutId, allowing us to clear this trigger if the mouse comes back over

    });
    $("#meshSlider").slider({
        value: 0,
        min: 1,
        max: 5,
        step: 1,
        slide: function (event, ui) {
            $("#mesh").text(ui.value + " Saltos");
        }
    });
    $("#meshSlider").slider({
        stop: function (event, ui) {
            setMeshHops(ui.value);
            sendDrawRequest();
        }
    });

}
function setHeatmapButton() {
    $('#checkHeatmap').button({
        icons: {
            primary: "heatmap"
        },
        text: false

    }).click(function () {
        $(this).blur();
        toggleHeatgrid();
    });
    $('#heatmapLabel').mouseenter(function () {
        clearTimeout($(this).data('timeoutId'));
        $('#heatmapCheckboxDiv').show();
        $('#SignalRadiusSliderDiv').show();
    }).mouseleave(function () {
        var someElement = $(this),
        timeoutId = setTimeout(function () {
            $('#heatmapCheckboxDiv').hide();
            $('#SignalRadiusSliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#heatmapCheckboxDiv').mouseenter(function () {
        clearTimeout($('#heatmapLabel').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#heatmapLabel'),
        timeoutId = setTimeout(function () {
            $('#heatmapCheckboxDiv').hide();
            $('#SignalRadiusSliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#SignalRadiusSliderDiv').mouseenter(function () {
        clearTimeout($('#heatmapLabel').data('timeoutId'));
    }).mouseleave(function () {
        var someElement = $('#heatmapLabel'),
        timeoutId = setTimeout(function () {
            $('#heatmapCheckboxDiv').hide();
            $('#SignalRadiusSliderDiv').hide();
        }, 1);
        someElement.data('timeoutId', timeoutId);
    });
    $('#heatmapCheckbox').click(function () {
        toggleHeatmapPointVisibility();
    });
    $("#SignalRadiusSlider").slider({
        value: validCellRadius,
        min: 1,
        max: 50,
        step: 1,
        slide: function (event, ui) {
            $("#SignalRadius").text(ui.value + " metros");
            
        }
    });
    $("#SignalRadiusSlider").slider({
        stop: function (event, ui) {
            setSignalRadius(ui.value);
        }
    });
}
function setValidCellRadius(val) {
    $("#SignalRadius").text(val + " metros");
    validCellRadius = val;
}


  