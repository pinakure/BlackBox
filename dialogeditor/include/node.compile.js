Node.prototype.compileText = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
        ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}

Node.prototype.compileChoice = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}

Node.prototype.compileWait = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}


Node.prototype.compilePicture = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}

Node.prototype.compileRumble = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}

Node.prototype.compileFont = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}

Node.prototype.compileStyle = function(indent=0){
    var ret = '';
    for(ai in this.attributes.lines.value){
        var at = this.attributes.lines.value[ai];
    //    ret += getIndent(indent)+`typewriter.enqueue('${at}')\n`
    }
    return ret;
}


Node.prototype.compile = function(indent=0){
    var code = '';
    switch(this.type){
        case Types.TEXT:    code += this.compileText(indent); break;
        case Types.CHOICE:  code += this.compileChoice(indent); break;
        case Types.WAIT:    code += this.compileWait(indent); break;
        case Types.PICTURE: code += this.compilePicture(indent); break;
        case Types.RUMBLE:  code += this.compileRumble(indent); break;
        case Types.FONT:    code += this.compileFont(indent); break;
        case Types.STYLE:   code += this.compileStyle(indent); break;
    }
    for(ci in this.children){
        var child = this.children[ci];
        code += child.compile();
    }
    return code;
}
