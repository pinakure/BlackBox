from entity import Entity



class Bonus(Entity):
    game        = None

    @staticmethod
    def initialize(game):
        Bonus.game = game

    def __init__(self, bonus_type=0x00):
        self.bonus_type = bonus_type


"""
var BonusGfx = false;

var BONUS_X = (13 * 8);
var BONUS_Y = (17 * 8) - 4;

function Bonus(game){
	this.game = game;
	this.self = this;
	this.x = BONUS_X;
	this.y = BONUS_Y;
	this.tx = parseInt(BONUS_X/8);
	this.ty = parseInt(BONUS_Y/8);

	this.score = 1000;
	
	this.isAlive 	= false;
	this.eaten 		= false;
	this.timeout	= 100;
	
	this.sprite = false;
	if(!BonusGfx)this.loadGraphics();
}

Bonus.prototype.reset = function(){
	//TODO: Read BONUS_X and BONUS_Y from Map entity
	this.x = BONUS_X;
	this.y = BONUS_Y;
	this.tx = 13;
	this.ty = 17;

	this.score = 400 + (100* (this.game.level + 1));

	BonusGfx.setCurrentFrame(this.game.level % BonusGfx.frameCount);
	
	this.isAlive 	= false;
	this.eaten 		= false;
	this.timeout	= 400 + (50 * (this.game.level+1)) + parseInt(Math.random()*50);	
}

Bonus.prototype.loadGraphics = function(){
	info("BONUS", "Requesting graphic data");
	try {            
		this.sprite = new SpriteSheet(this.game.engine, "gfx/bonus.png", 16, 16, [255,0,255], this.self);
	} catch(e) {
		complaint('BONUS', 'Failed to load graphics', e);
		BonusGfx = false;
	}
}

Bonus.prototype.processGraphics = function(me){
	info("BONUS", "Processing graphic data");

	BonusGfx = [];
	var x,y,i=0;
	var totalFrames = 0;
	BonusGfx = new Animation(	me.sprite, 
								0,  0, 
								7, 13, 
								true, 1, false);
	
	totalFrames = 	BonusGfx.frameCount;
	
	me.reset();
	
	cout('^3'+totalFrames+'^8 frames extracted');	
}


Bonus.prototype.update = function(){
	if(this.eaten)return;
	if(!this.isAlive){
		if(this.timeout) this.timeout--;
		else this.isAlive = true;
		return;
	}
	if((this.game.pacman.ty == this.ty)&&( (this.game.pacman.tx == this.tx) || (this.game.pacman.tx == this.tx+1) )){
		this.game.addScore(this.score);
		this.eaten	= true;
		this.isAlive= false;
	}
}

Bonus.prototype.render = function(){
	if(this.isAlive){
		BonusGfx.draw(this.x, this.y+24);
	}
}
"""