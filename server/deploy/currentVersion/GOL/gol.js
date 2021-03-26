var GOL = {
    width : 300,
    height: 150,
    hcells: [],
    cells : [],
    speed : 16,
    screen : undefined,
    buffer : undefined,

    initialize : function(w, h){
        GOL.width = w;
        GOL.height = h;        
        GOL.screen = $('canvas')[0].getContext('2d');
        $('canvas').attr('width', w);
        $('canvas').attr('height', h);
        GOL.buffer = GOL.screen.createImageData(w, h);
        for(y=0;y<GOL.height; y++){
            GOL.cells[y] = [];
            for(x=0;x<GOL.width; x++){
                GOL.cells[y][x] = new Cell(x,y);
            }
        }
    },

    diehard : function(x,y){
        x = parseInt(x);
        y = parseInt(y);
        GOL.cells[y+1][ x ].v = true;
        GOL.cells[y+1][x+1].v = true;
        GOL.cells[y+2][x+1].v = true;
        GOL.cells[y+2][x+5].v = true;
        GOL.cells[y+2][x+6].v = true;
        GOL.cells[y+2][x+7].v = true;
        GOL.cells[ y ][x+6].v = true;
    },

    acorn : function(x,y){
        x = parseInt(x);
        y = parseInt(y);
        GOL.cells[ y ][ x ].v = true;
        GOL.cells[y-2][x+1].v = true;
        GOL.cells[ y ][x+1].v = true;
        GOL.cells[y-1][x+3].v = true;
        GOL.cells[ y ][x+4].v = true;
        GOL.cells[ y ][x+5].v = true;
        GOL.cells[ y ][x+6].v = true;        
    },

    oscilator : function(x,y){
        x = parseInt(x);
        y = parseInt(y);
        GOL.cells[ y ][x-1].v = true;
        GOL.cells[ y ][ x ].v = true;
        GOL.cells[ y ][x+1 ].v = true;
    },

    start : function(w=320,h=240){
        GOL.initialize(w,h);
        GOL.acorn(w/2,h/2);
        for(i=0; i<10;i++){
            GOL.diehard(5+(Math.random()*(w-10)),5+(Math.random()*(h-10)));
        }
        //GOL.oscilator(w/2,h/2);
        GOL.loop();
    },

    snapshot : function(){
        for(y=0;y<GOL.height; y++){
            GOL.hcells[y] = []; 
            for(x=0;x<GOL.width; x++){
                GOL.hcells[y][x] = new Cell(GOL.cells[y][x].x, GOL.cells[y][x].y, GOL.cells[y][x].v);
            }
        }
    },

    update : function(){
        GOL.snapshot();
        for(y=0;y<GOL.height; y++){
            for(x=0;x<GOL.width; x++){
                GOL.cells[y][x].update();
            }
        }
    },

    display : function(){
        for(y=0;y<GOL.height; y++){
            for(x=0;x<GOL.width; x++){
                var u = ((y*GOL.width)+x)*4;
                var c = GOL.cells[y][x].v ? [0,0,0, 128] : [255,255,255,255];
                GOL.buffer.data[u] = c[0];
                GOL.buffer.data[u+1] = c[1];
                GOL.buffer.data[u+2] = c[2];
                GOL.buffer.data[u+3] = c[3];                
            }
        }
        GOL.screen.putImageData(GOL.buffer, 0, 0);     
    },

    loop : function(){
        if(Math.random()*100 < 1){
            for(i=0; i<10;i++){
                GOL.diehard(7+(Math.random()*(GOL.width-14)),7+(Math.random()*(GOL.height-14)));
            }
        }
        if(Math.random()*100 < 1){
            for(i=0; i<10;i++){
                GOL.acorn(7+(Math.random()*(GOL.width-14)),7+(Math.random()*(GOL.height-14)));
            }
        }
        GOL.display();        
        GOL.update();
        setTimeout(GOL.loop, GOL.speed);
    },
};