function truerand(mult){
    return parseInt(((Math.random()*Number.MAX_SAFE_INTEGER)/Number.MAX_SAFE_INTEGER)*mult);
}

function totable(json){
    var html = '<tr style="width:100%">';
    for(ji in json){
        var js = json[ji];
        html += `<td>${ji}:&nbsp;&nbsp;${js}&nbsp;&nbsp;</td>`;
    }
    return html + '</tr>';
}
function normalize(n){
    return n;
}

var Tetris = {
    q               : 3.5,
    w               : 1.0,
    e               : 100,
    r               : 100,
    width           : 0,
    height          : 0,
    map             : undefined,
    simulations     : [],
    bestmove        : undefined,
    inistate        : undefined,
    run             : true,
    speed           : 1,
    all_ancestors_samescore : false,
    animate         : true,
    lines           : 0,
    max_lines       : 0,
    generations     : [],
    generation      : 0,
    time            : 0,
    target          : 1000,
    sim_scores      : false,
    draw_sim        : false,
    draw_map        : true,
    ancestors       : [ { generation:1, q:3.5, w:1, e:100, r:100, lines:99, time:20697, score:0.0099, } ],
    destiny         : { data : [], index:0 },
    breed_size      : 8,//16,
    mutation_rate   : 2,// the lower, the higher mutations
    
    predestination : true,
    
    initialize  : function(width=10, height=20, q=3.5, w=1.0, e=100, r=100){
        var ancestor = Tetris.best_ancestor();
        Tetris.q = ancestor.q;//q;
        Tetris.w = ancestor.w;//w;
        Tetris.e = ancestor.e;//e;
        Tetris.r = ancestor.r;//r;
        Tetris.width = width;
        Tetris.height = height;
        for(r=0; r<65535;r++){
            Tetris.destiny.data[r] = truerand(7);
        }
        $('#output').append(totable({'BEGIN GENERATION':Tetris.generation}));
        Tetris.restart();        
    }, 

    restart     : function(){
        Tetris.time = performance.now();
        Tetris.map = new TetrisMap(Tetris.width, Tetris.height);
        if(Tetris.max_lines<Tetris.lines)Tetris.max_lines=Tetris.lines;
        Tetris.lines = 0;
        Tetris.destiny.index = 0;
        Tetris.newPiece();
        Tetris.loop();
    },

    storeresults : function(){
        var generation = {
            generation  : Tetris.generations.length+1,
            q           : Tetris.q,
            w           : Tetris.w,
            e           : Tetris.e,
            r           : Tetris.r,
            lines       : Tetris.lines,
            time        : performance.now()-Tetris.time,
            score       : Tetris.lines / Tetris.target,
        };
        Tetris.generations[Tetris.generations.length] = generation;
        $('#output').append(totable(generation));
        if(Tetris.generations.length==Tetris.breed_size){
            Tetris.ancestors=Tetris.generations;
            var ancestor = Tetris.best_ancestor();
            $('#output').append(Tetris.all_ancestors_samescore?"EVOLUTION DISCARDED":"");
            if(!Tetris.all_ancestors_samescore){
                Tetris.mutation_rate*=1.25;//afine
            } else {
                Tetris.mutation_rate/=2.5;//blur
            }
            Tetris.generations = [];
            Tetris.generation++;
            Tetris.q = ancestor.q;
            Tetris.w = ancestor.w;
            Tetris.e = ancestor.e;
            Tetris.r = ancestor.r;
            $('#output').append(totable({'BEGIN GENERATION':Tetris.generation}));
        } else {
            Tetris.q += (truerand(20000)-10000)/Tetris.mutation_rate;
            Tetris.w += (truerand(20000)-10000)/Tetris.mutation_rate;            
            Tetris.e += (truerand(20)-10)/Tetris.mutation_rate;
            Tetris.r += (truerand(200)-100)/Tetris.mutation_rate;                        
        }
        if(Tetris.q < 0)Tetris.q *= -1;
        if(Tetris.w < 0)Tetris.w *= -1;
        if(Tetris.e < 0)Tetris.e *= -1;
        if(Tetris.r < 0)Tetris.r *= -1;
        normalize(Tetris.q);
        normalize(Tetris.w);
        normalize(Tetris.e);
        normalize(Tetris.r);
        Tetris.restart();
        Tetris.show_info(!Tetris.animate);
    },

    show_info   : function(condition){
        if(!condition)return;
        /*$('#info').html(`
            <table class="lines"><tbody>
            <tr><th>GENERATION</th> <td>&nbsp;</td><td>${Tetris.generation}</td></tr>
            <tr><th>BREED</th>      <td>&nbsp;</td><td>${Tetris.generations.length} / ${Tetris.breed_size}</td></tr>
            <tr><th>LINES</th>      <td>&nbsp;</td><td>${Tetris.lines} / ${Tetris.max_lines}</td></tr>
            <tr><th>MUTABILITY</th> <td>&nbsp;</td><td>${`${1/Tetris.mutation_rate}`}</td></tr>
            <tr><th>Q</th>          <td>&nbsp;</td><td>${`${Tetris.q}`.substr(0, 6)}</td></tr>
            <tr><th>W</th>          <td>&nbsp;</td><td>${`${Tetris.w}`.substr(0, 6)}</td></tr>
            <tr><th>E</th>          <td>&nbsp;</td><td>${`${Tetris.e}`.substr(0, 6)}</td></tr>
            <tr><th>R</th>          <td>&nbsp;</td><td>${`${Tetris.r}`.substr(0, 6)}</td></tr>
            <tr><th>T</th>          <td>&nbsp;</td><td>${`${(`${(performance.now()-Tetris.time)/1000}`).substr(0,4)}${(Tetris.generations.length>0?` / ${Tetris.generations[Tetris.generations.length-1].time/1000}`:'').substr(0,7)}`}</td></tr>
            </tbody>
            </table>
        `);*/    
        $('#info').html(`
            <div><b>GENERATION</b> ${Tetris.generation}</div>
            <div><b>BREED</b>      ${Tetris.generations.length} / ${Tetris.breed_size}</div>
            <div><b>LINES</b>      ${Tetris.lines} / ${Tetris.max_lines}</div>
            <div><b>MUTABILITY</b> ${`${1/Tetris.mutation_rate}`}</div>
            <div><b>Q</b>          ${`${Tetris.q}`.substr(0, 6)}</div>
            <div><b>W</b>          ${`${Tetris.w}`.substr(0, 6)}</div>
            <div><b>E</b>          ${`${Tetris.e}`.substr(0, 6)}</div>
            <div><b>R</b>          ${`${Tetris.r}`.substr(0, 6)}</div>
            `);    
            //<div><b>T</b>          ${`${(`${(performance.now()-Tetris.time)/1000}`).substr(0,4)}${(Tetris.generations.length>0?` / ${Tetris.generations[Tetris.generations.length-1].time/1000}`:'').substr(0,7)}`}</div>
    },
    
    loop        : function(){
        Tetris.inistate   = new TetrisMap(Tetris.map.width, Tetris.map.height, Tetris.map.data);
        if(!Tetris.animate){
            /* This is because at level 9, pieces spend 500ms to reach the bottom, so each block travelled
            means a loss of a fraction of the whole time , therefore we can simulate 'time spent playing'*/
             Tetris.time -=500/Tetris.height;
        } 
        
        if(!Tetris.piece.logic()){
            Tetris.show_info(Tetris.animate);
            Tetris.inistate = Tetris.piece.write(Tetris.inistate);
            Tetris.map = Tetris.piece.write(Tetris.map);            
            if(Tetris.piece.y<0){
                return Tetris.storeresults();                 
            }
            Tetris.newPiece();
            if(!Tetris.animate){
                Tetris.display();
            }
        } else {
            Tetris.show_info(Tetris.animate);
            Tetris.map = Tetris.piece.write(Tetris.map);            
        }
        if(Tetris.animate){
            Tetris.display();
        }
        Tetris.map = new TetrisMap(Tetris.map.width, Tetris.map.height, Tetris.inistate.data);
        l =  Tetris.map.getLines();
        if(l>0){
            Tetris.lines += l;
            Tetris.map.removeLines();
            Tetris.show_info(!Tetris.animate);
        }
        if(Tetris.run){
            setTimeout(Tetris.loop, Tetris.speed);
        }
    },

    newPiece    : function(){
        Tetris.piece = undefined;
        do {
            if(Tetris.predestination){
                Tetris.piece = Tetris.addPiece(Tetris.destiny.data[Tetris.destiny.index]);
                Tetris.destiny.index++;
                Tetris.destiny.index %= 65535;
            } else { 
                Tetris.piece = Tetris.addPiece(truerand(7));
            }
        } while(!Tetris.piece);
        Tetris.simulations = Tetris.simulate();
        Tetris.bestmove = Tetris.bestMovement();
        Tetris.piece.x        = Tetris.bestmove.x;
        Tetris.piece.rotation = Tetris.bestmove.rotation;
        if(Tetris.draw_sim) Tetris.displaySimulations();        
    },

    addPiece        : function(type){
        var column  = 0;
        var row     = -Shapes[type].height;
        var rotation= 0;
        return Tetris.map.addShape(type, column, row, rotation);
    },

    evaluateSimulations: function(){
        var scores = [];
        var angle_count = Shapes[Tetris.piece.type].data.length;
        var best_score = 0;
        var best_index = 0;            
        var best_offset= 0;            
        for(ai=0; ai < angle_count; ai++){
            var simset = Tetris.simulations[ai];
            best_score = 0;
            best_index = 0;
            best_offset = 0;
            for(rz=0; rz < simset.length; rz++){
                var sim = simset[rz];
                if( Tetris.sim_scores ){
                    var node = $(`body > div.canvasbox.canvas-${ai} > div.canvaswrapper.canvaswrapper-angle_${ai}_${rz} .score`);
                    node.html(`&nbsp;<b>S:${sim.score}</b>&nbsp;<b class="red">P:${sim.penalty}<b>&nbsp;<b class="green">CS:${sim.columnscore}<b>`);
                }
                if(sim.score <= best_score)continue;                    
                best_score = sim.score;
                best_offset= sim.piece.landingColumn();
                best_index = sim.position - best_offset;
            }
            scores[ai] = { value: best_score, index: best_index, offset: best_offset };
        }
        return scores;
    },
   
    bestMovement: function(){
        var scores = Tetris.evaluateSimulations();        
        var best_score = 0;
        var best_index = 0;
        var best_offset = 0;
        var best_angle = 0;
        for(si=0; si<scores.length;si++){
            var score = scores[si];
            if(score.value <= best_score)continue;
            best_score = score.value;
            best_index = score.index;
            best_offset= score.offset;
            best_angle = si;
        }
        if(Tetris.show_sim){
            $('.canvaswrapper').css('background', '#ffffff');
            $(`.canvaswrapper-angle_${best_angle}_${best_index+best_offset}`).css('background', '#ffa0a0');
        }
        return new Movement(best_index, Tetris.piece.y, best_angle);        
    },

    display     : function(){
        if(Tetris.draw_map)Tetris.displayMap();
    },

    displayMap : function(){
        if(!map)return;
        Tetris.map.render('map');
    },
    displaySimulations:function(){
        for(ai=0; ai < Shapes[Tetris.piece.type].data.length; ai++){
            var simset = Tetris.simulations[ai];
            if(!simset)continue;
            simset[0].map.render(`angle_${ai}`);
            for(si=0; si<simset.length; si++){
                var sim = simset[si];
                sim.render(`angle_${ai}_${si}`);
            }
        }
    },

    show_results : function(){
        window.open("data:text/json," + encodeURIComponent(JSON.stringify(Tetris.generations)),"_blank").focus();
    },
    
    simulate    : function(){
        piece = Tetris.piece;
        var simulations = [];
        var angle_count = Shapes[piece.type].data.length;
        for(a=0; a<angle_count; a++){
            simulations[a] = [];
            for(z=0; z<Tetris.map.width; z++){
                simulations[a][z] = new Simulation(Tetris.map, piece, z, a, Tetris.q, Tetris.w, Tetris.e, Tetris.r);
            }
        }
        return simulations;
    },

    best_ancestor : function(){
        var best = undefined;
        var hi_score = 0
        for(ai=0; ai<Tetris.ancestors.length; ai++){
            var an = Tetris.ancestors[ai];
            if(an.score > hi_score) hi_score = an.score;
        }
        Tetris.all_ancestors_samescore = true;
        for(ai=0; ai<Tetris.ancestors.length; ai++){
            var an = Tetris.ancestors[ai];
            if(an.score != hi_score)Tetris.all_ancestors_samescore = false;
        }
        for(ai=0; ai<Tetris.ancestors.length; ai++){
            var an = Tetris.ancestors[ai];
            if(an.score > (best ? best.score : 0)){
                best = an;
            }
        }
        return best;
    },
};

