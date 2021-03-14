
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
        self.energy_type = 0
        self.time = 0
        self.alive = True
        self.anim_fire   = createanim(32,32,BigExplosion.tileset,0,0,3,1,False,0.25)
        self.anim_plasma = createanim(32,32,BigExplosion.tileset,0,2,3,3,False,0.25)
        self.anim = self.anim_fire
    
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
    
    def update(self, delta):
        if not self.alive: 
            return
        self.time+=1
        if self.time > 30:
            self.alive = False

    def spawn(self, x, y, energy_type=0):
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.energy_type = energy_type
        self.anim = self.anim_plasma if self.energy_type == 1 else self.anim_fire

    def draw(self):
        if not self.alive: 
            return
        updateanim(self.anim)        
        drawanim(self.anim, int(self.x), int(self.y))
        