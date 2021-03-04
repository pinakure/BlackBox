from vpu import *
from random import random

class SmallExplosion:
    TYPE_A = 0x00
    TYPE_B = 0x01
    # ----------- #    
    game = None
    tileset = None
    initialized = False

    @staticmethod
    def initialize(game):
        SmallExplosion.game = game
        print("Initializing SmallExplosion...")
        SmallExplosion.tileset = createsprite("explosion_small")
        SmallExplosion.initialized = True

    @staticmethod
    def destroy():
        if SmallExplosion.tileset:
            deletesprite(SmallExplosion.tileset)
            SmallExplosion.tileset = None
        SmallExplosion.initialized = False

    def __init__(self, x=0, y=0, type=TYPE_A):
        self.type = type
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = createanim(16, 16,SmallExplosion.tileset, 0, 0, 7, 0, False, 0.25)

    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None

    def spawn(self):
        self.x = int(SmallExplosion.game.dims[1][0]/4) + int(random()*(SmallExplosion.game.dims[1][0]/2))-16
        self.y = int(SmallExplosion.game.dims[1][1]/4) + int(random()*(SmallExplosion.game.dims[1][1]/2))-16
        self.time = 0
        self.alive = True

    def update(self, delta):
        if not self.alive: 
            return
        self.time+=1
        if self.time > 30:
            self.alive = False

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        drawanim(self.anim, self.x, self.y)
        if self.time > 30:
            self.alive = False
    
