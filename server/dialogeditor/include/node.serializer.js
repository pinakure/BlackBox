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
                attr.value[i].node   = $(`#${attr.id}_${i}_n option:selected`).val();                
            }
            break;
        
        case AttributeTypes.BRANCH:
            attr.value = $(`#${attr.id}__n option:selected`).val();           
            debugger    
            break;
        
        case AttributeTypes.VECTOR: // same as choice, but with numbers
            attr.value.x = $(`#${attr.id}_x`).val();
            attr.value.y = $(`#${attr.id}_y`).val();
            break;

        case AttributeTypes.STRING: // Single string, for input=text 
            attr.value = $(`#${attr.id}`).val();
            break;
        
        case AttributeTypes.BIGSTRING: // For textareas...
            attr.value = $(`#${attr.id}`).val();
            break;
    }
}
