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
            this.multiple_children = true; 
            this.max_children      = 256;            
            this.attributes.question= new NodeAttribute(this, 'question', '', AttributeTypes.FILE);
            this.attributes.choices = new NodeAttribute(this, 'choices', [ { choice : '', node : '' } ], AttributeTypes.CHOICE);
            this.form = function(){
                return  NavCard(this.attributes.question) +
                        NavCard(this.attributes.choices) +
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

        case Types.SCRIPT:
            this.attributes.code = new NodeAttribute(this, 'code', '', AttributeTypes.BIGSTRING);
            this.form = function(){
                return NavCard(this.attributes.code);
            };
            break;

        case Types.BRANCH:
            this.attributes.condition = new NodeAttribute(this, 'condition' , '', AttributeTypes.STRING);
            this.attributes.true      = new NodeAttribute(this, 'true' , '', AttributeTypes.BRANCH);
            this.attributes.false     = new NodeAttribute(this, 'false', '', AttributeTypes.BRANCH);
            this.multiple_children    = true;
            this.max_children         = 2;
            this.form = function(){
                return  NavCard(this.attributes.condition) +
                        NavCard(this.attributes.true) +
                        NavCard(this.attributes.false);
            };
            break;

        default:
            debugger
            break;
    }    
    return '';    
}
