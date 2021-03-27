from random                 import random
from data.scripts.common    import *
from data.scripts.tristemap import Map
from data.scripts.shapes    import ShapeTypes
from data.scripts.colors    import Colors
"""
RULES:

- Select first holes > 1 cell (penalty single holes)
- Select first holes accesible lower (penalty those possible but higher)
- Select position if all lower squares fit ground or another square

"""


class Triste:
    CONTINUE    = 0x1
    NEWSHAPE    = 0x2
    GAMEOVER    = 0x3
    colors      = Colors
    gameover    = False
    width       = 16
    height      = 32
    map         = []
    piececount  = 0
    currentpiece= None
    game        = None
    
    @staticmethod
    def initialize(game):
        # Initialize map memory 
        Triste.game = game
        Triste.map = Map(Triste, Triste.width, Triste.height)
        Triste.map.create()
        # Create first piece 
        Triste.newshape()
        
        # Call first logic cycle 
        # not needed, its replaced by update calls from engine.
        # window.setTimeout(Triste.logic, 60);
        
    @staticmethod
    def newgame():
        Triste.initialize(Triste.game)
    
    @staticmethod
    def newshape():
        Triste.currentpiece = None
        while Triste.currentpiece is None:
            Triste.currentpiece = Triste.add_piece(int(random()*7))
        column  = Triste.find_optimal_column()
        rotation= Triste.find_optimal_angle()
    
    @staticmethod
    def add_piece(shape_type):
        shape    = ShapeTypes.get(shape_type)
        column   = 0; #Triste.find_optimal_column()
        row      = -shape.height
        rotation = 0; # Triste.find_optimal_angle()
        return Triste.map.add_shape(shape, column, row, rotation)        

    @staticmethod
    def find_optimal_angle():
        """ Evaluate match score for each angle """
        shape = Triste.currentpiece
        angle = shape.angle
        angles = {}
        for __i in range(0, len(shape.data)-1):
            angles[__i] = [ shape.get_columns() ]; # angles[__i] = [ shape.getRows() ]; 
            # move forward in angle list 
            shape.rotation += 1;  
            # clamp to angle set size 
            shape.rotation %= len(shape.data); 
        # scores = get_scores(angles);
        return 0
        
    @staticmethod
    def find_optimal_column():
        return 1
        """
        var optimal_value = Triste.height;
        var optimal_index = -1;
        var columns = [];
        for(ci=0;ci<Triste.width; ci++){
            columns[ci] = Triste.getColumnWeight(ci);
        }
        for(ci=0;ci<Triste.width; ci++){
            if(columns[ci] < optimal_value){
                optimal_index = ci;
                optimal_value = columns[ci];
            };
        }
        return optimal_index;
        """
    
    @staticmethod
    def draw():
        pass

    @staticmethod
    def update(delta):
        p = Triste.currentpiece
        status = p.logic()
        if status == Triste.GAMEOVER:
            print("Game Over!")
            Triste.gameover = True
            Triste.newgame()
        elif status == Triste.NEWSHAPE:
            Triste.newshape()
        
        #not needed 
        #if not Triste.gameover:
        #    window.setTimeout(Triste.logic, 60)
'''

    
    
    @staticmethod
    def get_column_weight(column):
        weights = Triste.map.get_weights()
        return weights[column]
        
    @staticmethod
    def removeLines(mapdata, lineset):
        """ Returns mapdata without new lines, inserting new lines over the top """        
        new_mapdata = []
        map_height = mapdata.length
        map_width  = mapdata[0].length
        for map_row_index in lineset:
            mapdata[map_row_index] = []
        for map_row_index in mapdata:
            if len(mapdata[map_row_index])>0: 
                new_mapdata[new_mapdata.length] = mapdata[map_row_index]
        new_row = []
        for col_index in range(0, map_width):
            new_row[new_row.length] = 0
        for line_index in range(0, map_height-new_mapdata.length):
            new_mapdata.splice(0,0,new_row)
        return new_mapdata
    
    @staticmethod
    def find_lines(mapdata):
        """ Return list of line indices to be removed if any """
        lines = []
        for map_row_index in mapdata:
            line = True
            for map_col_index in mapdata:
                if not mapdata[map_row_index][map_col_index]: 
                    line = False
            if line:
                lines[lines.length] = map_row_index
        return lines
    
    @staticmethod
    def would_cause_line(shape, x, y):
        """ Return true if given shape data would produce a new line , scoring """
        virtual_map = Triste.map
        for row_index in shape:
            row = shape[row_index]
            for col_index in row:
                virtual_map[y+row_index][x+col_index] &= shape[row_index][col_index]
        lines = Triste.find_lines(virtual_map)
        return lines.length > 0

def Intent(self, shapedata):
    self.penalty= 0
    self.shape  = shapedata
'''