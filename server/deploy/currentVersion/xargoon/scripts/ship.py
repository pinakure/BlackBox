from vpu import *
from data.scripts.flame import Flame

class Ship:
    TYPE_A = 0x01
    TYPE_B = 0x02
    # ------------- #
    game = None
    tileset = None
    initialized = False

    @staticmethod
    def initialize(game):
        Ship.game = game
        print("Initializing Ship...")
        Ship.tileset = createsprite("ship01",10)
        Ship.initialized = True

    @staticmethod
    def destroy():
        if Ship.tileset:
            deletesprite(Ship.tileset)
            Ship.tileset = None
        Ship.initialized = False

    def __init__(self,x=0, y=0, type=TYPE_A):
        self.x = x
        self.y = y
        self.w = 32
        self.h = 32
        self.type = type
        self.status = 0
        self.shooting = False
        self.anim = createanim(self.w, self.h, Ship.tileset, 0, 0, 3, 1)
        self.flame = Flame(self.x, self.y + self.h)
        
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
        if self.flame: 
            self.flame = None

    def draw(self):
        drawanim(self.anim, self.x, self.y)
        self.flame.x = self.x
        self.flame.y = self.y + self.h - 2
        self.flame.draw()
        self.flame.x = self.x+self.w-8 
        self.flame.y = self.y + self.h - 2
        self.flame.draw()