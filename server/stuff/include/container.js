/*--------------------------------------------------------------------------------------------------------------------------------
	CONTAINER CLASS - 28-10-2015
----------------------------------------------------------------------------------------------------------------------------------

	Container class :: Interfaces
	
	setPosition(x, y)		Sets container position to x,y TILE coordinates
	setFocus()				Gives focus and restablishes all containers' z accordingly	
	setCode(code)			Sets the code to be executed
	start()					Marks the code of the container as runnable therefore it will run next time

--------------------------------------------------------------------------------------------------------------------------------*/



function System(){
	this.run = true;
	this.containers = [];
	
}

System.prototype.update = function(){
	var i=0;
	for(;i<this.containers.length;i++){
		this.containers[i].dispatch();
	}
}

System.prototype.getContainerCount = function(){
	return this.containers.length;
}

System.prototype.getContainerCount = function(index){
	if(index>this.containers.length)return 0;
	return this.containers[index];
}

System.prototype.addContainer = function(container){
	this.containers[this.containers.length] = container;
	vpu.console.print("^7SYSTEM: ^6Added container "+container.name);
}

function Container(){
	// Position vars
	this.x = 2;
	this.y = 3;
	this.z = 0;
	
	this.width = 10;
	this.height = 10;
	
	// Style flags
	this.border = true;
	this.borderStyle = 0;
	this.borderColor = 0;
	
	// State flags	
	this.focus = false;
	this.redraw = false;
	this.running = false;
	this.alwaysOnTop = false;
	this.maximized = false;
	this.minimized = false;
	
	// Status flags
	// TODO: Make this be a proper flag variable instead of a group of boolean vars
	this.moving = false;
	this.resizing = false;
	this.restoring = false;
	this.minimizing = false;
	this.maximizing = false;
	
	this.name = "unnamed";
	this.title= "New container";
	
	this.args = [];//WIP
	
	this.properties = "";

	//if(Math.random()*10 > 5)this.code = new ContainerSample1(this.args); 
	//else this.code = new ContainerSample2(this.args); 
	
	this.system = 0;
	
	//this.messages = [];
	
}

Container.prototype.setCode = function(code){
	try {
		this.code = eval("new function(){ldelim}"+code+"{rdelim};"); 
	} catch(err){
		vpu.console.print("^3CONTAINER SCRIPT : ^4"+err.message);
	}	
}

Container.prototype.init = function(system){
	// Sets this.sytem to point to existing system instanceof
	this.system = system;
	vpu.console.print("^6CONTAINER : ^dBinded system");
	
	try{
		this.code.logic(this);
	} catch(err){
		vpu.console.print("^6CONTAINER : ^3Bad logic method! : ^5"+err.message);
	}
	
	this.redraw = true;
	this.running = true;
}

Container.prototype.dump = function(){
	vpu.console.print("^cCONTAINER ^d" + this.name);
	vpu.console.print("^c \xc3 ^fwidth:^7" + this.width + " ^fheight:^7" + this.height);
	vpu.console.print("^c \xc3 ^fx:^7" + this.x +" ^fy:^7" + this.y);
	vpu.console.print("^c \xc3 ^fborder:^7" + this.border + " ^fborderStyle:^7"+this.borderStyle + " ^fborderColor:^7" + this.borderColor);
	vpu.console.print("^c \xc0 ^fwordWrap:^7" + this.wordWrap + " ^fdocking:^7" + this.docking);
}

Container.prototype.deserialize = function(o){
	this.width = o['width'];
	this.height= o['height'];
	this.x = o['x'];
	this.y = o['y'];
	this.z = 0;
	//vpu.system.getFocus(c);
	this.borderStyle = o['borderStyle'];
	this.borderColor= o['borderColor'];
	this.name = o['name'];
	this.data = o['data'];
}

Container.prototype.dispatch = function(){
	if(this.redraw)this.render();
	//this.redraw = false;
	
	if(this.running){
		this.code.logic(this);
	}
}

Container.prototype.render = function(){
	//TODO : render container
	if(this.height < 1) return;
	
	var x = this.x;
	var y = this.y;
	var w = this.width;
	var h = this.height;
	var d = this.data;
	
	// If border > 0, draw it and reduce text area in concordance
	if(this.border){
		var origin = this.borderStyle * 0x10;
		var ly = y + h;
		var lx = x + w;
		var bx, by;
		
		for(bx = x + 1; bx < lx - 1; bx++) {
			vpu.putHud(bx>>1, y, origin + 0x9, this.borderColor);
			vpu.putHud(bx>>1, ly, origin + 0x9, this.borderColor);	
			vpu.putText(bx>>1, y, 0x100,0);	
			vpu.putText(bx>>1, ly, 0x100,0);	
		}
			
		for(by = y+1; by < ly; by++) {
			vpu.putHud(x>>1, by, origin + 0xa, this.borderColor);
			vpu.putHud(lx>>1, by, origin + 0xa, this.borderColor);	
			vpu.putText(x>>1, by,0x100,0);	
			vpu.putText(lx>>1, by,0x100,0);	
		}
		vpu.putHud(x>>1, y,  origin + 0x6, this.borderColor);
		vpu.putHud(lx>>1,y,  origin + 0x5, this.borderColor);
		vpu.putHud(x>>1, ly, origin + 0x8, this.borderColor);
		vpu.putHud(lx>>1,ly, origin + 0x7, this.borderColor);
		// Clear txt
		vpu.putText(x>>1, y,  0x100, 0);
		vpu.putText(lx>>1,y,  0x100, 0);
		vpu.putText(x>>1, ly, 0x100, 0);
		vpu.putText(lx>>1,ly, 0x100, 0);


		//Substract border size from container size and offset position
		w -=2;
		h -=2;
		x++;
		y++;
	}
	
	
	/*
			
			
		
			
		
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
	
	*/
}


Container.prototype.setPosition = function(x, y){	
	var xmax = {($width / $tile_width)-1};
	var ymax = {($height / $tile_height)-1};
	
	if(x > xmax) x = xmax;
	else if(x < 0) x = 0;	
	if(y > ymax) y = ymax;
	else if(y < 0) y = 0;
	
	this.x = x;
	this.y = y;
	
	this.redraw = true;
}

Container.prototype.setFocus = function(){
	
	if(this.z == 0)return;
	
	var il = this.system.getContainerCount();
	var container;
	var i=0;
	
	for(;i < il; i++){
		container = this.system.getContainer(i);
		if(container){
			container.z++;
		}
	}
	
	this.z = 0;
	
	this.redraw = true;
}
