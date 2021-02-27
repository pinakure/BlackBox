{* smarty *}

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
	<script src="include/jquery.min.js"></script>	
<!-- scripting  -->
<script id="code">
// Replaces the debug output
{if $debug_enable}
function debugSet(text){	
	document.getElementById("debug").innerHTML = '<a href="?clean=1" alt="Clean Template cache">Clear Cache</a><br/>'+text;
}

function debugAppend(text){
	$("#debug").append(text);
}
{/if}
{include "../include/auxiliary.js"};
{include "../include/message.js"};
{include "../include/input.js"};
{include "../include/console.js"};
{include "../include/vpu.js"};
{include "../include/container.js"};
{include "../include/control.js"};
{include "../include/ui.js"};
{include "../include/dispatch.js"};

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

</script>
</head>
<!-- styling -->
<style>{include "../style/vpu.css"};</style>
<!-- layout -->
<body id="body">
	<section>		
		<input id="input" type="text"/>
		{if $debug_enable}<div id="debug"></div>{/if}
		<img id="tileset_1" src="resources/in_bg.png"/>
		{for $i = 0 to 15 }
		<img id="tileset_2_{$i}" src="resources/in_hud_{$i}.png"/>
		<img id="tileset_3_{$i}" src="resources/bg_font_{$i}.png"/>
		{/for}
		<canvas id="canvas" width="{$width}" height="{$height}"></canvas>
	</section>
	<script>
	document.body.onclick=function()
        {
            var elem=document.getElementById("input");
            elem.focus();
        };
	</script>
</body>

</html>
