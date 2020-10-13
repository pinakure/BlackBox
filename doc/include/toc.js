var global = {
    name        : 'index',
    toc         : {},//Will hold every subterm
   
    /*'runcycle' : { 
        usage : '', 
        brief : 'Reinitialize video processing unit (vpu)',
    },*/

    index       : function(){
        // generate TOC 
        this.inspect(blackbox);
        this.inspect(con);
        this.inspect(typewriter);
        this.inspect(vpu);
        // render TOC html
        return this.render(this.toc);        
    },

 
    render      : function(toc){
        var ret = document.getElementById('info').innerHTML;
        for(mi in toc){
            var module = toc[mi];
            var html = '';
            // Render toc into a navigable index screen
            var keys = Object.keys(module);
            for(ti in keys){
                var key = keys[ti];
                if(key=='name') continue;
                var value = module[key];
                html += this.term(key, value);// append Term explanation
            }
            ret+= `<hr/><h2>${module.name}</h2><ul>${html}</ul>`;
        }
        return ret; 
    },

    term        : function(key, value){
        return `
            <li onclick="global.click('${key}')" class="toc_entry" id="${key}">
                <header title="${value.brief}"><b>${key}</b><span class="dark"><b>(</b>&nbsp;${value.usage}&nbsp;<b>)</b><br></span></header>
                <footer>${value.brief}<br/><span class="return">${value.return ?`Returns ${value.return}`:''}</span></footer>
                <section style="padding-right: 16px;padding-left: 0px;"><pre style="overflow-x: auto;padding: 8px 8px 8px 8px; background: #f0f0f0f0; border-radius: 8px 8px 8px 8px;">${value.example}</pre></section>
                <p></p>
            </li>
        `;
    },

    click: function (id) {
        var d = document.getElementById(id);
        var all = document.getElementsByClassName('toc_entry active');
        for (ai in all) {
            var t = all[ai];
            t.className = 'toc_entry';
        }
        d.className = 'toc_entry active'
    },

    inspect     : function(object){
        // Inspect self object and add unskipped attributes to TOC object (easyier to browse)
        var keys = Object.keys(object);
        this.toc[object.name] = [];
        for(ki in keys){
            var key = keys[ki];
            switch(key){
                case 'toc':
                case 'term':
                case 'index':
                case 'click':
                case 'render':
                case 'renderTerm':
                case 'inspect':
                case '':
                    // Skip these, as they would produce stack overflow
                    continue;
                default:
                this.toc[object.name][key] = object[key];
                    continue;
            }
        }
    },
};