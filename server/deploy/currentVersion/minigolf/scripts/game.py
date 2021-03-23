#############################################################################################################
#                                                                                                           #
#                                     ,ad8888ba,     ,ad8888ba,    88           88888888888                 #
#                                    d8"'    `"8b   d8"'    `"8b   88           88                          #
#                                   d8'            d8'        `8b  88           88                          #
#                      88       88  88             88          88  88           88aaaaa                     #
#                      88       88  88      88888  88          88  88           88"""""                     #
#                      "8a,   ,a88  Y8,        88  Y8,        ,8P  88           88                          #
#                       `"YbbdP'Y8   Y8a.    .a88   Y8a.    .a8P   88           88                          #
#                               88    `"Y88888P"     `"Y8888Y"'    88888888888  88                          #
#                                                                                                           #
#############################################################################################################
# to import classes in this folder: 
from data.scripts.map           import map
from scripts.main               import menu
from tiledmap                   import TiledMap
from random                     import random
from direction                  import *
from basicgame                  import BasicGame
import                          vpu
        
class Game(BasicGame):    
    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240)
            Game.buffer[1] = vpu.createsurf(320, 240)

            # create map
            Game.setmap(TiledMap(Game, 40, 30))
            if not Game.map.load_tileset("golf"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/golf.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            Game.loadmap(map)
        except:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            quit()
        
    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)            
            
    @staticmethod
    def update(delta):
        # do stuff
        # ...

        # required stuff
        BasicGame.update(delta)
        
    @staticmethod
    def draw():
        if Game.map.need_redraw:
            #clear map buffer
            vpu.select(Game.buffer[0])
            vpu.fill(0,0,0,0)
            
            # draw map
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )
            Game.map.draw()

            # draw things over map layer
            # ...
            
        # clear sprite buffer 
        vpu.select(Game.buffer[1])
        vpu.fill(0,0,0,0)
        
        # draw sprites
        # ...
        
        vpu.select(0)
        left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
        top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
        vpu.drawsurf(Game.buffer[0], left, top)
        vpu.drawsurf(Game.buffer[1], left, top)
        # required stuff
        BasicGame.draw()        

def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()