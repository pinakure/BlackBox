function Cell(x,y,v=false){
    this.x = x;
    this.y = y;
    this.v = v;
}

/*
El "tablero de juego" es una malla plana formada por cuadrados (las "células") que se extiende por el infinito en todas las direcciones. 
Cada célula tiene 8 células "vecinas", que son las que están próximas a ella, incluidas las diagonales. 
Las células tienen dos estados: están "vivas" o "muertas" (o "encendidas" y "apagadas"). 
El estado de las células evoluciona a lo largo de unidades de tiempo discretas (se podría decir que por turnos). 
El estado de todas las células se tiene en cuenta para calcular el estado de las mismas al turno siguiente. 
Todas las células se actualizan simultáneamente en cada turno, siguiendo estas reglas:
Una célula muerta con exactamente 3 células vecinas vivas "nace" (es decir, al turno siguiente estará viva).
Una célula viva con 2 o 3 células vecinas vivas sigue viva, en otro caso muere (por "soledad" o "superpoblación").
*/

Cell.prototype.update = function(){
    var neighbors = [
        /*this.y > 0 && this.x > 0                        ? GOL.hcells[(this.y-1)%GOL.height][(this.x-1)&GOL.width].v : false,
        this.x > 0                                      ? GOL.hcells[(this.y  )%GOL.height][(this.x-1)%GOL.width].v : false,
        this.x > 0 && this.y < (GOL.height-1)           ? GOL.hcells[(this.y+1)%GOL.height][(this.x-1)%GOL.width].v : false,
        this.y > 0                                      ? GOL.hcells[(this.y-1)%GOL.height][(this.x  )%GOL.width].v : false,
        this.y < (GOL.height-1)                         ? GOL.hcells[(this.y+1)%GOL.height][(this.x  )%GOL.width].v : false,
        this.y > 0 && this.x < (GOL.width-1)            ? GOL.hcells[(this.y-1)%GOL.height][(this.x+1)%GOL.width].v : false,
        this.x < (GOL.width-1)                          ? GOL.hcells[(this.y  )%GOL.height][(this.x+1)%GOL.width].v : false,
        this.y < (GOL.height-1) && this.x <(GOL.width-1)? GOL.hcells[(this.y+1)%GOL.height][(this.x+1)%GOL.width].v : false,
        */
        this.y > 0 && this.x > 0                        ? GOL.hcells[(this.y-1)][(this.x-1)].v : false,
        this.x > 0                                      ? GOL.hcells[(this.y  )][(this.x-1)].v : false,
        this.x > 0 && this.y < (GOL.height-1)           ? GOL.hcells[(this.y+1)][(this.x-1)].v : false,
        this.y > 0                                      ? GOL.hcells[(this.y-1)][(this.x  )].v : false,
        this.y < (GOL.height-1)                         ? GOL.hcells[(this.y+1)][(this.x  )].v : false,
        this.y > 0 && this.x < (GOL.width-1)            ? GOL.hcells[(this.y-1)][(this.x+1)].v : false,
        this.x < (GOL.width-1)                          ? GOL.hcells[(this.y  )][(this.x+1)].v : false,
        this.y < (GOL.height-1) && this.x <(GOL.width-1)? GOL.hcells[(this.y+1)][(this.x+1)].v : false,
    ];
    
    var alive = 0;
    for(ni =0; ni<8; ni++){
        var n = neighbors[ni];
        if(n) alive++;
    }
    if( !this.v && (alive==3))this.v = true
    else if( this.v && (alive > 1) && (alive < 4)) this.v = true;
    else if(this.v && (alive < 2) || (alive > 3)) this.v = false;    
}