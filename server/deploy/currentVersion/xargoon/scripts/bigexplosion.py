
from vpu import *
from random import random

class BigExplosion:
    game = None
    tileset = None
    initialized = False
    
    @staticmethod
    def initialize(game):
        BigExplosion.game = game
        print("Initializing BigExplosion...")
        BigExplosion.tileset = createsprite("explosion_big",13)
        if not BigExplosion.tileset: print("ERROR: Cannot load BigExplosion tileset!")
        else: BigExplosion.initialized = True
    
    @staticmethod
    def destroy():
        if BigExplosion.tileset:
            deletesprite(BigExplosion.tileset)
            BigExplosion.tileset = None
        BigExplosion.initialized = False
    
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = createanim(32,32,BigExplosion.tileset,0,0,1,3,False,0.25)
    
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
    
    def update(self, delta):
        if not self.alive: 
            return
        self.time+=1
        if self.time > 30:
            self.alive = False

    def spawn(self):
        # self.x = int(BigExplosion.game.dims[1][0]/4) + int(random()*(BigExplosion.game.dims[1][0]/2))-16
        # self.y = int(BigExplosion.game.dims[1][1]/4) + int(random()*(BigExplosion.game.dims[1][1]/2))-16
        self.x = int(random()*BigExplosion.game.dims[1][0])-16
        self.y = int(random()*BigExplosion.game.dims[1][1])-16
        self.time = 0
        self.alive = True

    def draw(self):
        if not self.alive: 
            return
        drawanim(self.anim, int(self.x), int(self.y))
        