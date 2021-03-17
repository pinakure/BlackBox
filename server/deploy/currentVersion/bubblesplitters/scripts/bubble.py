from entity import *
import vpu

class Bubble(Entity):
    game    = None
    sprites = {}
    # gfx     = []
    sizes   = [ 8, 16, 32, 40]
    initialized = False
    
    @staticmethod
    def initialize(game):
        Bubble.game = game
        Bubble.sprites[0]= vpu.createsprite(f'bubble8')
        Bubble.sprites[1]= vpu.createsprite(f'bubble16')
        Bubble.sprites[2]= vpu.createsprite(f'bubble32')
        Bubble.sprites[3]= vpu.createsprite(f'bubble40')
        # Bubble.gfx.append(vpu.createanim(Bubble.sprites[0], 8, 8, 0, 0, 3, 0))
        # Bubble.gfx.append(vpu.createanim(Bubble.sprites[1], 8, 8, 0, 0, 3, 0))
        # Bubble.gfx.append(vpu.createanim(Bubble.sprites[2], 8, 8, 0, 0, 3, 0))
        # Bubble.gfx.append(vpu.createanim(Bubble.sprites[3], 8, 8, 0, 0, 3, 0))
        Bubble.initialized = True

    def __init__(self, bubble_size=0):
        self.size   = bubble_size%4
        self.width  = Bubble.sizes[bubble_size]
        self.height = Bubble.sizes[bubble_size]
        Entity.__init__(self, Bubble.game, self.width, self.height, f"Bubble({bubble_size})")
        self.gfx    = vpu.createanim(self.width, self.height, Bubble.sprites[bubble_size], 0, 0, 3, 0, False, 0.0, False)
        if not self.gfx:
            print("Cannot create bubble animation")
        self.setanimation(self.gfx)
        self.addcontroller(EntityController.CONTROLLER_MOVE)
        self.addcontroller(EntityController.CONTROLLER_BOUNCE)
        self.enable()
        
    def update(self, delta):
        Entity.update(self, delta)

    
        

