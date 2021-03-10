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
    weapon_speed = [ .125   , .250  , .80  , 2     ]
    weapon_level = [ 2      , 2     , 0     , 2     ]
    weapon_rate  = [ 2048   , 2048  , 512   , 1024  ]
        

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
        self.foe_type         = foe_type
        self.x                = x
        self.y                = y
        self.delta_x          = 0
        self.delta_y          = 0
        self.angle            = 0
        self.shooting         = False
        self.rapid_fire       = 4
        self.alive            = True
        self.target           = None
        self.weapon_type      = Projectile.TYPE_C
        self.projectile_speed = Foe.weapon_speed[ self.weapon_type ]
        self.weapon_level     = Foe.weapon_level[ self.weapon_type ]
        self.shoot_rate       = Foe.weapon_rate[ self.weapon_type ]
        self.burst            = 3
        self.current_burst    = 3
        self.timer            = int(random() * self.shoot_rate)

    def update(self, delta):
        if not self.alive: return
        rx = int(self.x) >> 3  # / 8
        ry = int(self.y) >> 4  # / 16
        for projectile in Foe.game.projectiles:
            if projectile.owner == Foe: continue
            if (int(projectile.x)>>3 == rx):
                if (int(projectile.y)>>4 == ry):
                    projectile.alive = False
                    Foe.game.score += 1
                    self.die()
                    return
        self.find_target()
        self.x += self.delta_x
        self.y += self.delta_y
        if self.target is not None:
            self.face_target()
            self.attack()
            self.pursuit()
            #self.wander()
            #self.flee()
            
    def spawn(self):
        self.x = int(random() * Foe.game.dims[1][0] ) - 16
        self.y = int(random() * Foe.game.dims[1][1] ) - 16
        self.foe_type = int(random()*(Foe.TYPE_MAX))
        self.current_burst = 3
        self.timer = int(random() * self.shoot_rate)
        self.alive = True
        
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
        x1 = min(self.target.x, self.x)
        x2 = max(self.target.x, self.x)
        y1 = min(self.target.y, self.y)
        y2 = max(self.target.y, self.y)
        distance = int((x2-x1)+(y2-y1))
        if distance < 160:
            if (self.timer % (self.shoot_rate >> self.rapid_fire)==0):
                distance *= self.projectile_speed
                if distance == 0: distance = 1 # avoid zero division!
                dx = int(self.target.x - self.x)
                dy = int(self.target.y - self.y)
                w = Foe.width>>1
                Projectile.spawn(self.x, self.y, self.weapon_type, self.weapon_level,  dx/distance , dy/distance, Foe)

    def die(self):
        self.alive = False
        #spawn explosion
        for x in Foe.game.explosions:
            if x.alive: continue
            x.alive = True
            x.time = 0
            x.x = int(self.x)-(Foe.width>>1)
            x.y = int(self.y)-(Foe.height>>1)
            break