<?php /* Smarty version 3.1.24, created on 2015-11-07 01:30:06
         compiled from "C:/xampp/htdocs/out/pinakure/stuff/include/message.js" */ ?>
<?php
/*%%SmartyHeaderCode:2041563d460e6ebf86_10820384%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '12a276f8f9a567b4ceff3a93e792874c97b627ac' => 
    array (
      0 => 'C:/xampp/htdocs/out/pinakure/stuff/include/message.js',
      1 => 1446856183,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '2041563d460e6ebf86_10820384',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_563d460e6f0278_08532015',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_563d460e6f0278_08532015')) {
function content_563d460e6f0278_08532015 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '2041563d460e6ebf86_10820384';
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