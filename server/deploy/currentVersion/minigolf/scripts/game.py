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
# from data.scripts.ship              import Ship 
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
        # initialize subcomponents:
        # Call static class.initialize(Game) methods for each base class
        # ...
        
        print("GAME: Initializing object pools...")
        # Create object pools:
        # for i in range(0, Game.xxx_pool_size):
        #    Game.generate_random_xxx() <- preallocate memory
        # ...

        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)
        
        
    @staticmethod
    def destroy():
        pass

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
        if blackbox.ctrlc():
            print("Control+C pressed.")
            Game.destroy()
            Game.running = False
            
        if joypad.menu():
            menu()
        
    @staticmethod
    def draw():
        # clear buffer
        vpu.select(1)
        vpu.fill(0,0,0,0)
        
        #draw stuff
        # ...

        # raster screen and update input
        vpu.setscale(1,Game.scale, Game.scale)
        vpu.update()

def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()