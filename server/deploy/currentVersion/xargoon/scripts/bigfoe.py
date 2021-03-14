from vpu import *

class BigFoe:
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03
    game = None
    tileset = None
    gfx = {}
    width  = 32
    height = 32

    @staticmethod
    def initialize(game):
        BigFoe.game = game
        BigFoe.initialized = True

    @staticmethod
    def destroy():
        BigFoe.initialized = False    

    def __init__(self, x, y, foe_type=TYPE_A):
        self.foe_type = foe_type
        self.x = x
        self.y = y
        self.energy = 16
        self.alive = True
        self.target = None

    def update(self, delta):
        if not self.alive: return
        pass

    def draw(self):
        if not self.alive: return
        pass

    def find_target(self):
        pass

    def wander(self):
        pass

    def flee(self):
        pass

    def attack(self):
        pass

    def receive_impact(self, x, y):
        #if self.shield.energy > 0:
        #    self.shield.sync_position( self )
        #    self.shield.activate(x, y)
        #else:
        self.damage()

    def damage(self, pain=1):
        self.energy -= 1
        if self.energy <= 0:
            self.die()

    def die(self):
        self.alive = False
        #spawn explosion
        for x in BigFoe.game.explosions:
            if x.alive: continue
            x.spawn(int(self.x)-(BigFoe.width>>1),int(self.y)-(BigFoe.height>>1))
            break