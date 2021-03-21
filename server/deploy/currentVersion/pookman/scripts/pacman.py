from entity import Entity

class Pacman(Entity):
    game = None

    @staticmethod
    def initialize(game):
        Pacman.game = game

    def __init__(self, ghost_type=0x00):
        self.ghost_type = ghost_type

"""
var PacmanGfx = false;



var	DIR_RIGHT	= 0x00,
DIR_DOWN	= 0x01,
DIR_LEFT	= 0x02,
DIR_UP		= 0x03;



var DirNames = [];

DirNames[DIR_UP   ]	= 'upwards';

DirNames[DIR_DOWN ] = 'downwards';

DirNames[DIR_LEFT ] = 'leftwards';

DirNames[DIR_RIGHT] = 'rightwards';



var DirOpposite = [];

DirOpposite[DIR_DOWN ] = DIR_UP;

DirOpposite[DIR_UP	 ] = DIR_DOWN;

DirOpposite[DIR_LEFT ] = DIR_RIGHT;

DirOpposite[DIR_RIGHT] = DIR_LEFT;






var	PACMAN_DEAD		= 0x00,
PACMAN_READY	= 0x01,
PACMAN_WALKING	= 0x02,
PACMAN_DIEING	= 0x03;



function Pacman(game){
this.self 		= this;
this.game 		= game;
this.rx 		= (13*8)+4;
this.ry	 		= (23*8);
this.x 			= parseInt(this.rx);
this.y 			= parseInt(this.ry);
this.tx 		= 1;
this.ty	 		= 1;
this.steady		= true;

this.status		= PACMAN_DEAD;
this.energized	= 0;
this.energyQ	= 500;

this.inIntersection = false;

this.speed 		= 1.25;//.8;
this.frame 		= 0;
this.frameDir 	= 1;
this.frameSpd 	= 1.5;
this.eating		= false;
this.multiplier	= 1;	

this.sprite		= false;
this.direction	= DIR_RIGHT;
this.nextDir	= DIR_RIGHT;

if(!PacmanGfx) this.loadGraphics();
this.ready();

}



Pacman.prototype.ready = function(){
this.steady		= true;
this.ry	 		= (23*8);
this.rx 		= (13*8)+4;
this.x 			= this.rx;
this.y			= this.ry;
this.tx 		= parseInt(this.x / 8);
this.ty	 		= parseInt(this.y / 8);
this.game.pills = 0;
this.spawn();

}



Pacman.prototype.loadGraphics = function(){
info("PACMAN", "Requesting graphic data");
try {            
	this.sprite = new SpriteSheet(this.game.engine, "gfx/pacman.png", 16, 16, [255,0,255], this.self);
} catch(e) {
	complaint('PACMAN', 'Failed to load graphics', e);
	ParticleGfx = false;
}

}



Pacman.prototype.processGraphics = function(me){
info("PACMAN", "Processing graphic data");


PacmanGfx = [];
var x,y,i=0;
var totalFrames = 0;
PacmanGfx[DIR_RIGHT] = new Animation(	me.sprite, 
										0,  0, 
										16, 0, 
										true, 1, true);
PacmanGfx[DIR_DOWN ] = new Animation(	me.sprite, 
										0,  1, 
										16, 1, 
										true, 1, true);
PacmanGfx[DIR_LEFT ] = new Animation(	me.sprite, 
										0,  2, 
										16, 2, 
										true, 1, true);
PacmanGfx[DIR_UP   ] = new Animation(	me.sprite, 
										0,  3, 
										16, 3, 
										true, 1, true);


totalFrames = 	PacmanGfx[DIR_RIGHT].frameCount + PacmanGfx[DIR_DOWN].frameCount +
				PacmanGfx[DIR_LEFT].frameCount	+ PacmanGfx[DIR_UP].frameCount;

cout('^3'+totalFrames+'^8 frames extracted');	

}



Pacman.prototype.render = function(){

if(this.status == PACMAN_DEAD) return;

if(debug[DEBUG_PACMAN]){ this.game.engine.graphics.setAlpha(0.05); }

if(this.status != PACMAN_DIEING){
	PacmanGfx[this.direction].setCurrentFrame(9 + parseInt(this.frame));
}

PacmanGfx[this.direction].draw(this.x - 4, this.y + 20);		


if(debug[DEBUG_PACMAN]){
	this.game.engine.graphics.setAlpha(0.25);
	var ctx = this.game.engine.graphics.context[this.game.engine.graphics.defaultContext];
	ctx.fillStyle = "#ffff00";
	var x = this.tx * 8;
	var y = this.ty * 8;
	ctx.fillRect(x, y + 24, 8,8);
	
	this.game.engine.graphics.setAlpha(0.5);
	x = this.x;
	y = this.y;
	
	ctx.fillStyle = "#888800";
	ctx.fillRect(x, 	y + 24, 8,8);
	ctx.fillStyle = "#ffff00";
	ctx.fillRect(x+1, 	y + 25, 6,6);
	ctx.fillRect(x, 	y + 26, 8,4);
	ctx.fillRect(x+2,	y + 24, 4,8);

	
	
	
	this.game.engine.graphics.setAlpha(1);
}


}



Pacman.prototype.spawn = function(){
this.rx = (13*8)+4;
this.ry = (23*8);
this.x = parseInt(this.x);
this.y = parseInt(this.y);
this.tx = parseInt(this.x / 8);
this.ty = parseInt(this.y / 8);
this.status = PACMAN_READY;
this.direction = DIR_RIGHT;

}



Pacman.prototype.die = function(){
info("PACMAN", "die...");
this.status = PACMAN_DIEING;
this.frame = PacmanGfx[this.direction].frameCount-1;

}



Pacman.prototype.setSpeed = function(){
/** Adjust speed **/
if(this.game.level == 0){
	this.energyQ = 500;
	if(this.energized > 0){ if(this.eating) this.speed = 0.79; else this.speed = 0.9; 	}
	else if(this.eating) this.speed = 0.71; else this.speed = 0.8;
} else if(this.game.level < 4){
	this.energyQ = 250;
	if(this.energized > 0){ if(this.eating) this.speed = 0.83; else this.speed = 0.95; 	}
	else if(this.eating) this.speed = 0.79; else this.speed = 0.9;
} else if(this.game.level < 19){
	this.energyQ = 125;
	if(this.energized > 0){ if(this.eating) this.speed = 0.87; else this.speed = 1.0;		}
	else if(this.eating) this.speed = 0.87; else this.speed = 1.0;
} else {
	this.energyQ = 1;
	if(this.energized > 0){ if(this.eating) this.speed = 0.79; else this.speed = 0.9; 	}
	else if(this.eating) this.speed = 0.79; else this.speed = 0.9;
}

//this.speed *= 1;

}



Pacman.prototype.update = function(delta){
var i = (this.ty * this.game.map.map.width) + this.tx;



if(this.energized > 0) {
	if(this.game.isGhost(i)	) this.eatGhost(i);
	this.energized--;
} else {
	if(this.game.isGhost(i)	) {
		this.die();	
		return;
	}
}


this.setSpeed();


switch(this.status){
	case PACMAN_DEAD: 
		this.game.resetSystems();
		return;
	
	case PACMAN_DIEING: 
		this.game.freeze = 5;			
		if( this.frame > 0 ){ 
			this.frame--;				
		} else this.status = PACMAN_DEAD;
		PacmanGfx[this.direction].frame = this.frame;
		return;
		break;
	
	case PACMAN_READY:
		//this.energized = 0;
		if(this.steady)break;
		if(this.canWalk(this.nextDir)) {
			this.direction = this.nextDir;
			this.status = PACMAN_WALKING;
		} break;
		
	case PACMAN_WALKING:			
		if(!this.canWalk(this.direction)) {
			this.status = PACMAN_READY;
			this.rx = this.tx * 8;
			this.ry = this.ty * 8;
			this.rx = this.x;
			this.ry = this.y;
		}
		
		if(this.canWalk(this.nextDir)) {
			if(this.direction != this.nextDir){
				this.rx = this.x;
				this.ry = this.y;
			}
			this.direction = this.nextDir;
		}
				
		if(this.canWalk(this.direction)){
			switch(this.direction){
				case DIR_RIGHT:	this.moveRight();	break;
				case DIR_DOWN:	this.moveDown();	break;
				case DIR_LEFT:	this.moveLeft();	break;
				case DIR_UP: 	this.moveUp();		break;
			}			
	
			/** Animate Pacman**/
			this.frame+= this.frameSpd * this.frameDir;			
			if(this.frame > 6) { this.frame = 6; this.frameDir = -1; }
			if(this.frame < 0) { this.frame = 0; this.frameDir =  1; }
		
			
		}
	
		/** Exit if pacman is outside the map **/
		if(this.tx > this.game.map.map.width)return;
		if(this.ty > this.game.map.map.height)return;
		
		/** Check if there is food in the middle of this position **/


		if(this.inIntersection){
			i = (this.ty * this.game.map.map.width) + this.tx;
			if(this.game.isPill(i)		) this.eatPill(i); 		else
			if(this.game.isEnergizer(i)	) this.eatEnergizer(i);	
		}
		break;			
}

}



Pacman.prototype.eatGhost = function(position){
// Set ghost status to scared (eyes returning to the zone)

this.game.addScore(200 * this.multiplier);
this.multiplier			*= 2;
this.eating				= true;
this.game.freeze		= 25;

var w = this.game.map.map.width;
var h = this.game.map.map.height;
var pos = [];

pos[GHOST_BLINKY]	= (this.game.blinky.ty	* w) + this.game.blinky.tx;
pos[GHOST_PINKY]	= (this.game.pinky.ty 	* w) + this.game.pinky.tx;
pos[GHOST_INKY] 	= (this.game.inky.ty 	* w) + this.game.inky.tx;
pos[GHOST_CLYDE]	= (this.game.clyde.ty 	* w) + this.game.clyde.tx;


if((pos[GHOST_BLINKY] == position) && (this.game.blinky.mode == STATUS_FRIGHTENED)) return this.game.blinky.switchMode(STATUS_RETURNING);
if((pos[GHOST_PINKY ] == position) && (this.game.pinky.mode  == STATUS_FRIGHTENED)) return this.game.pinky.switchMode(STATUS_RETURNING);
if((pos[GHOST_INKY  ] == position) && (this.game.inky.mode   == STATUS_FRIGHTENED)) return this.game.inky.switchMode(STATUS_RETURNING);
if((pos[GHOST_CLYDE ] == position) && (this.game.clyde.mode  == STATUS_FRIGHTENED)) return this.game.clyde.switchMode(STATUS_RETURNING);

}



Pacman.prototype.eatPill = function(i){
this.game.map.map.layers[0].data[i]	= 40;
this.game.addScore(10);
this.game.map.redraw 	= true;
this.eating 			= true;
if(this.game.blinky.timer	< GhostDelays[GHOST_BLINKY]) this.game.blinky.timer++; 	else
if(this.game.pinky.timer 	< GhostDelays[GHOST_PINKY ]) this.game.pinky.timer++;	else
if(this.game.inky.timer		< GhostDelays[GHOST_INKY  ]) this.game.inky.timer++;		else
if(this.game.clyde.timer	< GhostDelays[GHOST_CLYDE ]) this.game.clyde.timer++;

}



Pacman.prototype.eatEnergizer = function(i){
this.game.map.map.layers[0].data[i] = 40;
this.energized 			+= this.energyQ;
this.game.addScore(50);
this.multiplier			= 1;
this.game.map.redraw 	= true;
this.eating 			= true;

}



Pacman.prototype.canTurn = function(direction){
// Return true if xposition < 2 or xposition > 6

}



Pacman.prototype.canWalk = function(direction){
var map = this.game.map.map.layers[0].data;
var w = this.game.map.map.width;
var h = this.game.map.map.height;
	
var right	= parseInt( ( this.ty * w ) + this.tx + 1 );
var down	= parseInt( ((this.ty + 1) * w) + this.tx);
var left 	= parseInt( ( this.ty * w ) + this.tx - 1); 
var up 		= parseInt( ((this.ty - 1) * w) + this.tx);

var outside	= (this.tx < 1) || (this.ty < 1) || (this.tx >= w-1) || (this.ty >= h-1);
var hInt = (this.x % 8) == 0;
var vInt = (this.y % 8) == 0;

switch(direction){
	case DIR_RIGHT:	return ((map[right] > 30) && (vInt)) || (outside);
	case DIR_DOWN:	return ((map[down ] > 30) && (hInt)) || (outside);
	case DIR_LEFT:	return ((map[left ] > 30) && (vInt)) || (!hInt) || (outside);
	case DIR_UP: 	return ((map[up   ] > 30) && (hInt)) || (!vInt) || (outside);
}

}



Pacman.prototype.go = function(nextdirection){
this.steady = false;
/**Avoid entering the ghost house!**/
if(this.ty == DOOR_Y-1){
	if(nextdirection == DIR_DOWN){
		if((this.tx >= DOOR_X[0]-2)&&(this.tx <= DOOR_X[1]+2)) return;
	}
}

this.nextDir = this.direction;


if(this.x > (this.game.map.map.width*8) - 16) return;
if(this.y > (this.game.map.map.height*8) - 16) return;
if(this.y < 8) return;
if(this.x < 8) return;

this.nextDir = nextdirection;

}



Pacman.prototype.clamp = function(){
var w = this.game.map.map.width;
var h = this.game.map.map.height;
var tw = this.game.map.map.tileWidth;
var th = this.game.map.map.tileHeight;
var height	= h*th;
var width 	= w*tw;

if(this.ry > height	+16) this.ry -= height+24; else 
if(this.ry < -16		) this.ry += height+24;
	
if(this.rx > width +16	) this.rx -= width+24; else 
if(this.rx < -16		) this.rx += width+24;

this.x  = parseInt(this.rx);
this.y  = parseInt(this.ry);

this.ty = parseInt(this.y / 8);
this.tx = parseInt(this.x / 8);

//this.inIntersection = (((this.y % 8) == 0)&&((this.x % 8) == 0));	
var corner = 4.5;
this.inIntersection 	= (((((this.rx % 8) > 8-(corner+this.speed) )||((this.rx % 8)< 1 + (corner + this.speed) )))&&(((this.ry % 8) > 8-(corner + this.speed) )||((this.ry % 8)< 0 + (corner + this.speed) )));

}


Pacman.prototype.moveRight = function(){
this.rx += this.speed;
this.clamp();

}



Pacman.prototype.moveDown = function(){
this.ry += this.speed;
this.clamp();

}



Pacman.prototype.moveLeft = function(){
this.rx -= this.speed;
this.clamp();

}



Pacman.prototype.moveUp = function(){
this.ry -= this.speed;
this.clamp();

}
"""