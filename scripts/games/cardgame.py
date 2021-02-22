def initialize():
    cards = [
        { "id" : 19, "title" : "Flechas Aéreas"      , "element" : 3, "top" : 2, "left" : 3, "right" : 4, "bottom" : 5, "score": 5, "picture" : "/media/item_14.png"      },
        { "id" : 20, "title" : "Tomo: Inundacion"    , "element" : 2, "top" : 4, "left" : 5, "right" : 6, "bottom" : 4, "score": 6, "picture" : "/media/item_29.png"      },
        { "id" : 21, "title" : "Lanza Draconiana"    , "element" : 3, "top" : 4, "left" : 5, "right" : 5, "bottom" : 4, "score": 9, "picture" : "/media/item_7.png"       },
        { "id" : 22, "title" : "Antorcha"            , "element" : 1, "top" : 2, "left" : 3, "right" : 4, "bottom" : 3, "score": 2, "picture" : "/media/13.png"           },
        { "id" : 23, "title" : "Poción Acuática"     , "element" : 2, "top" : 5, "left" : 4, "right" : 2, "bottom" : 1, "score": 9, "picture" : "/media/33.png"           },
        { "id" : 24, "title" : "Poción Ígnea"        , "element" : 1, "top" : 1, "left" : 2, "right" : 3, "bottom" : 6, "score": 7, "picture" : "/media/37.png"           },
        { "id" : 25, "title" : "Pocion Electrica"    , "element" : 5, "top" : 5, "left" : 2, "right" : 2, "bottom" : 1, "score": 9, "picture" : "/media/34.png"           },
        { "id" : 26, "title" : "Pergamino: Tsunami"  , "element" : 2, "top" : 7, "left" : 2, "right" : 3, "bottom" : 4, "score": 9, "picture" : "/media/10_LRpjZGL.png"   },
        { "id" : 27, "title" : "Pergamino: Terremoto", "element" : 4, "top" : 5, "left" : 6, "right" : 7, "bottom" : 8, "score": 8, "picture" : "/media/9.png"            },
        { "id" : 28, "title" : "Anillo electrico"    , "element" : 5, "top" : 7, "left" : 2, "right" : 3, "bottom" : 4, "score": 6, "picture" : "/media/19.png"           },
        { "id" : 29, "title" : "Flechas de Fuego"    , "element" : 1, "top" : 3, "left" : 4, "right" : 5, "bottom" : 6, "score": 5, "picture" : "/media/7.png"            },
        { "id" : 30, "title" : "Espada Hielo"        , "element" : 2, "top" : 2, "left" : 3, "right" : 4, "bottom" : 5, "score": 9, "picture" : "/media/16.png"           },
        { "id" : 31, "title" : "Hechizo Fuego"       , "element" : 1, "top" : 7, "left" : 4, "right" : 5, "bottom" : 2, "score": 4, "picture" : "/media/tome.png"         },
        { "id" : 32, "title" : "Pocion Incendiaria"  , "element" : 1, "top" : 5, "left" : 4, "right" : 3, "bottom" : 4, "score": 9, "picture" : "/media/potionRed.png"    },
        { "id" : 33, "title" : "Arco de Fuego"       , "element" : 1, "top" : 3, "left" : 4, "right" : 5, "bottom" : 3, "score": 6, "picture" : "/media/bow.png"          },
        { "id" : 34, "title" : "Hacha Gaia"          , "element" : 4, "top" : 4, "left" : 3, "right" : 2, "bottom" : 1, "score": 9, "picture" : "/media/upg_axe.png"      },
        { "id" : 35, "title" : "Martillo Acuático"   , "element" : 2, "top" : 5, "left" : 4, "right" : 2, "bottom" : 3, "score": 5, "picture" : "/media/hammer.png"       },
        { "id" : 36, "title" : "Espada Trueno"       , "element" : 5, "top" : 3, "left" : 5, "right" : 2, "bottom" : 4, "score": 3, "picture" : "/media/upg_sword.png"    },
        { "id" : 37, "title" : "Espada Marciana"     , "element" : 1, "top" : 3, "left" : 5, "right" : 2, "bottom" : 6, "score": 4, "picture" : "/media/item_9.png"       },
        { "id" : 38, "title" : "Escoba Magica"       , "element" : 3, "top" : 5, "left" : 3, "right" : 2, "bottom" : 4, "score": 7, "picture" : "/media/10.png"           },
        { "id" : 39, "title" : "Cetro Eléctrico"     , "element" : 5, "top" : 2, "left" : 4, "right" : 5, "bottom" : 3, "score": 9, "picture" : "/media/6.png"            },
        { "id" : 40, "title" : "Cetro de tierra"     , "element" : 4, "top" : 5, "left" : 4, "right" : 3, "bottom" : 2, "score": 9, "picture" : "/media/8.png"            },
        { "id" : 41, "title" : "Cetro de Fuego"      , "element" : 1, "top" : 2, "left" : 3, "right" : 4, "bottom" : 5, "score": 8, "picture" : "/media/2.png"            } 
    ]
    for card in cards:
        #create cards
        pass

    def setElements():
        elements = [
            { "id" : 1, "name" : "Fire"      , "picture" : "/media/element_fire.png"     , "color" : "f04000", "weak" : 2, "pown" : 4 },
            { "id" : 2, "name" : "Water"     , "picture" : "/media/element_water.png"    , "color" : "0080f0", "weak" : 5, "pown" : 1 },
            { "id" : 3, "name" : "Air"       , "picture" : "/media/element_air.png"      , "color" : "c0e0f0", "weak" : 4, "pown" : 5 },
            { "id" : 4, "name" : "Earth"     , "picture" : "/media/element_earth.png"    , "color" : "c08040", "weak" : 1, "pown" : 3 },
            { "id" : 5, "name" : "Lightning" , "picture" : "/media/element_lightning.png", "color" : "f0c0f0", "weak" : 4, "pown" : 2 } 
        ]
        for element in elements:
            #create elements 
            pass
        for element in elements:
            #logWrite('element tier', element['name'])        
            #c = Element.objects.get(id=element['id'])
            #if element['weak']:
            #    c.weak = Element.objects.get(id=element['weak'])
            #if element['pown']:
            #    c.pown = Element.objects.get(id=element['pown'])
            #c.save()


var gamedata = undefined;
var cards = undefined;
var elements = undefined;
var players = undefined;

var POSITION = {
    'LEFT'  : 1,
    'TOP'   : 2,
    'RIGHT' : 3,
    'BOTTOM': 4,
};

var POSITION_NAME = {
    1 : 'left',
    2 : 'top',
    3 : 'right',
    4 : 'bottom',
};

var GAMESTATUS = {
    'CHALLENGE' : 0,
    'COIN_TOSS' : 1,
    'HELLO'     : 2,
    'TURN_START': 3,
    'TURN_RESULT':4,
    'TURN_END'  : 5,
    'VICTORY'   : 10,
    'DEFEAT'    : 11,
    'DRAW'      : 12,
    'END'       : 100,
};

function GameZone(game_id){
    this.game_id = game_id;
    this.cards = undefined;
    this.cells = undefined;
    $('body').append('<gamezone></gamezone>');
    this.dom = $('gamezone');
    this.newGame();
}

GameZone.prototype.isFree = function(column, row){
    index = (row * 3)+column;
    node = $(`gamezone div:nth-child(${index+1}) > cardwrapper`);
    if(node[0] == undefined){
        return true;
    }
    return false;
}

GameZone.prototype.addCard = function(card, column, row){    
    index = (row*3)+column;
    this.cards[ index ] = card;
    this.cells[ index ].html(card.dump());
}


GameZone.prototype.click = function(self){
    cx = parseInt(self.id % 3);
    cy = parseInt(self.id / 3);    
    free = game.gamezone.isFree(cx,cy);
    if(free){
        board = game.board[game.turn] 
        if(board.selection == -1) return;
        board.target_col = cx;
        board.target_row = cy;            
        if(board.moveCard()){
            game.attack(cx, cy);            
        }
        return;        
    }
    console.log(this.cards[(cy*3)+cx]);
}

GameZone.prototype.newGame = function(){
    this.dom.html("");
    this.cards = [];
    this.cells = [];
    for(div_i = 0; div_i < 9; div_i++){
        this.dom.append(`<div id="${div_i}"></div>`);
        this.cells[div_i] = $(`gamezone > div*:nth-child(${div_i+1})`);
        gz_self = this;
        this.cells[div_i].click(function(){gz_self.click(this);});
    }    
}

function Board(player_index) {
    this.player     = player_index;
    this.position   = this.player == 0 ? "bottom":"top" ;
    this.turn       = false;
    this.cells      = undefined;
    this.hand       = undefined;    
    this.target_col = -1;
    this.target_row = -1;
    this.selection  = -1;
    $('body').append(`<board class="${this.position}"></board>`);
    
    this.newBoard();
}

Board.prototype.newBoard = function(){
    this.cells      = [];
    this.hand       = [];
}

Board.prototype.selectCard = function(card_index){
    if(game.turn != this.player) return;
    this.selection = card_index;
    $(`board.${this.position} cardwrapper card`).each(function(){
        $(this).removeClass('selected');
    });

    this.cells[card_index].children('card').addClass('selected');
}

Board.prototype.moveCard = function(){
    column = this.target_col;
    row = this.target_row;
    card_index = this.selection;
    // console.log(`moveCard(${card_index}, ${column}, ${row})`);
    /* First integrity check */
    if(card_index < 0 || card_index >= 5) return 0;
    /* Second integrity check */
    card = this.hand[card_index];
    if(card === undefined || card == undefined ) return 0;
    /* Third integrity check */
    if(!game.gamezone.isFree(column,row))return 0;
    /* Move card */
    delete this.hand[card_index];
    this.cells[card_index].remove();
    game.gamezone.addCard(card, column, row);
    this.target_col = -1;
    this.target_row = -1;
    this.selection = -1;
    // $(`board.${this.position}`).removeClass('turn');
    // $(`board.${game.board[this.player^1].position}`).addClass('turn');
    return 1
}

Board.prototype.addCard = function(card_json){
    card = new Card(card_json, this.player);
    this.hand.push(card);
            
    $(`board.${this.position}`).append(card.dump());
    this.cells[this.cells.length]=$(`board.${this.position} cardwrapper:last-child`);    
    this.cells[this.cells.length-1].attr('id', this.cells.length-1);
    this.cells[this.cells.length-1].attr('player', this.player);
    this.cells[this.cells.length-1].attr('onclick', `game.board[${this.player}].selectCard(${this.cells.length-1})`);
}

function Card(cardspecimen, player_index){
    this.id = cardspecimen.id;
    this.level = cardspecimen.level;
    this.stock = cardspecimen.stock;
    this.title = cardspecimen.title;
    this.score = cardspecimen.score;
    this.picture = cardspecimen.picture;    
    this.top = cardspecimen.top;
    this.bottom = cardspecimen.bottom;
    this.left = cardspecimen.left;
    this.right = cardspecimen.right;
    this.element = cardspecimen.element;
    this.playerclass = player_index==0 ?'A':'B';
    this.player = player_index;
}

function darker(color){
    r = parseInt(`${color[0]}${color[1]}`, 16);
    g = parseInt(`${color[2]}${color[3]}`, 16);
    b = parseInt(`${color[4]}${color[5]}`, 16);
    r = parseInt(r / 4).toString(16);
    g = parseInt(g / 4).toString(16);
    b = parseInt(b / 4).toString(16);
    if (r.length % 2) r = '0' + r;
    if (g.length % 2) g = '0' + g;
    if (b.length % 2) b = '0' + b;
    return `${r}${g}${b}`
}

Card.prototype.dump = function(){
    return(`<cardwrapper>`+
        `<card class="${this.playerclass}">`+
        `<cardcolor style="background: linear-gradient(12deg,  #${this.element > 0 ? darker(elements[this.element-1].color) : "#808080"}, #${elements[this.element-1].color}) !important;"></cardcolor>`+
        `<cardimage style="background-image:url('${this.picture}');"></cardimage>`+
        `<cardtitle>${this.title}</cardtitle>`+
        `<cardstats>`+
            `<stat id="top">${this.top}</stat>`+
            `<stat id="left">${this.left}</stat>`+
            `<stat id="right">${this.right}</stat>`+
            `<stat id="bottom">${this.bottom}</stat>`+
            (this.element > 0 ? `<stat id="element"><img src="${elements[this.element-1].picture}" width="32" height="32"/></stat>`: '')+
        `</cardstats>`+
        `<cardscore>${this.score}</cardscore>`+
    `</card></cardwrapper>`);
}

/*@SERVER */
function Game(cards, elements, gamedata, players){
    $('gamezone').each( function(){ $(this).remove(); });
    $('board').each( function(){ $(this).remove(); });
    $('score').each( function(){ $(this).remove(); });
    this.flipping = false;
    this.score = 5; // if score 0 = score player 1 = 9, player2 = 0, if == 9 = score player 1 = 0, player2 = 9
    this.gamezone = new GameZone(gamedata.id);
    this.board = [ new Board(0), new Board(1)];
    this.cards = cards;
    this.elements = elements;
    this.players = gamedata.players;
    this.turn = 0;
    this.chain_combo_enable = true;
    this.element_adjustment_enable = true;
    this.turns = 9;
    this.debug = 1;
    if(this.debug>=2){
        console.log("GAMEDATA", gamedata);
        console.log("PLAYERS", players);
        console.log("ELEMENTS", elements);
        console.log("CARDS", cards);
    }
    this.status = GAMESTATUS.CHALLENGE;
    $('body').append('<score class="top">5</score><score class="bottom turn">5</score>');
}

Game.prototype.flip = function(column, row) {
    self = this;
    if(this.flipping) {
        setTimeout(function(){game.flip(column, row)}, 500); 
        return;
    }
    index = (row * 3) + column;
    card = this.gamezone.cards[index];
    node = $(`gamezone div:nth-child(${index+1})  cardwrapper card`);

    if(card.playerclass=='A'){
        game.score++;
        node.removeClass('A flipBA flipAB');
        card.player = 1;        
        node.addClass('flipAB');       
        node.parent().parent().addClass('flipAB');
    } else if(card.playerclass=='B'){
        game.score--;
        node.removeClass('B flipBA flipAB');
        card.player = 0;
        node.addClass('flipBA');       
        node.parent().parent().addClass('flipBA');
    }
    setTimeout(function(){ game.assignCard(index,card.player); }, 800);
    this.flipping = true;

    $('score.top').html(5 + (game.score-5));
    $('score.bottom').html(5 - (game.score-5));
    if(game.chain_combo_enable){
        game.attack(column, row, true /*ignore turn rotation*/ );// chain
    }
}


/*@SERVER */
Game.prototype.evaluate = function(attack_card, defend_card, defending_position){
    function __cardFromSamePlayer(){ console.log(`${POSITION_NAME[defending_position]}:The card is from same player`); return false; }
    function __noDefendCard(){ console.log(`${POSITION_NAME[defending_position]}:No defending card`); return false; }
    function __noAttackCard(){ console.log(`${POSITION_NAME[defending_position]}:No attacking card`); return false; }
    function __cardStrong(){ console.log(`${POSITION_NAME[defending_position]}:Attacking card is strong against ${defend_card.element}`); return false; }
    function __cardWeak(){ console.log(`${POSITION_NAME[defending_position]}:Attacking card is weak against ${defend_card.element}`); return false; }
    function __flipping(){ console.log(`${POSITION_NAME[defending_position]}:Flipping card`); return false; }


    if(defend_card == undefined) return __noDefendCard();
    if(attack_card == undefined) return __noAttackCard();
    if(attack_card.player == defend_card.player) return __cardFromSamePlayer();

    defense = 0;
    attack = 0;

    switch(defending_position){
        case POSITION.TOP   : defense = defend_card.top; attack = attack_card.bottom; break;
        case POSITION.BOTTOM: defense = defend_card.bottom; attack = attack_card.top; break;
        case POSITION.LEFT  : defense = defend_card.left; attack = attack_card.right; break;
        case POSITION.RIGHT : defense = defend_card.right; attack = attack_card.left; break;
    }
    
    if(game.element_adjustment_enable){
        attack_element = game.elements[attack_card.element-1];
        defend_element = game.elements[defend_card.element-1];
        if(attack_element.weak == defend_element.id){ __cardWeak(); attack--; game.display("WEAK!");}
        if(attack_element.pown == defend_element.id){ __cardStrong(); attack++;game.display("STRONG!"); }
        if(defend_element.pown == attack_element.id) { __cardWeak(); attack--; game.display("WEAK!");}
        if(defend_element.weak == attack_element.id) { __cardStrong(); attack++;game.display("STRONG!"); }
        if(attack > defense)__flipping();
    }

    return attack > defense;
}

/*@SERVER */
Game.prototype.tier = function(column, row, position){
    if(row < 0)return false;
    if(row > 2)return false;
    if(column < 0)return false;
    if(column > 2)return false;
    attack_card = this.gamezone.cards[(row*3)+column];
    part = POSITION.INVALID;
    switch(position){
        case POSITION.TOP:     if(row==0)return false; defend_card = this.gamezone.cards[( (row-1)  * 3 )+  ( column ) ]; part = POSITION.BOTTOM;break;
        case POSITION.BOTTOM:  if(row==2)return false; defend_card = this.gamezone.cards[( (row+1)  * 3 )+  ( column ) ]; part = POSITION.TOP;   break;
        case POSITION.LEFT:    if(column==0)return false; defend_card = this.gamezone.cards[( ( row )  * 3 )+  (column-1) ]; part = POSITION.RIGHT; break;
        case POSITION.RIGHT:   if(column==2)return false; defend_card = this.gamezone.cards[( ( row )  * 3 )+  (column+1) ]; part = POSITION.LEFT;  break;
    }
    return game.evaluate(attack_card, defend_card, part);
}

Game.prototype.draw = function(){
    setTimeout(function(){game.display("GAME DRAW!");}, 1500);
    setTimeout(function(){game = new Game(cards, elements, gamedata, gamedata.players); game.newGame();}, 5000);    
}

Game.prototype.lose = function(){
    setTimeout(function(){game.display("YOU LOSE!");}, 1500);
    setTimeout(function(){game = new Game(cards, elements, gamedata, gamedata.players); game.newGame();}, 5000);    
}

Game.prototype.win = function(){
    setTimeout(function(){game.display("YOU WIN!");}, 1500);
    setTimeout(function(){game = new Game(cards, elements, gamedata, gamedata.players); game.newGame();}, 5000);    
}

/*@SERVER */
Game.prototype.attack = function(column, row, ignoreturn=false){
    console.log(` >>> ATTACK(${column},${row})-------------------------------------------`);
    if(this.tier(column, row, POSITION.LEFT))   {this.flip(column-1,row); if(ignoreturn)game.display("CHAIN!");}
    if(this.tier(column, row, POSITION.RIGHT))  {this.flip(column+1,row); if(ignoreturn)game.display("CHAIN!");}
    if(this.tier(column, row, POSITION.TOP))    {this.flip(column,row-1); if(ignoreturn)game.display("CHAIN!");}
    if(this.tier(column, row, POSITION.BOTTOM)) {this.flip(column,row+1); if(ignoreturn)game.display("CHAIN!");}
    if(!ignoreturn){
        this.turns--;
        this.turn ^= 1;
        // game.display(`PLAYER ${this.turn+1}`, 250);
        $('score.top').toggleClass('turn');
        $('score.bottom').toggleClass('turn');
        $('board.top').toggleClass('turn');
        $('board.bottom').toggleClass('turn');
    }
     if(this.turns == 0){
        if(this.score == 5)this.draw();
        else if(this.score > 5)this.lose();
        else this.win();
    }
}

Game.prototype.display = function(text, timeout=1700){
    $('display').html(text);
    setTimeout(function(){$('display').html("");},timeout);
}

Game.prototype.assignCard = function(card_index, player_index){
    classes = [ 'A', 'B' ];
    card = this.gamezone.cards[card_index];
    /* Remove ZINDEX TOP Style from div parent */
    $(`gamezone div:nth-child(${card_index+1})`).removeClass('flipAB flipBA');
    /* Select CARD node */
    node = $(`gamezone div:nth-child(${card_index+1})  cardwrapper card`);
    card.playerclass = classes[player_index];    
    node.removeClass('flipAB flipBA');    
    node.addClass(classes[player_index]);
    this.flipping = false;
}

Game.prototype.coinToss = function(){
    // todo
}

Game.prototype.deal = function(){
    log("Dealing.");
    count = 0;
    out = [];
    while(count < 10){
        
        dado = Math.max(parseInt(Math.random()*this.cards.length)-1, 0);
        if(out.includes(dado))continue;
        out.push(dado);
        
        if(count%2==0)
            this.board[0].addCard(cards[dado]);
        else 
            this.board[1].addCard(cards[dado]);        
        count++;
    }
    log("Finished Dealing.");
}

Game.prototype.newGame = function(){
    $('board').each(function(){$(this).removeClass("turn")});
    $('board').each(function(){$(this).html("")});
    $('gamezone div').each(function(){$(this).removeClass("turn")});
    $('gamezone div').each(function(){$(this).html("")});
    $('score').each(function(){$(this).html(5)});
    this.turn = 0;
    $(`board.${game.board[this.turn].position}`).addClass('turn');
    this.deal();
    this.coinToss();
    // if score 0 = score player 1 = 9, player2 = 0
    // if score 9 = score player 1 = 0, player2 = 9    
    this.score = 5;
    game.display("GAME START<br/><br/>PLAYER ONE");
}

function log(txt){
    if(game.debug>=1){
        console.log('[LOG] >> '+txt);
    }
}
