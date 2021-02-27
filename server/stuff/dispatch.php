<?PHP
/* ---------------------------------------------------------------------------------------------------------
 * ISSUES / TODO
 * ---------------------------------------------------------------------------------------------------------
 * - Accents not supported (check putChar_2()....)
 * - Content selector
 * 
 *
 *
 *
 * ---------------------------------------------------------------------------------------------------------
 * Example
 * ---------------------------------------------------------------------------------------------------------
 *	$vpu = new Vpu;
 *	 
 *	$vpu->setColor(1);
 *	 $vpu->locateCursor(0,1);
 *	$vpu->putAscii("Line 1");
 *	 
 *	$vpu->setColor(2);
 *	 $vpu->locateCursor(0,2);
 *	$vpu->putAscii("Line 2");
 *	 
 *	$vpu->setColor(3);
 *	 $vpu->locateCursor(0,3);
 *	$vpu->putAscii("Line u");
 *	 
 *	$vpu->setColor(6);
 *	 $vpu->locateCursor(0,18);
 *	$vpu->putAscii('Lorem Ipsum es simplemente el texto de relleno de las imprentas y archivos de texto.');
 *	 	
 *	$vpu->putTile($vpu->data2, 14,14, 0x4, 0x3);
 *	 
 *	$vpu->dump();
 * ---------------------------------------------------------------------------------------------------------
 */
 
function response($type, $content){
	echo "{" . 	'"command" : "'.$type.'", ';
	foreach($content as $key=>$value){
		echo '"'.$key.'" : "'.$value.'" ,';
	}
	echo '"tail" : "NULL" }';
}

function encode($text){
	$quote = array("'", '"');
	$quoteReplacement = array("\'", '\"');
	return str_replace($quote, $quoteReplacement, $text);
}

require("include/vpu.class.php");
require("include/container.class.php");

if(array_key_exists('page', $_POST)){
	
	$fields = explode(' ', $_POST['page']);
	if(count($fields)==0)$page = $_POST['page'];
	else { 
		$page = array_shift($fields);		
		$args = $fields;
	}
	
	switch($page){
		case 'code':
			// Sends code to a container. Notice this sends requested javascript 
			// code to anybody
			include('include/db.php');
			$code = dbvalue("programs", $_POST['program'], "code");
			
			// Avoid setting empty code or no logic function...
			if((!strstr($code, "this.logic"))||(!$code)) {
				$code = "this.logic = function(){}";
			}

			echo '{ "code" : '.json_encode($code).' }';
			die();

		case 'program':
			// Sends a program command, attaching code to be executed ONCE on the
			// client console
			
			$firstArg = count($args) >= 1?$args[0]:$args;			
			include('include/db.php');
			$code = dbvalue("programs", $firstArg, "code");
			
			// Avoid loading container code as executable code...
			if(strstr($code, "this.logic")) $code = "";
			
			if($code) {
				echo '{ "command" : "program", "name": "'.$firstArg.'", "data" : ' . json_encode($code) . '}';
			} else echo '{ "command" : "print", "text" : "^3ERROR : ^5Unknown program ^7'.$firstArg.' ^5requested" }';
			die();
			
		case 'container':
			// WARNING / NOTE: THIS FUNCTION ALLOWS CODE TO BE EXECUTED ON CLIENT MACHINE. 
			// THIS MAKES NEED OF A SECURE SYSTEM TO DISALLOW UNAUTHORIZED EXECUTIONS
			include('include/db.php');
			$code_guid = dbvalue("containers", $args[0], "code_guid");
			if(!$code_guid){
				echo '{ "command" : "print", "text" : "^3ERROR : ^5Container ^7'.$args[0].'^5 does not exist." }';
				return;
			}
			
			$vpu = new Vpu;
			$container = new Container($vpu);
			$container->deserialize($args[0]);
			
			if(array_key_exists(1, $args))$container-> x = intval($args[1]);
			if(array_key_exists(2, $args))$container-> y = intval($args[2]);
			
			echo "{" .	'"command" : "container", '.
				'"container" : ' . json_encode($container) . "}";
			die();
			break;
				
		case 'screen':
			$vpu = new Vpu;
			$container = new Container($vpu);

			$container->setGeometry(25,AUTO);
			//$container->border = false;
			$container->borderStyle = rand()%5;//BORDER_CURVES;
			$container->borderColor = rand()%16;
			$container->setContent("^7This a static container");
			$container->dock(DOCK_TOP | DOCK_RIGHT);
			$container->dump();

			//$container->border = true;
			$container->borderColor = rand()%16;
			$container->setGeometry(12,14);
			$container->borderStyle = rand()%5;//BORDER_SINGLE;
			$container->setContent(	"^2   TIPS   \n".
									"\n".
									"CONTAINER\n^1You can set fixed heights, or use ^8AUTO\n\n\n\n");
			$container->dock(DOCK_LEFT | DOCK_TOP);
			$container->dump();

			$container->borderColor = rand()%16;
			$container->borderStyle = rand()%5;
			$container->setGeometry(32, AUTO);
			$container->setContent(file_get_contents("changes.txt"));
			$container->dock(DOCK_CENTER);
			$container->dump();
			
			$vpu->dump();
			die();
			break;
	}
}
?>