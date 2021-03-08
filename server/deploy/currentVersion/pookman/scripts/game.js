/*

CHANGES:
-----------------------------------
30-3-16 : 	Speed for pacman and ghosts in every mode setActive
			CanWalk bugFix : Not more wallthrough!

*/


var	ACTION_DEBUG_TEST_INVENTORY	= false,
	ACTION_DEBUG_NEXT_SONG 		= false,
	ACTION_DEBUG_ABORT 			= false,
	ACTION_ACTIVATE 			= false,
	ACTION_NEXT_POWERUP 		= false,
	ACTION_PREV_POWERUP 		= false,
	ACTION_MOVE_LEFT 			= false,
	ACTION_MOVE_RIGHT 			= false,
	ACTION_MOVE_UP 				= false,
	ACTION_MOVE_DOWN 			= false,
	ACTION_CANCEL 				= false,
	ACTION_PAUSE 				= false;

var debug = [];
var	DEBUG_PACMAN 	= 0x00,
	DEBUG_GHOSTS 	= 0x01,
	DEBUG_SLOWMO 	= 0x02,
	DEBUG_GAME		= 0x03;
	
debug[DEBUG_PACMAN] = false;
debug[DEBUG_GHOSTS] = false;
debug[DEBUG_GAME] 	= false;
debug[DEBUG_SLOWMO] = false;


var DOOR_X = [13, 14];
var DOOR_Y = 12;


function main(){
	info('JSEngine', 'Initializing');
	
	game = new Game(224, 288, true, 1);
}

Game.prototype.addScore = function(score){	
	this.score += score;
	$('#score').html((("00000000"+(this.score.toString())).substr(-8)));
}

Game.prototype.continueLoad = function(){	
	try {
		setTimeout(update, 15);
		cout("^6------------------- Starting new game --------------------");
	} catch (e){
		complaint('GAME', 'EXCEPTION RAISED', e);
	}
}

Game.prototype.resetInput = function(){ 
	ACTION_DEBUG_ABORT 		= false;
	ACTION_DEBUG_NEXT_SONG 	= false;
	ACTION_CANCEL 			= false;
	ACTION_PAUSE 			= false;        
}

Game.prototype.handleInput = function(){
	//if(this.demo.playback)return;
	this.engine.input.update();
	this.resetInput();
	var input = this.engine.input;
	
	// Handle movement
	if(input.keyPress[input.KEY_A]) this.pacman.go(DIR_LEFT);	else 
	if(input.keyPress[input.KEY_D]) this.pacman.go(DIR_RIGHT);	else 
	if(input.keyPress[input.KEY_W]) this.pacman.go(DIR_UP);		else 
	if(input.keyPress[input.KEY_S]) this.pacman.go(DIR_DOWN);	
			
	if(input.keyDown[input.KEY_X]) debug[DEBUG_SLOWMO] ^= 1;
	
	if(input.keyDown[input.KEY_N]) this.pacman.spawn();	
	
	if(input.keyDown[input.KEY_P]		) debug[DEBUG_PACMAN] ^= 1;		else 	
	if(input.keyDown[input.KEY_O]		) debug[DEBUG_GAME] ^= 1;		else 	
	if(input.keyDown[input.KEY_I]		) debug[DEBUG_GHOSTS] ^= 1;		else 	
	
	
	if(input.keyDown[input.KEY_1]		) this.setMode(MODE_GAME_ARCADE);		else 
	if(input.keyDown[input.KEY_2]		) this.setMode(MODE_GAME_TIMEATTACK); 	else 
	if(input.keyDown[input.KEY_3]		) this.setMode(MODE_GAME_PUZZLE); 		else 
	if(input.keyDown[input.KEY_4]		) this.setMode(MODE_GAME_VERSUS); 		else
	if(input.keyDown[input.KEY_G]		) this.setMode(MODE_CURTAIN_GAMEOVER);	else 
	if(input.keyDown[input.KEY_M]		) this.setMode(MODE_MENU);				else 
	if(input.keyDown[input.KEY_END]		) ACTION_DEBUG_ABORT 		= true;	
	//if(input.keyDown[input.KEY_N]    	) ACTION_DEBUG_NEXT_SONG  	= true;
	if(input.keyDown[input.KEY_F1]    	) this.nextMap();

	if(input.keyDown[input.KEY_RIGHT]){	ACTION_BALLTIME 	= true; 
										ACTION_FORCE_TILT 	= true; 
										ACTION_TILT_RIGHT 	= true; } else
	if(input.keyDown[input.KEY_LEFT] ){	ACTION_BALLTIME 	= true; 
										ACTION_FORCE_TILT 	= true; 
										ACTION_TILT_LEFT  	= true; }	
	if(input.keyDown[input.KEY_DOWN] ){	ACTION_BALLTIME 	= true; 
										ACTION_FORCE_TILT 	= true; 
										ACTION_TILT_DOWN 	= true; } else
	if(input.keyDown[input.KEY_UP]   ){	ACTION_BALLTIME 	= true; 
										ACTION_FORCE_TILT 	= true; 
										ACTION_TILT_UP   	= true; }
	
	if(input.keyDown[input.KEY_SPACE] ) ACTION_ACTIVATE = true;
	if(input.keyDown[input.KEY_ESCAPE]) ACTION_CANCEL   = true;
}

Game.prototype.loadGraphics = function(){
	new Menu().loadGraphics();
	new ParticleSystem().loadGraphics();
	
	for(var i=0; i<FONT_FILES.length; i++){
		new Display(0,0,0,0,i).loadGraphics();
	}
}

var	MODE_GAME_ARCADE 		= 10,
	MODE_GAME_TIMEATTACK 	= 11,
	MODE_GAME_VERSUS 		= 12,
	MODE_GAME_PUZZLE 		= 13,
	MODE_CURTAIN_ROUNDCLEAR = 20,
	MODE_CURTAIN_BONUSCOINS = 21,
	MODE_CURTAIN_ENTERLEVEL = 22,
	MODE_CURTAIN_GAMEOVER 	= 23,
	MODE_MENU 				= 0,
	MODE_EXIT 				= 100;

function Game(width, height, fullScreen, scale){
	this.engine 	= new Engine("Game", width, height, fullScreen, scale);

	this.graphics 	= this.engine.graphics;	
	this.particles 	= this.engine.particles;			
    this.accel 		= this.engine.accel;
	this.maxAccel 	= this.engine.maxAccel;
	this.input 		= this.engine.input;
	this.graphics	= this.engine.graphics;

	this.player 	= "anonymous";	
    this.gameOver 	= false;
	this.cheats 	= false;
    this.map 		= false;	
	this.menu 		= false;	
    this.lives 		= 3;
    this.level 		= 0;
    this.score 		= 0;
	this.gameMode 	= 0;
	this.pills		= 0;
	this.pillsLeft 	= 0;

	this.mapWidth	= 0;
	this.mapHeight	= 0;
	
	this.music		= new MusicPlayer(this);
	this.menu 		= new Menu(this);
	this.demo 		= new Demo(this);
	this.bonus		= new Bonus(this);
	this.pacman		= new Pacman(this);
	this.blinky		= new Ghost(GHOST_BLINKY, this);
	this.inky		= new Ghost(GHOST_INKY, this);
	this.pinky		= new Ghost(GHOST_PINKY, this);
	this.clyde		= new Ghost(GHOST_CLYDE, this);
	this.map 		= new Map(MAP_LIST[0],this);
	
	this.freeze		= 0;
	
    this.mode 		= MODE_GAME_ARCADE;
    this.status 	= 0;
    
	resources.wait(this.continueLoad, this);
}
     
Game.prototype.resetSystems = function(){
	this.engine.particles.reset();
	this.blinky.reset();
	this.pinky.reset();
	this.inky.reset();
	this.clyde.reset();
	this.pacman.ready();	
}

Game.prototype.New = function() {
	cout("^6------------------- Starting new game --------------------");

	this.gameOver 	= false;
	this.cheats 	= false;
    this.map 		= false;	
	this.menu 		= false;	
    this.lives 		= 0;
    this.level 		= 0;
    this.score 		= 0;
	this.gameMode 	= 0;
	this.engine.timeScale = 1.0;
	this.resetSystems();

	this.countPills();
	this.mapWidth	= this.map.map.tileWidth 	* this.map.map.width;
	this.mapHeight	= this.map.map.tileHeight	* this.map.map.height;
	
	
	this.bonus.reset();
	
	this.snapshot();
	
	cout("New game started");		
}   

Game.prototype.closeHouse = function(){
	this.map.set(DOOR_X[0], DOOR_Y, 0x20);
	this.map.set(DOOR_X[1], DOOR_Y, 0x20);
	this.map.redraw = true;
}	

Game.prototype.openHouse = function(){
	this.map.set(DOOR_X[0], DOOR_Y, 0x1F);
	this.map.set(DOOR_X[1], DOOR_Y, 0x1F);
	this.map.redraw = true;
}	
	
Game.prototype.nextMap = function(){
	this.resetSystems();
	this.level = this.level + 1;
	//this.musicplayer.nextTrack();
	this.map.loadMap(MAP_LIST[this.level % (MAP_LIST.length-1)]);
	this.pacman.spawn();
	this.blinky.reset();
	this.inky.reset();
	this.pinky.reset();
	this.clyde.reset();
	this.closeHouse();
	this.bonus.reset();	
}

Game.prototype.setMode = function(mode){
	this.mode = mode;
	this.status = 0;
	this.timeout = 0;
}

Game.prototype.oneUp = function(lives){
	this.lives += lives;        
}

Game.prototype.oneDown = function(lives){
	this.lives -= lives;
	if( this.lives <= 0 ) {
		this.lives = 0;
		this.gameOver = true;		
	}
}
 
Game.prototype.isGhost = function(i) {
	if(this.pacman.status == PACMAN_DIEING) return false;
	else if(this.pacman.status == PACMAN_DEAD) return false;
	
	var w = this.map.map.width;
	var h = this.map.map.height;
	var pos = [];
	
	pos[GHOST_BLINKY]	= (this.blinky.ty	* w) + this.blinky.tx;
	pos[GHOST_PINKY]	= (this.pinky.ty 	* w) + this.pinky.tx;
	pos[GHOST_INKY] 	= (this.inky.ty 	* w) + this.inky.tx;
	pos[GHOST_CLYDE]	= (this.clyde.ty 	* w) + this.clyde.tx;
	
	return	((this.blinky.mode	< STATUS_RETURNING)&&(pos[GHOST_BLINKY] == i))||
			((this.pinky.mode	< STATUS_RETURNING)&&(pos[GHOST_PINKY ] == i))||
			((this.inky.mode 	< STATUS_RETURNING)&&(pos[GHOST_INKY  ] == i))||
			((this.clyde.mode	< STATUS_RETURNING)&&(pos[GHOST_CLYDE ] == i));
}
	
Game.prototype.isPill = function(i) {	
	return this.map.map.layers[0].data[i] == 33;
}

Game.prototype.isEnergizer = function(i) {	
	return this.map.map.layers[0].data[i] == 34;
}



Game.prototype.countPills = function() {	
	this.pillsLeft = 0;
	for(var i=0; i<this.map.map.layers[0].data.length; i++){
		if(this.map.map.layers[0].data[i] == 33)this.pillsLeft++;
	}
	if(this.pillsLeft==0) {
		this.pillsLeft = -1;
		this.nextMap();
	}
}

Game.prototype.update = function(delta) {	
	this.handleInput();

	
	
	// Freeze game while kill sound is playing
	if(this.freeze <= 0){
		this.bonus.update();
		// Delay pacman a frame when it eats 
		if(!this.pacman.eating){	
			this.pacman.update(delta);
		} else {
			this.countPills();
			this.pacman.eating = false;
		}
		
		this.blinky.update(delta);
		this.inky.update(delta);
		this.pinky.update(delta);
		this.clyde.update(delta);
		if(this.pinky.mustLeave)this.openHouse();
		else this.closeHouse();
		
		if(debug[DEBUG_SLOWMO]) this.freeze = 10;
		
	} else this.freeze--;
	
	// var	t = parseInt(Math.random() * ParticleGfx.length),
	// x = parseInt(Math.random() * this.engine.width),
	// y = parseInt(Math.random() * this.engine.height);	
	//this.engine.particles.update(delta);
}

Game.prototype.render = function() {

	this.engine.graphics.setContext(0);
	
	// Draw lives
	if(this.map.redraw){
	for(var i=0; i<this.lives; i++){
		PacmanGfx[DIR_RIGHT].draw(i<<4, 272);
	}
	
	// Draw current level (bonus)
	for(var i=0, li = (this.level > 2) ? 3 : (this.level+1); i<li; i++){
		BonusGfx.setCurrentFrame(this.level-((li-1)-i));
		BonusGfx.draw(208 - (i<<4), 272);
	}
	}
	
	this.map.render(this.engine.graphics);
	this.bonus.render();
	//this.game.engine.graphics.setContext(1);
	//this.engine.particles.render(this.engine.graphics);
	PacmanGfx[DIR_RIGHT].setCurrentFrame(9);
	
	this.engine.graphics.setContext(2);
	this.engine.graphics.clear();
	this.pacman.render(this.engine.graphics);
	
	this.engine.graphics.setContext(3);
	this.engine.graphics.clear();
	
	if(this.pacman.status != PACMAN_DIEING) {
		this.blinky.render(this.engine.graphics);	
		this.inky.render(this.engine.graphics);
		this.pinky.render(this.engine.graphics);
		this.clyde.render(this.engine.graphics);
	}
}