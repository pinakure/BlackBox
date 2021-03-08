var GhostGfx = [];

var GhostsLoaded = false;

var	STATUS_CHASE		= 0x00,
	STATUS_SCATTER		= 0x01,
	STATUS_FRIGHTENED	= 0x02,
	STATUS_RETURNING	= 0x03,/** Lone Eyes! **/
	STATUS_LAST			= 0x04,
	STATUS_PAUSE		= 0x05;

var	GHOST_BLINKY		= 0x00,
	GHOST_INKY 			= 0x01,
	GHOST_PINKY 		= 0x02,
	GHOST_CLYDE 		= 0x03,
	GHOST_PREY	 		= 0x04,
	GHOST_DEAD	 		= 0x05,
	GHOST_BLINK 		= 0x06;
	

var	GhostSpawn 	= [];
GhostSpawn[GHOST_BLINKY]	= [(13*8)+4, (11*8)];
GhostSpawn[GHOST_INKY] 		= [(11*8)+4, (14*8)];
GhostSpawn[GHOST_PINKY] 	= [(13*8)+4, (14*8)];
GhostSpawn[GHOST_CLYDE] 	= [(15*8)+4, (14*8)];

var	GhostTarget = [];
GhostTarget[GHOST_BLINKY]	= [23,	3];
GhostTarget[GHOST_INKY] 	= [20,	27];
GhostTarget[GHOST_PINKY] 	= [4,	3];
GhostTarget[GHOST_CLYDE] 	= [7,  27];

var	GhostDelays = [];
GhostDelays[GHOST_BLINKY]	= 0;
GhostDelays[GHOST_INKY] 	= 30;
GhostDelays[GHOST_PINKY] 	= 2;
GhostDelays[GHOST_CLYDE] 	= 60;

var GhostColors = [];
GhostColors[GHOST_BLINKY] 	= "#ff0000";
GhostColors[GHOST_INKY  ] 	= "#00ffff";
GhostColors[GHOST_PINKY ] 	= "#ffaaaa";
GhostColors[GHOST_CLYDE ] 	= "#ffaa00";

/** Set time limits to switch between modes **/
/** Note: When ghosts are in frightnened mode their timers WONT INCREASE!! **/
var ChaseTime 	= [];
ChaseTime[0] 	= [ 20, 20,   20, 9999];//Level = 0
ChaseTime[1] 	= [ 20, 20, 1033, 9999];//Level >0 & <5
ChaseTime[2] 	= [ 20, 20, 1037, 9999];//Level >= 5

var ScatterTime	= [];
ScatterTime[0] 	= [  7,  7,    5,  	 5];
ScatterTime[1] 	= [  7,  7,    5,    1];// Last value is  1/60, not 1
ScatterTime[2] 	= [  5,  5,    5,    1];// Last value is  1/60, not 1



function Target(ghost){
	this.ghost		= ghost;
	this.x 			= 0;
	this.y 			= 0;
	this.distance	= 0;
	this.direction	= DIR_UP;
	this.solid		= false;
	this.viable		= false;
	this.color		= "#ffff00";
}

Target.prototype.asString = function(){
	return 	"[ "+DirNames[this.direction]+" "+ (this.viable ? (this.solid ? "SOLID" : "VIABLE") : "FORBID") +
			" D: "	+ this.distance.toFixed(2) + 
			" ]";			 
}

Target.prototype.set = function(x,y){
	this.viable = true;
	this.x = x;
	this.y = y;
	// Calculate distance to target (ox, oy)
	var ox = this.ghost.targetX;
	var oy = this.ghost.targetY;
	
	this.distance = Math.sqrt( ((this.x-ox) * (this.x-ox)) + ((this.y-oy) * (this.y-oy)) );
	this.direction = (this.x > this.ghost.tx) ? DIR_RIGHT : (this.x < this.ghost.tx) ? DIR_LEFT : (this.y > this.ghost.ty) ? DIR_DOWN : (this.y < this.ghost.ty) ? DIR_UP : DIR_DOWN;
			
	// Set viability and color
	if(this.solid = this.ghost.isSolid(this.x, this.y)){
		this.viable	= false;
		this.color 	= "#0000ff";
	} else {
		this.viable	= true;
		this.color = "#666000";
	}
}

Target.prototype.drawStaticTarget = function(x,y,color,isTarget){
	var ctx = game.graphics.context[game.graphics.defaultContext];	
	if(isTarget){
		ctx.strokeStyle = color;
		ctx.beginPath();
		ctx.moveTo(x, 	y+24);
		ctx.lineTo(x+8, y+8+24);
		ctx.moveTo(x, y+8+24);
		ctx.lineTo(x+8, y+24);
		ctx.stroke();
	} else {
		ctx.fillStyle = color;
		ctx.fillRect(x, 	y + 24, 8,8);
		ctx.clearRect(x+1, 	y + 25, 6,6);
		ctx.clearRect(x, 	y + 26, 8,4);
		ctx.clearRect(x+2,	y + 24, 4,8);
	}
}

Target.prototype.draw = function(g){
	var x = this.x<<3;
	var y = this.y<<3;
	var ctx = g.context[g.defaultContext];	
	ctx.fillStyle = this.color;
	ctx.fillRect(x, 	y + 24, 8,8);
	ctx.clearRect(x+1,	y + 25, 6,6);
	ctx.clearRect(x, 	y + 26, 8,4);
	ctx.clearRect(x+2,	y + 24, 4,8);	
	
	ctx.strokeStyle = this.color;
	ctx.beginPath();
	switch(this.direction){
		case DIR_LEFT:
			ctx.moveTo(x,y+28);
			ctx.lineTo(x+8, y+24);
			ctx.moveTo(x,y+28);
			ctx.lineTo(x+8, y+32);
			break;
		case DIR_DOWN:
			ctx.moveTo(x+4,y+32);
			ctx.lineTo(x, y+24);
			ctx.moveTo(x+4,y+32);
			ctx.lineTo(x+8, y+24);
			break;
		case DIR_UP:
			ctx.moveTo(x+4,y+24);
			ctx.lineTo(x, y+32);
			ctx.moveTo(x+4,y+24);
			ctx.lineTo(x+8, y+32);
			break;
		case DIR_RIGHT:
			ctx.moveTo(x+8,y+28);
			ctx.lineTo(x, y+24);
			ctx.moveTo(x+8,y+28);
			ctx.lineTo(x, y+32);
			break;			
			break;
	}
	ctx.stroke();			
}


function Ghost(type, game){
	this.game 		= game;
	this.self 		= this;
	this.type 		= type;
	
	// Positioning registers	
	this.x			= GhostSpawn[this.type][0];		//Real X (with decimals)
	this.y			= GhostSpawn[this.type][1];		//Real Y (with decimals)
	this.rx 		= parseInt(this.x);
	this.ry			= parseInt(this.y);
	this.tx			= parseInt(this.rx / 8);		//Tile X
	this.ty			= parseInt(this.ry / 8);		//Tile Y 

	this.inIntersection	= false;
	this.inHouse		= false;
	this.inDoor			= false;
	this.behindDoor		= false;
	this.inFrontOfDoor	= false;
	this.inFrontOfHouse	= false;
	this.mustLeave		= false;
	this.inTunnel		= false;
	
	this.speed		= 1;
	this.timer		= 0;
	this.gfx		= this.type;

	
	this.direction	= DIR_LEFT;
	this.mode		= STATUS_SCATTER;
	this.frame 		= 0;
	
	this.chaseTime	= 0;
	this.scaterTime	= 0;
	
	
	this.targetX 	= GhostTarget[type][0];
	this.targetY 	= GhostTarget[type][1];
	this.targets	= [ new Target(this),
						new Target(this),
						new Target(this),
						new Target(this)];
	
	this.sprite 	= false;
	if(!GhostsLoaded)this.loadGraphics();
}

Ghost.prototype.reset = function(){
	// Positioning registers	
	this.x			= GhostSpawn[this.type][0];		//Real X (with decimals)
	this.y			= GhostSpawn[this.type][1];		//Real Y (with decimals)
	this.rx 		= parseInt(this.x);
	this.ry			= parseInt(this.y);
	this.tx			= parseInt(this.rx / 8);		//Tile X
	this.ty			= parseInt(this.ry / 8);		//Tile Y 
	this.inIntersection	= false;
	this.inHouse		= false;
	this.inDoor			= false;
	this.behindDoor		= false;
	this.inFrontOfDoor	= false;
	this.inFrontOfHouse	= false;
	this.mustLeave		= false;
	this.inTunnel		= false;
	this.speed		= 1;
	this.timer		= 0;
	this.gfx		= this.type;
	this.direction	= DIR_LEFT;
	this.mode		= STATUS_SCATTER;
	this.frame 		= 0;
	this.chaseTime	= 0;
	this.scaterTime	= 0;
	this.targetX 	= GhostTarget[this.type][0];
	this.targetY 	= GhostTarget[this.type][1];
	return;
	
	this.rx 		= GhostSpawn[this.type][0];
	this.ry			= GhostSpawn[this.type][1];
	this.x 			= parseInt(this.x);
	this.y 			= parseInt(this.y);
	this.tx			= parseInt(this.x / 8);
	this.ty			= parseInt(this.y / 8);
	this.speed		= 1;
	this.timer		= 0;
	this.gfx		= this.type;

	this.direction	= DIR_UP;
	this.mode		= STATUS_SCATTER;
	this.frame 		= 0;
	
	this.chaseTime	= 0;
	this.scaterTime	= 0;
	//this.updateFlags();
}

Ghost.prototype.loadGraphics = function(){
	info("GHOST", "Requesting graphic data");
	GhostsLoaded = true;
	try {            
		this.sprite = new SpriteSheet(this.game.engine, "gfx/ghosts.png", 16, 16, [255,0,255], this.self);
	} catch(e) {
		complaint('GHOST', 'Failed to load graphics', e);
		GhostGfx = false;
	}
}

Ghost.prototype.processGraphics = function(me){
	info("GHOST", "Processing graphic data");
	
	var totalFrames = 0;
	
	GhostGfx = [];
	
	for(var i=0; i < 4; i++){
		GhostGfx[i] = [];
		GhostGfx[i][DIR_RIGHT] = new Animation(me.sprite, 
												0, 		i, 
												1, 		i, 
												true, 	5, true);
		GhostGfx[i][DIR_DOWN ] = new Animation(me.sprite, 
												2, 		i, 
												3, 		i, 
												true, 	5, true);
		GhostGfx[i][DIR_LEFT ] = new Animation(me.sprite, 
												4, 		i, 
												5, 		i, 
												true,	5, true);
		GhostGfx[i][DIR_UP   ] = new Animation(me.sprite, 
												6, 		i, 
												7, 		i, 
												true, 	5, true);	
		GhostGfx[i][DIR_UP   ].pingPong = true;
		GhostGfx[i][DIR_DOWN ].pingPong = true;
		GhostGfx[i][DIR_LEFT ].pingPong = true;
		GhostGfx[i][DIR_RIGHT].pingPong = true;
												
		totalFrames += 	GhostGfx[i][DIR_RIGHT].frameCount + GhostGfx[i][DIR_DOWN].frameCount + GhostGfx[i][DIR_LEFT].frameCount	+ GhostGfx[i][DIR_UP].frameCount;
	}

	GhostGfx[GHOST_DEAD] = [];
	GhostGfx[GHOST_DEAD][DIR_RIGHT] = new Animation(me.sprite, 
											0, 		GHOST_DEAD, 
											1, 		GHOST_DEAD, 
											true, 	5, true);
	GhostGfx[GHOST_DEAD][DIR_DOWN ] = new Animation(me.sprite, 
											2, 		GHOST_DEAD, 
											3, 		GHOST_DEAD, 
											true, 	5, true);
	GhostGfx[GHOST_DEAD][DIR_LEFT ] = new Animation(me.sprite, 
											4, 		GHOST_DEAD, 
											5, 		GHOST_DEAD, 
											true, 	5, true);
	GhostGfx[GHOST_DEAD][DIR_UP   ] = new Animation(me.sprite, 
											6, 		GHOST_DEAD, 
											7, 		GHOST_DEAD, 
											true, 	5, true);
	GhostGfx[GHOST_DEAD][DIR_RIGHT].pingPong = true;
	GhostGfx[GHOST_DEAD][DIR_LEFT ].pingPong = true;
	GhostGfx[GHOST_DEAD][DIR_DOWN ].pingPong = true;
	GhostGfx[GHOST_DEAD][DIR_UP	  ].pingPong = true;
	totalFrames += 	GhostGfx[GHOST_DEAD][DIR_RIGHT].frameCount + GhostGfx[GHOST_DEAD][DIR_DOWN].frameCount + GhostGfx[GHOST_DEAD][DIR_LEFT].frameCount + GhostGfx[GHOST_DEAD][DIR_UP].frameCount;

	/** Load scared and scared blinking animation **/
	GhostGfx[GHOST_PREY] = [];
	GhostGfx[GHOST_PREY][DIR_RIGHT] = new Animation(me.sprite, 
											0, 		GHOST_PREY, 
											3, 		GHOST_PREY, 
											true, 	15, true);
	GhostGfx[GHOST_PREY][DIR_RIGHT].pingPong = true;											
	GhostGfx[GHOST_PREY][DIR_LEFT ] = GhostGfx[GHOST_PREY][DIR_RIGHT];
	GhostGfx[GHOST_PREY][DIR_DOWN ] = GhostGfx[GHOST_PREY][DIR_RIGHT];
	GhostGfx[GHOST_PREY][DIR_UP	  ] = GhostGfx[GHOST_PREY][DIR_RIGHT];
	totalFrames += 	GhostGfx[GHOST_PREY][DIR_RIGHT].frameCount + GhostGfx[GHOST_PREY][DIR_DOWN].frameCount + GhostGfx[GHOST_PREY][DIR_LEFT].frameCount + GhostGfx[GHOST_PREY][DIR_UP].frameCount;
	
	GhostGfx[GHOST_BLINK] = [];
	GhostGfx[GHOST_BLINK][DIR_RIGHT] = new Animation(me.sprite, 
											0, 		GHOST_PREY, 
											7, 		GHOST_PREY, 
											true, 	15, true);
	GhostGfx[GHOST_BLINK][DIR_RIGHT].pingPong = true;
	GhostGfx[GHOST_BLINK][DIR_LEFT ] = GhostGfx[GHOST_BLINK][DIR_RIGHT];
	GhostGfx[GHOST_BLINK][DIR_DOWN ] = GhostGfx[GHOST_BLINK][DIR_RIGHT];
	GhostGfx[GHOST_BLINK][DIR_UP   ] = GhostGfx[GHOST_BLINK][DIR_RIGHT];
	totalFrames += 	GhostGfx[GHOST_BLINK][DIR_RIGHT].frameCount	+ GhostGfx[GHOST_BLINK][DIR_DOWN].frameCount + GhostGfx[GHOST_BLINK][DIR_LEFT].frameCount + GhostGfx[GHOST_BLINK][DIR_UP].frameCount;
	
	cout('^3'+totalFrames+'^8 frames extracted');	
}

Ghost.prototype.drawBox = function(x, y){
	var ctx = this.game.engine.graphics.context[this.game.engine.graphics.defaultContext];	
	ctx.fillStyle = GhostColors[this.type];
	ctx.fillRect(x, y + 24, 8,8);
}
	
Ghost.prototype.render = function(g){	
	this.game.engine.graphics.setAlpha(1);
	
	//$('#debug_l').text(this.
	
	if(debug[DEBUG_GHOSTS]){
		// Draw target
		Target.prototype.drawStaticTarget(this.targetX<<3, 	this.targetY<<3, GhostColors[this.type], true);
		
		this.game.engine.graphics.setAlpha(0.5);		
		
		
		// Draw Next possible Positions
		this.targets[0].draw(this.game.graphics);
		this.targets[1].draw(this.game.graphics);
		this.targets[2].draw(this.game.graphics);
		this.targets[3].draw(this.game.graphics);
		
		// Draw Relative Position
		Target.prototype.drawStaticTarget(this.x, 				this.y,			GhostColors[this.type], false);
		
		// Draw absolute Position
		this.game.engine.graphics.setAlpha(.4);
		this.drawBox(this.tx<<3, this.ty<<3);
		
		// Set opacity for sprite almost transparent
		this.game.engine.graphics.setAlpha(.05);
	} else this.game.engine.graphics.setAlpha(.65);
	
	GhostGfx[this.gfx][this.direction].currentFrame = parseInt(this.frame);
	GhostGfx[this.gfx][this.direction].draw(this.x - 4, this.y + 20);
	
	this.game.engine.graphics.setAlpha(1);
	
	//if(this.inIntersection)debugger;
}


Ghost.prototype.handleHouse = function(delta){
	if(this.inHouse){
		if(this.mustLeave){
			
			this.targetY = DOOR_Y - 1;
			this.targetX = DOOR_X[0];
			
			// This ghost must leave the house
			if(this.inDoor)this.direction = DIR_UP;
			if(this.behindDoor){
				this.x = this.tx * 8;
				this.direction = DIR_UP;
			}
		} else if(this.behindDoor) this.direction = DIR_LEFT;

		if(this.inIntersection){	
			this.setTargets([this.tx, this.tx, this.tx, this.tx], [this.ty, this.ty, this.ty, this.ty]);
			
			var onTop 		= (this.ty <= DOOR_Y+1)&&((this.y%8)==0);
			var onBottom 	= this.ty >= DOOR_Y+2;
			var onLeft 		= this.tx <= DOOR_X[0]-2;
			var onRight 	= this.tx >= DOOR_X[1]+1;

			/*if(onTop) this.direction = DIR_DOWN; 
			if(onBottom	) this.direction = DIR_UP; */
			
			if(onLeft 	&& onBottom	) this.direction = DIR_RIGHT; 
			if(onRight	&& onBottom	) this.direction = DIR_UP; 
			if(onRight	&& onTop 	) this.direction = DIR_LEFT; 
			if(onLeft	&& onTop 	) this.direction = DIR_DOWN; 				
		}	
		return true;
	}
	return false;
}

Ghost.prototype.update = function(delta){	
		
	// Perform specific actions depending on current status
	switch(this.mode){
		case STATUS_PAUSE:
			if(this.game.pacman.status != PACMAN_DEAD) this.switchMode(STATUS_SCATTER);
			return;
			
		case STATUS_SCATTER:
			if(!this.handleHouse()){
				if(this.game.pacman.energized == this.game.pacman.energyQ - 1) {
					this.switchMode(STATUS_FRIGHTENED);		
					break;
				}
			
				this.targetX = GhostTarget[this.type][0];
				this.targetY = GhostTarget[this.type][1];
				
				//when timer is over, jump to chase mode!
				if(this.scaterTime > 400) this.switchMode(STATUS_CHASE);
				else this.scaterTime++;
			}
			this.move();
			break;
			
		case STATUS_CHASE: 
			if(this.game.pacman.energized == this.game.pacman.energyQ - 1) {
				this.switchMode(STATUS_FRIGHTENED);		
				break;
			}
			
			this.setChaseTarget();
			
			//when timer is over, jump to scatter mode!
			if(this.type != GHOST_BLINKY){
				if(this.chaseTime > 400) this.switchMode(STATUS_SCATTER);
				else this.chaseTime++;
			}
			this.move();
			break;
		
		case STATUS_RETURNING:
			var x = this.tx;
			var y = this.ty;

			// Help eyes to reach home in certain checkpoints
			if(this.inIntersection){
				this.x = this.tx<<3;
				this.y = this.ty<<3;
				if(((x == 6)&&(y == 14))&&(this.inIntersection))this.direction = DIR_RIGHT;
				else if((x ==21)&&(y ==14)) this.direction = DIR_LEFT;
				else if((x == 9)&&(y == 5)) this.direction = DIR_DOWN;
				else if((x == 6)&&(y ==20)) this.direction = DIR_UP;
				else if((x ==21)&&(y ==20)) this.direction = DIR_UP;
				else if((x ==18)&&(y == 5)) this.direction = DIR_DOWN;
				else if((x == 9)&&(y ==23)) this.direction = DIR_RIGHT;
				else if((x ==12)&&(y ==23)) this.direction = DIR_UP;
				else if((x ==15)&&(y ==23)) this.direction = DIR_UP;
				else if((x ==18)&&(y ==23)) this.direction = DIR_LEFT;
			}
			this.speed = 2;
			this.targetX = DOOR_X[1];
			this.targetY = DOOR_Y + 1;
			if((this.tx == this.targetX)&&(this.ty == this.targetY)) this.switchMode(STATUS_SCATTER);
			this.move();
			break;
			
		case STATUS_FRIGHTENED:
			var r = [parseInt(Math.random()*3),parseInt(Math.random()*3)];
			this.targetX = this.targets[r[0]].x + (-1 + r[1]);
			this.targetY = this.targets[r[0]].y + (-1 + r[0]);
			
			if(this.game.pacman.energized < this.game.pacman.energyQ/2) this.gfx = GHOST_BLINK; else this.gfx = GHOST_PREY;
			if(this.game.pacman.energized <= 0) this.switchMode(STATUS_SCATTER);
			this.move();
			break;
	}

	if(this.game.pacman.status == PACMAN_DEAD) this.switchMode(STATUS_PAUSE);
}

Ghost.prototype.setChaseTarget = function(){
	switch(this.type){
		case GHOST_BLINKY: 	
			this.targetX = this.game.pacman.tx;
			this.targetY = this.game.pacman.ty;
			break;
		
		case GHOST_PINKY:	
			//Must return bx-px*2 , by-py*2, the position on double vector of blinky -> pacman
			this.targetX = this.targetX;
			this.targetY = this.targetY;
			break;
							
		case GHOST_INKY:	
			//Compute up to several vars, the direction pacman is looking, target of blinky
			this.targetX = this.targetX;
			this.targetY = this.targetY;
			break;
							
		case GHOST_CLYDE:	
			this.targetX = this.targetX;
			this.targetY = this.targetY;
			break;
	}
}

Ghost.prototype.isSolid = function(x,y){
	var map = this.game.map.map.layers[0].data;
	var w = this.game.map.map.width;
	var c = map[parseInt( ( y * w) + x)];
	
	return c < 31;//31 is the door!
}

function targetCompare(a, b){
	return a.distance > b.distance;
}

Ghost.prototype.nearestTarget = function(){	
	var i;
	// Update targets before doing anything
	this.calcNext();

	/*$('#debug_0').text(	this.targets[0].asString() + " - " + 
						this.targets[1].asString() + " - " + 
						this.targets[2].asString() + " - " + 
						this.targets[3].asString());*/
	this.targets.sort(targetCompare);
	/*$('#debug_1').text(	this.targets[0].asString() + " - " + 
						this.targets[1].asString() + " - " + 
						this.targets[2].asString() + " - " + 
						this.targets[3].asString());
	*/
	for(i=0; i<4; i++) {
		if(this.targets[i].direction == DirOpposite[this.direction]) this.targets[i].viable = false;
	}
	for(i=0; i<4; i++) {
		if(this.targets[i].viable) {this.targets[i].color = "#fff000"; return this.targets[i];}
	}
	for(i=0; i<4; i++) {
		if(!this.targets[i].solid) {this.targets[i].color = "#ff00ff"; return this.targets[i];}
	}
	return false;
}

Ghost.prototype.move = function(){	
	// Animate
	GhostGfx[this.gfx][this.direction].update();
	
	this.setSpeed();

	if(!this.canWalk(this.direction)){
		this.moveTo(this.x, this.y);
	}
	
	if(this.canWalk(this.direction)){
		// switch(this.direction){
			// case DIR_LEFT:	this.moveTo(this.rx-this.speed,this.ty<<3); break;
			// case DIR_RIGHT:	this.moveTo(this.rx+this.speed,this.ty<<3); break;
			// case DIR_UP: 	this.moveTo(this.tx<<3,this.ry-this.speed); break;
			// case DIR_DOWN:	this.moveTo(this.tx<<3,this.ry+this.speed); break;			
		// }
		switch(this.direction){
			case DIR_LEFT:	this.moveTo(this.rx-this.speed,parseInt((this.ry+this.speed)/8)*8); break;
			case DIR_RIGHT:	this.moveTo(this.rx+this.speed,parseInt((this.ry+this.speed)/8)*8); break;
			case DIR_UP: 	this.moveTo(parseInt((this.rx+this.speed)/8)*8,this.ry-this.speed); break;
			case DIR_DOWN:	this.moveTo(parseInt((this.rx+this.speed)/8)*8,this.ry+this.speed); break;			
		}
	} else {
		console.log(" Cant go " + DirNames[this.direction]);
						
	}
		
	if(this.inTunnel) return;
		
	if(this.inIntersection){
		//this.centerInTile();
		var nearest = this.nearestTarget();
		if(nearest.direction != this.direction) {
			//console.log("Turning " + DirNames[nearest.direction]);				
			
		}			
		
		
		if(this.canWalk(nearest.direction)) { 
			this.direction = nearest.direction;			
		} else {
			console.log("Going " + DirNames[this.direction]+" cant turn "+DirNames[nearest.direction]+
						" from "+this.tx+","+this.ty+" to "+nearest.x+","+nearest.y);
		}
	} 
}

Ghost.prototype.canWalk = function(direction){
	
	// Straight line over the ghost house
	if(this.inFrontOfHouse) {	
		if(direction == DIR_UP) return false;
		if((direction == DIR_RIGHT)||(direction == DIR_LEFT)) return true;
		// Ignore IA If not on RETURNING status when being in front of ghost house
		if(this.mode != STATUS_RETURNING) return false;
		return true;
	}
	
	if(this.inHouse) return true;		
	
	var map = this.game.map.map.layers[0].data;
	var row	= this.game.map.map.width;
	var r 	= (this.ty * row) + this.tx;

	var hInt = (this.x % 8) == 0;
	var vInt = (this.y % 8) == 0;
	
	
	switch(direction){
		case DIR_RIGHT:	return ((map[r + 1] 							> 30)&&(vInt)) || this.inTunnel;
		case DIR_DOWN:	return ((map[((this.ty+1) * row) + this.tx]	> 30)&&(hInt)) || this.inTunnel;
		case DIR_LEFT:	return ((map[r - 1]								> 30)&&(vInt)) || (!hInt) || this.inTunnel;
		case DIR_UP: 	return ((map[((this.ty-1) * row) + this.tx]	> 30)&&(hInt)) || (!vInt) || this.inTunnel;
	}
}


// Updates Ghost internal flags. TBC initially and each time the ghost moves
Ghost.prototype.updateFlags = function(){
	var	tx	= this.tx, 
		ty	= this.ty,
		x	= this.x,
		y 	= this.y,
		cx = ((this.rx - this.x) + (this.x%8)),
		cy = ((this.ry - this.y) + (this.y%8));
	
	this.inIntersection 	= ( ((cx>8-this.speed)||(cx<0+this.speed)) && ((cy>8-this.speed)||(cy< 0+this.speed)) );
	this.inHouse			= (((tx >= 11)&&(ty >= 12))&&((tx <= 16)&&(ty <= 15)));
	this.inDoor				= ((ty == DOOR_Y)&&((tx >= DOOR_X[0])&&(tx <= DOOR_X[1])))&&((this.x % 8)==0);
	this.behindDoor			= ((((y % 8)==0))&&((ty == DOOR_Y+1)&&((tx >= DOOR_X[0])&&(tx <= DOOR_X[1]))));
	this.inFrontOfDoor		= ((ty == DOOR_Y-1)&&((tx >= DOOR_X[0])&&(tx <= DOOR_X[1])));
	this.inFrontOfHouse		= ((y % 8 == 0) && ((ty == DOOR_Y - 1) && (tx >= DOOR_X[0]-1) && (tx <= DOOR_X[1]+1)));
	this.inTunnel			= ( (this.tx <= 0)||(this.tx >= this.game.map.map.width-1)||(this.ty >= this.game.map.map.height-1)||(this.ty <= 0) );
	this.mustLeave			= (this.timer >= GhostDelays[this.type]);
}

Ghost.prototype.calcNext = function(){	
	this.targets[0].set(this.tx, 	this.ty-1);
	this.targets[1].set(this.tx-1, this.ty);
	this.targets[2].set(this.tx, 	this.ty+1);
	this.targets[3].set(this.tx+1,	this.ty);
	return;	
}

Ghost.prototype.moveTo = function(x,y){
	var ix 		= parseInt(x);
	var iy 		= parseInt(y);
	
	this.game.mapWidth = this.game.map.map.width * 8;
	this.game.mapHeight= this.game.map.map.height * 8;
	
	// Clamp and set real X and Y
	if(iy > this.game.mapHeight	) this.ry = iy - this.game.mapHeight - 8; 	
	else if(iy < -8				) this.ry = iy + this.game.mapHeight + 8; 	
	else this.ry = y;
	if(ix > this.game.mapWidth	) this.rx = ix - this.game.mapWidth - 8; 	
	else if(ix < -8				) this.rx = ix + this.game.mapWidth + 8; 	
	else this.rx = x;	
	// Set rounded position (for use in drawing and calculations)
	
	this.x		= parseInt(this.rx);	
	this.y		= parseInt(this.ry);		
	// Set relative position (for use in IA)
	this.tx 	= parseInt(this.x / 8);
	this.ty		= parseInt(this.y / 8);
	
	this.updateFlags();
}

Ghost.prototype.setTargets = function(x, y, ox, oy){
	this.targets[0].set(x[0], y[0]);
	this.targets[1].set(x[1], y[1]);
	this.targets[2].set(x[2], y[2]);
	this.targets[3].set(x[3], y[3]);
}

Ghost.prototype.switchMode = function(mode){		

	switch(mode){
		case STATUS_CHASE:
			this.chaseTime = 0;
			this.gfx 	= this.type;
			this.reverse();
			break;
		
		case STATUS_SCATTER:
			this.scaterTime = 0;
			this.gfx	= this.type;
			this.reverse();
			break;

		case STATUS_FRIGHTENED:
			this.gfx	= GHOST_PREY;
			this.reverse();
			break;

		case STATUS_RETURNING:
			this.gfx	= GHOST_DEAD;
			this.reverse();
			break;
	}
	
	this.mode = mode;
}

Ghost.prototype.reverse = function(){	
	switch(this.direction){
		case DIR_RIGHT:	this.direction = DIR_LEFT;	break;
		case DIR_DOWN:	this.direction = DIR_UP;	break;
		case DIR_LEFT:	this.direction = DIR_RIGHT;	break;
		case DIR_UP: 	this.direction = DIR_DOWN;	break;
	}		
}

Ghost.prototype.setSpeed = function(){
	if(this.mode == STATUS_RETURNING){ return; }
	/** Adjust speed **/
	if(this.game.level == 0){
		if(this.mode == STATUS_FRIGHTENED) this.speed = 0.5; else 
		if(this.inTunnel) this.speed = 0.4; else this.speed = 0.75;
	} else if(this.game.level < 4){
		if(this.mode == STATUS_FRIGHTENED) this.speed = 0.55; else 
		if(this.inTunnel) this.speed = 0.45; else this.speed = 0.85;
	} else if(this.game.level < 19){
		if(this.mode == STATUS_FRIGHTENED) this.speed = 0.6; else 
		if(this.inTunnel) this.speed = 0.5; else this.speed = 0.95;
	} else {
		if(this.inTunnel) this.speed = 0.5; else this.speed = 0.95;
	}
}