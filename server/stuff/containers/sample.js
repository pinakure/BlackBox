this.z = 0;

this.x = parseInt(Math.random()*20);
this.y = parseInt(Math.random()*20);

this.color = parseInt(Math.random()*16);

this.samples = [];

this.loadSample = function(sample){
	
	this.samples[0] = parseInt(Math.random()*8);
	
	for(i=1;i<256;i++) {
		var rand = parseInt(Math.random()*3) - 1;
		this.samples[i] = (this.samples[i-1] + rand)&7;
	}
}

this.logic = function() {
	
	if(this.samples.length < 1)this.loadSample('sample');
	
	var sample = [];
	
	for(i=0;i<8;i++){
		sample[0] = this.samples[(this.z + i) % 256];
		sample[1] = this.samples[(this.z + i + 1) % 256];
		vpu.putHud(this.x+i, this.y, 0xc0 + (sample[0]*8) + sample[1], this.color);
	}
	this.z++;
	this.z %= 256;
};