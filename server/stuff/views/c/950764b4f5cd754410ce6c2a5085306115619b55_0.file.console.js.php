<?php /* Smarty version 3.1.24, created on 2021-02-26 00:01:35
         compiled from "C:/codigo/BlackBox/server/stuff/include/console.js" */ ?>
<?php
/*%%SmartyHeaderCode:95902639660383a5fbd7c79_19915141%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '950764b4f5cd754410ce6c2a5085306115619b55' => 
    array (
      0 => 'C:/codigo/BlackBox/server/stuff/include/console.js',
      1 => 1452809035,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '95902639660383a5fbd7c79_19915141',
  'variables' => 
  array (
    'width' => 0,
    'tile_width' => 0,
    'height' => 0,
    'tile_height' => 0,
  ),
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_60383a5fd523d0_99434198',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_60383a5fd523d0_99434198')) {
function content_60383a5fd523d0_99434198 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '95902639660383a5fbd7c79_19915141';
?>
/*--------------------------------------------------------------------------------------------------------------------------------
   CALLBACKS
--------------------------------------------------------------------------------------------------------------------------------*/
// CONSOLE COMMAND CALLBACKS
function command_cls(args){
	vpu.console.clean();
	vpu.console.dump();
	vpu.clean();	
}

function command_echo(args){
	vpu.console.print(args.join('\0'));
	vpu.console.dump();	
}

function command_help(args){
	var commandList = vpu.console.commandList;	
	var help = "";
	var i = 0;
	
	if((args[0] == 'toc') 	||
	   (args[0]== '-toc') 	||
	   (!args[0]) 	  		||
	   (args[0] == '') 	  	){
		vpu.console.print("^1----------------------");
		vpu.console.print("^1Available command list");
		vpu.console.print("^1----------------------");
		for(;i<commandList.length;i+=4){
			vpu.console.print("^2"+commandList[i]+"^6 - ^d" + commandList[i+3]);
		}
		return;
	}
	
	for(;i<commandList.length;i+=4){
		if(commandList[i]==args[0]){
			help = commandList[i+3];
			break;
		}
	}
	if(help != ""){
		vpu.console.print("Displaying help for ^6" + args[0]);
		vpu.console.print(help);
	} else {
		vpu.console.print("^5No help found for ^3" + args[0]);
	}
	
	//TODO: Look in the database for existing programs
	
	vpu.console.dump();
}

function command_viewport(args){
	vpu.loadMapData();
	vpu.redraw();
}


function command_todo(args){
	var todo = [ 	"^3-fix double print long lines issue, first one overwrited!",
					"^3-fix color codes computing as displayable chars!",
					"^3-fix color code dissapearing when new line!",
					"^7-proper todo function",
					"^7-finish business project about this shit"
				];
				
	vpu.console.print("^d-------------");
	vpu.console.print("^3BUG^d/^7TODO^2 ^6LIST");
	vpu.console.print("^d-------------");
	for(i=0; i<todo.length;i++){ 
		vpu.console.print(todo[i]);
	}
}

function command_reload(args){
	vpu.console.print("^9Reloading with ^1fresh cache^9...");
	window.location.assign("?clean=1");
}

function command_fire(args){
	vpu.console.print("^9Reloading with ^1fire^9...");
	var q = args.join(" ");
	if(q == "") q = "25";
	window.location.assign("?fire=0."+q);
}

function command_program(args){
	vpu.dataRequest("program "+args.join(' '));	
}

function command_modplay(args){
	vpu.dataRequest("program modplay "+args.join(' '));
}

function command_container(args){
	vpu.dataRequest("container "+args.join(' '));
}

function command_kill(args){
	var containers = vpu.system.containers;
	var e = containers.length;
	for(i=0;i<e;i++){
		if(args[0] == containers[i].name){
			containers.splice(i, 1);
			return;
		}
	}
	//var programs = vpu.system.programs;
	//var e = programs.length;
	//for(i=0;i<e;i++){
	//if(args[0] == programs[i].name){
		//programs.splice(i, 1);
	//	return;
	//}
}


/*--------------------------------------------------------------------------------------------------------------------------------
   _____                      _         _____ _               
  / ____|                    | |       / ____| |              
 | |     ___  _ __  ___  ___ | | ___  | |    | | __ _ ___ ___ 
 | |    / _ \| '_ \/ __|/ _ \| |/ _ \ | |    | |/ _` / __/ __|
 | |___| (_) | | | \__ \ (_) | |  __/ | |____| | (_| \__ \__ \
  \_____\___/|_| |_|___/\___/|_|\___|  \_____|_|\__,_|___/___/
--------------------------------------------------------------------------------------------------------------------------------*/
// CONSOLE CLASS AND METHODS
// SINGLETON - refer to it by vpu.console() to work safe
function Console(vpu){
	this.vpu = vpu;
	this.redraw = true;
	this.buffer = [];
	this.bufferSize= 5; // 5 times the visible screen
	this.lines = [];
	this.lineCount = 0;
	this.currentLine = ""; 
	this.lineCursor = -1; //Negative means currentLine, any higher value will correspond with a index inside buffer[] array
	
	this.init();
	this.dump();

	this.disabled = false;
	
	this.input = 0;
	
	this.commandList = [];
	
	this.history = [];
	this.historyCursor = 0;
	
	this.clock = 0;
	setInterval(this.tick, 100);
	
	this.addCommand("cls", 0, command_cls, "^7Clear^6 the screen");
	this.addCommand("echo", 1, command_echo, "^7Print^6 passed text on the console");
	this.addCommand("help", 0, command_help, "^7Show^6 command ^7help^6 information");//+commandlist
	this.addCommand("fire", 0, command_fire, "^7Reload ^6with fire effect");//+commandlist
	this.addCommand("todo", 0, command_todo, "^7Shows^6 pending project tasks");//+commandlist
	this.addCommand("buglist", 0, command_todo, "^7Shows^6 current discovered bugs on the project");//+commandlist
	this.addCommand("viewport", 0, command_viewport, "^7Change ^6 current viewport");//+commandlist
	this.addCommand("program", 1, command_program, "^7Request ^6given program to the server");//+commandlist
	this.addCommand("reload", 0, command_reload, "^7Reloads^6 a clean instance of the program");//+commandlist
	this.addCommand("modplay", 0, command_modplay, "^7Start^6 mod player");//+commandlist
	this.addCommand("container", 0, command_container, "^7Test ^6 container object");//+commandlist
	this.addCommand("kill", 0, command_kill, "^7Kill^6 container or program by name");//+commandlist
	
}

Console.prototype.addCommand = function(name, minArgs, callback, helpData){
	var i= this.commandList.length;
	this.commandList[i] = name;
	this.commandList[i+1] = minArgs;
	this.commandList[i+2] = callback;
	this.commandList[i+3] = helpData;
}

Console.prototype.backspace = function(){
	if(this.currentLine.length > 0)this.currentLine = this.currentLine.substr(0, this.currentLine.length-1);
	this.redraw = true;
	vpu.console.dump();
}

Console.prototype.historyPrevious = function(){
	if(this.historyCursor > 0) {
		this.historyCursor--;
		this.currentLine = this.history[this.historyCursor];
		this.redraw=true;
		this.dump();
	}
}


Console.prototype.historyNext = function(){
	if(this.historyCursor < this.history.length-1) {
		this.historyCursor++;
		this.currentLine = this.history[this.historyCursor];
		this.redraw=true;
		this.dump();
	}
}

Console.prototype.parseCommandLine = function(){
	var command = vpu.console.currentLine;
	var args = command.split("\0");
	
	/* Get first readable word after first spaces found (if any) */
	var commandLine = "";
	while(args.length > 0){
		commandLine = args.shift();
		if(commandLine != "")break;
		
	}
	// Exit if command line was just a few spaces and no command was found
	if(commandLine == "")return;
	
	var notFound = true;
	for(x=0; x<this.commandList.length; x+=4){
		if(this.commandList[x] == commandLine.toLowerCase()){
			vpu.console.currentLine = "";
			if(args.length < this.commandList[x+1]){				
				vpu.color = 0x7;
				vpu.console.print("Help for '"+commandLine+"' : "+this.commandList[x+3] + ".");
				vpu.console.print("This program takes"+(this.commandList[x+1]>0? " " + this.commandList[x+1] + " argument"+(this.commandList[x+1]>1?"s":""):" no arguments")+".");
			} else this.commandList[x+2](args);
			
			this.history[this.history.length] = command;
			this.historyCursor = this.history.length;
			notFound = false;
		}
	}
	
	if(notFound) vpu.console.print("^3Syntax error : ^5'" + commandLine + "'"); 		
	
	vpu.console.currentLine = "";
	this.redraw = true;
	vpu.console.dump();
}

Console.prototype.tick = function(){
	vpu.console.clock++;
}

Console.prototype.addChar = function(c){
	if(vpu.console.disabled)return;
	this.currentLine += c;
	this.redraw = true;
	vpu.console.dump();
}

Console.prototype.drawCursor = function(){
	if(vpu.console.disabled)return;
	
	/*var pos = (this.currentLine.length/<?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
)*<?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
;*/
	var len = this.len(this.currentLine);
	var width = parseInt(<?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
);
	var lines = parseInt(len / width);
	var pos = len - (lines > 0 ? lines * width : 0);
	
	px = pos * <?php echo $_smarty_tpl->tpl_vars['tile_width']->value;?>
;
	py = <?php echo ($_smarty_tpl->tpl_vars['height']->value)-$_smarty_tpl->tpl_vars['tile_height']->value;?>
;
	if(vpu.console.clock&7 > 3)vpu.ctx.drawImage(vpu.data0,2*<?php echo $_smarty_tpl->tpl_vars['tile_width']->value;?>
, <?php echo $_smarty_tpl->tpl_vars['tile_height']->value;?>
<<0,<?php echo $_smarty_tpl->tpl_vars['tile_width']->value;?>
,<?php echo $_smarty_tpl->tpl_vars['tile_height']->value;?>
,px>>1,py,<?php echo $_smarty_tpl->tpl_vars['tile_width']->value;?>
>>1,<?php echo $_smarty_tpl->tpl_vars['tile_height']->value;?>
);
}

Console.prototype.dump = function() {
	if(vpu.console.disabled || !this.redraw)return;
	this.redraw = false;	
	var i;
	for(i=<?php echo ($_smarty_tpl->tpl_vars['width']->value/$_smarty_tpl->tpl_vars['tile_width']->value)*($_smarty_tpl->tpl_vars['height']->value/$_smarty_tpl->tpl_vars['tile_height']->value);?>
-<?php echo $_smarty_tpl->tpl_vars['width']->value/$_smarty_tpl->tpl_vars['tile_width']->value;?>
; i<<?php echo ($_smarty_tpl->tpl_vars['width']->value/$_smarty_tpl->tpl_vars['tile_width']->value)*($_smarty_tpl->tpl_vars['height']->value/$_smarty_tpl->tpl_vars['tile_height']->value);?>
; i++){
		vpu.map.maps[0][i] = 0x05;
		vpu.map.maps[1][i] = 0x100;
		vpu.map.maps[2][i] = 0x100;
	}
	vpu.clean();
	
	for(i=0; i<this.lineCount; i++){
		vpu.locateCursor(0, i);
		vpu.print(this.lines[this.lineCount-(i+1)], 0xd);
	}
	
	var lineCount = parseInt(this.currentLine.length / <?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
);
	
	vpu.locateCursor(0, i);
	vpu.print(this.currentLine.substring(lineCount*<?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
) ,0x2);	
	
}

Console.prototype.init = function() {
	this.lineCursor = -1;
	this.lineCount = 0;
	
	// Create visible lines
	for(i=0; i<<?php echo $_smarty_tpl->tpl_vars['height']->value/$_smarty_tpl->tpl_vars['tile_height']->value;?>
-1; i++){
		this.lines[i]="";
		this.lineCount++;
	}
	
	// Create prompt
	this.currentLine = "";
	
	input = new Input(this);
	
}

Console.prototype.scroll = function(){
	var i;
	for(i=vpu.console.lineCount; i>0; i--){
		vpu.console.lines[i] = vpu.console.lines[i-1];
	}
}

Console.prototype.len = function (line){
	// Counts text length skipping color escape codes
	
	var length = 0;
	var e = line.length;
	var i = 0;
	for(;i < e; i++){
		
		// Skip 2 bytes, the color code values are not displayed
		if(line.substr(i, 1) == "^"){
			i++;
		} else length++;
	}
	
	return length;
}
	

Console.prototype.print = function(text) {
	var i;
	var lineCount;
	
	var textLength = this.len(text);
	var lineWidth = <?php echo $_smarty_tpl->tpl_vars['width']->value/($_smarty_tpl->tpl_vars['tile_width']->value/2);?>
;
	
	if(textLength > lineWidth ){
		lineCount = textLength / lineWidth;
		var i = 0;
		for(; i < lineCount; i++){
			vpu.console.lines[0] = text.substring( i*lineWidth, (i*lineWidth) + lineWidth);
			if(i < lineCount-1 ) vpu.console.scroll();
		}		
	} else {	
		for(i=vpu.console.lineCount; i>0; i--){
			vpu.console.lines[i] = vpu.console.lines[i-1];
		}
		vpu.console.lines[0] = text;
	}
	
	this.redraw = true;	
	return vpu.console.dump();
}

Console.prototype.clean = function() {
	for(i=0; i<this.lineCount; i++){
		this.lines[i]="";		
	}
	this.currentLine = "";
	this.redraw = true;
}

<?php }
}
?>