/*--------------------------------------------------------------------------------------------------------------------------------
     __      _______  _    _    _____ _               
	 \ \    / /  __ \| |  | |  / ____| |              
	  \ \  / /| |__) | |  | | | |    | | __ _ ___ ___ 
	   \ \/ / |  ___/| |  | | | |    | |/ _` / __/ __|
		\  /  | |    | |__| | | |____| | (_| \__ \__ \
		 \/   |_|     \____/   \_____|_|\__,_|___/___/		
--------------------------------------------------------------------------------------------------------------------------------*/
function Videodata(width, height, layercount){
	this.layerlength = width*height;
	this.layercount = layercount;
	this.width = width;
	this.height = height;
	this.maps = [];	
	
	// Allocate data
	try {
		for(l = 0; l < this.layercount; l++){
			
			this.maps[l] = [];
		
			for(i = 0; i < this.layerlength; i++){
				this.maps[l][i] = 0;
			}
		}
	} catch(e){ error("VIDEODATA: "+e.message); }
}

function Vpu(){
	this.count = 0;
	this.variator = 0;
	
	this.loadedTilesets = 0; // count loaded tileset IMGs TODO: Rename to tilesetCount
	this.data0;
	this.data1 = [];//since bg1 and bg2 can be multicolored, they have colored versions of themselves instead of being a single instance
	this.data2 = [];

	this.mapsize = (({$width}/{$tile_width})*({$height}/{$tile_height}));

	this.redrawFlag = false;
	this.ctx = 0;

	this.enableRandomizer = false;
	
	// Tilesets 
	this.mapData0_0 = [];
	this.mapData0_1 = [];
	this.mapData0_2 = [];
	
	// Screen buffer
	this.map = new Videodata({$width}/{$tile_width},{$height}/{$tile_height},3);
	this.map.pals = [];
	this.map.pals[1] = [];
	this.map.pals[2] = [];

	this.fps = 0;
	this.cursorX = 0;
	this.cursorY = 0;
	this.cursorP = 0; //(cursorY * {$width * $tile_width}) + cursorX;
	this.color = 0;
	this.paused = true;
	this.width = {$width / $tile_width};
	this.height = {$height / $tile_height};
	this.console = 0;
	this.system = 0;
	
	
	{if $fire_enable}
	this.postProcessor = this.postProcessor_fire;
	{else}
	this.postProcessor = this.postProcessor_none;
	{/if}
}

Vpu.prototype.init = function(){
	
	// Create system object, responsible, amongst other things, of the GUI elements behavior
	this.system = new System();
	
	//// Load the map, set resolution 8 times smaller (tile indexes)
	//this.initMapMemory(this.width, this.height);	// Optimizable (division -> shift)
		
	// Get references to used HTML elements
	var c = document.getElementById("canvas");
	this.ctx = c.getContext("2d");
		
	// Get tileset IMG container references
	this.data0 = document.getElementById("tileset_1");
	
	// For layer #3, we must have 16 versions, one for each color in the palette
	
	for(i=0; i<16; i++){
		this.data1[i] = document.getElementById("tileset_2_"+i.toString());
		this.data2[i] = document.getElementById("tileset_3_"+i.toString());
	}
	this.resume();	
}

Vpu.prototype.clean = function(){
	var i=0;
	for(var py=0;py < this.height; py++){
		for(var px=0; px < this.width; px++){
			this.map.maps[0][i] = 0x0; 
			this.map.maps[1][i] = 0x0;
			this.map.maps[2][i] = 0x100;
			i++;
		}
	
	}
	//this.ctx.drawImage(vpu.data0,0, 0,{$tile_width},{$tile_height},0,0,{$width},{$height});
}

Vpu.prototype.putBg = function(x, y, v, c){
	this.map.maps[0][ ( y * this.width ) + x ] = v;
};

Vpu.prototype.putHud = function(x, y, v, c){ 
	this.map.maps[1][ ( y * this.width ) + x ] = (v&0x0fff);
	this.map.pals[1][ ( y * this.width ) + x ] = c;
};
Vpu.prototype.putText = function(x, y, v, c){ 	
	this.map.maps[2][ ( y * this.width ) + x ] = (v&0x0fff);
	this.map.pals[2][ ( y * this.width ) + x ] = c;
};


Vpu.prototype.pause = function(){
	if(this.paused)return;
	this.paused = true;	
	clearInterval(redraw);
};

Vpu.prototype.resume = function(){
	if(!this.paused)return;
	this.paused = false;
	setInterval(redraw, 1000/{$fps});
};
	
Vpu.prototype.initMapMemory = function(x, y){
	var i = 0;
	while(i<(x*y)){
		this.map.maps[0][i] = 0x0; 
		this.map.maps[1][i] = 0x0;
		this.map.maps[2][i] = 0x100;
		this.map.pals[1][i] = 0x6;
		this.map.pals[2][i] = 0x6;
		i++;
	}
}

Vpu.prototype.randomize = function(){
	this.map.maps[0][parseInt(Math.random() * this.mapsize)] = parseInt(Math.random()*10);
	this.map.maps[1][parseInt(Math.random() * this.mapsize)] = parseInt(Math.random()*512);
	this.map.maps[2][parseInt(Math.random() * this.mapsize)] = parseInt(Math.random()*1024);		
}

Vpu.prototype.getR = function(data,x, y){ return data[( ((y*{$width})+x) <<2)]; }
Vpu.prototype.getG = function(data,x, y){ return data[( ((y*{$width})+x) <<2)+1]; }
Vpu.prototype.getB = function(data,x, y){ return data[( ((y*{$width})+x) <<2)+2]; }
Vpu.prototype.getA = function(data,x, y){ return data[( ((y*{$width})+x) <<2)+3]; }

Vpu.prototype.postProcessor_none = function(){
	// nop
}

Vpu.prototype.postProcessor_fire = function(){
	// Quick jsFire by pinakure
	// M4Geek, October 2015
	var pix = 0;
	var r,g,b;
	var px,py;
	var d = this.ctx.getImageData(0, 0, {$width}, {$height});
	var data = d.data;

	for(py=0;py < {$height}; py++){
		for(px=0; px < {$width}; px++){
			var rand = parseInt(Math.random()*1024);
			if(rand%2==0){
				if(this.variator==0)data[(pix)+0] = (this.getR(data,px, py) + (Math.random()*16) + this.getR(data,px, py+1) + this.getR(data,px+1, py))*{$fire_coefficient};
				if(this.variator==1)data[(pix)+1] = (this.getG(data,px, py) + (Math.random()*16) + this.getG(data,px, py+1) + this.getG(data,px+1, py))*{$fire_coefficient};
				if(this.variator==2)data[(pix)+2] = (this.getB(data,px, py) + (Math.random()*16) + this.getB(data,px, py+1) + this.getB(data,px+1, py))*{$fire_coefficient};
			} else {
				if(this.variator==0)data[(pix)+0] = (this.getR(data,px, py) + (Math.random()*16) + this.getR(data,px, py+1) + this.getR(data,px-1, py))*{$fire_coefficient};
				if(this.variator==1)data[(pix)+1] = (this.getG(data,px, py) + (Math.random()*16) + this.getG(data,px, py+1) + this.getG(data,px-1, py))*{$fire_coefficient};
				if(this.variator==2)data[(pix)+2] = (this.getB(data,px, py) + (Math.random()*16) + this.getB(data,px, py+1) + this.getB(data,px-1, py))*{$fire_coefficient};
			}
			data[(pix)+3] = 255;
			pix+=4;
			this.variator++;
			this.variator%=3;
		}
		this.variator++;
		this.variator%=3;
	}
	
	this.ctx.putImageData(d, 0,0);
}

function redraw() {	
	var py=0;
	var px=0;
	
	var position = 0;

	if(!vpu.map.maps[0]) return;
	if(vpu.map.maps[0].length == 0) return;
	
	
	vpu.system.update();
	
	
	for(;py < {$height}; py+={$tile_height}){
		for(px=0; px < {$width}; px+={$tile_width}){
	
			// Find out index corresponding tileset coordinates			
			var ty = [(vpu.map.maps[0][position])>>4, (vpu.map.maps[1][position])>>4, (vpu.map.maps[2][position])>>4]; 
			var tx = [(vpu.map.maps[0][position])&15, (vpu.map.maps[1][position])&15, (vpu.map.maps[2][position])&15]; 
			
			{if $fire_enable}if((tx[0] >0)&&(ty[0]>0)){/if}(vpu.ctx).drawImage(vpu.data0,tx[0]<<3, ty[0]<<3,{$tile_width},{$tile_height},px,py,{$tile_width},{$tile_height});
			(vpu.ctx).drawImage(vpu.data1[vpu.map.pals[1][position]],tx[1]<<3, ty[1]<<3,{$tile_width},{$tile_height},px,py,{$tile_width},{$tile_height});
			(vpu.ctx).drawImage(vpu.data2[vpu.map.pals[2][position]],tx[2]<<3, ty[2]<<3,{$tile_width},{$tile_height},px,py,{$tile_width},{$tile_height});
			
			position++;
		}
	}
	
	vpu.redrawFlag= false;

	if(vpu.enableRandomizer)vpu.randomize();
	
	vpu.console.drawCursor();	
	
	vpu.postProcessor();
}

Vpu.prototype.locateCursor = function(x, y){
	this.cursorX = x;
	this.cursorY = y;
	this.cursorP = (this.cursorY << {$tile_width_factor+1}) + this.cursorX;
}

Vpu.prototype.print = function(text, color){
	this.color = color;

	{if $debug_enable}
	debugSet("Last print call text dump:<br/>");
	{/if}

	var i;
	for(i=0; i<text.length; i++){
		if(text.substr(i, 1)=="^"){
			var c = color;
			if(i<text.length-1){
				i++;
				c = parseInt("0x"+text.substr(i, 1),16);
				
			}
			this.color = c;			
		} else this.putChar(text.charCodeAt(i)+0x100, this.color);
	}
}

Vpu.prototype.putChar = function(index, color){
	
	if(index>0x0fff)this.color = index >> 12;
	else this.color = color;
	
	index = index & 0x0fff;
	
	// Fucking dirty hack...
	switch(index){
		case 256+" ".charCodeAt(0): index = 256; break;
		case 256+"á".charCodeAt(0): index = 256 + 160; break;
		case 256+"é".charCodeAt(0): index = 256 + 130; break;
		case 256+"í".charCodeAt(0): index = 256 + 161; break;
		case 256+"ó".charCodeAt(0): index = 256 + 162; break;
		case 256+"ú".charCodeAt(0): index = 256 + 163; break;		
		case 256+"Á".charCodeAt(0): index = 256 + 160; break;//181
		case 256+"É".charCodeAt(0): index = 256 + 144; break;//144
		case 256+"Í".charCodeAt(0): index = 256 + 161; break;//214
		case 256+"Ó".charCodeAt(0): index = 256 + 162; break;//224
		case 256+"Ú".charCodeAt(0): index = 256 + 163; break;//233
		
		case 256+"\r".charCodeAt(0):
			this.cursorX=0;
			return;

		case 256+"\n".charCodeAt(0):
			this.cursorX=0;
			this.cursorY++;
			while(this.cursorY >= {$height / ($tile_height)}) {
				this.cursorY = vpu.cursorY-1;
				this.cursorX=0;
			}
			return;
			
		default:
			if(index<1024)break;
			index%=1024;
			break;
	}
	
	
	this.cursorP = (this.cursorY * {$width / ($tile_width/2)}) + this.cursorX;		
	
	if(this.cursorX%2) {
		this.cursorP = (this.cursorY * {$width / ($tile_width/2)}) + this.cursorX>>1;
		this.map.maps[2][this.cursorP] = ((index)&0x0FFF);
		this.map.pals[2][this.cursorP] = this.color;
	} else {
		this.cursorP = (this.cursorY * {$width / ($tile_width/2)}) + this.cursorX>>1;
		this.map.maps[2][this.cursorP] = 0x100;
		this.map.maps[1][this.cursorP] = ((index)&0x0FFF);
		this.map.pals[1][this.cursorP] = this.color;
	}
	
	
	this.cursorX++;
	
	if(this.cursorX == {$width / ($tile_width/2)} ) {
		this.cursorX = 0;
		this.cursorY++;
	}
	
	while(this.cursorY >= {$height / ($tile_height)}) {
		this.cursorY = this.cursorY-1;
		this.cursorX=0;
		this.console.scroll();
	}
}


Vpu.prototype.loadMapData = function(){
	var i;
	for(i = 0; i < this.mapData0_0.length; i++){
		this.map.maps[0][i] = this.mapData0_0[i];
		this.map.maps[1][i] = this.mapData0_1[i]&0x0FFF;
		this.map.maps[2][i] = this.mapData0_2[i]&0x0FFF;;
		this.map.pals[1][i] = (this.mapData0_1[i]&0xF000 )>>12;
		this.map.pals[2][i] = (this.mapData0_2[i]&0xF000 )>>12;
	}
}

// TODO: MOVE THIS TO A PROPER DISPATCHER CLASS!!!
Vpu.prototype.dataRequest = function(request){	
	AJAX(request);	 
}

var vpu = new Vpu(); /* important */