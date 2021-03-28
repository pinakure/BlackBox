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
import importlib 
import data.scripts.map
importlib.reload(data.scripts.map) 

from data.scripts.map           import map
from data.scripts.editor        import Editor
from scripts.main               import menu
from tiledmap                   import TiledMap
from random                     import random
from direction                  import *
from basicgame                  import BasicGame
import                          vpu
import                          joypad
        
class Terrain:
    game            = None
    width           = 40
    height          = 30
    invert          = False
    depth_range     = 32
    data            = None
    seed            = 0x1337DEAD
    scale_factor    = 2.0
    octave          = 7

    @staticmethod
    def initialize(game):
        Terrain.data = vpu.createsurf(Terrain.width, Terrain.height)            
        Terrain.game = game
        x, y = Terrain.randomize()
        Terrain.sethole(x, y)
        Terrain.setpole(x, y)
        Terrain.setflag(x, y)

    @staticmethod
    def randomize():
        #make noise
        Terrain.seed = int(random() * 0xFFFFFF)
        Terrain.scale_factor = 1+(random()*3.0)
        Terrain.octave = 5+int(random()*15)
        vpu.perlin(Terrain.data, Terrain.depth_range,0,0, Terrain.scale_factor, Terrain.octave, Terrain.seed) # Will produce up to 5 valid values...
        #vpu.perlin(Game.terrain, 32, 32, 32, 2.0, 7)
        vpu.select(Terrain.data)
        for y in range(0,30):
            for x in range(0,40):
                c = vpu.getpixel(x,y) & 0xFF
                c = int((c / Terrain.depth_range) * 5) # 5 is the number of different turfs
                if c == 0x00: Terrain.game.map.set(x,y, 0x00 if not Terrain.invert else 0x80, 0)
                elif c==0x01: Terrain.game.map.set(x,y, 0x20 if not Terrain.invert else 0x60, 0)
                elif c==0x02: Terrain.game.map.set(x,y, 0x40 if not Terrain.invert else 0x40, 0)
                elif c==0x03: Terrain.game.map.set(x,y, 0x60 if not Terrain.invert else 0x20, 0)
                elif c==0x04: Terrain.game.map.set(x,y, 0x80 if not Terrain.invert else 0x00, 0)
                else: print(hex(c))
        return int(random()*Terrain.width), int(random()*Terrain.height)

    @staticmethod
    def setpole(x,y):
        if y<2:y=2
        Terrain.game.map.set(x,y-2, 0x4F, 2)
        Terrain.game.map.set(x,y-1, 0x6F, 2)
        Terrain.game.map.set(x,  y, 0x8F, 2)
    
    @staticmethod
    def setflag(x,y):
        if y<2:y=2
        Terrain.game.map.set(x,y-2, 0x4E, 2)
    
    @staticmethod
    def sethole(x,y):
        if y<2:y=2
        Terrain.game.map.set(x, y, 0x4D, 1)
        

class Game(BasicGame):    

    buffer  = [ None, None, None ]    
    
    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer[0] = vpu.createsurf(320, 240) 
            Game.buffer[1] = vpu.createsurf(320, 240) 
            Game.buffer[2] = vpu.createsurf(320, 240) 
    
            # create map
            Game.setmap(TiledMap(Game, 40, 30, 3))
            if not Game.map.load_tileset("golf"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/golf.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            Game.loadmap(map)
            vpu.setfont('tiny')
            Terrain.initialize(Game)
            Editor.initialize(Game)

        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            print(str(E))
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
        if joypad.select()==1:
            Terrain.randomize()

        # required stuff
        BasicGame.update(delta)
        
    @staticmethod
    def destroy():
        for i in range(0,3):
            if Game.buffer[i]: 
                vpu.deletesurf(Game.buffer[i])
        BasicGame.destroy()

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
            
        # clear sprite buffers
        vpu.select(Game.buffer[1])
        vpu.fill(0,0,0,0)
        vpu.select(Game.buffer[2])
        vpu.fill(0,0,0,0)
        vpu.setcolor(255,255,255,255)            
        vpu.textout(f"Editor Value: {hex(Editor.current)}"              ,2, 200)
        vpu.textout(f"Scale Factor: {str(Terrain.scale_factor)[0:4]}"   ,2, 210)
        vpu.textout(f"Noise Octave: {Terrain.octave}"                   ,2, 220)
        vpu.textout(f"Random  Seed: 0x{hex(Terrain.seed).upper()[2:]}"  ,2, 230)
        
        # draw editor
        Editor.draw()
        Editor.update()
        
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