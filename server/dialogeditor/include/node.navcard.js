function NavCard(attribute){
    var ret = '';   
    var style = ''; // override <navcard> style! 
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
                        />${childForm(attribute, `${i}`, callback)}
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

        case AttributeTypes.BIGSTRING:
            ret +=  `<label>${attribute.name}</label>`;
            style = 'height: calc(100% - 50px);'
            ret +=  `<textarea
                        style="resize: none;width: calc(100% - 6px);height: calc(100% - 22px);"
                        id="${attribute.id}" 
                        type="text" 
                        onchange="${callback}" 
                    />${attribute.value}</textarea>`;
            break;

        case AttributeTypes.STRING:
            ret +=  `<label>${attribute.name}</label>`;
            ret +=  `<input 
                        id="${attribute.id}" 
                        type="text" 
                        value="${attribute.value}"
                        onchange="${callback}" 
                        maxlength="256" 
                    />`;
            break;
    
        case AttributeTypes.BRANCH:
            ret +=  `<label>${attribute.name}</label>`;
            ret +=  `${childForm(attribute, ``, callback)}`;
            break;
        
    }
    return `<navcard${style?` style="${style}"`:''}>${ret}</navcard>`;
}   
