
function createGrid(){
	var grid = {
		cells: null,
		minX: null,
		minY: null,
		cellSize: null,
		drawnCells: [],

/*  		startGrid: function (v,v2,cS) {
			//Mesma coisa que implementado em C++.
			if (cS <= 0)
				return;
			vUnion = [];
			for (var i = 0; i < v.length; i++)
				vUnion.push(v[i]);
			for (var i = 0; i < v2.length; i++)
				vUnion.push(v2[i]);

			var mindx = this.getMinX(vUnion);
			var mindy = this.getMinY(vUnion);
			var maxdx = this.getMaxX(vUnion);
			var maxdy = this.getMaxY(vUnion);
			var nx = Math.ceil((maxdx - mindx) / cS);
			var ny = Math.ceil((maxdy - mindy) / cS);

			this.minX = mindx;
			this.minY = mindy;
			this.cellSize = cS;
			this.cells = {};
		}, */
		startGridWithPositions: function (v, cS) {
			//Mesma coisa que implementado em C++.
			if (cS <= 0)
				return;

			var mindx = this.getMinX(v);
			var mindy = this.getMinY(v);
			var maxdx = this.getMaxX(v);
			var maxdy = this.getMaxY(v);
			var nx = Math.ceil((maxdx - mindx) / cS);
			var ny = Math.ceil((maxdy - mindy) / cS);

			this.minX = mindx;
			this.minY = mindy;
			this.cellSize = cS;
			this.cells = {};




		}, 
		startGrid: function (cS) {
			//Mesma coisa que implementado em C++.
			if (cS <= 0)
				return;
			this.cellSize = cS;
			this.cells = {};
		},
		putPosition: function(p) {
			if((p.position.lat() < this.minX || p.position.lng() < this.minY) || (this.minX == null || this.minY == null)){ //Significa que entrou um novo cara que não vai ter célula
			//Tem que refazer o grid usando essa nova posição como o mínimo
			//Isso só vai acontecer na hora que o usuário inserir manualmente algum objeto.
				var allElements = this.getAllElements();
				allElements.push(p);
				this.startGridWithPositions(allElements,this.cellSize);
				this.putPositions(allElements);
			}
			else{
				var posX = 0;
				var posY = 0;
				if (p.position.lat() != this.minX)
					posX = Math.ceil((p.position.lat() - this.minX)/this.cellSize)-1;
				if (p.position.lng() !=	this.minY)
					posY = Math.ceil((p.position.lng() -this.minY)/this.cellSize)-1;
				
				//var posXY = {X: posX, Y: posY};
				var posXY = posX + ";" + posY;
				//var cellPos = this.cells[posXY];
				if (posXY in this.cells) {
				   this.cells[posXY].push(p);
				} else {
				   this.cells[posXY] = [];
				   this.cells[posXY].push(p);
				}
			}
			
		},
		putPositions: function(v) {
			for(var i = 0; i < v.length; i++){
				this.putPosition(v[i]);
			}
		},
		getCell: function(reference) {
			var ret = [];
			var posX = -1;
			var posY = -1;
			if (reference.lat() != this.minX)
				posX = Math.ceil((reference.lat() - this.minX) / this.cellSize) - 1;
			if (reference.lng() != this.minY)
				posY = Math.ceil((reference.lng() - this.minY) / this.cellSize) - 1;
			if (posX == -1 || posY == -1)
				 return ret;
			
			//var pair = {X: posX, Y: posY};
			var pair = posX + ";" + posY;
			ret = this.cells[pair];


			return ret;
		},
		getCellsInWindow: function(map) { //Pega todas as células que estão na janela que o usuário está visualizando
			 var bounds = map.getBounds();
			// var sw = bounds.getSouthWest();
			// var ne = bounds.getNorthEast();
			var cellsInWindow = [];
			for (key in this.cells) { //Verificar pra toda as células quais pertencem a essa janela. Se tiver alguma coordenada de seus vértices dentro da janela, então ela pertence.
	        	if (this.cells.hasOwnProperty(key)) {
	        		var res = key.split(";");
	        		var posX = this.minX + (parseFloat(res[0]))*this.cellSize;
	        		var posY = this.minY + (parseFloat(res[1]))*this.cellSize;
	        		var latLng1 = new google.maps.LatLng(posX, posY);
	        		var latLng2 = new google.maps.LatLng(posX+this.cellSize, posY);
	        		var latLng3 = new google.maps.LatLng(posX, posY+this.cellSize);
	        		var latLng4 = new google.maps.LatLng(posX+this.cellSize, posY+this.cellSize);
	        		if (bounds.contains(latLng1) || bounds.contains(latLng2) || bounds.contains(latLng3) || bounds.contains(latLng4)) {
			            cellsInWindow.push(this.cells[key]);    
			        }
	        	}
    		}
			return cellsInWindow;
			

		},
		getAllElements: function() {
			var ret = [];
			for (key in this.cells) {
	        	if (this.cells.hasOwnProperty(key)) {
	        		var aux = this.cells[key];
	        		for(var i = 0; i < aux.length; i++){
	        			ret.push(aux[i]);
	        		}
	        	}
    		}
			return ret;
		},
		getMinX: function (v){

			var dx = v[0].position.lat();
			for (var i = 1; i < v.length; i++)
			{
				if (v[i].position.lat() < dx)
					dx = v[i].position.lat();
			}
			return dx;

		},
		getMinY: function(v){

			var dy = v[0].position.lng();
			for (var i = 1; i < v.length; i++)
			{
				if (v[i].position.lng() < dy)
					dy = v[i].position.lng();
			}
			return dy;
		},
		getMaxX: function(v){

			var dx = v[0].position.lat();
			for (var i = 1; i < v.length; i++)
			{
				if (v[i].position.lat() > dx)
					dx = v[i].position.lat();
			}
			return dx;

		},
		getMaxY: function(v){
		
			var dy = v[0].position.lng();
			for (var i = 1; i < v.length; i++)
			{
				if (v[i].position.lng() > dy)
					dy = v[i].position.lng();
			}
			return dy;
		},
		getNumberOfCells: function () {
			var size = 0;
			for (key in this.cells) {
				size++;
			}
			return size;
		},
		drawCells: function () {
			for(var i = 0; i < this.drawnCells.length; i++){
				this.drawnCells[i].setVisible(false);
			}
			for (key in this.cells) {
					var res = key.split(";");
	        		var posX = this.minX + (parseFloat(res[0]))*this.cellSize;
	        		var posY = this.minY + (parseFloat(res[1]))*this.cellSize;
				var rectangle = new google.maps.Rectangle({
					strokeColor: '#FF0000',
					strokeOpacity: 0.8,
					strokeWeight: 2,
					fillColor: '#FF0000',
					fillOpacity: 0.35,
					map: map,
					bounds: new google.maps.LatLngBounds(
					  new google.maps.LatLng(posX, posY),
					  new google.maps.LatLng(posX+this.cellSize, posY+this.cellSize))
				  });
				  this.drawnCells.push(rectangle);
			}
		}
	}
	return grid;
}

