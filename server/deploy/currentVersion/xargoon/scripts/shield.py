from vpu import *
from random import random

class Shield:
    ORIENTATION_NORTH = 0
    ORIENTATION_NE    = 1
    ORIENTATION_EAST  = 2
    ORIENTATION_ES    = 3
    ORIENTATION_SOUTH = 4
    ORIENTATION_SW    = 5
    ORIENTATION_WEST  = 6
    ORIENTATION_NW    = 7
    TYPE_MAX = 0x03
    # ----------- #    
    gfx = {}
    game = None
    tileset = None
    # ------------#
    initialized = False
    alive = True
    orientation = ORIENTATION_NORTH
    x = 0
    y = 0
    time = 0

    @staticmethod
    def initialize(game):
        Shield.game = game
        print("Initializing Shield...")
        Shield.tileset = createsprite("shield")
        if Shield.tileset:
            for i in range(0,8):
                Shield.gfx[ i ] =  createanim(64, 64, Shield.tileset, 0, 0, 7, 0, False,1/1.5,False)
            Shield.initialized = True
        else:
            print("ERROR: Cannot load Shield tileset!")        
        

    @staticmethod
    def destroy():
        for i in Shield.gfx: 
            deleteanim(i)
        Shield.gfx = {}        
        
        if Shield.tileset:
            deletesprite(Shield.tileset)
        Shield.tileset = None

        Shield.initialized = False

    @staticmethod
    def spawn(x, y, orientation):
        Shield.x = x
        Shield.y = y
        Shield.time = 0
        Shield.alive = True
        Shield.orientation = orientation
        #resetanim(Shield.gfx[Shield.orientation])
        
    @staticmethod
    def update(delta):
        if not Shield.alive: 
            return
        Shield.x = Shield.game.ship.x
        Shield.y = Shield.game.ship.y + 12
        time = 8 * 1.5
        if (Shield.time % time) == time-1:
            Shield.time = 0
            Shield.orientation += 1 #int(random() * 4)
            Shield.orientation = Shield.orientation % 8
            Shield.alive = False

    @staticmethod
    def draw():
        if not Shield.alive: 
            return
        drawanim(Shield.gfx[Shield.orientation], int(Shield.x), int(Shield.y), Shield.orientation*(3.141591/4))
        updateanim(Shield.gfx[Shield.orientation])
        Shield.time+=1#*delta
   