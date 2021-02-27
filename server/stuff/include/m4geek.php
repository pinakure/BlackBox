<?PHP

define("TILESET_TEXT_ASCII", 0x100); // Both BG1 & BG2
define("TILESET_TEXT_BIGALPHA", 192);
define("TILESET_TEXT_BIGNUMBERS", 112);
define("TILESET_TEXT_TEXT", 10);
define("TILESET_TEXT_FONTS", 572);
define("TILESET_TEXT_UNCHECK", 160);
define("TILESET_TEXT_CHECK", 161);
define("TILESET_TEXT_TRUE", 162);
define("TILESET_TEXT_FALSE", 163);
define("TILESET_TEXT_WARNING", 179);

define("TILESET_HUD_SUBPIXELS", 96);
define("TILESET_HUD_VUMETER_V1", 112);
define("TILESET_HUD_VUMETER_H1", 120);
define("TILESET_HUD_WAVEFORM", 192);
define("TILESET_HUD_ASCII", 0x100); // Both BG1 & BG2

define("TILESET_BG_BLOCKS", 33); 
 
if(array_key_exists('clean', $_GET)){
	unlink('views/c/9b97dbda3f5c07ef768a223ae1e5f0c76d808566_6.file.vpu.conf.php');
	unlink('views/c/20de944ec3d6ce62ea6007cd74680fecd9796ab1_0.file.input.js.php');
	unlink('views/c/2031aa9aa08816c8eb85295915a71a290c5b2867_0.file.console.js.php');
	unlink('views/c/13c083cc2a152d24d80a5c182f4b60d15c9aa82f_0.file.vpu.js.php');
	unlink('views/c/14d6c688fb82834c04cbd6fee66b85955fbddc15_0.file.vpu.css.php');
	unlink('views/c/dae27357fdf8e14b0f1f886588fbeb78a91919b6_0.file.default.tpl.php');
}
 
require('include/smarty/Smarty.class.php');

function factorize($n){
	$i = 0;
	while($n > 0){
		$n = $n >> 1;
		$i++;		
	}
	return $i;
}


class SmartyHandler extends Smarty {
	
	function SmartyHandler() {

		
		$this->template_dir = 'views/';
		$this->compile_dir = 'views/c/';
		$this->config_dir = 'config/smarty/';
		$this->cache_dir = 'cache/';
		

		$this->setTilesetWidth(16);
		$this->setTileGeometry(8, 8);
		$this->setFrameRate(60);
		$this->setResolution(array_key_exists('width', $_GET)?$_GET['width']:512, array_key_exists('height', $_GET)?$_GET['height']:160);
		$this->setOptions();
	
		$this->display('default.tpl');
	}
	
	function setOptions(){
		$this->assign('fire_coefficient', array_key_exists('fire', $_GET)?$_GET['fire']:"0.304");		
		$this->assign('fire_enable', array_key_exists('fire', $_GET)?true:false);
		
		$this->assign('debug_enable', array_key_exists('debug', $_GET)?true:false);
		
	}
	
	// Setup the video buffer resolution
	// @arg	$width Video buffer internal width, in pixels
	// @arg	$height	Video buffer internal height, in pixels
	function setResolution($width, $height){
		if($width < 1) $width = 320;
		if($height < 1) $height = 240;	
		$this->assign('width', $width);
		$this->assign('height', $height);
	}
	
	
	function setTilesetWidth($width){
		if($width < 1) $width = 1;
		$this->assign('tileset_width', $width);
		$this->assign('tileset_width_factor', factorize($width));
	}
	
	function setTileGeometry($width, $height){
		if($width < 1) $width = 1;
		if($height < 1) $height = 1;
		$this->assign('tile_width', $width);
		$this->assign('tile_height', $height);
		$this->assign('tile_width_factor', factorize($width));
		$this->assign('tile_height_factor', factorize($height));
	}
	
	function setFrameRate($desiredFrameRate){
		if($desiredFrameRate < 1) $desiredFrameRate = 60;
		$this->assign('fps', $desiredFrameRate);
	}
};


?>