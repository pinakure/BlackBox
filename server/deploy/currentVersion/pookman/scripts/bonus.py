import          vpu
from entity     import Entity
from animation  import *
from random     import random

class Bonus(Entity):
    sprite      = None
    game        = None
    gfx         = {}
    x           = 13*8
    y           = (17*8)-4
    initialized = True
    bonus_type  = 0
    tx          = 0
    ty          = 0
    score       = 1000        
    isAlive     = False
    eaten       = False
    timeout     = 100

    @staticmethod
    def initialize(game):
        Bonus.game = game
        Entity.__init__(Bonus, Bonus.game)
        Bonus.sprite = vpu.createsprite("bonus")
        if Bonus.sprite:
            Bonus.initialized = True
            Bonus.gfx = {}
            i=0
            for y in range(0,32):
                for x in range(0,8):
                    Bonus.gfx[i] = Animation(16,16, Bonus.sprite, x, y, x, y, False, 0, False)
                    i+=1
        else: 
            print('Failed to load bonus sprite sheet')
            Bonus.initialized = False

        Bonus.bonus_type = 0
        Bonus.game       = game
        Bonus.tx         = int( Bonus.x / 8 )
        Bonus.ty         = int( Bonus.y / 8 )
        Bonus.score      = 1000        
        Bonus.isAlive    = False
        Bonus.eaten      = False
        Bonus.timeout    = 100
        Bonus.reset()
    
    @staticmethod
    def destroy():
        pass
    
    @staticmethod
    def update(delta=1.0):
        if Bonus.eaten: 
            return
        if Bonus.isAlive:
            if Bonus.timeout: Bonus.timeout-=1
            else: Bonus.isAlive = True
            return
        if (Bonus.game.pacman.ty == Bonus.ty) and ( (Bonus.game.pacman.tx == Bonus.tx) or (Bonus.game.pacman.tx == Bonus.tx+1) ):
            Bonus.game.score += Bonus.score
            Bonus.eaten        = True
            Bonus.isAlive    = False

    @staticmethod
    def draw(i, x, y):
        Bonus.gfx[i].draw(x, y)

    @staticmethod
    def reset():
        Bonus.x          = Bonus.x
        Bonus.y          = Bonus.y
        Bonus.tx         = 13
        Bonus.ty         = 17
        Bonus.score      = 400 + (100* (Bonus.game.level + 1))
        Bonus.isAlive    = False
        Bonus.eaten      = False
        Bonus.timeout    = 400 + (50 * (Bonus.game.level+1)) + int(random()*50)