import tm
import console
from pixel          import Pixel
from boundaries     import Boundaries
from direction      import Direction
from debug          import debug, error

class TiledMap:
    game = None

    def __init__(self, game, width, height, layer_count=1, tile_width=8, tile_height=8):
        TiledMap.game = game
        self.x           = ( game.width   >> 1 ) - (( width  * tile_width  ) >> 1 )
        self.y           = ( game.height  >> 1 ) - (( height * tile_height ) >> 1 )
        self.width       = width        
        self.height      = height
        self.layer_count = layer_count
        self.boundaries  = Boundaries(0, 0, (width*tile_width)-(game.width>>2), (height*tile_height)-(game.height>>2))
        self.scroll      = Pixel(0,0)
        self.tile_width  = tile_width
        self.tile_height = tile_height
        self.target      = 0 # background
        self.need_redraw = False
        self.handle      = tm.create(width, height, layer_count, tile_width, tile_height)

    def load(self, data, layer=0):
        i=0
        try:
            for y in range(0, self.height):
                for x in range(0,  self.width):
                    self.setvalue(x,y, data[i], layer)
                    i+=1
        except:
            debug('TiledMap', "WARNING: Insufficient data provided to fill map!")


    def needsredraw(self):
        return tm.needsredraw(self.handle)

    def redraw(self):
        self.need_redraw = True
        tm.redraw(self.handle, -1)
    
    def draw(self):
        tm.draw(self.handle, self.x, self.y)
        self.need_redraw = False

    def setboundaries(self, left, top, right, bottom):
        self.boundaries.left    = left
        self.boundaries.top     = top
        self.boundaries.right   = right
        self.boundaries.bottom  = bottom
        tm.setboundaries(self.handle, left, top, right, bottom)
        console.echo(f"Map boundaries set from ({left},{top}) to ({right},{bottom})")
    def setactive(self, disable=False):         tm.setactive(self.handle,disable)
    def setsurface(self, surface_handle):       tm.setsurface(self.handle, surface_handle)
    def set(self, x, y, tile_index, layer=0):   tm.set(self.handle, x, y, tile_index, layer) # duplicated!
    def setvalue(self, x,y, value, layer=0):    tm.set(self.handle, x, y, value, layer)    
    def count(self, value, layer_index=-1):     return tm.count(self.handle, value, layer_index)
    def fill(self, value, layer_index=-1):      tm.fill(self.handle, value, layer_index)
    def clear(self, layer_index=-1):            tm.clear(self.handle, layer_index)
    def get(self, x, y, layer_index=0):         return tm.get(self.handle, x, y, layer_index)
    def gettileset(self, x, y, layer_index=0):  return (tm.get(self.handle, x, y, layer_index) & 0xFF000000) >> 6
    def gettile(self, x, y, layer_index=0):     return (tm.get(self.handle, x, y, layer_index) & 0x00FFFFFF)
    def getscroll(self):                        
        p = Pixel()
        scroll = tm.getscroll(self.handle)
        p.x = scroll[0]
        p.y = scroll[1]
        return p
    def getdata(self, layer_index=0):           return tm.getdata(self.handle, layer_index)
    def setdata(self, data, layer_index=0):     tm.setdata(self.handle, data, layer_index)
    def settarget(self, surface_handle):        tm.settarget(self.handle, surface_handle)
    def setposition(self, x, y):                
        self.x = x
        self.y = y
        tm.setposition(self.handle, self.x, self.y)
    def setscroll(self, x, y):                  tm.setscroll(self.handle, x, y)    
    def scroll_up(self   , delta=1):            tm.scroll(self.handle, delta, Direction.UP)    
    def scroll_down(self , delta=1):            tm.scroll(self.handle, delta, Direction.DOWN)  
    def scroll_left(self , delta=1):            tm.scroll(self.handle, delta, Direction.LEFT)  
    def scroll_right(self, delta=1):            tm.scroll(self.handle, delta, Direction.RIGHT) 
    def load_tileset(self, tileset_filename):   return tm.loadtileset(self.handle, tileset_filename)
    def update(self, delta):                    tm.update(self.handle, delta)