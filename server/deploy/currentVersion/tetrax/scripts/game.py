########################################################################################################
#                                                                                                      #
#         888888888888  88888888888  888888888888  88888888ba          db         8b        d8         #
#              88       88                88       88      "8b        d88b         Y8,    ,8P          #
#              88       88                88       88      ,8P       d8'`8b         `8b  d8'           #
#              88       88aaaaa           88       88aaaaaa8P'      d8'  `8b          Y88P             #
#              88       88"""""           88       88""""88'       d8YaaaaY8b         d88b             #
#              88       88                88       88    `8b      d8""""""""8b      ,8P  Y8,           #
#              88       88                88       88     `8b    d8'        `8b    d8'    `8b          #
#              88       88888888888       88       88      `8b  d8'          `8b  8P        Y8         #
#                                                                                                      #
########################################################################################################
try: del Map
except: pass
try: del Game
except: pass
try: del Triste
except: pass

from data.scripts.triste    import Triste
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap

import console
import blackbox
import vpu
import joypad

class Game(BasicGame):
    
    buffer = [None, None]

    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240) 
            Game.buffer[1] = vpu.createsurf(320, 240) 
            # create map
            Game.setmap(TiledMap(Game, 40, 30, 3))
            if not Game.map.load_tileset("blocks"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/blocks.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            Game.map.fill(0x7E, 0)
            Game.map.fill(0x7F, 1)
            Game.map.fill(0x7F, 2)
            
            Triste.initialize(Game)

            #Game.loadmap(map)
            vpu.setfont('smk')

            

        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            console.echo(f'ERROR: {str(E)}')                        
        
    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)            
            
    @staticmethod
    def update(delta):
        # do stuff
        Triste.update(delta)
        
        # required stuff
        BasicGame.update(delta)
        
    @staticmethod
    def draw():
        Triste.draw()
        if Game.map.need_redraw:
            #clear map buffer
            vpu.select(Game.buffer[0])
            vpu.fill(0,0,0,0)
            
            # draw map
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )
            Game.map.draw()

            # draw things over map layer
            
        # clear sprite buffers
        vpu.select(Game.buffer[1])
        vpu.fill(0,0,0,0)
        vpu.setcolor(255,255,255,255)            
        # vpu.textout(f"Editor Value: {hex(Editor.current)}"              ,2, 200)
        # vpu.textout(f"Scale Factor: {str(Terrain.scale_factor)[0:4]}"   ,2, 210)
        # vpu.textout(f"Noise Octave: {Terrain.octave}"                   ,2, 220)
        # vpu.textout(f"Random  Seed: 0x{hex(Terrain.seed).upper()[2:]}"  ,2, 230)
        
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