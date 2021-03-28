# to import classes in this folder: 
# from data.scripts.ship              import Ship 
import blackbox
import vpu
import joypad
import console
from data.scripts.map       import data,map
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap
from pixel                  import Pixel        

class Game(BasicGame):
    
    buffer = [None, None]

    scroll = Pixel(0,0)

    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240) 
            Game.buffer[1] = vpu.createsurf(320, 240) 
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 2, 16, 16))
            if not Game.map.load_tileset("terrain"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/blocks.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            print("Filling 0x00")
            Game.clear()            
            Game.loadmap(data   , 0)
            Game.loadmap(map    , 1)
            
            Game.autoupdate = False
            for y in range(1,Game.map.height - 2):
                for x in range(1,Game.map.width - 2):
                    if int(random()*3)==0:
                        if int(random()*10)==0:
                            c = int(random()*16)
                            Game.map.set(x,y,0x28+c,1)
                    elif int(random()*3)==0:
                        Game.map.set(x,y,0x7,1)

            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
            Game.map.setboundaries(0, 0, (Game.map.width*Game.map.tile_width)-320, (Game.map.height*Game.map.tile_height)-240)
                        
            #Game.map.fill(0x07,1)
            
        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            console.echo(f'ERROR: {str(E)}')                        
        
    @staticmethod
    def clear():
        pass
        #Game.map.fill(0x00,0)
        #Game.map.fill(0x07,1)

    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)
            
    @staticmethod
    def update(delta):
        # required stuff
        BasicGame.update(delta)
        
        if joypad.left():       Game.map.scroll_left()
        elif joypad.up():       Game.map.scroll_up()
        elif joypad.down():     Game.map.scroll_down()
        elif joypad.right():    Game.map.scroll_right()
        
        
    @staticmethod
    def draw_stats():
        pass

    @staticmethod
    def draw_scores():
        pass

    @staticmethod
    def draw():
        if Game.map.needsredraw():
            #clear map buffer
            
            #vpu.select(Game.buffer[0])
            #vpu.fill(0,0,0,0)
            #vpu.select(Game.buffer[1])
            #vpu.fill(0,0,0,0)
            
            # draw map
            Game.map.draw()

            vpu.select(Game.buffer[1])
            vpu.fill(0,0,0,0)
            
            #rasterize layers
            vpu.select(0)
            left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
            top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
            for buffer in Game.buffer:
                if buffer:
                    vpu.drawsurf(buffer, left, top)
        
        # required stuff
        BasicGame.draw()
    
def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()