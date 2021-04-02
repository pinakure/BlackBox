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
import importlib 
import data.scripts.map
importlib.reload(data.scripts.map) 

from data.scripts.triste    import Triste
from data.scripts.map       import data
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap

import console
import blackbox
import vpu
import joypad

class Game(BasicGame):
    
    triste = None
    buffer = [None, None]

    @staticmethod
    def destroy(): pass


    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240) 
            Game.buffer[1] = vpu.createsurf(320, 240) 
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 3))
            if not Game.map.load_tileset("blocks"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/blocks.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            Game.map.fill(0x7E, 0)
            Game.loadmap(data)
            Game.clear()            
            Game.triste = [ Triste(Game), Triste(Game, True) ]
            
        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            console.echo(f'ERROR: {str(E)}')                        
        
    @staticmethod
    def clear():
        Game.map.fill(0x7F, 1)
        Game.map.fill(0x7F, 2)

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
        
        # do stuff
        if Game.running and Game.map.need_redraw:
            Game.triste[0].update(delta)
            Game.triste[1].update(delta)
        
        
    @staticmethod
    def draw_stats():
        pass

    @staticmethod
    def draw_scores():
        vpu.setfont('smk')
        score_a = ("0"*(13-(len(str(Game.triste[0].score))))) + str(Game.triste[0].score)
        score_b = ("0"*(13-(len(str(Game.triste[1].score))))) + str(Game.triste[1].score)
        lines_a = ("0"*(3-(len(str(Game.triste[0].lines))))+str(Game.triste[0].lines)) if Game.triste[0].lines < 999 else "999"
        lines_b = ("0"*(3-(len(str(Game.triste[1].lines))))+str(Game.triste[1].lines)) if Game.triste[1].lines < 999 else "999"
        level_a = ("0"*(3-(len(str(Game.triste[0].level))))+str(Game.triste[0].level)) if Game.triste[0].level < 999 else "999"
        level_b = ("0"*(3-(len(str(Game.triste[1].level))))+str(Game.triste[1].level)) if Game.triste[1].level < 999 else "999"
        vpu.setcolor(192,16,0,32)
        vpu.textout(score_a,0, 0)
        vpu.setcolor(0,16,192,32)
        vpu.textout(score_b,320-(8*13), 0)
        vpu.setfont('tiny')
        vpu.setcolor(192,16,0,32)
        vpu.textout(lines_a, 138, 48)
        vpu.textout(level_a, 138, 80)
        vpu.setcolor(0,16,192,32)
        vpu.textout(lines_b, 162, 48)
        vpu.textout(level_b, 162, 80)

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

            vpu.select(Game.buffer[1])
            vpu.fill(0,0,0,0)
            Game.triste[0].draw()
            Game.triste[1].draw()
            
            vpu.select(Game.buffer[1])
            Game.draw_scores()
            Game.draw_stats()
            
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