var Triste = {
    CONTINUE    : 0x1,
    NEWSHAPE    : 0x2,
    GAMEOVER    : 0x3,
    colors      : [ 'green', 'red', 'yellow', 'blue'],
    gameover    : false,
    width       : 16,
    height      : 32,
    map         : [],
    piececount  : 0,
    currentpiece: undefined,
    /*-----------------------------------------------------------------------*/    
    initialize      : function(){
        /* Initialize map memory */
        Triste.map = new Map(Triste.width, Triste.height);
        Triste.map.create();
        /* Create first piece */
        Triste.newshape();
        /* Call first logic cycle */
        window.setTimeout(Triste.logic, 60);        
    },
    newgame         : function(){
        $('#scene').remove();
        this.initialize();
    },
    /*-----------------------------------------------------------------------*/
    newshape        : function(){
        Triste.currentpiece = undefined;
        do {
            Triste.currentpiece = Triste.addPiece(parseInt(Math.random()*7));
        } while(!Triste.currentpiece);
        var column  = Triste.findOptimalColumn();
        var rotation= Triste.findOptimalAngle();
        
    },
    /*-----------------------------------------------------------------------*/
    addPiece        : function(type){
        var shape   = Shapes[type];
        var column  = 0;//Triste.findOptimalColumn();
        var row     = -shape.height;
        var rotation= 0;//Triste.findOptimalAngle();
        return Triste.map.addShape(shape, column, row, rotation)        
    },
    /*-----------------------------------------------------------------------*/
    logic           : function(){
            var p = Triste.currentpiece;
            switch( p.logic() ){
                case Triste.GAMEOVER:
                    alert("Game Over!");
                    Triste.gameover = true;
                    Triste.newgame();
                    return;
                case Triste.NEWSHAPE:
                    Triste.newshape();
                    break;
                default:
                case Triste.CONTINUE:
                    break;
            }
            if(!Triste.gameover) window.setTimeout(Triste.logic, 60);
            
        
    },
    /*-----------------------------------------------------------------------*/
    findOptimalColumn : function(){
        return 1;
        var optimal_value = Triste.height;
        var optimal_index = -1;
        var columns = [];
        for(ci=0;ci<Triste.width; ci++){
            columns[ci] = Triste.getColumnWeight(ci);
        }
        for(ci=0;ci<Triste.width; ci++){
            if(columns[ci] < optimal_value){
                optimal_index = ci;
                optimal_value = columns[ci];
            };
        }
        return optimal_index;
    },
    
    /*-----------------------------------------------------------------------*/
    findOptimalAngle: function(){
        /* Evaluate match score for each angle */
        var shape = Triste.currentpiece;
        var angle = shape.angle;
        var angles = [];
        for(__i = 0; __i < shape.data.length; __i++){
            angles[__i] = [ shape.getColumns() ]; 
            //angles[__i] = [ shape.getRows() ]; 
            shape.rotation++;  /* move forward in angle list */
            shape.rotation%=shape.data.length;/* clamp to angle set size */
        }
        /*
        var scores = getScores(angles);
        
        */
        return 0;
    },
    /*-----------------------------------------------------------------------*/
    getColumnWeight : function(column){
        var weights = Triste.map.getWeights();
        return weights[column];
    },
    /*-----------------------------------------------------------------------*/
    removeLines : function(mapdata, lineset){
        /* Returns mapdata without new lines, inserting new lines over the top */
        var new_mapdata = [];
        var map_height = mapdata.length;
        var map_width  = mapdata[0].length;
        for(map_row_index in lineset){
            mapdata[map_row_index] = [];
        }
        for(map_row_index in mapdata){
            if(mapdata[map_row_index] != []) new_mapdata[new_mapdata.length] = mapdata[map_row_index];
        }
        var new_row = [];
        for(col_index=0; col_index < map_width; col_index++){
            new_row[new_row.length] = 0;
        }
        for(line_index=0; line_index < map_height-new_mapdata.length; line_index++){
            new_mapdata.splice(0,0,new_row);   
        }
        return new_mapdata;
    },
    /*-----------------------------------------------------------------------*/
    findLines    : function(mapdata){
        /* Return list of line indices to be removed if any */
        var lines = [];
        for(map_row_index in mapdata){
            var line = true;
            for(map_col_index in mapdata){
                if(!mapdata[map_row_index][map_col_index]) line = false;
            }
            if(line) lines[lines.length] = map_row_index;
        }    
        return lines;
    },
    /*-----------------------------------------------------------------------*/
    wouldCauseLine  : function(shape, x, y){
        /* Return true if given shape data would produce a new line , scoring */
        var virtual_map = Triste.map;
        for(row_index in shape){
            row = shape[row_index];
            for(col_index in row){
                virtual_map[y+row_index][x+col_index] &= shape[row_index][col_index];
            }
        }
        var lines = findLines(virtual_map);
        return lines.length > 0;
    },
};

function Intent(shapedata){
    this.penalty= 0;
    this.shape  = shapedata;
    
}


/*
RULES:

- Select first holes > 1 cell (penalty single holes)
- Select first holes accesible lower (penalty those possible but higher)
- Select position if all lower squares fit ground or another square

*/
