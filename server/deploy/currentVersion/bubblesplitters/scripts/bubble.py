from random import random
from entity import *
import vpu

class Bubble(Entity):
    game        = None
    pool        = []
    sprites     = {}
    gfx         = {}
    sizes       = [ 8, 16, 32, 40]
    quantities  = [ 128, 64, 32, 16]
    initialized = False

    @staticmethod
    def spawn(x, y, size,delta):
        for bubble in Bubble.pool:
            if bubble.size == size and not bubble.enabled:
                bubble.x = x
                bubble.y = y
                bubble.time   = 0
                bubble.enable()
                vpu.updateanim(bubble.animation, 0)
                bubble.setposition(x,y)
                bubble.controllers[EntityController.CONTROLLER_BOUNCE].set_delta(0.25*delta,0.01)
                bubble.controllers[EntityController.CONTROLLER_MOVE].set_delta(0.25*delta,0.01)
                bubble.explode_frame = 0
                bubble.explode = False
                return

    @staticmethod
    def initialize(game):
        Bubble.game = game
        Bubble.sprites[0]= vpu.createsprite(f'bubble8')
        Bubble.sprites[1]= vpu.createsprite(f'bubble16')
        Bubble.sprites[2]= vpu.createsprite(f'bubble32')
        Bubble.sprites[3]= vpu.createsprite(f'bubble40')
        for i in range(0,4):
            Bubble.gfx[i] = vpu.createanim(
                Bubble.sizes[i], Bubble.sizes[i], 
                Bubble.sprites[i], 
                0, 0, 3, 0, 
                False, 0.0, False
            )
        
        Bubble.initialized = True
        Bubble.preallocate()

    @staticmethod 
    def preallocate():
        Bubble.pool=[]        
        for size in range(0,4):
            for i in range(0, Bubble.quantities[size]):
                Bubble.pool.append(Bubble(size))
                Bubble.pool[len(Bubble.pool)-1].setposition(
                    ((Bubble.game.dims[1][0]/2)-160)+(Bubble.sizes[size]>>1)+((float(i)/float(Bubble.quantities[size]))*float(320.0-(Bubble.sizes[size]>>1))),
                    ((Bubble.game.dims[1][1]/2)-120)+(Bubble.sizes[size]>>1)+((float(i)/float(Bubble.quantities[size]))*float(100.0))
                )
                Bubble.pool[i].disable()
        for i in range(1,Bubble.quantities[3]):
            Bubble.pool[len(Bubble.pool)-i].enable()
        

    def __init__(self, bubble_size=0):
        self.size   = bubble_size%4
        self.time   = len(Bubble.pool)*16
        self.width  = Bubble.sizes[self.size]
        self.height = Bubble.sizes[self.size]
        Entity.__init__(self, Bubble.game, self.width, self.height, f"Bubble({self.size})")
        #self.gfx    = Bubble.gfx[self.size]
        self.gfx    = vpu.createanim(self.width, self.height, Bubble.sprites[self.size], 0, 0, 3, 0, False, 0.0, False)
        if not self.gfx:
            print("Cannot create bubble animation")
        self.setanimation(self.gfx)
        self.addcontroller(EntityController.CONTROLLER_MOVE)
        self.addcontroller(EntityController.CONTROLLER_BOUNCE)
        self.explode_frame = 0
        self.explode = False
        self.disable()
        
    def update(self, delta):
        if self.explode: 
            self.pang()

    def pang(self):
        self.explode = True
        if self.explode_frame < 19:
            self.explode_frame+=1
            vpu.updateanim(self.animation, int(self.explode_frame/5))
        else:
            if self.size == 0: 
                # choose whether to drop an item or not
                self.disable()
                return
            
            self.time = 0
            vpu.updateanim(self.animation, 0)
            self.explode_frame = 0
            self.explode = False
            self.disable()
            s = Bubble.sizes[self.size]
            self.x, self.y = entitygetpos(self.handle)
            Bubble.spawn(self.x-(s>>5), self.y, self.size-1, -1.0)
            Bubble.spawn(self.x+(s>>5), self.y, self.size-1, 1.0)
        

        

        

