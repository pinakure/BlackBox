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
        this.setName('', '');
        $('navcontent').html('');
        if(this.selected_node) this.selected_node.selected = false;
        this.selected_node = node;
        if(!node)return;
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

var text; 

$(document).ready(function(){               
    Editor.initialize();
    text = Editor.nodes[Editor.addNode(Types.TEXT)];    
    text.attributes.lines.value[0] = 'Hello world!';
    $('content, content *, nav, nav *').attr('onclick', `$('#menu').hide()`);
}).ready(function(){
    var choice1 = Editor.nodes[Editor.addNode(Types.CHOICE)];
    Editor.selectNode(choice1);
    addChoice();
    choice1.attributes.choices.value[0].choice = 'A';
    choice1.attributes.choices.value[1].choice = 'B';
    choice1.setParent(text);
    var text1 = Editor.nodes[Editor.addNode(Types.TEXT)];
    var text2 = Editor.nodes[Editor.addNode(Types.TEXT)];
    text1.setParent(choice1);
    text2.setParent(choice1);
    choice1.attributes.choices.value[0].node = text1.id;
    choice1.attributes.choices.value[1].node = text2.id;
    text1.attributes.lines.value[0] = 'CHOSEN A';
    text2.attributes.lines.value[0] = 'CHOSEN B';
    
    var choice2 = Editor.nodes[Editor.addNode(Types.CHOICE)];
    Editor.selectNode(choice2);
    addChoice();
    choice2.attributes.choices.value[0].choice = 'C';
    choice2.attributes.choices.value[1].choice = 'D';
    choice2.setParent(text2);
    var text3 = Editor.nodes[Editor.addNode(Types.TEXT)];
    var text4 = Editor.nodes[Editor.addNode(Types.TEXT)];
    text3.setParent(choice2);
    text4.setParent(choice2);
    choice2.attributes.choices.value[0].node = text3.id;
    choice2.attributes.choices.value[1].node = text4.id;
    text3.attributes.lines.value[0] = 'CHOSEN C';
    text4.attributes.lines.value[0] = 'CHOSEN D';
    
    Editor.selectNode(null);
});