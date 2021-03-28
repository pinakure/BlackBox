from tiledmap               import TiledMap
from random                 import random
from data.scripts.common    import *
from data.scripts.tristemap import Map
from data.scripts.piece     import Piece
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
    width       = 15
    height      = 26
        
    def __init__(self, game, flip=False):
        self.gameover     = False
        self.score        = 0
        self.flip         = flip
        self.x            = 0 if not flip else 160
        self.piececount   = 0
        self.currentpiece = None
        self.nextpiece    = None
        self.game         = game
        self.lines        = 0
        self.map          = Map(self, Triste.width, Triste.height)
        self.display      = TiledMap(self.game, Triste.width, Triste.height,2)
        self.nextmap      = TiledMap(self.game, 3, 3, 1)
        self.display.fill(0x7F, 0)
        self.display.fill(0x7F, 1)
        self.nextmap.fill(0x7F)
        self.display.load_tileset("blocks")
        self.nextmap.load_tileset("blocks")
        self.map.create()
        self.newgame()

    def newgame(self):
        self.gameover     = False
        self.currentpiece = None
        self.nextpiece = None
        self.piececount   = 0
        self.score        = 0
        self.lines        = 0
        self.map.clear()
        self.newnext()
        self.newshape()
        self.display.fill(0x7F, 0)
        self.display.fill(0x7F, 1)
        self.nextmap.fill(0x7F)
    
    def newnext(self):
        self.nextpiece = Piece(self, ShapeTypes.get(int(random()*7)), 0, 0, int(random()*4))
        
    def newshape(self):
        self.currentpiece = None
        while self.currentpiece is None:
            self.currentpiece = self.add_piece(self.nextpiece.shape.index)            
        self.newnext()
        
    def add_piece(self, shape_type):
        shape    = ShapeTypes.get(shape_type)
        column   = self.find_optimal_column()
        row      = -shape.height
        rotation = 0 # self.find_optimal_angle()
        return self.map.add_shape(shape, column, row, rotation)

    def find_optimal_angle(self):
        """ Evaluate match score for each angle """
        shape = self.currentpiece
        angle = shape.angle
        angles = {}
        for __i in range(0, len(shape.data)):
            angles[__i] = [ shape.get_columns() ]; # angles[__i] = [ shape.getRows() ]; 
            # move forward in angle list 
            shape.rotation += 1;  
            # clamp to angle set size 
            shape.rotation %= len(shape.data); 
        # scores = get_scores(angles);
        return 0
        
    def find_optimal_column(self):
        return 3+int(random()*(self.width-6))

    def draw(self):
        # draw board to the map object
        #self.map.draw(self.game.map, 1 if not self.flip else 24, 3)
        self.display.x = ((self.game.width>>1) - 152) if not self.flip else ((self.game.width>>1)+32)
        self.display.y = ((self.game.height>>1)- 96)  if not self.flip else ((self.game.height>>1)-96)
        self.display.fill(0x7F, 1)
        self.currentpiece.draw(1)
        self.display.redraw()
        self.display.draw()

    def update(self, delta):
        status = self.currentpiece.logic()
        if status == Triste.GAMEOVER:
            print("Game Over!")
            self.gameover = True
            self.newgame()
        elif status == Triste.NEWSHAPE:
            self.currentpiece.draw(0)
            self.newshape()
   
'''
    def get_column_weight(self, column):
        weights = self.map.get_weights()
        return weights[column]
        
    def removeLines(self, mapdata, lineset):
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
    
    def find_lines(self, mapdata):
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
    
    def would_cause_line(self, shape, x, y):
        """ Return true if given shape data would produce a new line , scoring """
        virtual_map = self.map
        for row_index in shape:
            row = shape[row_index]
            for col_index in row:
                virtual_map[y+row_index][x+col_index] &= shape[row_index][col_index]
        lines = self.find_lines(virtual_map)
        return lines.length > 0

def Intent(self, shapedata):
    self.penalty= 0
    self.shape  = shapedata
'''