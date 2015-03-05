
function createGrid(){
	var grid = {
		cells: null,
		minX: null,
		minY: null,
		cellSize: null,

		startGrid: function (v,v2,cS) {
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
		},
		startGrid: function (v, cS) {
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
		putPosition: function(p) {
			var posX = 0;
			var posY = 0;
			if (p.position.lat() != this.minX)
				posX = Math.ceil((p.position.lat() - this.minX)/this.cellSize)-1;
			if (p.position.lng() !=	this.minY)
				posY = Math.ceil((p.position.lng() -this.minY)/this.cellSize)-1;
			
			var posXY = {X: posX, Y: posY};
			var cellPos = this.cells[posXY];
			if (cellPos in this.cells) {
			   this.cells[posXY].push(p);
			} else {
			   this.cells[posXY] = [];
			   this.cells[posXY].push(p);
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
			
			var pair = {X: posX, Y: posY};
			ret = this.cells[pair];


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
		}

	}
	return grid;
}

