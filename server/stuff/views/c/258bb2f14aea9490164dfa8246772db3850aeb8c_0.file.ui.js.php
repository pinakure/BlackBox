<?php /* Smarty version 3.1.24, created on 2015-11-07 01:33:07
         compiled from "C:/xampp/htdocs/out/pinakure/stuff/include/ui.js" */ ?>
<?php
/*%%SmartyHeaderCode:23343563d46c3dc0dc3_15886862%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '258bb2f14aea9490164dfa8246772db3850aeb8c' => 
    array (
      0 => 'C:/xampp/htdocs/out/pinakure/stuff/include/ui.js',
      1 => 1446856272,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '23343563d46c3dc0dc3_15886862',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_563d46c3dc5c75_24168535',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_563d46c3dc5c75_24168535')) {
function content_563d46c3dc5c75_24168535 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '23343563d46c3dc0dc3_15886862';
?>
function UI(vpuInstance){

	if(!vpuInstance) error("UI.init(vpu) : vpu is missing");	
	this.vpu = vpuInstance;
	
	this.focusedControl = 0;
	
	this.messages = [];
	this.listeners = [];
	this.controls = [];
	
	report("UI", "Initialized");
}

UI.prototype.addControl = function(control){
	this.controls[this.controls.length] = control;
	report("UI", "Added control ^e"+control.name);
}

UI.prototype.dispatch = function(){
	//TODO: Dispatch messages
	
}

UI.prototype.drawControl = function(control){
	//TODO: Draw control
}
<?php }
}
?>