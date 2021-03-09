from vpu import *
from random import random
from math import atan2

class Projectile:
    TYPE_A   = 0x00
    TYPE_B   = 0x01
    TYPE_C   = 0x02
    TYPE_D   = 0x03
    TYPE_MAX = 0x04
    #-------------#
    game = None
    initialized = False
    gfx = {}

    @staticmethod
    def initialize(game):
        Projectile.game = game
        print("Initializing Ship...")
        sprite = createsprite("particles",10)
        tileset = subsprite(sprite,0, 16, 128, 32)
        deletesprite(sprite)
        if tileset:
            for i in range(0, 4):   
                Projectile.gfx[ i ] = {}
            for i in range(0, 16):  
                Projectile.gfx[ i % 4 ] [ int(i/4) ] = subsprite(tileset, i*8, 0, (i*8)+8, 16)
            deletesprite(tileset)            
            Projectile.initialized = True
    
    @staticmethod
    def destroy():
        if len(Projectile.gfx) > 0:
            for o in range(0,16):
                if Projectile.gfx[o % 4][int(o/4)]:
                    deletesprite(Projectile.gfx[o % 4][int(o/4)])
                    Projectile.gfx[o % 4][int(o/4)] = None
            Projectile.gfx = {}
        Projectile.initialized = False

    def __init__(self, x, y, projectile_type=TYPE_A, level=0):
        self.x = x
        self.y = y
        self.projectile_type = projectile_type
        self.delta_x = 0.0
        self.delta_y = -1.5
        self.alive = True
        self.angle = 0.0
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
        self.angle = atan2(-self.delta_x, -self.delta_y)*-1
        if self.delta_x > -.1 and self.delta_x < .1 and self.delta_y > -.1 and self.delta_y < .1: self.alive = False
            
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
        drawsprite(Projectile.gfx[self.projectile_type][self.level], int(self.x), int(self.y), self.angle)
        