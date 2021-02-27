<?php /* Smarty version 3.1.24, created on 2015-11-12 08:54:46
         compiled from "C:/xampp/htdocs/out/pinakure/stuff/include/input.js" */ ?>
<?php
/*%%SmartyHeaderCode:3721564445c64e1843_52828374%%*/
if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '4381bd5674bca24929ea1f2e932b91ae2b465fc3' => 
    array (
      0 => 'C:/xampp/htdocs/out/pinakure/stuff/include/input.js',
      1 => 1447314718,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '3721564445c64e1843_52828374',
  'has_nocache_code' => false,
  'version' => '3.1.24',
  'unifunc' => 'content_564445c6544985_87276586',
),false);
/*/%%SmartyHeaderCode%%*/
if ($_valid && !is_callable('content_564445c6544985_87276586')) {
function content_564445c6544985_87276586 ($_smarty_tpl) {

$_smarty_tpl->properties['nocache_hash'] = '3721564445c64e1843_52828374';
?>
/*--------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------*/
var keyboardNames = [
		"0"			/*0*/,	"1"			/*1*/, 	"2"			/*2*/,	"3"			/*3*/,	"4"			/*4*/,	"5"			/*5*/,	"6"			/*6*/,	"7"			/*7*/, 
		"backspace"	/*8*/,	"tab"		/*9*/, 	"10"		/*10*/,	"11"		/*11*/, "12"		/*12*/,	"return"	/*13*/,	"14"		/*14*/,	"15"		/*15*/,
		"shift"		/*16*/,	"ctrl"		/*17*/,	"alt" 		/*18*/,	"pause/break"/*19*/,"capslock"	/*20*/,	"21"		/*21*/,	"22"		/*22*/,	"23"		/*23*/,
		"24"		/*24*/,	"25"		/*25*/,	"26"		/*26*/,	"escape"	/*27*/,	"28"		/*28*/,	"29"		/*29*/,	"30"		/*30*/,	"31"		/*31*/,
		"spacebar"	/*32*/,	"pageup"	/*33*/,	"pagedown"	/*34*/,	"end"		/*35*/,	"home"		/*36*/,	"leftarrow"	/*37*/,	"uparrow"	/*38*/,	"rightarrow"/*39*/,
		"downarrow"	/*40*/,	"41"		/*41*/,	"42"		/*42*/,	"43"		/*43*/,	"44"		/*44*/,	"insert" 	/*45*/,	"delete" 	/*46*/,	"47"		/*47*/,
		"0"			/*48*/,	"1"			/*49*/,	"2"			/*50*/,	"3"			/*51*/,	"4"			/*52*/,	"5"			/*53*/,	"6"			/*54*/,	"7"			/*55*/,
		"8"			/*56*/,	"9"			/*57*/,	"58"		/*58*/,	"59"		/*59*/,	"60"		/*60*/,	"61"		/*61*/,	"62"		/*62*/,	"63"		/*63*/,
		"64"		/*64*/,	"a"			/*65*/,	"b"			/*66*/,	"c"			/*67*/,	"d"			/*68*/,	"e"			/*69*/,	"f"			/*70*/,	"g"			/*71*/,
		"h"			/*72*/,	"i"			/*73*/,	"j"			/*74*/,	"k"			/*75*/,	"l"			/*76*/,	"m"			/*77*/,	"n"			/*78*/,	"o"			/*79*/,
		"p"			/*80*/,	"q"			/*81*/,	"r"			/*82*/,	"s"			/*83*/,	"t"			/*84*/,	"u"			/*85*/,	"v"			/*86*/,	"w"			/*87*/,
		"x"			/*88*/,	"y"			/*89*/,	"z"			/*90*/,	"leftwindow"/*91*/,	"rightwindow"/*92*/,"selectkey"	/*93*/,	"94"		/*94*/,	"95"		/*95*/,
		"numpad0"	/*96*/,	"numpad1"	/*97*/,	"numpad2"	/*98*/,	"numpad3"	/*99*/,	"numpad4"	/*100*/,"numpad5"	/*101*/,"numpad6"	/*102*/,"numpad7"	/*103*/,
		"numpad8"	/*104*/,"numpad9"	/*105*/,"multiply"	/*106*/,"add"		/*107*/,"108"		/*108*/,"subtract"	/*109*/,"decpoint"	/*110*/,"divide"	/*111*/,
		"f1"		/*112*/,"f2"		/*113*/,"f3"		/*114*/,"f4"		/*115*/,"f5"		/*116*/,"f6"		/*117*/,"f7"		/*118*/,"f8"		/*119*/,
		"f9"		/*120*/,"f10"		/*121*/,"f11"		/*122*/,"f12"		/*123*/,"124"		/*124*/,"125"		/*125*/,"126"		/*126*/,"127"		/*127*/,
		"128"		/*128*/,"129"		/*129*/,"130"		/*130*/,"131"		/*131*/,"132"		/*132*/,"133"		/*133*/,"134"		/*134*/,"135"		/*135*/,
		"136"		/*136*/,"137"		/*137*/,"138"		/*138*/,"139"		/*139*/,"140"		/*140*/,"141"		/*141*/,"142"		/*142*/,"143"		/*143*/,
		"numlock"	/*144*/,"scrolllock"/*145*/,"146"		/*146*/,"147"		/*147*/,"148"		/*148*/,"149"		/*149*/,"150"		/*150*/,"151"		/*151*/,
		"152"		/*152*/,"153"		/*153*/,"154"		/*154*/,"155"		/*155*/,"156"		/*156*/,"157"		/*157*/,"158"		/*158*/,"159"		/*159*/,
		"160"		/*160*/,"161"		/*161*/,"162"		/*162*/,"163"		/*163*/,"164"		/*164*/,"165"		/*165*/,"back"		/*166*/,"forward"	/*167*/,
		"web-refresh"/*168*/,"web-stop"	/*169*/,"170"		/*170*/,"bookmarks"	/*171*/,"172"		/*172*/,"volumemute"/*173*/,"volumedown"/*174*/,"volumeup"	/*175*/,
		"media-next"/*176*/,"media-prev"/*177*/,"media-stop"/*178*/,"playpause"	/*179*/,"email"		/*180*/,"mediaplayer"/*181*/,"fileman"	/*182*/,"calculator"/*183*/,
		"184"		/*184*/,"185"		/*185*/,"semi-colon"/*186*/,"equalsign"	/*187*/,"comma"		/*188*/,"dash"		/*189*/,"period"	/*190*/,"forwardslash"/*191*/,
		"graveaccent"/*192*/,"193"		/*193*/,"194"		/*194*/,"195"		/*195*/,"196"		/*196*/,"197"		/*197*/,"198"		/*198*/,"199"		/*199*/,
		"200"		/*200*/,"201"		/*201*/,"202"		/*202*/,"203"		/*203*/,"204"		/*204*/,"205"		/*205*/,"206"		/*206*/,"207"		/*207*/,
		"208"		/*208*/,"209"		/*209*/,"210"		/*210*/,"211"		/*211*/,"212"		/*212*/,"213"		/*213*/,"214"		/*214*/,"215"		/*215*/,
		"216"		/*216*/,"217"		/*217*/,"218"		/*218*/,"openbracket"/*219*/,"backslash"/*220*/,"closebraket"/*221*/,"singlequote"/*222*/,"223"		/*223*/,
		"224"		/*224*/,"225"		/*225*/,"lessthan"	/*226*/,"227"		/*227*/,"228"		/*228*/,"229"		/*229*/,"230"		/*230*/,"231"		/*231*/,
		"232"		/*232*/,"233"		/*233*/,"234"		/*234*/,"235"		/*235*/,"236"		/*236*/,"237"		/*237*/,"238"		/*238*/,"239"		/*239*/,
		"240"		/*240*/,"241"		/*241*/,"242"		/*242*/,"243"		/*243*/,"244"		/*244*/,"245"		/*245*/,"246"		/*246*/,"247"		/*247*/,
		"248"		/*248*/,"249"		/*249*/,"250"		/*250*/,"251"		/*251*/,"252"		/*252*/,"253"		/*253*/,"254"		/*254*/,"255"		/*255*/];
/*--------------------------------------------------------------------------------------------------------------------------------
     _____                   _      _____ _               
    |_   _|                 | |    / ____| |               
      | |  _ __  _ __  _   _| |_  | |    | | __ _ ___ ___ 
      | | | '_ \| '_ \| | | | __| | |    | |/ _` / __/ __|
     _| |_| | | | |_) | |_| | |_  | |____| | (_| \__ \__ \
    |_____|_| |_| .__/ \__,_|\__|  \_____|_|\__,_|___/___/
--------------------------------------------------------------------------------------------------------------------------------*/


function Input(consoleInstance){
	this.console = consoleInstance;
	this.name = "Unnamed";

	this.min = 0;
	this.max = 15;
	this.clamp = true;
	this.value = this.min;		

	this.shiftFlag = false;
	
	this.waitingForColorCode = false;
	
	this.listener = [];
	
	document.addEventListener('keydown', this.keyDown);	
	document.addEventListener('keyup', this.keyUp);	
	
	
    var rx = /INPUT|SELECT|TEXTAREA/i;

    $(document).bind("keydown keypress", function(e){
        var v = e.which;
		if(!rx.test(e.target.tagName) || e.target.disabled || e.target.readOnly ){
            e.preventDefault();
	    }
    });
}

Input.prototype.keyDown = function(event){
	var i = event.which;
	
	if(this.waitingForColorCode){
		switch(i){
			case 48 : vpu.console.addChar('^0'); break;
			case 49 : vpu.console.addChar('^1'); break;
			case 50 : vpu.console.addChar('^2'); break;
			case 51 : vpu.console.addChar('^3'); break;
			case 52 : vpu.console.addChar('^4'); break;
			case 53 : vpu.console.addChar('^5'); break;
			case 54 : vpu.console.addChar('^6'); break;
			case 55 : vpu.console.addChar('^7'); break;
			case 56 : vpu.console.addChar('^8'); break;
			case 57 : vpu.console.addChar('^9'); break;
			case 65 : vpu.console.addChar('^A'); break;
			case 66 : vpu.console.addChar('^B'); break;
			case 67 : vpu.console.addChar('^C'); break;
			case 68 : vpu.console.addChar('^D'); break;
			case 69 : vpu.console.addChar('^E'); break;
			case 70 : vpu.console.addChar('^F'); break;
		}
		this.waitingForColorCode = false;
		return;
	} else {	
		/****/	if((i >= 48)&&(i <= 57)) return vpu.console.addChar(String.fromCharCode(i+(this.shiftFlag?10:0))); //numbers
		else 	if((i >= 65)&&(i <= 90)) return vpu.console.addChar(String.fromCharCode(i+(this.shiftFlag?0:32))); //alphabet
	}
		
	switch(keyboardNames[i]){
		case 'semi-colon':	this.waitingForColorCode = true; return;
		case 'spacebar': 	return vpu.console.addChar("\0"); 					//  spacebar, dont ask why this works right....
		case 'backspace': 	return vpu.console.backspace(); 					// tab		
		case 'return': 		return vpu.console.parseCommandLine();				// return
		case 'shift': 		return this.shiftFlag = true;						// shift
		case 'uparrow':		return vpu.console.historyPrevious();
		case 'downarrow':	return vpu.console.historyNext();
		case 'f5':			return command_reload([]);
		case 'escape':		vpu.console.disabled ^= 1;
							if(vpu.console.disabled)return vpu.loadMapData(); 
							else return vpu.console.dump();
		
		default:			return vpu.console.print(keyboardNames[i] + " is not bound.");
	}
}

Input.prototype.keyUp = function(event){
	var i = event.which;
	
	switch(keyboardNames[i]){
		case 'shift': return this.shiftFlag = false;
			
		default: return;
	}
}

/* this is for control, not for input!!!!! */
Input.prototype.more = function(quantity) {
	this.value += quantity;
	if(this.clamp)this.value %= this.max;
	else this.value = this.value > this.max?this.max:this.value;
}

Input.prototype.less = function(quantity) {
	this.value -= quantity;
	if(this.clamp)this.value %= this.max;
	else this.value = (this.value < this.min)?this.min:this.value;
}<?php }
}
?>