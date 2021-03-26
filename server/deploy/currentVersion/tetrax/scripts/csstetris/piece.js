function Piece(shape, x, y, rotation=0){
    var color_index = parseInt(Math.random()*Triste.colors.length);
    this.color = Triste.colors[color_index];
    this.shape = shape;
    this.data = shape.data;
    this.width = shape.width;
    this.height = shape.height;
    this.rotation = rotation;
    this.rows = this.getRows();
    this.columns = this.getColumns();
    this.x = x;
    this.y = y;
    this.id = Triste.piececount;
    this.active = true;
    this.target = undefined;
    this.target_angle = rotation;
}


Piece.prototype.getWeight = function(column){
    var weight=0;
    var cols = this.getColumns();
    column = cols[column];
    for(col_index=0; col_index < column.length; col_index++){
        weight += column[col_index];
    }
    return weight;
}

Piece.prototype.getAngleScores = function(){
    
}

Piece.prototype.findOptimalColumn = function(){
    var optimal_value = 0;
    var optimal_index = -1;
    var columns = [];
    for(foci=0; foci < this.shape.width; foci++){
        columns[foci] = this.getWeight(foci);
    }
    for(foci=0;foci < this.shape.width; foci++){
        if(columns[foci] > optimal_value){
            optimal_index = foci;
            optimal_value = columns[foci];
        }
    }        
    return optimal_index;
}

Piece.prototype.followAngle = function(){
    var fallback = this.rotation;
    if(this.rotation > this.target_angle)this.rotation--;
    if(this.rotation < this.target_angle)this.rotation++;
    if(!this.isPlaceable(Triste.map))this.rotation = fallback;
    else {
        $(`#${this.id}`).remove();
        this.render();
    }
}

Piece.prototype.followTarget = function(){
    this.followAngle();
    if(this.target == undefined ) return false;
    var old_x = this.x;
    if(this.target  > this.x    ) this.x++;
    else if(this.target  < this.x    ) this.x--;
    if(this.target == this.x    ) this.target = undefined;
    if(!this.isPlaceable(Triste.map))this.x = old_x;
}

Piece.prototype.findTarget = function(){
    /*
    List all possible landing spots, 
    Considerations:
    - Size of each spot is defined by horizontal surface in this angle
    - Every landing spot sharing same vertical dimension can be overlapped in the list
    - The lower the landing spot, the higher the score for this algoritm
    - More than a row can be taken in count in the piece and the map

    */

    if(this.y<0)return;
    while(!this.target){
        var old_x = this.x;
        var pseudo_x = parseInt(Math.random()*Triste.map.width);
        this.target_angle = parseInt(Math.random()*this.shape.data.length);
        this.x = pseudo_x;
        if(this.isPlaceable(Triste.map)) this.target = pseudo_x;
        this.x = old_x;
        this.update()
    }
}

Piece.prototype.getRealWidth = function(){
    /* 
    Return real piece width, skipping unpopulated columns
    */    
    var gwr = 0;
    var gwrlist = this.getColumns();
    for(gwri in gwrlist){
        var gw = gwrlist[gwri];
        var gwrskip=true;
        for(oz in gw){
            if(gw[oz])gwrskip=false;
        }
        if(!gwrskip)gwr++;
    }
    return gwr;
}


Piece.prototype.getLandingRow = function(){
    /*Returns first column whose block is landing, starting from bottom left */
    var glclist = this.getRows();
    /* Iterate all rows from bottom to top */
    for(glci=glclist.length-1; glci>=0; glci--){
        var glcrow = glclist[glci];
        /* Find first populated column in this row and return row index when found */
        for(glcri in glcrow){
            var glb = glcrow[glcri];
            if(glb) return glci;
        }
    }
}

Piece.prototype.getLandingColumn = function(){
    /*Returns first column whose block is landing, starting from bottom left */
    var glclist = this.getRows();
    /* Iterate all rows from bottom to top */
    var landing_row = this.getLandingRow();
    for(glci in glclist[landing_row]){
        var glb = glclist[landing_row][glci];
        if(glb) return glci;
    }
}

Piece.prototype.isPlaceable = function(_map){
    var ip_index=0;
    for(ip_row=0; ip_row < this.shape.height; ip_row++){
        for(ip_col=0; ip_col < this.shape.width; ip_col++){            
            if(this.shape.data[this.rotation][ip_index]==1){
                var x = this.x + ip_col;
                var y = this.y + ip_row;
                if(!_map.check(x,y)){
                    return false;
                }
            }
            ip_index++;
        }
    }
    return true;
}

Piece.prototype.logic = function(){
    this.update();
    this.y++;
    if(!this.isPlaceable(Triste.map)){
        this.y--;
        this.update();
        this.active = false;
        if(this.write(Triste.map))return Triste.NEWSHAPE;            
        else return Triste.GAMEOVER;
        
    }
    if(this.target) this.followTarget();
    else this.findTarget();
    this.update();
    var penalty = Triste.map.getPenalty(this, this.x);
    return Triste.CONTINUE;
}

Piece.prototype.write = function(_map){
    try {
        for(write_y = 0; write_y < this.height; write_y++){
            for(write_x = 0; write_x < this.width; write_x++){
                if(this.shape.data[this.rotation][(write_y*this.width) + write_x]) 
                    _map.data[this.y + write_y][this.x + write_x] = 1;
            }
        }
        return true;
    } catch(e){
        console.error(e);
        return false;    
    }
}

Piece.prototype.update = function(){
    var node = $(`table.triste#${this.id}`);
    node.attr('x', this.x);
    node.attr('y', this.y);
    node.css('left' , `${parseInt(node.attr('x')) * 16}px`);
    node.css('top'  , `${parseInt(node.attr('y')) * 16}px`);
}

Piece.prototype.render = function() {
    var html = '';
    for(py=0; py < this.shape.height; py++){
        html += '<tr>'
        for(px=0; px < this.shape.width; px++){
            var value =  this.data[this.rotation][(py*this.width)+px];
            var piece_class = `${this.color} ${ value ? 'piece':''}`;
            html += `<td><span class="${piece_class}"><span></td>`;
        }
        html += '</tr>';
    }
    $('#scene').append(`<table 
        id="${this.id}" 
        class="triste ${this.type}" 
        x="${this.x}" 
        y="${this.y}" 
        width="${this.shape.width}" 
        height="${this.shape.height}" 
        type="${this.shape.name}" 
        rotation="${this.rotation}"
    >
        ${html}
    </table>`);
    this.update();
};

Piece.prototype.toTable = function() {
    var shape = this.data[this.rotation];
    var hrows = '';
    var rows = '';
    var cells = '';
    for(x=0; x<this.width+1; x++){
        cells += `<th><strong>${x>0?x-1:''}</strong></th>`;
    }
    hrows = `<thead><tr>${cells}</tr></thead>`;
    for(y=0; y<this.height; y++){
        cells = `<th><strong>${y}</strong></th>`;
        for(x=0; x<this.width; x++){
            cells += `<td class="${shape[(y*this.width)+x]?'dark':''}">${shape[(y*this.width)+x]}</td>`;
        }
        var row =`<tr>${cells}</tr>`;
        rows += row;    
    }
    return `<table>${hrows}<tbody>${rows}</tbody></table>`;
}

Piece.prototype.getRows = function(){
    /*
    Returns a set of rows from the set of rows conforming the shape
    */
    var rows = [];
    for(row_index=0; row_index < this.shape.height; row_index++){
        var row = [];
        for(column_index=0; column_index < this.shape.width; column_index++){
            row[column_index] = this.shape.data[this.rotation][(row_index * this.shape.width)+column_index];
        }
        rows[row_index] = row;        
    }
    return rows;
}

Piece.prototype.getColumns = function(){
    /*
    Returns a set of columns from the set of rows conforming the shape
    */
    var cols = [];
    for(column_index=0; column_index < this.shape.width; column_index++){
        var col = [];
        for(row_index=0; row_index < this.shape.height; row_index++){
            col[row_index] = this.shape.data[this.rotation][(row_index*this.shape.width)+column_index];
        }
        cols[column_index] = col; 
    }
    return cols;
}
