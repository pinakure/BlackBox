function Map(width, height) {
    this.width = width;
    this.height = height;
    this.data = [];
}

Map.prototype.copy = function(d){
    for(di in d){
        var arr = [];
        var c = d[di];
        for(ci in c){
            dc = c[ci];
            arr[ci] = dc;
        }
        this.data[di] = arr;
    }
}

Map.prototype.create = function(){
    $('#triste').append('<div id="scene"></div>');
    var i=0;
    this.data = [];
    for(y=0; y < this.height; y++){
        this.data[y] = []
        for(x=0; x < this.width; x++){
            $('#scene').append(`<div id="sector_${i}"></div>`);
            this.data[y][x] = 0;
            i++;
        }
    }
}

Map.prototype.addShape = function(shape, x=0, y=0, angle=0){
    angle = angle % shape.data.length;
    var piece = new Piece(shape, x, y, angle);
    
    //if(shape.center[0] > -1) piece.x -= shape.center[0];
        
    if(piece.isPlaceable(Triste.map)) {
        Triste.piececount++;
        var html = piece.render();
        piece.logic();
        return piece;
    }
    var offset = piece.findOptimalColumn();
    return false;
}

Map.prototype.getHeights = function(){
    /* 
    Returns array of column heights 
    */
    var height_score = [];
    var columns = this.getColumns();
    for(col_index in columns){
        var column = columns[col_index];
        var block_found = false;
        var h_score = this.height;
        for(row_index=0; row_index<this.height && !block_found; row_index++){
            if(column[row_index]){
                block_found = true;
            }
            h_score--;
        }
        height_score[height_score.length] = h_score;
    }
    return height_score;
}

Map.prototype.maxHeight = function(){
    /* 
    Returns the highest point reached by a column
    */
    var heights = this.getHeights();
    var hi_height = 0;
    for(h_index in heights){
        var val = heights[h_index];
        if(val > hi_height) hi_height = val;
    }
    return val;
}

Map.prototype.getColumns = function(){
    /*
    Returns a set of columns from the set of rows conforming the map
    */
    var columns = [];
    for(column_index=0; column_index < this.width; column_index++){
        var column = [];
        for(row_index=0; row_index < this.height; row_index++){
            column[row_index] = this.data[row_index][column_index];
        }
        columns[column_index] = column;
    }
    return columns;
}

Map.prototype.getWeights = function(){
    /* 
    Returns array of scores based on how much space next to this column exists. 
    Columns with less populated neighbor columns will score less
    */
    var columns = this.getColumns();
    var ret = [];
    for(col_index in columns){
        var column = columns[col_index];
        
    }
    return ret;
}

Map.prototype.getHoleCount = function(){
    /* 
    Returns array of scores based on how much lost holes are there in each column.
    A lost hole is a block which is not set, but has a block over it 
    */
    var hole_score = [];
    var columns = this.getColumns();
    for(col_index in columns){
        var column = columns[col_index];
        var holes = 0;
        var block_found = false;
        for(index in column){
            if(column[index]) block_found = true;
            else if (!block_found){
                holes++;
            }
        }
        hole_score[hole_score.length] = holes;
    }
    return hole_score;
}

Map.prototype.check = function(x,y){
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
    if(x < 0) return false;
    if(y >= this.height) return false;
    return this.data[y][x] == 0;
}

Map.prototype.getScores = function(){
    var heights = this.getHeights();
    var ret = [];
    for(hi=0; hi<heights.length; hi++){
        ret[hi] = this.height-heights[hi];
    }
    return ret; 
}

Map.prototype.toTable = function(extra=false){
    var shape = this.data;
    var hrows = '';
    var rows = '';
    var cells = '';
    for(x=0; x<this.width+1; x++){
        cells += `<th><strong>${x<this.width?x:''}</strong></th>`;
    }
    hrows = `<thead><tr>${cells}</tr></thead>`;
    for(y=0; y<this.height; y++){
        cells = '';
        for(x=0; x<this.width; x++){
            cells += `<td class="${shape[y][x]?'dark':''}">${shape[y][x]}</td>`;
        }
        cells += `<th><strong>${y}</strong></th>`;
        var row =`<tr>${cells}</tr>`;
        rows += row;    
    }
    if(extra){
        var score_array = this.getScores();
        var scores = '';
        for(si in score_array){
            scores += `<th>${score_array[si]}</th>`;
        }
        rows += `<tr></tr><tr>${scores}<td colspan=>S</td></tr>`;
        score_array = this.getLandingZones(Triste.currentpiece);
        scores = '';
        var d = 0;
        for(i=0;i<this.width; i++){
            scores += `<th>${score_array.includes(i)?score_array[d]:'--'}</th>`;
            d+= score_array.includes(i)*1;
        }
        rows += `<tr></tr><tr>${scores}<td colspan=>LZ</td></tr>`;
    }
    return `<table>${hrows}<tbody>${rows}</tbody></table>`;
}

Map.prototype.simulate = function(shape, column){
    /* Simulate piece in map and return map data */
    var map = new Map(this.width, this.height);
    map.copy(this.data);
    for(shape.y;shape.y<map.height;shape.y++){        
        if(!shape.isPlaceable(map)){
            shape.y--;
            shape.write(map);
            return map;
        }
    }
    return map;
}

Map.prototype.getLandingZones = function(shape){
    var lc = shape.getLandingColumn();
    var zones = [];
    var s = new Piece(shape.shape, shape.x, shape.y);
    for(i=0; i<this.width; i++){
        var tx = i-lc;
        s.x = tx;
        if(s.isPlaceable(this)) zones[zones.length] = i;
    }
    return zones;
}

Map.prototype.getPenalty = function(shape, column){
    if(shape.y<0)return 0;
    var shape = new Piece(shape.shape, column, shape.y, shape.rotation);
    var s = shape.data[shape.rotation];
    var map = this.simulate(shape, column);
    var penalty = 0;
    var w = shape.getRealWidth();
    for(ri=map.height-1; ri>shape.y; ri--){
        for(ci=0; ci<w;ci++){
            var ii = shape.x + ci;
            var col = map.data[ri][ii];
            if(!col)penalty++;
        }
    }
    print(Triste.map.toTable(true), 'map');
    print(map.toTable(), 'map', false);
    print(`<pre style="float: right">Penalty: ${penalty}</pre>`, 'map', false)
    print(shape.toTable(), 'pieces');

}

