function Movement(x,y,rotation) {
    this.x = x;
    this.rotation = rotation;
};

function Simulation(map, piece, z_position, rotation, q=3.5, w=1.0, e=100.0, r=100){
    this.w = w; // Column weight
    this.r = r; // Reverse Column weight
    this.q = q; // Penalty weight
    this.e = e; // Line weight
    this.map = new TetrisMap(map.width, map.height, map.data);
    this.map.color = [0,255,0,255];
    this.piece = new TetrisPiece(piece.type, z_position, piece.y, rotation);
    this.position = z_position;
    this.landing_column = this.piece.landingColumn();
    this.piece.x-=this.piece.landingColumn() ;
    this.score = 0;
    this.penalty = 0;
    this.map.removeLines();
    this.columnscore = this.map.getColumnScore(this.position);
    this.run();
}

Simulation.prototype.run = function(){
    // Calculate movement 
    // Set Simulation.score = { value:value, column_index:index }
    this.score = 0;
    for(y=this.piece.y;y<Tetris.height;y++){
        this.piece.y = y;
        if(!this.piece.isPlaceable(this.map)){
            this.piece.y--;
            if(this.piece.y<=0){
                this.score = -1;                
            }
            this.penalty = this.calculatePenalty();
            return;
        }
    }
    
}

Simulation.prototype.calculatePenalty = function(){
    if(this.score == -1)return 99999;
    var penalty = 0;
    var blocks = this.piece.getEvaluableBlocks();
    this.piece.write(this.map,3);
    for(b=0; b<blocks.length; b++){
        var block = blocks[b];
        var x = this.piece.x+block[0];
        if(x>=this.map.width)continue; else if(x<0)continue;
        var wells = 0;
        for(y=this.piece.y+block[1]+1;y<this.map.height; y++){
            //if(y>=this.map.height)continue; else 
            if(y<0)continue;
            if(this.map.data[y][x]==0){
                /*
                */
                wells++;
                penalty+=((this.map.height+3)-y)*(this.r/100);
                penalty *= this.q;
                penalty = parseInt(penalty);                
            }
            if(!this.map.data[y][x]==0){y=this.map.height;}            
        }
    }
    if(wells>4)
    penalty+= (wells*4)*this.q;
    var c = this.map.color;
    this.map.color = [255,0,0,255];
    this.piece.write(this.map, 3);
    this.map.color = c;
    var lines = this.map.getLines() * 100* this.e;
    var cs = (this.score >=0) 
        ? this.columnscore*this.w 
        : this.score;
    this.score = (cs + lines) - (penalty);
    return penalty;
}

Simulation.prototype.render = function(target){
    this.map.render(target);
}