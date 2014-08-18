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
    $(document).tooltip();
		$("#DisplayXML").click(function() {
			showNodesXml();
			setInfoWindowNull();
		});
		$("#Technology").click(function() {
			setInfoWindowNull();
		});
		$("#Scenario").click(function() {
			setInfoWindowNull();
		});
		$("#help").click(function () {
		    $("#dialog").dialog("open");
		    //$.blockUI({
		    //    message: $('#displayBox'),
		    //    onOverlayClick: $.unblockUI,
		    //    css: {
		    //        top: ($(window).height() - 451) / 2 + 'px',
		    //        left: ($(window).width() - 654) / 2 + 'px',
		    //        width: '654px',
            //        height: '451px'
		    //    }
		        
		    //});
		});
		$("#xml").click(function () {
		    showNodesKml();
		    showPolesXml();
		    $("#xmltextnodes").dialog("open");
		    $("#xmltextpoles").dialog("open");
		    //$.blockUI({
		    //    message: $('#displayBox'),
		    //    onOverlayClick: $.unblockUI,
		    //    css: {
		    //        top: ($(window).height() - 451) / 2 + 'px',
		    //        left: ($(window).width() - 654) / 2 + 'px',
		    //        width: '654px',
		    //        height: '451px'
		    //    }

		    //});
		});

		
		var radioSize = 155;
	   $('#insertionBackground').buttonset().find('label').width(radioSize);
	   $('#powerbackground').buttonset().find('label').width(76);
	   $('#scenarioBackground').buttonset().find('label').width(radioSize);
	   $('#technologyBackground').buttonset().find('label').width(radioSize);
	  // $('#configMessage').html(getConfigurations());
	   //$('#check').click(function (ev) { ev.preventDefault(); });
    // $('#check').unbind('mouseout keyup mouseup hover');
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
	           refresh();
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
        $.blockUI({  message: '<h1><img src="/assets/siri2.gif" /> Carregando </h1>' });
        // autoPlanningGrasp();
        setTimeout('applyPlanning()', 1000);
        if (meshEnabled) {
            connectViaMesh();
           // meshEnabled = true;
            //markerCluster.clearMarkers();
            //$('#checkRFMesh').button({
            //    icons: {
            //        primary: "ui-icon-check"
            //    }
            //})
        }



    });
    
    $('#checkRFMesh').button({
        icons: {
            primary: "mesh"
        },
 
        text: false

    }).click(function () {
        $(this).blur();
        $.blockUI({ fadeIn: 0, message: '<h1><img src="siri2.gif" /> Carregando </h1>' });
        if (meshEnabled) {
            resetMesh();
            //$(this).button({
            //    icons: {
            //        primary: "ui-icon-closethick"
            //    }
            //})
        }

        else {
            //setRFMesh();
            connectViaMesh();         
            
            //markerCluster.clearMarkers();
            //$(this).button({
            //    icons: {
            //        primary: "ui-icon-check"
            //    }
            //})
        }
        meshEnabled = !meshEnabled;

        //$('#check').button.removeClass("ui-state-focus ui-state-hover");
        $.unblockUI();


    });
    $('#checkCluster').button({
        icons: {
            primary: "ui-icon-check"
        }

    }).click(function () {
        $(this).blur();
        $.blockUI({ fadeIn: 0, message: '<h1><img src="siri2.gif" /> Carregando </h1>' });
        enableMarkerClusterer = !enableMarkerClusterer;
        if (enableMarkerClusterer) {
            clusterMap();
            $(this).button({
                icons: {
                    primary: "ui-icon-check"
                }
            })
        }

        else {
            unclusterMap();
            //markerCluster.clearMarkers();
            $(this).button({
                icons: {
                    primary: "ui-icon-closethick"
                }
            })
        }

        //$('#check').button.removeClass("ui-state-focus ui-state-hover");
        $.unblockUI();


    });
	  // $('#RFMesh').button();
	   $("#dialog").dialog({
	       autoOpen: false,
	       show: {
	           effect: "scale",
	           duration: 1000
	       },
	       hide: {
	           effect: "scale",
	           duration: 1000
	       },
	       width: 680
	   });
	   $("#xmltextnodes").dialog({
	       autoOpen: false,
	       show: {
	           effect: "scale",
	           duration: 1000
	       },
	       hide: {
	           effect: "scale",
	           duration: 1000
	       },
	       width: 680,
           height: 500
	   });
	   $("#xmltextpoles").dialog({
	       autoOpen: false,
	       show: {
	           effect: "scale",
	           duration: 1000
	       },
	       hide: {
	           effect: "scale",
	           duration: 1000
	       },
	       width: 680,
	       height: 500
	   });

       
	        $("#menu").menu();
       
	   $( "#radioBackground" ).buttonset().find('label').width(122);

		
	   //$('#dapRadio').button({
	   //    icons: { primary: 'ui-icon-dap', secondary: null }
	   //});
	   
		$("#ZigBee").click(function () {
		    $("#technology").text("ZigBee");
		    technology = "802_15_4";
		    BIT_RATE = 1/4;
		    refresh();
		});
		$("#80211a").click(function () {
		    $("#technology").text("802.11a");
		    technology = "802_11_a";
		    BIT_RATE = 6;
		    refresh();
		});
		$("#80211g").click(function () {
		    $("#technology").text("802.11g");
		    technology = "802_11_g";
		    BIT_RATE = 6;
		    refresh();
		});
		$("#urbanRadio").click(function () {
		    $("#scenario").text("Urbano");
		    scenario = "Urbano";
		    refresh();
		});
		$("#suburbanRadio").click(function () {
		    $("#scenario").text("Suburbano");
		    scenario = "Suburbano";
		    refresh();
		});
		$("#ruralRadio").click(function () {
		    $("#scenario").text("Rural");
		    scenario = "Rural";
		    refresh();
		});
		
		//$("#metropolitanRadio").click(function() 
		//{
		//	setScenario("Metropolitan")

		//});
		//$("#urbanRadio").click(function()
		//{
		//	setScenario("Urban")

		//});
		//$("#ruralRadio").click(function() 
		//{
		//	setScenario("Rural")
		//	$('#configMessage').html(getConfigurations());
		//});

		
		//$("#ZigBee").click(function() 
		//{
		//	setCurrentTech("ZigBee");
		//	setInsertionOptions("DAP")
		//	setDapsToTechnology();
		//	$('#configMessage').html(getConfigurations());
			
		//});
		//$("#80211").click(function()
		//{		
		//	setCurrentTech("w80211");
		//	setInsertionOptions("DAP")
		//	setDapsToTechnology();
		//	$('#configMessage').html(getConfigurations());
		//});

		//$("#z0dbm").click(function() 
		//{
		//	setdbm("dbm0");
		//	$('#configMessage').html(getConfigurations());
		//});
		//$("#z6dbm").click(function() 
		//{
		//	setdbm("dbm6");
		//	$('#configMessage').html(getConfigurations());
		//});
		//$("#z12dbm").click(function() 
		//{
		//	setdbm("dbm12");
		//	$('#configMessage').html(getConfigurations());
		//});
		//$("#z18dbm").click(function() 
		//{
		//	setdbm("dbm18");
		//	$('#configMessage').html(getConfigurations());
		//});
		//$("#z24dbm").click(function() 
		//{
		//	setdbm("dbm24");
		//	$('#configMessage').html(getConfigurations());
		//});
		//$("#z30dbm").click(function() 
		//{
		//	setdbm("dbm30");
		//	$('#configMessage').html(getConfigurations());
		//});		
		
			
	
		
	
	$("#lineRadio").click(function()
	{
		setRadioMode("Line");
		setLinesVisible();
		setCirclesInvisible();
	});
	$("#radiusRadio").click(function()
	{
		setRadioMode("Radius");
		setLinesInvisible();
		setCirclesVisible();
	});
}
