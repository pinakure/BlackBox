from vpu import *
from data.scripts.flame import Flame
from data.scripts.projectile import Projectile
from random import random

class Ship:
    TYPE_A = 0x01
    TYPE_B = 0x02
    width = 32
    height = 32
    # ------------- #
    game = None
    timer = 0
    tileset = None
    initialized = False

    @staticmethod
    def initialize(game):
        Ship.game = game
        print("Initializing Ship...")
        Ship.tileset = createsprite("ship01",10)
        Ship.initialized = True
        Ship.timer = 0

    @staticmethod
    def destroy():
        if Ship.tileset:
            deletesprite(Ship.tileset)
            Ship.tileset = None
        Ship.initialized = False

    def __init__(self,x=0, y=0, ship_type=TYPE_A):
        self.x = x
        self.y = y
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
        self.anim = createanim(Ship.width, Ship.height, Ship.tileset, 0, 0, 3, 0, False, 0.125)
        self.flames =  [
            Flame(self.x, self.y + Ship.height),
            Flame(self.x, self.y + Ship.height),
        ]
        
    def __del__(self):
        if self.anim: deleteanim(self.anim)
        self.anim = None
        if len(self.flames)>0: 
            self.flames = []

    def move(self):
        left = 480
        right = 800-Ship.width
        top = 360
        bottom = 600-Ship.height
        self.x += self.delta_x
        self.y += self.delta_y
        self.delta_x = -.125 if self.x > right  else .125 if self.x < left else self.delta_x
        self.delta_y = -.125 if self.y > bottom else .125 if self.y < top  else self.delta_y
        self.x = right  if self.x > right   else left if self.x < left else self.x
        self.y = bottom if self.y > bottom  else top  if self.y < top  else self.y
        
    def randomize(self):
        if int(random() * 100)<50: self.west()
        else: self.east()
        if int(random() * 100)<50: self.north()
        else: self.south()        

    def update_flame(self):
        self.flames[0].x = int(self.x) - (Ship.width>>1)
        self.flames[0].y = int(self.y) + (Ship.height>>1)- 2
        self.flames[1].x = int(self.x) + (Ship.width>>1)- 8
        self.flames[1].y = int(self.y) + (Ship.height>>1)- 2
        #select flame frame depending on vertical velocity
        if self.delta_y < -1.5: 
            self.flames[0].flame_type = Flame.TYPE_D
            self.flames[1].flame_type = Flame.TYPE_D 
        elif self.delta_y < -0.75: 
            self.flames[0].flame_type = Flame.TYPE_C
            self.flames[1].flame_type = Flame.TYPE_C 
        elif self.delta_y < 0: 
            self.flames[0].flame_type = Flame.TYPE_B
            self.flames[1].flame_type = Flame.TYPE_B 
        else:
            self.flames[0].flame_type = Flame.TYPE_A
            self.flames[1].flame_type = Flame.TYPE_A 
        
    def update(self, delta):
        Ship.timer += 1
        if not self.alive: return
        
        self.randomize()
        self.move()
        self.update_flame()
        
        #divide operator to get rapid fire bonus        
        if Ship.timer % (64 >> self.rapid_fire) == 0: self.shooting = True
                
        if self.shooting:
            self.weapon_type = int(random()*Projectile.TYPE_MAX)
            self.weapon_level = int(random()*4)%4
            delta_y = -1.5 + (self.delta_y) if self.delta_y < 0 else -1.5
            delta_x = .05
            Projectile.spawn(self.x+2-(Ship.width>>1), self.y-2, self.weapon_type, self.weapon_level,  delta_x , delta_y)
            Projectile.spawn(self.x+(Ship.width>>1)-2, self.y-2, self.weapon_type, self.weapon_level, -delta_x , delta_y)
            self.shooting = False
    
    def west(self):
        self.delta_x -= .125
        self.delta_x = -2.0 if self.delta_x < -2.0 else self.delta_x
        
    def east(self):
        self.delta_x += .125
        self.delta_x = 2.0 if self.delta_x > 2.0 else self.delta_x

    def north(self):
        self.delta_y -= .125
        self.delta_y = -2.0 if self.delta_y < -2.0 else self.delta_y

    def south(self):
        self.delta_y += .125
        self.delta_y = 2.0 if self.delta_y > 2.0 else self.delta_y       

    def draw(self):
        drawanim(self.anim, int(self.x)-(Ship.width>>1), int(self.y)-(Ship.height>>1))
        self.flames[0].draw()
        self.flames[1].draw()
            