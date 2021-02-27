<?php /* Smarty version 3.1.24, created on 2015-11-07 01:33:07
         compiled from "C:/xampp/htdocs/out/pinakure/stuff/include/control.js" */ ?>
<?php
/*%%SmartyHeaderCode:17678563d46c3db17e3_19282815%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '88548f9f68f572dfdef15160d2dbba7e5dd1e3d5' => 
    array (
      0 => 'C:/xampp/htdocs/out/pinakure/stuff/include/control.js',
      1 => 1446856231,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '17678563d46c3db17e3_19282815',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_563d46c3db8605_57231481',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_563d46c3db8605_57231481')) {
function content_563d46c3db8605_57231481 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '17678563d46c3db17e3_19282815';
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