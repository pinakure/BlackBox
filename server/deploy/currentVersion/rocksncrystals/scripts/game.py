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
            vpu.setrotation(0,0)
            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240) 
            Game.buffer[1] = vpu.createsurf(320, 240) 
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 2, 16, 16))
            if not Game.map.load_tileset("terrain"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/blocks.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            print("Filling 0x00")
            Game.loadmap(data   , 0)
            Game.loadmap(map    , 1)
            
            # randomize contents
            for y in range(1,Game.map.height - 2):
                for x in range(1,Game.map.width - 2):
                    if int(random()*3)==0:
                        if int(random()*10)==0:
                            c = int(random()*16)
                            Game.map.set(x,y,0x28+c,1)
                    elif int(random()*3)==0:
                        Game.map.set(x,y,0x7,1)

            # center the map on the screen
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
            # give the engine info about the scrollable area
            Game.map.setboundaries(0, 0, (Game.map.width*Game.map.tile_width)-320, (Game.map.height*Game.map.tile_height)-240)
            # tell engine we want this map to be rendered automatically on vpu.update(), right after entities
            Game.map.setactive() 
            # tell engine which surface the map must be rendered onto
            Game.map.setsurface(Game.buffer[0]) 
            
        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            console.echo(f'ERROR: {str(E)}')                        
        
    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            if Game.map.needsredraw():
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

def destroy():
    Game.map.setactive(False)
    Game.map.setsurface(-1)
    return Game.destroy()