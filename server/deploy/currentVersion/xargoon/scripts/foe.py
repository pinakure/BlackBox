from vpu import *
from math import atan2
from random import random
from data.scripts.projectile import Projectile
         
class Foe:
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03
    TYPE_E = 0x04
    TYPE_F = 0x05
    TYPE_G = 0x06
    TYPE_H = 0x07
    TYPE_MAX = 0x08
    game = None
    tileset = None
    gfx = {}
    width = 16
    height = 16
    initialized = False

    @staticmethod
    def initialize(game):
        Foe.game = game
        sprite = createsprite("foe0",10)
        for i in range(0x00, 0x08):
            Foe.gfx[i] = subsprite(sprite, i*Foe.width,  0, (i*Foe.width)+Foe.width, Foe.height)
        deletesprite(sprite)
        Foe.initialized = True

    @staticmethod 
    def destroy():
        for i in range(0x00, 0x08):
            deletesprite(Foe.gfx[i])
        Foe.gfx = {}
        Foe.initialized = False


    def __init__(self, x, y, foe_type=TYPE_A):
        self.foe_type = foe_type
        self.x = x
        self.y = y
        self.delta_x = 0
        self.delta_y = 0
        self.angle = 0
        self.shooting = False
        self.rapid_fire = 4
        self.alive = True
        self.projectile_speed = .250
        self.weapon_type = Projectile.TYPE_B
        self.weapon_level = 2
        self.burst = 3
        self.current_burst = 3
        self.target = None
        self.shoot_rate = 2048
        self.timer = int(random() * self.shoot_rate)

    def update(self, delta):
        if not self.alive: return
        self.find_target()
        self.x += self.delta_x
        self.y += self.delta_y
        if self.target is not None:
            self.face_target()
            self.attack()
            self.pursuit()
            #self.wander()
            #self.flee()
            
    def spawn(self, x, y, foe_type):
        pass
        
    def draw(self):
        if not self.alive: return
        if self.target is not None:            
            setcolor(255,0,0,8)
            #line(int(self.x), int(self.y), int(self.target.x), int(self.target.y))
        drawsprite(Foe.gfx[self.foe_type], int(self.x), int(self.y), self.angle)
        
    def face_target(self):
        dx = int(self.target.x - self.x)
        dy = int(self.target.y - self.y)
        self.angle = atan2(dx, dy)*-1        
        
    def find_target(self):
        self.target = Foe.game.ship

    def wander(self):
        pass

    def pursuit(self):
        dx = int(self.target.x - self.x)
        dy = int(self.target.y - self.y)
        x1 = min(self.target.x, self.x)
        x2 = max(self.target.x, self.x)
        y1 = min(self.target.y, self.y)
        y2 = max(self.target.y, self.y)
        #distance = int((x2-x1)+(y2-y1))*256
        #self.delta_x += dx / (1 if distance==0 else distance)
        #self.delta_y += dy / (1 if distance==0 else distance)
        distance = int((x2-x1)+(y2-y1))
        self.delta_x = dx / (1 if distance==0 else distance)
        self.delta_y = dy / (1 if distance==0 else distance)

    def flee(self):
        pass

    def attack(self):
        self.timer += 1
        if (self.timer % (self.shoot_rate >> self.rapid_fire)==0):
            dx = int(self.target.x - self.x)
            dy = int(self.target.y - self.y)
            w = Foe.width>>1
            x1 = min(self.target.x, self.x)
            x2 = max(self.target.x, self.x)
            y1 = min(self.target.y, self.y)
            y2 = max(self.target.y, self.y)
            distance = int((x2-x1)+(y2-y1))*self.projectile_speed
            if distance == 0: distance = 1 # avoid zero division!
            Projectile.spawn(self.x, self.y, self.weapon_type, self.weapon_level,  dx/distance , dy/distance)

    def die(self):
        pass