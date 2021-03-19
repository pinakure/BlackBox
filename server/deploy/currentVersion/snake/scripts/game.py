##############################################################################################################
#                                                                                                            # 
#                   ad88888ba   888b      88         db         88      a8P   88888888888                    #
#                  d8"     "8b  8888b     88        d88b        88    ,88'    88                             #
#                  Y8,          88 `8b    88       d8'`8b       88  ,88"      88                             #
#                  `Y8aaaaa,    88  `8b   88      d8'  `8b      88,d88'       88aaaaa                        #
#                    `"""""8b,  88   `8b  88     d8YaaaaY8b     8888"88,      88"""""                        #
#                          `8b  88    `8b 88    d8""""""""8b    88P   Y8b     88                             #
#                  Y8a     a8P  88     `8888   d8'        `8b   88     "88,   88                             #
#                   "Y88888P"   88      `888  d8'          `8b  88       Y8b  88888888888                    #
#                                                                                                            #
##############################################################################################################
                                                                        
from data.scripts.snake import Snake,Space,Direction
from scripts.main import menu
from random import random
import blackbox
import vpu
import joypad
        
class Game:
    running = True
    width = 0
    height = 0
    dims = {}
    scale = 2.0
    score = 0

    @staticmethod
    def setup():
        print("GAME: Setting up...")
        Game.running = True 
        
        # disable rendering
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
        vpu.setscale(0,Game.scale, Game.scale)
        vpu.setscale(1,Game.scale, Game.scale)
        vpu.select(0);vpu.setrotation(0,0)
        vpu.select(1);vpu.setrotation(1,0)
        vpu.select(2);vpu.setrotation(2,0)
        
        # get layer dimensions
        vpu.select(0); Game.dims[0] = vpu.dimensions()
        vpu.select(1); Game.dims[1] = vpu.dimensions()
        vpu.select(2); Game.dims[2] = vpu.dimensions()
        print(f"GAME: BG Resolution: {Game.dims[0][0]} x {Game.dims[0][1]}")
        print(f"GAME: FG Resolution: {Game.dims[1][0]} x {Game.dims[1][1]}")
        print(f"GAME: OL Resolution: {Game.dims[2][0]} x {Game.dims[2][1]}")
        
        # prepare initial layer state
        vpu.select(0); vpu.fill(8,16,32,255)
        vpu.select(1); vpu.fill(0,0,0,0)
        vpu.select(2); vpu.fill(255,255,0,255)
        
        # enable rendering back
        vpu.enable(0)
        vpu.enable(1)
        vpu.enable(2)
        
        print("GAME: Initializing classes...")
        Snake.initialize(Game)
        
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)
        Snake.spawn(4)
        
    @staticmethod
    def destroy():
        Snake.destroy()
        Space.destroy()

    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)
            
    @staticmethod
    def update(delta):
        
        Snake.update(delta)
        
        # required stuff
        if blackbox.ctrlc():
            print("Control+C pressed.")
            Game.destroy()
            Game.running = False
            
        if joypad.menu():
            menu()
        
    @staticmethod
    def draw():
        #draw stuff
        Space.draw()

        # raster screen and update input
        if joypad.up():     Snake.keydown(Direction.UP)
        elif joypad.down():   Snake.keydown(Direction.DOWN)
        if joypad.right():  Snake.keydown(Direction.RIGHT)
        elif joypad.left():   Snake.keydown(Direction.LEFT)
        
        vpu.update()

def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()