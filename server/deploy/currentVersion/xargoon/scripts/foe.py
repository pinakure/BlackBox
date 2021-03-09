from vpu import *

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
        self.alive = True
        self.target = None

    def update(self, delta):
        if not self.alive: return
        self.target = Foe.game.ship

    def draw(self):
        if not self.alive: return
        drawsprite(Foe.gfx[self.foe_type], int(self.x)-(Foe.width>>1), int(self.y)-(Foe.height>>1))
        if self.target is not None:
            setcolor(255,0,0,128)
            line(int(self.x), int(self.y), int(self.target.x), int(self.target.y))
        
    def find_target(self):
        pass

    def wander(self):
        pass

    def flee(self):
        pass

    def attack(self):
        pass

    def die(self):
        pass