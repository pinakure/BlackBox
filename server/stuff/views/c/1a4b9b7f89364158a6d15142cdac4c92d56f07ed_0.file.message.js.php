<?php /* Smarty version 3.1.24, created on 2021-02-26 00:01:35
         compiled from "C:/codigo/BlackBox/server/stuff/include/message.js" */ ?>
<?php
/*%%SmartyHeaderCode:14400951160383a5f912588_02885378%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '1a4b9b7f89364158a6d15142cdac4c92d56f07ed' => 
    array (
      0 => 'C:/codigo/BlackBox/server/stuff/include/message.js',
      1 => 1452809034,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '14400951160383a5f912588_02885378',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_60383a5f921ba5_06721911',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_60383a5f921ba5_06721911')) {
function content_60383a5f921ba5_06721911 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '14400951160383a5f912588_02885378';
?>
function Message(type, parameter){
	this.type = type;
	this.parameter = parameter;	
}

Message.prototype.REDRAW = 1;
Message.prototype.TRIGGER = 2;
Message.prototype.CALL = 3;
Message.prototype.DECREASE = 4;
Message.prototype.INCREASE = 5;

var MESSAGE = new Message(); // Global static message instance
<?php }
}
?>