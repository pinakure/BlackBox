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
