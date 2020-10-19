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
                        />${childForm(attribute, `${i}`, callback)}
                         <!--<input id="${attribute.id}_${i}_n" 
                                type="text" 
                                value="${attribute.value[i].node   }"
                                class="half" 
                                onchange="${callback}"
                        />-->
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
