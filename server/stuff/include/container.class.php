<?PHP

define("BORDER_CURVES", 1);
define("BORDER_DOUBLE", 2);
define("BORDER_SINGLE", 3);
define("BORDER_BLOCKY", 4);
define("BORDER_DIAGON", 5);
define("AUTO", "AUTO");

define("DOCK_NONE", 0);
define("DOCK_TOP", 1);
define("DOCK_LEFT", 2);
define("DOCK_BOTTOM", 4);
define("DOCK_RIGHT", 8);
define("DOCK_CENTER", 16);

define("VPU_HEIGHT", array_key_exists('height', $_GET)?$_GET['height']:20);
define("VPU_WIDTH", array_key_exists('width', $_GET)?$_GET['width']:30);


class Container {
	// Pointer to the vpu class which to output data to
	var $vpu;
	
	var $name;
	
	// These attributes will be specified in TILE values
	var $width;
	var $height;
	var $x;
	var $y;	
	
	var $border;
	var $borderStyle;
	var $borderColor;
	
	var $wordWrap;
	var $docking;
	
	var $code;
	
	// The data contained
	var $data;
	
	function Container($vpu){
		$this->vpu = $vpu;
		
		$this->vpu->setColor(0xF);
		$this->vpu->locateCursor(0,0);
		
		$this->wordWrap = true;
	
		$this->border = true;
		$this->borderStyle = BORDER_SINGLE;
		$this->borderColor = 0;
		
		$this->name = "^3UNLOADED";
		
		$this->width = 0;
		$this->height = AUTO;
		$this->docking = DOCK_NONE;
		$this->x = 0;
		$this->y = 0;		
		
		$this->code = "this.logic = function(){};";
						
		
		$this->dock($this->docking);
	}
	
	function deserialize($name){
		$db = dbopen();
		$res = dbquery("SELECT * FROM containers WHERE guid='".$name."'",$db);
		$rows = dbfetch($res);
		dbfree($res);
		dbclose($db);
		
		$this->x = intval($rows['x']);
		$this->y = intval($rows['y']);
		
		$this->width = intval($rows['width']);
		$this->height = intval($rows['height']);
		$this->name = $rows['code_guid'];
		$this->borderStyle = intval($rows['borderStyle']);
		$this->borderColor = intval($rows['borderColor']);
	}
	
	function dock($d){		
		// Apply dock property
		switch($d){
			case DOCK_TOP | DOCK_LEFT:
				$this->dockTop();
				$this->dockLeft(); 
				break;
			
			case DOCK_TOP | DOCK_RIGHT:
				$this->dockTop();
				$this->dockRight(); 
				break;
			
			case (DOCK_BOTTOM | DOCK_LEFT):
				$this->dockBottom();
				$this->dockLeft(); 
				break;
			case (DOCK_BOTTOM | DOCK_RIGHT):
				$this->dockBottom();
				$this->dockRight(); 
				break;
	
			case DOCK_CENTER: $this->dockCenter();break;
			case DOCK_TOP: $this->dockTop();break;
			case DOCK_BOTTOM: $this->dockBottom();break;
			case DOCK_LEFT: $this->dockLeft();break;
			case DOCK_RIGHT: $this->dockRight();break;			
				
			default:
			case "DOCK_NONE":
				break;
		}
		$this->docking = $d;
	}
	
	function dockRight(){
		$this->setPosition( ((VPU_WIDTH*2) - $this->width)-2, $this->y );
	}
	function dockLeft(){
		$this->setPosition( 1, $this->y );
	}
	function dockTop(){
		$this->setPosition( $this->x, 0);
	}
	function dockBottom(){
		$this->setPosition( $this->x, (VPU_HEIGHT - $this->height)-1 );
	}
	function dockCenter(){
		$this->x = ((VPU_WIDTH) - ($this->width>>1))-1;
		$this->y = (VPU_HEIGHT>>1) - ($this->height>>1);
	}
	
	
	function setGeometry($width, $height){
		$this->width = $width;
		$this->height = $height;		
	}
	
	// NOTE: You can change text color by sending ^ followed by the character corresponding to the color wanted
	function setContent($content){
		
		// Filter puntuation signs
		
		$search = [	'á'=>"a", 
					'é'=>"e", 
					'í'=>"i", 
					'ó'=>"o", 
					'ú'=>"u", 
					'Á'=>"a", 
					'É'=>"e", 
					'Í'=>"i", 
					'Ó'=>"o", 
					'Ú'=>"u"];
		$content = strtr($content, $search);
					 
  
		if($this->wordWrap)$content = wordwrap($content, $this->width-(($this->border?2:0)), "\n");
		$this->data = str_split($content);
		
		// If height is auto, calculate height based on existing \n characters
		if($this->height == AUTO){
			$assoc = array_count_values($this->data);
			$this->height = $assoc["\n"]+($this->border?2:0);
			//if($this->data[($this->data)-1] == "\n")$this->height--;
			if($this->height >= VPU_HEIGHT)$this->height = VPU_HEIGHT-1;
		}		
		
		$this->dock($this->docking);	
	}

	function setPosition($x, $y){
		$this->x = $x;
		$this->y = $y;
	}
	
	function len($line){
		$length = 0;
		$end = strlen($line);
		for($i=0;$i < $end; $i++){
			
			// Skip 2 bytes, the color code values are not displayed
			if($line[$i] == '^'){
				$i+=2;
			} else $length++;
		}
		
		return $length;
	}
	
	function dump(){
		if($this->height<1)return;
		$x = $this->x;
		$y = $this->y;
		$w = $this->width;
		$h = $this->height;
		$d = $this->data;
		
		
		// If border, print it and reduce text area in concordance
		if($this->border){
			$origin = $this->borderStyle * 0x10;
			
			$ly = $y+$h;
			$lx = $x+$w;
		
			for($bx = $x+1; $bx < $lx-1; $bx++){
				$this->vpu->putTile($this->vpu->data1, $bx>>1, $y, $origin + 0x9, $this->borderColor);
				$this->vpu->putTile($this->vpu->data1, $bx>>1, $ly, $origin + 0x9, $this->borderColor);	
				$this->vpu->putTile($this->vpu->data2, $bx>>1, $y,0x100);	
				$this->vpu->putTile($this->vpu->data2, $bx>>1, $ly,0x100);	
			}
			
			for($by = $y+1; $by < $ly; $by++){
				$this->vpu->putTile($this->vpu->data1, $x>>1, $by, $origin + 0xa, $this->borderColor);
				$this->vpu->putTile($this->vpu->data1, $lx>>1, $by, $origin + 0xa, $this->borderColor);	
				$this->vpu->putTile($this->vpu->data2, $x>>1, $by,0x100);	
				$this->vpu->putTile($this->vpu->data2, $lx>>1, $by,0x100);	
			}	
			
			$this->vpu->putTile($this->vpu->data1, $x>>1, $y,  $origin + 0x6, $this->borderColor);
			$this->vpu->putTile($this->vpu->data1, $lx>>1,$y,  $origin + 0x5, $this->borderColor);
			$this->vpu->putTile($this->vpu->data1, $x>>1, $ly, $origin + 0x8, $this->borderColor);
			$this->vpu->putTile($this->vpu->data1, $lx>>1,$ly, $origin + 0x7, $this->borderColor);
			// Clear txt
			$this->vpu->putTile($this->vpu->data2, $x>>1, $y,  0x100);
			$this->vpu->putTile($this->vpu->data2, $lx>>1,$y,  0x100);
			$this->vpu->putTile($this->vpu->data2, $x>>1, $ly, 0x100);
			$this->vpu->putTile($this->vpu->data2, $lx>>1,$ly, 0x100);
			
			$w -=2;
			$h -=2;
			$x++;
			$y++;
		}
		
		
		// Output text area
		$ly = $y+$h;
		$lx = $x+$w;
		$i=0;
		
		// Clean buffer (or fill)
		//TODO: Fill 
		 for($iy=$y; $iy <= $ly; $iy++){
			 $this->vpu->locateCursor($x, $iy);
			 for($ix=$x ; $ix < $lx; $ix++){
				 $this->vpu->putTile($this->vpu->data1, $ix>>1, $iy, 0x100);
				 $this->vpu->putTile($this->vpu->data2, $ix>>1, $iy, 0x100);	
			 }
		 }
		
		
		for(; $y < $ly+1; $y++){
			$this->vpu->locateCursor($x, $y);
			for($ix=$x ; $ix < $lx; $ix++){				
				if(!array_key_exists($i, $d)) return;
				else {
					
					// Color code
					if($d[$i]=="^"){
						$i++;
						$color = ord($d[$i]);
						$this->vpu->setColor($color);
					} else 					
					switch($d[$i]){						
						case "\r":
							$ix = 0;
							break;
							
						case "\n":
							//$ly++;
							$ix=$lx;
							break;
						default:
							$this->vpu->putChar_2(ord($d[$i])+0x100);
							break;
					}
				}
				
				$i++;
			}
			
			// Skip trailing /n (those who are in last line position)
			if(array_key_exists($i, $d)) if($d[$i]=="\n")$i++;
		}
	}
}

?>