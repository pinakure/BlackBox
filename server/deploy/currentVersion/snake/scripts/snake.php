<?php include "config.php"; ?>;
var cells = [];
var cellX=0, cellY=0;

var score = 0;
var snake = [];
var initlen;
var snakelen = initlen;
var nextdir = 'u';
var snakedir = 'u';
var snakemoved = false;
var snakeorient = 'vertical';
var snakepause = false;

function snakePart(x,y){
	this.x = x;
	this.y = y;
	this.o = snakeorient;
}

function initSnake(length){
	initlen = length;
	snake = "";
	snake = [];
	for(i=0; i<initlen; i++){
		snake[i] = new snakePart(<?=intval($width/2)?> , <?=intval($height/2)?> + (length - i) );
		cells[snake[i].y][snake[i].x] = "snake";
	}
	
	snakelen = initlen;
}

function initCells(){
	for(y=0; y < <?=$height?>; y++){
		cells[y] = [];
		for(x=0; x < <?=$width?>; x++){
			cells[y][x] = false;
		}
	}
}

function cellOn(x, y, color){
	// if(!color)color = "transparent";
	if(!color)color = "ground";
	$('#cell_'+y+'_'+x).removeClass('ground snake apple vertical horizontal  nexus up-left up-right down-left down-right left-up right-up left-down right-down');
	$('#cell_'+y+'_'+x).toggleClass(color);
	
}

function cellOff(x, y){
	$('#cell_'+y+'_'+x).removeClass('snake apple ground vertical horizontal nexus up-left up-right down-left down-right  left-up right-up left-down right-down');
	$('#cell_'+y+'_'+x).toggleClass('ground');
}


function renderCells(){				
	for(y=0; y<<?=$height?>; y++){
		for(x=0; x<<?=$width?>; x++){
			if(cells[y][x]) cellOn(x,y,cells[y][x]); else cellOff(x,y);
		}
	}
}	

function genDot(){
	var x = 1+parseInt(Math.random()*<?=$width-2?>);
	var y = 1+parseInt(Math.random()*<?=$height-2?>);
	cells[y][x] = 'apple';
	
	for(i=0; i<snakelen; i++){
		if((snake[i].x == x) && (snake[i].y == y)) return genDot();
	}
}

function addScore(x){
	score += x;
	$('#score').text(score)
}
function setScore(x){
	score = x;
	$('#score').text(score)
}

function snakeReset(){				
	initCells();
	
	initSnake(4);

	nextdir = 'u';
	snakedir = 'u';
	snakeorient = 'vertical';
	
	setScore(0);
	
	$('body').find('.cell').each(function(i){
		$(this).removeClass("snake apple ground vertical horizontal nexus up-left up-right down-left down-right  left-up right-up left-down right-down");
		$(this).toggleClass('ground');
	});
	
	/*level*/
	genDot();				
}

function snakeMoveTo(x, y){				

	/*check self collission*/
	for(i=0; i < snakelen; i++){
		for(t=0; t < snakelen; t++){
			/*skip self check!*/
			if(i==t)continue
			if((snake[t].x == snake[i].x)&&(snake[t].y == snake[i].y)){
				snakeReset();
				return;
			}
		}
	}
	
	/*lose 'energy'*/
	if(score > 0)setScore(score-1);
	
	/* handle border wrap*/
	if(y < 0) y = <?=$height-1?>;
	if(y >= <?=$height?>) y = 0;
	if(x < 0) x = <?=$width-1?>;
	if(x >= <?=$width?>) x = 0;
	
	/*erase from cell memory*/
	for(i=0; i<snakelen; i++){
		cells[snake[i].y][snake[i].x] = false;
	}
	
	/*move body*/
	for(i=0; i < snakelen-1; i++){
		snake[i].x = snake[i+1].x;
		snake[i].y = snake[i+1].y;
		snake[i].o = snake[i+1].o;
	}
	
	/*move head */
	snake[snakelen-1].x = x;
	snake[snakelen-1].y = y;
	if((snakeorient!='vertical')&&(snakeorient!='horizontal')){
		snake[snakelen-2].o = snakeorient;
		snakeorient = ((snakedir == 'u') || (snakedir == 'd')) ? 'vertical':'horizontal';
	}
	snake[snakelen-1].o = snakeorient;
	
	/*grow*/
	if(cells[y][x]){
		addScore(100);
		
		var newx = snake[snakelen-2].x - snake[snakelen-1].x;
		var newy = snake[snakelen-2].y - snake[snakelen-1].y;
		var tx = snake[snakelen-1].x - newx;
		var ty = snake[snakelen-1].y - newy;
		snake[snakelen] = new snakePart(tx,ty); 
		snakelen++;
		
		/*gen new dot*/
		genDot();
	}				
	
	
	/*draw on cell memory*/
	for(i=0; i<snakelen; i++){
		cells[snake[i].y][snake[i].x] = "snake "+snake[i].o;
	}
	snakemoved = false;
}

function snakeNotifyScore(name, score){
	$.post('index.php', {name: name, score:score, key:'<?=$_SESSION['key']?>'}, function(data){
		console.log(data);
	});
}

function snakeWalk(){				
	if(snakepause)return;

	switch(snakedir){
		case 'u':	snakeMoveTo(snake[snakelen-1].x, 	snake[snakelen-1].y-1);	break;						
		case 'l':	snakeMoveTo(snake[snakelen-1].x-1,	snake[snakelen-1].y);	break;						
		case 'd':	snakeMoveTo(snake[snakelen-1].x, 	snake[snakelen-1].y+1); break;						
		case 'r':	snakeMoveTo(snake[snakelen-1].x+1, 	snake[snakelen-1].y); 	break;
	}
	
	renderCells();
}

$(function() {
	snakeReset();				
	
	$('body').keydown(function (key) {
		if(snakemoved)return;
			snakemoved = true;
		switch(key.which){
			case 38: nextdir = 'u'; break;
			case 40: nextdir = 'd'; break;
			case 37: nextdir = 'l'; break;
			case 39: nextdir = 'r'; break;					
			case 13: snakemoved = false; snakepause ^= 1;break;
		}	

		lastorient = ((snakedir == 'u') || (snakedir == 'd')) ? 'vertical':'horizontal';
		lastdir = snakedir;
		
		switch(snakedir){
			case 'u':case 'd': snakedir = (nextdir == 'l'?'l':nextdir == 'r'?'r':snakedir);break;
			case 'l':case 'r': snakedir = (nextdir == 'u'?'u':nextdir == 'd'?'d':snakedir);break;
		}
		
		snakeorient = ((snakedir == 'u') || (snakedir == 'd')) ? 'vertical':'horizontal';
		if(snakeorient != lastorient){
			snakeorient = 'nexus ';
				  if((lastdir == 'u') && (snakedir == 'l'))snakeorient = 'nexus up-left' 
			else if((lastdir == 'u') && (snakedir == 'r'))snakeorient = 'nexus up-right' ;
			else if((lastdir == 'd') && (snakedir == 'l'))snakeorient = 'nexus down-left' ;
			else if((lastdir == 'd') && (snakedir == 'r'))snakeorient = 'nexus down-right' ;
			else if((lastdir == 'l') && (snakedir == 'd'))snakeorient = 'nexus left-down' ;
			else if((lastdir == 'l') && (snakedir == 'u'))snakeorient = 'nexus left-up' ;
			else if((lastdir == 'r') && (snakedir == 'd'))snakeorient = 'nexus right-down' ;
			else if((lastdir == 'r') && (snakedir == 'u'))snakeorient = 'nexus right-up' ;
		}
		
	});		
	
	setInterval(snakeWalk, <?=$speed?>);								
});
