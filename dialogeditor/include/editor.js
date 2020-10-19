var Editor = {
    
    nodes : {},
    selected_node : null,

    initialize : function(){
        setInterval(function(){Editor.update();}, 1000/60);
        document.getElementsByTagName('content')[0].addEventListener("scroll", Editor.scroll);
    },

    scroll : function(evt) {
        //document.getElementsByTagName('overlay')[0].scrollHeight = document.getElementsByTagName('content')[0].scrollHeight;
        document.getElementsByTagName('overlay')[0].scrollTop = document.getElementsByTagName('content')[0].scrollTop;
    },

    update : function(){
        for(ni in this.nodes){
            this.nodes[ni].update();            
        }
        $('output').html(this.nodes[0].compile(0));

    },
    
    addNode : function(type){
        $('#menu').hide();
        var node = new Node(type);
        this.nodes[node.id] = node;
        return node.id;
    },

    selectNode : function(node){        
        $('#menu').hide();
        if(this.selected_node) this.selected_node.selected = false;
        this.selected_node = node;
        this.setName(`${node.caption}<super># ${node.id}</super>`, node.icon);
        $('navcontent').html(node.controlForm()+'<hr style="filter: opacity(35%)"/>'+node.form())
        if(node.parent){
            $(`#parent_${node.id}`).val(node.parent.id);
        }
        node.selected = true;
    },

    refresh : function(){
        Editor.selectNode(Editor.nodes[Editor.selected_node.id]);
    },

    setName : function (name, icon){
        $('navname i')[0].className = `fa fa-${icon}`;
        $('navname span').html(name);
    },

    addChild : function(parent, type){
        var node = new Node(type);
        var id = parent.addChild(node);
        Editor.nodes[id] = node;
        return id;
    },

    addSibling : function(sibling, type){
        var node = new Node(type);
        var id = sibling.addSibling(node);
        Editor.nodes[id] = node;
        return id;
    },

};

$(document).ready(function(){               
    Editor.initialize();
    var id = Editor.addNode(Types.TEXT);
    /*var id = Editor.addChild(Editor.nodes[id], Types.WAIT);
    var id = Editor.addChild(Editor.nodes[id], Types.FONT);
    var id = Editor.addSibling(Editor.nodes[id], Types.RUMBLE);*/
    
    $('content, content *, nav, nav *').attr('onclick', `$('#menu').hide()`);
});