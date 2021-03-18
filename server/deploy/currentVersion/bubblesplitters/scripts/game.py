# ---------------------------------------------------------------------- #
#   _           _     _     _                 _ _ _   _                  #
#  | |         | |   | |   | |               | (_) | | |                 #
#  | |__  _   _| |__ | |__ | | ___  ___ _ __ | |_| |_| |_ ___ _ __ ___   #
#  | '_ \| | | | '_ \| '_ \| |/ _ \/ __| '_ \| | | __| __/ _ \ '__/ __|  #
#  | |_) | |_| | |_) | |_) | |  __/\__ \ |_) | | | |_| ||  __/ |  \__ \  #
#  |_.__/ \__,_|_.__/|_.__/|_|\___||___/ .__/|_|_|\__|\__\___|_|  |___/  #
#                                      | |                               #
#                                      |_|                               #
# -----------------------------------------------------------------------#

from data.scripts.bubble import Bubble
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
    lives = 3
    stage = 0
    world = 0
    time  = 0

    player = None

    @staticmethod
    def setup():
        print("GAME: Setting up...")
        Game.running = True
        
        # disable rendering
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
        vpu.setscale(0, 1.0, 1.0)
        vpu.setscale(1, 1.0, 1.0)
        
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
        Bubble.initialize(Game)
        
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)

    @staticmethod
    def newgame():
        Game.lives   = 3
        Game.stage   = 0
        Game.world   = 0
        Game.player.spawn()
        
    @staticmethod
    def destroy():
        pass

    @staticmethod
    def loop():
        vpu.setscale(1,Game.scale, Game.scale)
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)            
            
    @staticmethod
    def update(delta):
        # do stuff
        Game.time+=1
        if Game.time > 120:
            for bubble in Bubble.pool:
                if bubble.enabled:
                    if int(random() * 10)==1:
                        bubble.pang() 
                        Game.time = 0
                        break

        for bubble in Bubble.pool:
            if not bubble.enabled: continue
            bubble.update(delta)

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
        vpu.update()

def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()