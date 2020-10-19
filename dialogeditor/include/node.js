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
    SCRIPT : 8, // 
    BRANCH : 9, // Conditional Block
};

function getType(type){
    for(key in Types){
        if(Types[key]==type)return key;
    }
    return 'Unknown';
}

var AttributeTypes = {
    NONE    : 0, // 
    TEXT    : 1, // 
    CHOICE  : 2, // 
    FILE    : 3, // 
    NUMBER  : 4, // 
    VECTOR  : 5, //     
    STRING  : 6,
    BIGSTRING:7,
    BRANCH : 8,
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

function childForm(attribute, id, callback){
    var children = attribute.parent.children;
    var ret = '';
    for(ci in children){
        var child = children[ci];
        try{
            ret += `<option ${parseInt(attribute.value[id].node) ==child.id?'selected="selected"':''} id="${child.id}" value="${child.id}">${getType(child.type)} #${child.id}</option>`;
        } catch(e){
            ret += `<option ${parseInt(attribute.value) ==child.id?'selected="selected"':''} id="${child.id}" value="${child.id}">${getType(child.type)} #${child.id}</option>`;
        }
    }
    ret = `
        <select style="width: 49%" id="${attribute.id}_${id}_n" onchange="${callback}">
            <option id="null" value="null">NONE</option>
            ${ret}
        </select>
    `;  
    return ret;
}

function Node(type, args={}){
    this.x = 0;
    var i = 0;
    for(ni in Editor.nodes){
        var n = Editor.nodes[ni];
        if(!n.parent)
        i++;
    }
    this.y = i;
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

Node.prototype.addChild = function(node){
    node.setParent(this);
    return node.id;
}

Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

Node.prototype.setParent = function(node){
    if(this.parent)
        this.parent.removeChild(this);
    
    if(node==null){
        this.parent = null;
        this.x = 0;
        this.y=Object.size(Editor.nodes);
        i = 1;
        for(ni in Editor.nodes){
            var n = Editor.nodes[ni];
            if(!n.parent)
                n.y = i-1;
            else 
                n.y = n.parent.y + 2;
            n.update();
            i++;
        }
        this.update();
        return;
    }
    
    this.parent = node;  
    this.y = this.parent.y+1;
    this.parent.children[this.parent.children.length] = this;
    this.x = this.parent.x+this.parent.children.length;
    var i = 1;
    for(ni in this.parent.children){
        var n = this.parent.children[ni];
        n.x = this.parent.x > 0 ? this.parent.x + i - 1 : this.parent.x + i;
        n.y = this.parent.y + 2;
        n.update();
        i++;
    }
}

Node.prototype.removeSibling = function(node){
    var new_nodes = [];
    for(ni in this.siblings){
        var n = this.siblings[ni];
        if(n.id == node.id) continue; // Skip target sibling on new list
        new_nodes[n.id] = n;
    }
    this.siblings = new_nodes;
    Editor.update();
}

Node.prototype.removeChild = function(node){
    var new_nodes = [];
    var i = 1;
    for(ni in this.children){
        var n = this.children[ni];
        if(n.id == node.id)continue; // Skip target child on new list
        n.removeSibling(node);
        n.x = this.x+i;
        new_nodes[n.id] = n;
        i++;
    }
    this.children = new_nodes;
    Editor.update();
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

function assignParent(node_id){
    /* TO BE CALLED BY SELECT CONTROL BELOW */
    if(node_id==0)return;
    var parent_id = $(`#parent_${node_id} option:selected`).val();
    if(parent_id=='null')Editor.nodes[node_id].setParent(null);
    else Editor.nodes[node_id].setParent(Editor.nodes[parent_id]);
}

function highlightNode(id){
    $('node').each(function(){
        $(this).css('filter', 'brightness(1.0)');
    }); 
    var obj = $(`#parent_${id}`);
     var selection = $(`#parent_${id} option:hover`).val();
    console.log(selection);
    if(selection != 'null')
        $(`#node_${selection}`).css('filter', 'brightness(1.5)');
        
}

Node.prototype.parentControl = function(){
    var ret = '';
    ret += '<option style="background: #333;color: #f80; font-weight: 650;" id="null" value="null">NO PARENT</option>';
    for(ni in Editor.nodes){
        var node = Editor.nodes[ni];
        if(this.id == node.id)continue;
        var ischild = false;
        for(ci in this.children){
            var child = this.children[ci];
            if(child.id == node.id)ischild = true;
        }
        if(ischild)continue;
        if(this.id == node.id)continue;
        ret += `<option onfocus="highlightNode(${this.id})" style="background: #333;color: #f80; font-weight: 650;" id="${node.id}" value="${node.id}">${node.id} - ${getType(node.type)}</option>`; 
    }
    ret = `<select onmousemove="highlightNode(${this.id})"  onchange="assignParent(${this.id});" style="box-shadow: 0px 2px 3px #0008; background: #333; color: #f80; margin-top: 8px; height: 24px;width: 100%; border-radius: 8px 8px 8px 8px;" class="" id="parent_${this.id}">
        ${ret}
    </select>`;
    return ret;
}

Node.prototype.controlForm = function(){
    if(this.id==0)return '';
    var ret = this.parentControl();
    return ret;
}

function getIndent(indent){
    var indret = '';
    for(var ii=0; ii<indent; ii++){
        indret+='\t';
    }
    return indret;
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
        case Types.BRANCH:  this.icon = 'check-square-o';   this.caption = "Branch"; break;
        case Types.SCRIPT:  this.icon = 'file-o';           this.caption = "Script"; break;
    }    
    this.specialize(args);
    this.initialized = true;
    node_count++;
};

Node.prototype.render = function(args){
    // inject initial html
    var width  = (this.parent ? this.x - this.parent.x :  this.x ) * NODE_AREA;
    var height = (this.parent ? this.y - this.parent.y :  this.y ) * NODE_AREA;
    $('overlay').append(`<svg height="${height}" width="${width}" version="1.1" xmlns="http://www.w3.org/2000/svg" id="joint_${this.id}"><line x1="0" x2="0" y1="0" y2="0" /></svg>`);
    $('content').append(`<node root="${this.parent==null}" onclick="Editor.selectNode(Editor.nodes[${this.id}]);" id="node_${this.id}" title="${Object.keys(Types)[this.type]}(${this.id})"><i class="fa fa-${this.icon}"></i><div style="position: absolute; top: 0px; left: 0px; width: 100%; height: 100%; text-align: center;line-height: 22px;font-size: 12px;color:#ff8a;">${this.id}</div></node>`);
    // return jquery object
    return $(`#node_${this.id}`);
}

Node.prototype.getX = function(){
    if(this.id==0)return this.x;
    return this.parent ? this.x : this.x + 8;
}
Node.prototype.getY = function(){
    if(this.id==0)return this.y;
    return this.parent ? this.y : this.y - 1;
}

Node.prototype.update = function(args){
    var n = $(`#node_${this.id}`);
    var x = this.getX();
    var y = this.getY();
    if(n.length==0) n = this.render();
    n.css('left',   `${NODE_PADDING + (x*NODE_AREA)}px`);
    n.css('top',    `${NODE_PADDING + (y*NODE_AREA)}px`);
    n.attr('root',  `${this.parent==null}`);
    if(this.selected){
        n.attr('selected',true);
    } else {
        n.attr('selected',false);
    }
    var selected = this.selected;//$(`#node_${this.id}:hover`).length > 0;
    var j = $(`#joint_${this.id}`);
    var dx=0,dy=0;
    j.css('display', `${ this.parent ? 'inline-block' : 'none'}`);    
    j.css('width'  , `${(this.parent ? (x - this.parent.x)+2 :  x ) * NODE_AREA}px`);
    j.css('height' , `${(this.parent ? y - this.parent.y :  y ) * NODE_AREA}px`);
        
    if(this.parent){
        j.css('left', `${NODE_PADDING+((this.parent.x*NODE_AREA)+(NODE_AREA/2))}px`);
        j.css('top' , `${NODE_PADDING+((this.parent.y*NODE_AREA)+(NODE_AREA/2))}px`);
        tx = 0;ty = 0;
        dx = (this.parent ? (x - this.parent.x) :  x );dy = 1;
    } else {
        j.css('left', `${NODE_PADDING+((x*NODE_AREA)+(NODE_AREA/2))}px`);
        j.css('top' , `${NODE_PADDING+((y*NODE_AREA)+(NODE_AREA/2))}px`);
        tx = 0;ty = 0;    
        dx = 0;dy = 0;    
    }
    var j = $(`#joint_${this.id} line`);
    j.attr('x1', (tx*NODE_AREA)-1);
    j.attr('y1', ty*NODE_AREA);
    j.attr('x2', (dx*NODE_AREA)+1);
    j.attr('y2', (dy+1)*NODE_AREA);
    j.css('stroke', selected ? 'red' : '#000a');
    j.css('stroke-width', selected ? '2' : '1');

    for(si in this.siblings){
        this.siblings[si].update();        
    }
    for(ci in this.children){
        this.children[ci].update();        
    }
    
};
