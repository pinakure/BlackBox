<?php /* Smarty version 3.1.24, created on 2021-02-26 00:01:37
         compiled from "C:/codigo/BlackBox/server/stuff/include/control.js" */ ?>
<?php
/*%%SmartyHeaderCode:185914563460383a6103fc39_92513479%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '6b68e42e82732161de9a852abeda08a772dde8dd' => 
    array (
      0 => 'C:/codigo/BlackBox/server/stuff/include/control.js',
      1 => 1452809034,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '185914563460383a6103fc39_92513479',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_60383a6104d334_73101755',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_60383a6104d334_73101755')) {
function content_60383a6104d334_73101755 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '185914563460383a6103fc39_92513479';
?>

function Control(name){
	this.name = name;
	this.messages = [];
	this.listeners = [];

	// Control position
	this.x = 0;
	this.y = 0;	
	this.z = 0;
	
	// Adjacent controls
	this.up = 0;
	this.down = 0;
	this.left = 0;
	this.right = 0;
	
	// Callbacks
	this.action = 0;
	this.cancel = 0;
	
	// Byte value
	this.variable = 0;
	
	// Value type
	this.varType = 0;
}	

Control.prototype.addListener = function(listener){
	this.listeners[this.listeners.length] = listener;
}

Control.prototype.dispatch = function(){
	// TODO: Dispatch messages
}

Control.prototype.deserialize = function(jsonObj){
	try {
		var c = JSON.parse(jsonObj);
		this.x = c['x'];
		this.y = c['y'];
		this.z = c['z'];
		this.up = c['up'];
		this.down = c['down'];
		this.left = c['left'];
		this.right = c['right'];
		this.action = c['action'];
		this.cancel = c['cancel'];
		this.variable = c['variable'];
		this.varType = c['varType'];
		
		report("Control", "Deserialized");
		
	} catch(e){
		error(e.message);
	}
}<?php }
}
?>