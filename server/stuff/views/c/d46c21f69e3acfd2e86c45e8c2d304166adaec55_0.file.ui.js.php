<?php /* Smarty version 3.1.24, created on 2021-02-26 00:01:37
         compiled from "C:/codigo/BlackBox/server/stuff/include/ui.js" */ ?>
<?php
/*%%SmartyHeaderCode:29203583760383a610e1335_00880846%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    'd46c21f69e3acfd2e86c45e8c2d304166adaec55' => 
    array (
      0 => 'C:/codigo/BlackBox/server/stuff/include/ui.js',
      1 => 1452809034,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '29203583760383a610e1335_00880846',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_60383a610eab14_44222434',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_60383a610eab14_44222434')) {
function content_60383a610eab14_44222434 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '29203583760383a610e1335_00880846';
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