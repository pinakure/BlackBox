
from vpu import *

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
        self.anim = createanim(32,32,BigExplosion.tileset,0,0,1,3,False)
    
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
        
    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        drawanim(self.anim, self.x, self.y)
        if self.time > 16:
            self.alive = True
