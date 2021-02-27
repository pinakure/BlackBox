
function Control(name){
	this.name = name;
	this.messages = [];
	this.listeners = [];

	// Control position
	this.x = 0;
	this.y = 0;	
	this.z = 0;
	
	// Adjacent controls
	this.up = 0;
	this.down = 0;
	this.left = 0;
	this.right = 0;
	
	// Callbacks
	this.action = 0;
	this.cancel = 0;
	
	// Byte value
	this.variable = 0;
	
	// Value type
	this.varType = 0;
}	

Control.prototype.addListener = function(listener){
	this.listeners[this.listeners.length] = listener;
}

Control.prototype.dispatch = function(){
	// TODO: Dispatch messages
}

Control.prototype.deserialize = function(jsonObj){
	try {
		var c = JSON.parse(jsonObj);
		this.x = c['x'];
		this.y = c['y'];
		this.z = c['z'];
		this.up = c['up'];
		this.down = c['down'];
		this.left = c['left'];
		this.right = c['right'];
		this.action = c['action'];
		this.cancel = c['cancel'];
		this.variable = c['variable'];
		this.varType = c['varType'];
		
		report("Control", "Deserialized");
		
	} catch(e){
		error(e.message);
	}
}