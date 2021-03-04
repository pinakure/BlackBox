
from vpu import * 

class Flame:
    TYPE_A = 0x00
    TYPE_B = 0x04
    TYPE_C = 0x08
    TYPE_D = 0x0C
    #-------------#
    game = None
    initialized = False
    tileset = None
    gfx = {}

    @staticmethod
    def initialize(game):
        Flame.game = game
        print("Initializing Flames...")
        Flame.tileset = createsprite("particles",10)
        Flame.initialized = True
    
    @staticmethod
    def destroy():
        if len(Flame.gfx) > 0:
            for o in range(0,4):
                for i in range(0,4):
                    if Flame.gfx[o][i]:
                        deleteanim(Flame.gfx[o][i])
                        Flame.gfx[o][i] = None
                Flame.gfx[o] = {}
            Flame.gfx = {}
        if Flame.tileset:
            deletesprite(Flame.tileset)
            Flame.tileset = None
        Flame.initialized = False
    
    def __init__(self, x, y, flame_type=TYPE_A):
        self.flame_type = flame_type
        self.x = x
        self.y = y
        self.anim = createanim(8, 16, Flame.tileset, 0, 0, 7, 0, False, 0.25)

    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None

    def draw(self):
        drawanim(self.anim, self.x, self.y)
        