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
