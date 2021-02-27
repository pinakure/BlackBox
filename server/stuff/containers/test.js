this.initialized = false;
this.worldLen = 1000;
this.world = 0;

this.x = 15;
this.y = 10;
this.z = 0;

this.color = 1 + parseInt(Math.random()*15);


function Cell(x, y, z){
	this.x = x;
	this.y = y;
	this.z = z;
	
	this.color = parseInt(Math.random()*16);
	
	this.north = 0;
	this.south = 0;
	this.west = 0;
	this.east = 0;
	this.up = 0;
	this.down = 0;
	
	this.setNorth = function(cell){
		cell.x = this.x;
		cell.y = this.y-1;
		cell.z = this.z;
		this.north = cell;
	}
	
	this.setSouth = function(cell){
		cell.x = this.x;
		cell.y = this.y+1;
		cell.z = this.z;
		this.south = cell;
	}
	
	this.setEast = function(cell){
		cell.x = this.x+1;
		cell.y = this.y;
		cell.z = this.z;
		this.east = cell;
	}
	
	this.setWest = function(cell){
		cell.x = this.x-1;
		cell.y = this.y;
		cell.z = this.z;
		this.west = cell;
	}
	
	this.setUp = function(cell){
		cell.x = this.x;
		cell.y = this.y;
		cell.z = this.z-1;
		this.up = cell;
	}
	
	this.setDown = function(cell){
		cell.x = this.x;
		cell.y = this.y;
		cell.z = this.z+1;
		this.down = cell;
	}
	
	this.draw = function(x,y,color){
		vpu.putBg(this.x+x, this.y+y, 0x100, color);
		if(this.south == 0){
			vpu.putBg(this.x+x, this.y+y+1, 0x101, color);		
		}
	}
}

this.initialize = function(){
	this.initialized = true;
	
	var cell = new Cell(0,0,0);
	this.world = cell;
	
	for(i = 0; i < this.worldLen; i++){
		switch(parseInt(Math.random()*6)){
			case 0: cell.setNorth(new Cell(0,0,0)); cell = cell.north; continue;
			case 1: cell.setSouth(new Cell(0,0,0)); cell = cell.south; continue;
			case 2: cell.setEast(new Cell(0,0,0)); cell = cell.east; continue;
			case 3: cell.setWest(new Cell(0,0,0)); cell = cell.west; continue;
			case 4: cell.setUp(new Cell(0,0,0)); cell = cell.up; continue;
			case 5: cell.setDown(new Cell(0,0,0)); cell = cell.down; continue;
		}
	}
};

this.drawCell = function(cell){
	if(cell.north != 0)this.drawCell(cell.north);
	if(cell.south != 0)this.drawCell(cell.south);
	if(cell.east != 0)this.drawCell(cell.east);
	if(cell.west != 0)this.drawCell(cell.west);
	if(cell.up!= 0)this.drawCell(cell.up);
	if(cell.down!= 0)this.drawCell(cell.down);
	cell.draw(this.x, this.y, this.color);
}


this.logic = function() {
	if(!this.initialized)this.initialize();
	
	this.drawCell(this.world);
	
	this.z++;
};