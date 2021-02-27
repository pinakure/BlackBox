<?php /* Smarty version 3.1.24, created on 2021-02-26 00:01:34
         compiled from "views/default.tpl" */ ?>
<?php
/*%%SmartyHeaderCode:3171896860383a5eb34c35_04318458%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '814c5031a046b3a56b7ac1e2291c0f8edf35762f' => 
    array (
      0 => 'views/default.tpl',
      1 => 1452809042,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '3171896860383a5eb34c35_04318458',
  'variables' => 
  array (
    'debug_enable' => 0,
    'i' => 0,
    'width' => 0,
    'height' => 0,
  ),
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_60383a5f195b38_01430044',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_60383a5f195b38_01430044')) {
function content_60383a5f195b38_01430044 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '3171896860383a5eb34c35_04318458';
?>


<!DOCTYPE html>
<html>
<!-- header -->
<head>
	<title>
		M4GEEK 8BIT Experiments
	</title>
	<meta charset="UTF-8">
	<meta name="description" content="CSS Tiled System Experiment">
	<meta name="keywords" content="test php smarty tpl css ajax">
	<meta name="author" content="Al 'Pinakure' Perez aka 'Smiker'">
	<link rel	="shortcut icon" href="resources/favicon.ico">
	<?php echo '<script'; ?>
 src="include/jquery.min.js"><?php echo '</script'; ?>
>	
<!-- scripting  -->
<?php echo '<script'; ?>
 id="code">
// Replaces the debug output
<?php if ($_smarty_tpl->tpl_vars['debug_enable']->value) {?>
function debugSet(text){	
	document.getElementById("debug").innerHTML = '<a href="?clean=1" alt="Clean Template cache">Clear Cache</a><br/>'+text;
}

function debugAppend(text){
	$("#debug").append(text);
}
<?php }?>
<?php echo $_smarty_tpl->getSubTemplate ("../include/auxiliary.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/message.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/input.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/console.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/vpu.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/container.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/control.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/ui.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;
<?php echo $_smarty_tpl->getSubTemplate ("../include/dispatch.js", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;

// TODO: Move to GUI
function decorate(container){
	
}



/* 'Autoexec' function */
$( window ).load(function() {
	// Add default decorator to hardcoded containers
	$(".container").css("right", "0");
			 
	
			 
	vpu.init();
	// Request data to load on maps' data layers
	vpu.console = new Console(vpu);
	vpu.dataRequest("screen");	
	vpu.resume();
}); 

<?php echo '</script'; ?>
>
</head>
<!-- styling -->
<style><?php echo $_smarty_tpl->getSubTemplate ("../style/vpu.css", $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, $_smarty_tpl->cache_lifetime, array(), 0);
?>
;</style>
<!-- layout -->
<body id="body">
	<section>		
		<input id="input" type="text"/>
		<?php if ($_smarty_tpl->tpl_vars['debug_enable']->value) {?><div id="debug"></div><?php }?>
		<img id="tileset_1" src="resources/in_bg.png"/>
		<?php $_smarty_tpl->tpl_vars['i'] = new Smarty_Variable;$_smarty_tpl->tpl_vars['i']->step = 1;$_smarty_tpl->tpl_vars['i']->total = (int) ceil(($_smarty_tpl->tpl_vars['i']->step > 0 ? 15+1 - (0) : 0-(15)+1)/abs($_smarty_tpl->tpl_vars['i']->step));
if ($_smarty_tpl->tpl_vars['i']->total > 0) {
for ($_smarty_tpl->tpl_vars['i']->value = 0, $_smarty_tpl->tpl_vars['i']->iteration = 1;$_smarty_tpl->tpl_vars['i']->iteration <= $_smarty_tpl->tpl_vars['i']->total;$_smarty_tpl->tpl_vars['i']->value += $_smarty_tpl->tpl_vars['i']->step, $_smarty_tpl->tpl_vars['i']->iteration++) {
$_smarty_tpl->tpl_vars['i']->first = $_smarty_tpl->tpl_vars['i']->iteration == 1;$_smarty_tpl->tpl_vars['i']->last = $_smarty_tpl->tpl_vars['i']->iteration == $_smarty_tpl->tpl_vars['i']->total;?>
		<img id="tileset_2_<?php echo $_smarty_tpl->tpl_vars['i']->value;?>
" src="resources/in_hud_<?php echo $_smarty_tpl->tpl_vars['i']->value;?>
.png"/>
		<img id="tileset_3_<?php echo $_smarty_tpl->tpl_vars['i']->value;?>
" src="resources/bg_font_<?php echo $_smarty_tpl->tpl_vars['i']->value;?>
.png"/>
		<?php }} ?>
		<canvas id="canvas" width="<?php echo $_smarty_tpl->tpl_vars['width']->value;?>
" height="<?php echo $_smarty_tpl->tpl_vars['height']->value;?>
"></canvas>
	</section>
	<?php echo '<script'; ?>
>
	document.body.onclick=function()
        {
            var elem=document.getElementById("input");
            elem.focus();
        };
	<?php echo '</script'; ?>
>
</body>

</html>
<?php }
}
?>