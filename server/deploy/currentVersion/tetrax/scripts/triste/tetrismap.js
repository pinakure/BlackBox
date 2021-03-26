function TetrisMap(width, height, data=[]){
    this.width = width;
    this.height = height;
    this.buffer = undefined;
    this.data = [];    
    this.bg = [24,96,24,255];
    this.color = [255,255,0,255];
    if(data.length == 0) this.allocate();
    else this.copy(data);
}

TetrisMap.prototype.allocate = function(){
    for(y = 0; y<this.height; y++){
        this.data[y]= [];
       for(x = 0; x<this.width; x++){
        this.data[y][x] = 0;
       }
    }
}

TetrisMap.prototype.copy = function(data){
    for(y = 0; y<this.height; y++){
        this.data[y]= [];
       for(x = 0; x<this.width; x++){
        this.data[y][x] = data[y][x];
       }
    }
}

TetrisMap.prototype.addShape = function(type, x=0, y=0, angle=0){
    var piece = new TetrisPiece(type, x, y, angle);
    if(piece.isPlaceable(this)) {
        piece.logic();
        return piece;
    }
    return false;
}

TetrisMap.prototype.render = function(target){
    var canvas = document.getElementById(target); 
    var ctx = canvas.getContext('2d');
    
    if(!this.buffer) this.buffer = ctx.createImageData(this.width, this.height);
    
    var ri=0;
    for(y=0;y<this.height; y++){
        for(x=0;x<this.width; x++){
            switch(this.data[y][x]){
                case 1:
                    this.buffer.data[ ri ] = this.color[0];
                    this.buffer.data[ri+1] = this.color[1];
                    this.buffer.data[ri+2] = this.color[2];
                    this.buffer.data[ri+3] = this.color[3];//alpha
                    break;
                case 2:
                    this.buffer.data[ ri ] = this.color[2];
                    this.buffer.data[ri+1] = this.color[1];
                    this.buffer.data[ri+2] = this.color[0];
                    this.buffer.data[ri+3] = this.color[3];//alpha
                    break;
                case 0:
                    this.buffer.data[ ri ] = this.bg[0];
                    this.buffer.data[ri+1] = this.bg[1];
                    this.buffer.data[ri+2] = this.bg[2];
                    this.buffer.data[ri+3] = this.bg[3];//alpha
                    break;
            }
            ri+=4;
        }        
    }
    ctx.putImageData(this.buffer, 0, 0);
}

TetrisMap.prototype.check = function(x,y){
    /*
    Checks for obstacles in the put bloc procedure:
    - Returns false if...
        ...a block is found on given position or
        ...given position is outside the map or
        ...given position is already occupied by a block
    - Returns true if...
        ...no block is occuping given position or
        ...vertical position is negative
    */
    if(y < 0) return true;
    if(x >= this.width) return false;
    if(x < 0) {
        
        return false;
    }
    if(y >= this.height) return false;
    return (this.data[y][x] == 0 );

}

function isComplete(row){
    for(i=0; i<row.length; i++){
        if(!row[i])return false;
    }
    return true;
}

TetrisMap.prototype.getLines = function(){
    var lines = 0;
    for(y=0; y<this.data.length; y++){
        if(isComplete(this.data[y]))lines++;
    }
    return lines;
}

TetrisMap.prototype.getColumnScore = function(column){
    for(r=0; r<this.height; r++){
        if(this.data[r][column])return (r-1)*this.width /* originally just r-1 */;
    }
    return this.height*this.width;/* originally return this.height;*/
}

TetrisMap.prototype.hiscore = function(){
    var hi = {
        value: 0,
        index: -1
    };

    for(hindex=0; hindex < this.scores.length; hindex++){
        var sc = this.scores[hindex];
        if(sc > hi.value){
            hi.value = sc;
            hi.index = hindex;
        }
    }
    if(hi.value > -1) return hi;
    else return { value: 0, index: parseInt(1 +(Math.random()*(Tetris.map.width - 2)))};
}

TetrisMap.prototype.newRow = function(){
    var row = [];
    for(x=0; x<this.width; x++){
        row[x] = 0;
    }
    return row;
}

TetrisMap.prototype.removeLines = function(){
    var new_data = []
    var lines = 0;
    for(y=this.data.length-1,iy=y; y>=0; y--){
        if(isComplete(this.data[y]))continue;
        new_data[iy] = this.data[y];
        iy--;
    }
    for(y=0; y<this.data.length; y++){
        if(new_data[y] == undefined)new_data[y] = this.newRow();
    }
    this.data = new_data;
}