// WEBDINGS:
// SAVE: Í
// OPEN: Ì
// CLEAR: r
// COMPILE: `
const NODE_PADDING   = 8;
const NODE_AREA      = 64;
var node_count = 0;
var COLUMN = 0;

var Types = {
    NONE   : 0, // No type, no node!
    PICTURE: 1, // Used to set, move, scale and remove pictures from typewriter
    TEXT   : 2, // Simple text nodes
    CHOICE : 3, // Branching!
    WAIT   : 4, // Wait node, used to force engine to wait from dialog
    RUMBLE : 5, // Make dialog rumble
    FONT   : 6, // Font node
    STYLE  : 7, // Change box style
};

var AttributeTypes = {
    NONE    : 0, // 
    TEXT    : 1, // 
    CHOICE  : 2, // 
    FILE    : 3, // 
    NUMBER  : 4, // 
    VECTOR  : 5, //     
}; 

function addLine(){
    var attrs = Editor.nodes[Editor.selected_node.id].attributes;
    attrs.lines.value[ attrs.lines.value.length ] = '';
    Editor.refresh();
}

function addChoice(){
    var attrs = Editor.nodes[Editor.selected_node.id].attributes;
    attrs.choices.value[ attrs.choices.value.length ] = { choice : '', node : "" };
    Editor.refresh();
}

function Button(icon, callback){
    return `<button onclick="${callback}" type="button" style="width: 100%;"><i class="fa fa-${icon}"></i></button>`;
}

function Spinner(id, caption, value, callback){
    return `<label for="${id}" >${caption}</label><input id="${id}" type="number" onchange="${callback}" value="${value}"/>`;
}

function NodeAttribute(parent, name, value, type=AttributeTypes.NONE){
    this.id = `attr_${parent.id}_${name}`;
    this.parent = parent;
    this.name = name;
    this.value = value;
    this.type = type;
}

function serializeAttribute(node_id, attribute_name){
    var node = Editor.nodes[node_id];
    var attr = node.attributes[attribute_name];
    switch(attr.type){
        case AttributeTypes.TEXT:
            for(i in attr.value){ // 'Iterate each line'
                attr.value[i] = $(`#${attr.id}_${i}`).val();
            }
            break;
        
        case AttributeTypes.FILE:
            // TODO: READ File data and filename+
            console.log("WARNING: Function placeholder serializeAttribute(FILE)");
            attr.value = $(`#${attr.id}`).val();
            break;
            
        case AttributeTypes.NUMBER:
            attr.value = $(`#${attr.id}`).val();
            break;
            
        case AttributeTypes.CHOICE:
            for(i in attr.value){ // 'Iterate each choice (2 items per choice)'
                attr.value[i].choice = $(`#${attr.id}_${i}_c`).val();
                attr.value[i].node   = $(`#${attr.id}_${i}_n`).val();
            }
            break;
        
        case AttributeTypes.VECTOR: // same as choice, but with numbers
            attr.value.x = $(`#${attr.id}_x`).val();
            attr.value.y = $(`#${attr.id}_y`).val();
            break;
    }
}

function NavCard(attribute){
    var ret = '';    
    var callback = `serializeAttribute(${attribute.parent.id}, '${attribute.name}')`;
    switch(attribute.type){
        case AttributeTypes.TEXT:
            ret +=  `<label>${attribute.name}</label>`;
            for(i in attribute.value){
                ret +=  `<input 
                            id="${attribute.id}_${i}" 
                            type="text" 
                            value="${attribute.value[i]}"
                            onchange="${callback}" 
                            maxlength="64" 
                        />`;
            }
            break;

        case AttributeTypes.CHOICE:
            ret += `<label style="width:100%">${attribute.name}</label>`;
            for(i in attribute.value){
                ret +=  `<input id="${attribute.id}_${i}_c" 
                                type="text" 
                                value="${attribute.value[i].choice }"
                                class="half" 
                                onchange="${callback}"
                        />
                         <input id="${attribute.id}_${i}_n" 
                                type="text" 
                                value="${attribute.value[i].node   }"
                                class="half" 
                                onchange="${callback}"
                        />
                        `;    
            }
            break;

        case AttributeTypes.NUMBER:
            ret +=  `${Spinner(attribute.id, attribute.name, attribute.value, callback)}`;
            break;  
            
        case AttributeTypes.FILE:
            ret +=  `<label>${attribute.name}</label>
                    <input 
                        id="${attribute.id}" 
                        type="text" 
                        value="${attribute.value}"
                        onchange="${callback}" 
                    />`;
            break;
        
        case AttributeTypes.VECTOR:
            ret +=  `<label>${attribute.name}</label>
                    <input 
                        id="${attribute.id}_x" 
                        type="number"
                        value="${attribute.value.x }"
                        class="half" 
                        onchange="${callback}" 
                        />
                    <input 
                        id="${attribute.id}_y" 
                        type="number" 
                        value="${attribute.value.y }"
                        class="half" 
                        onchange="${callback}"                             
                    />`;
            break;
        
    }
    return `<navcard>${ret}</navcard>`;
}   

function Node(type, args={}){
    this.x = 0;
    this.y = node_count;
    this.initialized = false;
    this.selected = false;
    this.icon = '';
    this.caption = '';
    this.next = null;
    this.id = node_count;
    this.type = type;
    this.parent = null;
    this.form = function(){ return ""; };
    this.children = [];
    this.siblings = [];
    this.joint = null;
    this.attributes = {};// objects must provide get and set from string methods!
    this.initialize(args);
}

Node.prototype.setParent = function(node){
    COLUMN++;
    this.x = node.x + 1;
    this.y = node.y + 1;
    this.parent = node;    
}

Node.prototype.addChild = function(node){
    node.setParent(this);
    this.children[this.children.length] = node;
    return node.id;
}

Node.prototype.addSibling = function(node){
    this.siblings[this.siblings.length] = node;
    return node.id;
}

Node.prototype.destroy =function(){
    if(this.initialized){
        // Delete next!
        node_count--;
        this.initialized = false;
    }
}

Node.prototype.specialize = function(args){
    // Populate attributes depending on node type
    switch(this.type){

        case Types.TEXT:    
            this.attributes.lines = new NodeAttribute(this, 'lines', ['',], AttributeTypes.TEXT);
            this.form = function(){
                return  NavCard(this.attributes.lines) +  
                        Button('plus', `addLine()`);
            };
            break;

        case Types.CHOICE:  
            this.attributes.choices = new NodeAttribute(this, 'choices', [ { choice : '', node : '' } ], AttributeTypes.CHOICE);
            this.form = function(){
                return  NavCard(this.attributes.choices) +
                        Button('plus', `addChoice()`);
            };
            break;

        case Types.WAIT:    
            this.attributes.time = new NodeAttribute(this, 'time', 1, AttributeTypes.NUMBER);
            this.form = function(){
                return NavCard(this.attributes.time);
            };
            break;
        
        case Types.PICTURE: 
            /* Load picture and display on coordinates OR set to zero (remove picture) */
            this.attributes.filename = new NodeAttribute(this, 'filename', '', AttributeTypes.FILE);
            this.attributes.position = new NodeAttribute(this, 'position', { x:  0, y:  0}, AttributeTypes.VECTOR);
            this.attributes.geometry = new NodeAttribute(this, 'geometry', { x: 64, y: 64}, AttributeTypes.VECTOR);
            this.form = function(){
                return  NavCard(this.attributes.filename) +
                        NavCard(this.attributes.position) +
                        NavCard(this.attributes.geometry);
            };
            break;
        
        case Types.RUMBLE:  
            /* Causes the text box to rumble -TODO-parametrically-TODO */
            this.attributes.movement = new NodeAttribute(this, 'movement', { x:  60, y:  60}, AttributeTypes.VECTOR);
            this.attributes.time     = new NodeAttribute(this, 'time'    , 1                , AttributeTypes.NUMBER);
            this.form = function(){
                return NavCard(this.attributes.movement) +
                       NavCard(this.attributes.time);
            };
            break;

        case Types.FONT:    
            /* Change FONT used */
            this.attributes.filename = new NodeAttribute(this, 'filename', '', AttributeTypes.FILE);
            this.form = function(){
                return NavCard(this.attributes.filename);
            };
            break;
 
        case Types.STYLE:
            /* Causes the text box to rumble -TODO-parametrically-TODO */
            this.attributes.opacity  = new NodeAttribute(this, 'opacity' , 128  , AttributeTypes.NUMBER);
            this.form = function(){
                return NavCard(this.attributes.opacity);
            };
            /* Change COLOR of the textbox */
            break;

        default:
            debugger
            break;
    }    
    return '';    
}

Node.prototype.initialize = function(args){
    switch(this.type){
        case Types.NONE:    this.icon = '';                 this.caption = ''; return;//!!!
        case Types.PICTURE: this.icon = 'picture-o';        this.caption = "Picture"; break;
        case Types.TEXT:    this.icon = 'commenting-o';     this.caption = "Text"; break;
        case Types.CHOICE:  this.icon = 'question-circle';  this.caption = "Choice"; break;
        case Types.WAIT:    this.icon = 'clock-o';          this.caption = "Wait"; break;
        case Types.RUMBLE:  this.icon = 'rss';              this.caption = "Rumble"; break;
        case Types.FONT:    this.icon = 'font';             this.caption = "Font"; break;
        case Types.STYLE:   this.icon = 'cog';              this.caption = "Style"; break;
    }    
    this.specialize(args);
    this.initialized = true;
    node_count++;
};

Node.prototype.render = function(args){
    // inject initial html
    $('overlay').append(`<svg height="${NODE_AREA}" width="${NODE_AREA}" version="1.1" xmlns="http://www.w3.org/2000/svg" id="joint_${this.id}"><line x1="0" x2="0" y1="0" y2="0" /></svg>`);
    $('content').append(`<node root="${this.parent==null}" onclick="Editor.selectNode(Editor.nodes[${this.id}]);" id="node_${this.id}" title="${Object.keys(Types)[this.type]}(${this.id})"><i class="fa fa-${this.icon}"></i></node>`);
    // return jquery object
    return $(`#node_${this.id}`);
}

Node.prototype.update = function(args){
    var n = $(`#node_${this.id}`);
    if(n.length==0) n = this.render();
    n.css('left',   `${NODE_PADDING + (this.x*NODE_AREA)}px`);
    n.css('top',    `${NODE_PADDING + (this.y*NODE_AREA)}px`);
    n.attr('root',  `${this.parent==null}`);
    if(this.selected){
        n.attr('selected',true);
    } else {
        n.attr('selected',false);
    }
    var selected = this.selected;//$(`#node_${this.id}:hover`).length > 0;
    var j = $(`#joint_${this.id}`);
    var x = this.x, y = this.y;
    var dx=0,dy=0;
    
    if(this.parent){
        j.css('left', `${NODE_PADDING+((this.parent.x*NODE_AREA)+(NODE_AREA/2))}px`);
        j.css('top' , `${NODE_PADDING+((this.parent.y*NODE_AREA)+(NODE_AREA/2))}px`);
        x = 0;y = 0;
        dx = 1;dy = 1;
    } else {
        j.css('left', `${NODE_PADDING+((this.x*NODE_AREA)+(NODE_AREA/2))}px`);
        j.css('top' , `${NODE_PADDING+((this.y*NODE_AREA)+(NODE_AREA/2))}px`);
        x = 0;y = 0;    
        dx = 0;dy = 0;    
    }
    var j = $(`#joint_${this.id} line`);
    j.attr('x1', x*NODE_AREA);
    j.attr('y1', y*NODE_AREA);
    j.attr('x2', dx*NODE_AREA);
    j.attr('y2', dy*NODE_AREA);
    j.css('stroke', selected ? 'red' : '#000a');
    j.css('stroke-width', selected ? '2' : '1');

    for(si in this.siblings){
        this.siblings[si].update();        
    }
    for(ci in this.children){
        this.children[ci].update();        
    }
    
};
