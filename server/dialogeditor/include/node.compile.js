Node.prototype.compileText = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
        ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`;
    }
    /* Wait for user input! */
    ret +=  `${getIndent(indent)}while not typewriter.ready():\n`+
                `${getIndent(indent+1)}vpu.update()\n`;
    return ret;
}

Node.prototype.compileBranch = function(indent=0){
    if(!Editor.nodes[this.attributes.true.value])return '';
    else if(!Editor.nodes[this.attributes.false.value])return '';
    else if(!this.attributes.condition.value)return '';
    return  `${getIndent(indent)}if ${this.attributes.condition.value}:\n`+
                `${Editor.nodes[this.attributes.true.value].compile(indent+1)}`+
            `${getIndent(indent)}else:\n`+
                `${Editor.nodes[this.attributes.false.value].compile(indent+1)}`;
    return ret;
}

Node.prototype.compileChoice = function(indent=0){
    var ret = '';
    var aft = '';
    var first_choice=true;
    for(ai in this.attributes.choices.value){
        var t = this.attributes.choices.value[ai];
        var value = t.choice;
        var node  = (t.node && t.node!='null') ? Editor.nodes[t.node] : null;
        if(node){
            ret += `${getIndent(indent)}typewriter.addchoice('${value}', '${node.id}')\n`;
            aft += `${getIndent(indent)}${first_choice?'':'el'}if choice == '${node.id}':\n`+
                        `${ node.compile(indent+1)}`;
            first_choice=false;
        }
    }
    ret+= getIndent(indent)+`choice = ''\n`;
    ret+= getIndent(indent)+`while choice=='':\n`;
    ret+= getIndent(indent+1)+`vpu.update()\n`;
    ret+= getIndent(indent+1)+`choice = typewriter.getchoice('${this.attributes.question.value}')\n`;
            
    return ret+aft;
}

Node.prototype.compileWait = function(indent=0){
    var ret = getIndent(indent)+'#wait\n';
    /*for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }*/
    return ret;
}


Node.prototype.compilePicture = function(indent=0){
    var ret = getIndent(indent)+'#picture\n';
    /*for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }*/
    return ret;
}

Node.prototype.compileRumble = function(indent=0){
    var ret = getIndent(indent)+'#rumble\n';
    /*for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }*/
    return ret;
}

Node.prototype.compileFont = function(indent=0){
    var ret = getIndent(indent)+'#font\n';
    /*for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }*/
    return ret;
}

Node.prototype.compileScript = function(indent=0){
    var ret = '';
    var parts = this.attributes.code.value.split('\n');
    for(pi in parts){
        ret += `${getIndent(indent)}${parts[pi]}\n`;
    }
    return ret;
}

Node.prototype.compileStyle = function(indent=0){
    var ret = getIndent(indent)+'#style\n';
    /*for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }*/
    return ret;
}


Node.prototype.compileChildren = function(indent=0){
    var code = '';
    for(ci in this.children){
        var child = this.children[ci];
        code += child.compile(indent);
    }
    return code;
}

Node.prototype.compile = function(indent=0){
    var code = '';
    switch(this.type){
        /* BRANCHING NODES - DO NOT compile children! */

        case Types.BRANCH: return this.compileBranch(indent);
        case Types.CHOICE:  return this.compileChoice(indent);  
        
        /* NON-BRANCHING NODES - append children code or script will end there */

        case Types.FONT:    code += this.compileFont(indent);    break;
        case Types.PICTURE: code += this.compilePicture(indent); break;
        case Types.RUMBLE:  code += this.compileRumble(indent);  break;
        case Types.SCRIPT:  code += this.compileScript(indent);  break;
        case Types.TEXT:    code += this.compileText(indent);    break; 
        case Types.STYLE:   code += this.compileStyle(indent);   break;
        case Types.WAIT:    code += this.compileWait(indent);    break;
    }
    code += this.compileChildren(indent);
    return code;
}
