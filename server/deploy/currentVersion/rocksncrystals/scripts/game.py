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

from data.scripts.map       import data,map
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap
from debug                  import debug, error, panic

class Game(BasicGame):

    TILE_ROCK = 0x61
    TILE_TNT  = 0x60
    TILE_BLOCKS    = 0x28
    TILE_GEM_BIG   = 0x40
    TILE_GEM_SMALL = 0x50

    @staticmethod
    def setup():
        try:
            debug("Game", "Setting up")
            BasicGame.prepare()
            vpu.setrotation(0,0)
            #allocate custom video buffers
            Game.buffer = [ vpu.createsurf(320, 240) , vpu.createsurf(320, 240) ]
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 2, 16, 16))
            if not Game.map.load_tileset("terrain"):
                panic("Game", "Cannot load 'tilesets/blocks.png'", "Game could run perfectly, but we think it's better to abort current execution, as you wouldn't be table to see anything on the screen and that would be definitely bad.")
            Game.map.load(data,0)
            Game.map.load(map, 1)
            
            # randomize contents
            for y in range(1,Game.map.height - 1):
                for x in range(1,Game.map.width - 1):
                    if int(random()*25)==0: # block
                        c = int(random()*24)
                        Game.map.set(x,y,0x28+c,1)
                    elif int(random()*25)==0: # Small Gem
                        c = int(random()*16)
                        Game.map.set(x,y,0x50+c,1)
                    elif int(random()*100)==0: # TNT
                        Game.map.set(x,y,0x60,1)
                    elif int(random()*50)==0: # Big Gem
                        c = int(random()*16)
                        Game.map.set(x,y,0x40+c,1)
                    elif int(random()*3)==0:  # boulder
                        Game.map.set(x,y,0x61,1)
                    else:
                        Game.map.set(x,y,0x1,1) # dirt
            
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