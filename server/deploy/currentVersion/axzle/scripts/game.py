##############################################################################################################
#                                                                                                            #
#                        db         8b        d8  888888888888  88           88888888888                     #
#                       d88b         Y8,    ,8P            ,88  88           88                              #
#                      d8'`8b         `8b  d8'           ,88"   88           88                              #
#                     d8'  `8b          Y88P           ,88"     88           88aaaaa                         #
#                    d8YaaaaY8b         d88b         ,88"       88           88"""""                         #
#                   d8""""""""8b      ,8P  Y8,     ,88"         88           88                              #
#                  d8'        `8b    d8'    `8b   88"           88           88                              #
#                 d8'          `8b  8P        Y8  888888888888  88888888888  88888888888                     #
#                                                                                                            #
##############################################################################################################
# to import classes in this folder: 
# from data.scripts.ship              import Ship 
from scripts.main import menu
from random import random
import blackbox
import vpu
import joypad
from tiledmap import TiledMap
from basicgame import BasicGame

try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass
from data.scripts.map import data, overlay

class Game(BasicGame):
    
    buffer = []
    area = None

    @staticmethod
    def setup():
        print("GAME: Setup...")
        BasicGame.prepare()
        vpu.setrotation(0,0)
        vpu.transition() 
        while not vpu.update():pass
        # change screen content here
        Game.buffer = [ vpu.createsurf(320, 240), vpu.createsurf(320, 240) ]
        # create display map
        Game.area = TiledMap(Game, 16, 16, 1, 8, 8)
        Game.setmap(TiledMap(Game, 40, 30, 2, 8, 8))
        if not Game.map.load_tileset("tiles"):
            print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/tiles.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
            quit()
        Game.area.load_tileset("tiles")
        Game.map.fill(0x00,0)
        Game.map.fill(0x0f,1)
        Game.loadmap(data       , 0)
        Game.loadmap(overlay    , 1)
        Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
        Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
        Game.area.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
        Game.area.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120            
        Game.map.setsurface(Game.buffer[0])
        Game.area.setsurface(Game.buffer[1])

        # enable rendering back
        vpu.enable(0)
        vpu.enable(1)
        vpu.enable(2)
        
        
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)
        
        
    @staticmethod
    def destroy():
        pass

    @staticmethod
    def title():
        vpu.select(0)
        vpu.fill(0,0,0)
        vpu.perlin(0, 160, 80,0)
        vpu.setfont('magic')
        vpu.setcolor(255,255,255)
        vpu.textout("AXZLE", int(Game.width>>1)-64,  int(Game.height>>1)-64 )
        while not vpu.update():pass        
        while not joypad.start():
            vpu.select(1)
            vpu.fill(0,0,0,0)
            if vpu.frames()%20>10:
                vpu.setfont('mana')        
                vpu.setcolor(255,255,255)
                vpu.textout("PRESS START BUTTON", int(Game.width>>1)-72,  int(Game.height>>1)+64 )
            vpu.update()
        vpu.transition(int(random()*20)) 
        while not vpu.update():pass
        vpu.select(0);vpu.fill(0,0,0)
        vpu.select(1);vpu.fill(0,0,0,0)
        vpu.select(2);vpu.fill(0,0,0,0)        
        vpu.setfont('ibm')

    @staticmethod
    def updategrid():
        ox = 22
        oy = 12
        i = 0
        for y in range(0, 16):
            for x in range(0, 16):
                Game.map.set(ox + x, oy + y, 0x10+int(random()*8), 1)
                i+=1

    @staticmethod
    def loop():
        delta = 1.0
        Game.title()
        Game.map.setactive()
        while Game.running:
            if Game.map.needsredraw():
                Game.updategrid()
            # rasterize buffer onto bg layer
            vpu.select(0)
            left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
            top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
            for buffer in Game.buffer:
                if buffer:
                    vpu.drawsurf(buffer, left, top)
            
            BasicGame.draw()
            
            
            # if joypad.left():       Game.map.scroll_left()
            # elif joypad.right():    Game.map.scroll_right()
            # if joypad.up():         Game.map.scroll_up()
            # elif joypad.down():     Game.map.scroll_down()
            # required stuff
            BasicGame.update(delta)
            
def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    Game.map.setactive(True)
    Game.map.setsurface(-1)
    return Game.destroy()