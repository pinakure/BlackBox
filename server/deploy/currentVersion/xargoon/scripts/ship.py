from vpu import *
from data.scripts.flame import Flame
from data.scripts.projectile import Projectile
from random import random

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

    def __init__(self,x=0, y=0, ship_type=TYPE_A):
        self.x = x
        self.y = y
        self.w = 32
        self.h = 32
        self.ship_type = ship_type
        self.status = 0
        self.alive = True
        self.delta_x = 0
        self.delta_y = 0
        self.thrust = False
        self.shooting = False
        self.rapid_fire = 3
        self.weapon_type = int(random()*Projectile.TYPE_MAX)
        self.weapon_level = int(random()*4)
        self.anim = createanim(self.w, self.h, Ship.tileset, 0, 0, 3, 1, False, 0.125)
        self.flames =  [
            Flame(self.x, self.y + self.h),
            Flame(self.x, self.y + self.h),
        ]
        
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
        if len(self.flames)>0: 
            self.flames = []

    def update(self, delta):
        if not self.alive: return
        self.x += self.delta_x
        self.y += self.delta_y
        self.delta_x += .125 if int(random() * 100)<50 else -.125        
        if self.x > 800-self.w:self.delta_x = -0.1
        if self.x < 480:self.delta_x = 0.1
        self.flames[0].x = self.x
        self.flames[0].y = self.y + self.h - 2
        self.flames[1].x = self.x + self.w - 8 
        self.flames[1].y = self.y + self.h - 2
        #divide operator to get rapid fire bonus        
        if frames() % (64 >> self.rapid_fire) == 0: self.shooting = True
                
        if self.shooting:
            self.weapon_type = int(random()*Projectile.TYPE_MAX)
            self.weapon_level = int(random()*4)%4
            Projectile.spawn(self.x + 4 , self.y+12, self.weapon_type, self.weapon_level)
            Projectile.spawn(self.x + 28, self.y+12, self.weapon_type, self.weapon_level)
            self.shooting = False
        
    def draw(self):
        drawanim(self.anim, int(self.x), int(self.y))
        #self.flames[0].draw()
        #self.flames[1].draw()
            