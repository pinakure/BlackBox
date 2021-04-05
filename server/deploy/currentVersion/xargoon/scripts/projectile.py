from vpu        import *
from debug      import debug, error, panic
from random     import random
from math       import atan2

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
        debug("Projectile", "Initializing")
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

    def __init__(self, x, y, projectile_type=TYPE_A, level=0, owner=None):
        self.x = x
        self.y = y
        self.projectile_type = projectile_type
        self.delta_x = 0.0
        self.delta_y = -1.5
        self.alive = True
        self.owner = owner
        self.angle = 0.0
        self.level = level
        
    def __del__(self):
        pass

    def check_ship_collision(self):
        # return not None if projectile collides with an (alive) ship
        # set bounding box coordinates 
        target = self.game.ship if self.game.ship.shield.energy <= 0 else self.game.ship.shield
        if not self.game.ship.alive: return None
        tw = target.width - 12
        th = target.height - 12
        left  = int(target.x) - (tw>>1)
        top   = int(target.y) - (tw>>1)
        right = left + tw
        bottom= top  + th
        if self.x >= left and self.x <= right:
            if self.y >= top and self.y <= bottom:
                return self.game.ship
        return None
    
    def check_foe_collision(self):
        # return not None if projectile collides with an (alive) foe
        for foe in self.game.foes:
            if not foe.alive: continue
            # set bounding box coordinates 
            left  = int(foe.x) - (foe.width>>1)
            top   = int(foe.y) - (foe.height>>1)
            right = left + foe.width
            bottom= top  + foe.height
            if self.x >= left and self.x <= right:
                if self.y >= top and self.y <= bottom:
                    return foe
        return None
        
    def update(self, delta):
        from data.scripts.foe               import Foe
        from data.scripts.ship              import Ship
        if not self.alive: return
        self.x += self.delta_x
        self.y += self.delta_y
        if self.x <= 0: self.alive = False
        elif self.y <= 0: self.alive = False
        elif self.x > int(Projectile.game.width ): self.alive = False
        elif self.y > int(Projectile.game.height): self.alive = False
        self.angle = atan2(-self.delta_x, -self.delta_y)*-1
        if self.delta_x > -.1 and self.delta_x < .1 and self.delta_y > -.1 and self.delta_y < .1: self.alive = False
        if self.owner == Foe: target = self.check_ship_collision()
        elif self.owner == Ship: target = self.check_foe_collision()
        if target is not None:
            self.alive = False
            target.receive_impact(self.x, self.y)
            
    @staticmethod
    def spawn(x,y,projectile_type,level,delta_x=0.0, delta_y=-1.5, owner=None):
        for projectile in Projectile.game.projectiles:
            if projectile.owner != owner: continue
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
        