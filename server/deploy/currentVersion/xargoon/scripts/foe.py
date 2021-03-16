from vpu import *
from math import atan2
from random import random
from data.scripts.projectile import Projectile

class RandomPoint:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y
        self.alive = True

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
        if Foe.initialized:
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
        self.thrust           = 1.0
        self.angle            = 0
        self.shooting         = False
        self.rapid_fire       = 4
        self.alive            = True
        self.target           = None
        self.point            = RandomPoint(self.x, self.y)
        self.weapon_type      = foe_type%3
        self.projectile_speed = Foe.weapon_speed[ self.weapon_type ]
        self.weapon_level     = Foe.weapon_level[ self.weapon_type ]
        self.shoot_rate       = Foe.weapon_rate[ self.weapon_type ]
        self.burst            = 3
        self.current_burst    = 3
        self.energy           = 1
        self.path             = []
        self.timer            = int(random() * self.shoot_rate)

    def generate_random_path(self):
        min_steps = 2
        max_steps = 32
        x = self.x
        y = self.y
        alt_x = False
        alt_y = False
        mult = 8
        target = self.game.ship if self.game.ship.alive else self.point  
        delta_x = 0#-(x - target.x) / 10
        delta_y = 0#-(y - target.y) / 10
        for i in range(min_steps, max_steps):
            # new code
            if alt_x: delta_x += (mult+(random()*mult)) * int((random()*3)-1)
            else: delta_x += -(x - target.x) / 10
            if alt_y: delta_y += (mult+(random()*mult)) * int((random()*3)-1)        
            else: delta_y += -(y - target.y) / 10
            alt_x = alt_x ^ 1
            alt_y = alt_y ^ 1
            # old code 
            # delta_x = ((random()*100)/100)*(-1 if int(random()*2)%2==0 else 1)
            # delta_y = ((random()*100)/100)*(-1 if int(random()*2)%2==0 else 1)
            x = x + (delta_x*.025)
            y = y + (delta_y*.025)
            ship = self.game.ship
            left  = ship.x - (ship.width)
            right = ship.x + (ship.width)
            top   = ship.y - (ship.height)
            bottom= ship.y + (ship.height)
            if   x >= left   and x <= ship.x: x +=self.game.ship.width*3
            elif x <= right  and x >= ship.x: x -=self.game.ship.width*3
            if   y >= top    and y <= ship.y: y +=self.game.ship.height*3
            elif y <= bottom and y >= ship.y: y -=self.game.ship.height*3
            
            if x < 0: x+=64
            elif x > self.game.dims[1][0]: x-=64
            if y < 0: y+=64
            elif y > self.game.dims[1][1]: y-=64
            self.path.append([int(x), int(y)])
        
    def draw_path(self):
        ox = self.target.x if self.target is not None else None
        oy = self.target.y if self.target is not None else None
        if ox is None:return
        if oy is None:return
        for point in self.path:#[::-1]:
            setcolor(64,64,64,8)
            line(int(ox),int(oy),int(point[0]), int(point[1]))
            setcolor(200,200,0,8)
            ox = point[0]
            oy = point[1]
            pset(int(ox), int(oy))
            

    def update(self, delta):
        if not self.alive: return
        rx = int(self.x) >> 3  # / 8
        ry = int(self.y) >> 4  # / 16
        self.thrust += 0.00125
        #check collision with the ship
        if self.game.ship.alive:
            target = self.game.ship if self.game.ship.shield.energy <= 0 else self.game.ship.shield
            left  = int(target.x) - (target.width>>1)
            top   = int(target.y) - (target.height>>1)
            right = left + target.width
            bottom= top  + target.height
            if self.x >= left and self.x <= right:
                    if self.y >= top and self.y <= bottom:
                        self.receive_impact(self.x, self.y, True)
            
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
        self.weapon_type      = self.foe_type%3
        self.projectile_speed = Foe.weapon_speed[ self.weapon_type ]
        self.weapon_level     = Foe.weapon_level[ self.weapon_type ]
        self.shoot_rate       = Foe.weapon_rate[ self.weapon_type ]        
        self.current_burst = 3
        self.path = []
        self.thrust = 1.0
        self.generate_random_path()
        self.timer = int(random() * self.shoot_rate)
        self.alive = True
        
    def draw(self):
        if not self.alive: return
        if self.target is not None:            
            setcolor(255,0,0,8)
        #    line(int(self.x), int(self.y), int(self.target.x), int(self.target.y))
        #self.draw_path()
        drawsprite(Foe.gfx[self.foe_type], int(self.x), int(self.y), self.angle)
        
    def face_target(self):
        dx = int(self.target.x - self.x)
        dy = int(self.target.y - self.y)
        self.angle = atan2(dx, dy)*-1        
        
    def next_target(self):
        if len(self.path)==0:
            self.generate_random_path()

        self.point.x = self.path[0][0]
        self.point.y = self.path[0][1]
        del self.path[0]
        return self.point
        
    def find_target(self):
        if not self.target:
            self.target = self.next_target()
        
    def wander(self):
        pass

    def pursuit(self):
        acceleration = True
        dx = int(self.target.x - self.x)
        dy = int(self.target.y - self.y)
        x1 = min(self.target.x, self.x)
        x2 = max(self.target.x, self.x)
        y1 = min(self.target.y, self.y)
        y2 = max(self.target.y, self.y)
        if acceleration:
            self.delta_x *= 0.9925
            self.delta_y *= 0.9925
            distance = int((x2-x1)+(y2-y1))*128
            self.delta_x += (dx / (1 if distance==0 else distance))*self.thrust
            self.delta_y += (dy / (1 if distance==0 else distance))*self.thrust
            if distance <= 48*128: 
                self.flee()
        else:
            distance = int((x2-x1)+(y2-y1))
            self.delta_x = (dx / (1 if distance==0 else distance)) *self.thrust
            self.delta_y = (dy / (1 if distance==0 else distance)) *self.thrust
            if distance <= 48: 
                self.flee()
            
    def flee(self):
        self.target = None
        
    def attack(self):
        self.timer += 1
        if self.game.ship.alive: target = self.game.ship
        else: target = self.target
        x1 = min(target.x, self.x)
        x2 = max(target.x, self.x)
        y1 = min(target.y, self.y)
        y2 = max(target.y, self.y)
        distance = int((x2-x1)+(y2-y1))
        if distance < 320:
            if (self.timer % (self.shoot_rate >> self.rapid_fire)==0):
                distance *= self.projectile_speed
                if distance == 0: distance = 1 # avoid zero division!
                dx = int(target.x - self.x)
                dy = int(target.y - self.y)
                w = Foe.width>>1
                Projectile.spawn(self.x, self.y, self.weapon_type, self.weapon_level,  dx/distance , dy/distance, Foe)

    def receive_impact(self, x, y, energy_explosion=False):
        #if self.shield.energy > 0:
        #    self.shield.sync_position( self )
        #    self.shield.activate(x, y)
        #else:
        self.damage(1, energy_explosion)

    def damage(self, pain=1, energy_explosion=False):
        self.energy -= 1
        if self.energy <= 0:
            self.die(energy_explosion)

    def die(self, energy_explosion=False):
        self.alive = False
        #spawn explosion
        for x in Foe.game.explosions:
            if x.alive: continue
            x.spawn(int(self.x)-(Foe.width>>1), int(self.y)-(Foe.height>>1), energy_explosion)                
            Foe.game.score += 1           
            break
        if Foe.game.score % 5==1: 
            for t in Foe.game.tokens:
                if t.alive: continue
                t.spawn(int(self.x)-(Foe.width>>1), int(self.y)-(Foe.height>>1))                
                break
        
            