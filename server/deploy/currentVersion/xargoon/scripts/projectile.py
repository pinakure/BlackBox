from vpu import *
from random import random

class Projectile:
    TYPE_A   = 0x00
    TYPE_B   = 0x01
    TYPE_C   = 0x02
    TYPE_D   = 0x03
    TYPE_MAX = 0x04
    #-------------#
    game = None
    initialized = False
    tileset = None
    gfx = {}

    @staticmethod
    def initialize(game):
        Projectile.game = game
        print("Initializing Ship...")
        sprite = createsprite("particles",10)
        Projectile.tileset = subsprite(sprite,0, 16, 128, 32)
        deletesprite(sprite)
        Projectile.gfx[ Projectile.TYPE_A ] = {}
        Projectile.gfx[ Projectile.TYPE_B ] = {}
        Projectile.gfx[ Projectile.TYPE_C ] = {}
        Projectile.gfx[ Projectile.TYPE_D ] = {}
        Projectile.gfx[ Projectile.TYPE_A ] [0 ] = createanim(8,16, Projectile.tileset, 0, 0, 0, 0, False)
        Projectile.gfx[ Projectile.TYPE_B ] [0 ] = createanim(8,16, Projectile.tileset, 1, 0, 1, 0, False)
        Projectile.gfx[ Projectile.TYPE_C ] [0 ] = createanim(8,16, Projectile.tileset, 2, 0, 2, 0, False)
        Projectile.gfx[ Projectile.TYPE_D ] [0 ] = createanim(8,16, Projectile.tileset, 3, 0, 3, 0, False)
        Projectile.gfx[ Projectile.TYPE_A ] [1 ] = createanim(8,16, Projectile.tileset, 4, 0, 4, 0, False)
        Projectile.gfx[ Projectile.TYPE_B ] [1 ] = createanim(8,16, Projectile.tileset, 5, 0, 5, 0, False)
        Projectile.gfx[ Projectile.TYPE_C ] [1 ] = createanim(8,16, Projectile.tileset, 6, 0, 6, 0, False)
        Projectile.gfx[ Projectile.TYPE_D ] [1 ] = createanim(8,16, Projectile.tileset, 7, 0, 7, 0, False)
        Projectile.gfx[ Projectile.TYPE_A ] [2 ] = createanim(8,16, Projectile.tileset, 8, 0, 8, 0, False)
        Projectile.gfx[ Projectile.TYPE_B ] [2 ] = createanim(8,16, Projectile.tileset, 9, 0, 9, 0, False)
        Projectile.gfx[ Projectile.TYPE_C ] [2 ] = createanim(8,16, Projectile.tileset,10, 0,10, 0, False)
        Projectile.gfx[ Projectile.TYPE_D ] [2 ] = createanim(8,16, Projectile.tileset,11, 0,11, 0, False)
        Projectile.gfx[ Projectile.TYPE_A ] [3 ] = createanim(8,16, Projectile.tileset,12, 0,12, 0, False)
        Projectile.gfx[ Projectile.TYPE_B ] [3 ] = createanim(8,16, Projectile.tileset,13, 0,13, 0, False)
        Projectile.gfx[ Projectile.TYPE_C ] [3 ] = createanim(8,16, Projectile.tileset,14, 0,14, 0, False)
        Projectile.gfx[ Projectile.TYPE_D ] [3 ] = createanim(8,16, Projectile.tileset,15, 0,15, 0, False)
        Projectile.initialized = True
    
    @staticmethod
    def destroy():
        if len(Projectile.gfx) > 0:
            for o in range(0,4):
                for i in range(0,4):
                    if Projectile.gfx[o][i]:
                        deleteanim(Projectile.gfx[o][i])
                        Projectile.gfx[o][i] = None
                Projectile.gfx[o] = {}
            Projectile.gfx = {}
        if Projectile.tileset:
            deletesprite(Projectile.tileset)
            Projectile.tileset = None
        Projectile.initialized = False

    def __init__(self, x, y, projectile_type=TYPE_A, level=0):
        self.x = x
        self.y = y
        self.projectile_type = projectile_type
        self.delta_x = 0.0
        self.delta_y = -1.5
        self.alive = True
        self.rotation = 0.0
        self.level = level
        
    def __del__(self):
        pass

    def update(self, delta):
        if not self.alive: return
        self.x += self.delta_x
        self.y += self.delta_y
        if self.x <= 0: self.alive = False
        elif self.y <= 0: self.alive = False
        elif self.x > int(Projectile.game.dims[1][0]): self.alive = False
        elif self.y > int(Projectile.game.dims[1][1]): self.alive = False

    @staticmethod
    def spawn(x,y,projectile_type,level,delta_x=0.0, delta_y=-1.5):
        for projectile in Projectile.game.projectiles:
            if projectile.alive: continue
            projectile.level = level
            projectile.projectile_type = projectile_type
            projectile.x = x
            projectile.y = y
            projectile.delta_x = delta_x
            projectile.delta_y = delta_y
            projectile.alive = True
            return

    def draw(self):
        if not self.alive: return
        drawanim(Projectile.gfx[self.projectile_type][self.level], int(self.x)-4, int(self.y)-8)        
        