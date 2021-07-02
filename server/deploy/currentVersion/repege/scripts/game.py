# to import classes in this folder: 
# from data.scripts.ship              import Ship 
import blackbox
import vpu
import joypad
import console
try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass
from genericgame            import GenericGame
from data.scripts.map       import map
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap
from debug                  import debug, error, panic

class VolumetricTile:   
    OUTER_LEFT                      = 0x00 
    CENTER                          = 0x01
    OUTER_RIGHT                     = 0x02
    OUTER_CORNER_UP_LEFT            = 0x03
    OUTER_CORNER_UP_RIGHT           = 0x04
    INNER_CORNER_UP_LEFT            = 0x05
    INNER_CORNER_UP_RIGHT           = 0x06 
    INNER_CORNER_DOWN_LEFT          = 0x07 
    INNER_CORNER_DOWN_RIGHT         = 0x08
    OUTER_CORNER_DOWN_LEFT          = 0x09 
    OUTER_CORNER_DOWN_RIGHT         = 0x0A
    WALL_OUTER_CORNER_DOWN_LEFT     = 0x0B 
    WALL_OUTER_CORNER_DOWN_RIGHT    = 0x0C
    WALL_LEFT                       = 0x0D
    WALL_RIGHT                      = 0x0E
    WALL_BOTH                       = 0x0F
    DOOR                            = 0x10

class VolumetricMap:
    pass

class Game(BasicGame):

    @staticmethod
    def setup():
        try:
            debug("Game", "Setting up")
            BasicGame.prepare()
            vpu.setrotation(0,0)
            #allocate custom video buffers
            Game.buffer = [ vpu.createsurf(320, 240) , vpu.createsurf(320, 240) ]
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 4, 16, 16))
            if not Game.map.load_tileset("chipset"):
                panic("Game", "Cannot load tileset", "Game could run perfectly, but we think it's better to abort current execution, as you wouldn't be table to see anything on the screen and that would be definitely bad.")
            Game.map.clear()
            Game.map.load(map, 0)
            
            # randomize contents
            map_pointer = 0
            for y in range(0,Game.map.height):
                for x in range(0,Game.map.width):
                    val = map[map_pointer]
                    if val < 0x20:
                        # expand the tile in each layer
                        Game.map.set(x,y-2,val      , 3) 
                        Game.map.set(x,y-1,val+0x20 , 2) 
                        Game.map.set(x,y,val+0x40   , 1) 
                        Game.map.set(x,y,0x60   , 0) 
                    else:
                        Game.map.set(x,y-2,0x1f     , 3) 
                        Game.map.set(x,y-1,0x1f     , 2) 
                        Game.map.set(x,y,0x1f       , 1) 
                        Game.map.set(x,y,val        , 0) 
                    map_pointer += 1
                        
            # center the map on the screen
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
            # give the engine info about the scrollable area
            Game.map.setboundaries(0, 0, (Game.map.width*Game.map.tile_width)-320, (Game.map.height*Game.map.tile_height)-240)
            # tell engine we want this map to be rendered automatically on vpu.update(), right after entities
            Game.map.setactive() 
            # tell engine which surface the map must be rendered onto
            Game.map.setsurface(Game.buffer[0]) 
            BasicGame.autoredraw = False            
            
        except Exception as E:
            panic("Game", "Setup Failed", str(E))
        
    @staticmethod
    def destroy(): pass

    @staticmethod
    def loop():
        delta = 1.0
        Game.map.need_redraw = True
        while Game.running:
            if Game.map.needsredraw() or Game.map.need_redraw:
                # draw to buffer[1]
                vpu.select(Game.buffer[1])
                vpu.setcolor(255,0,0)
                vpu.fill(0,0,0,0)
                scroll = Game.map.getscroll()
                vpu.textout(f"{scroll.x},{scroll.y}", 0, 0)
                # rasterize buffer onto bg layer
                vpu.select(0)
                left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>2)
                top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>2)
                for buffer in Game.buffer:
                    if buffer:
                        vpu.drawsurf(buffer, left, top)
                # required stuff
            BasicGame.draw()
                
            if joypad.left():       Game.map.scroll_left()
            elif joypad.right():    Game.map.scroll_right()
            if joypad.up():         Game.map.scroll_up()
            elif joypad.down():     Game.map.scroll_down()
            # required stuff
            BasicGame.update(delta)
            

def setup():
    return Game.setup()

def loop():
    return Game.loop()