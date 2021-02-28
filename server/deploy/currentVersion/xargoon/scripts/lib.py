import blackbox
import vpu

class HudIcon:
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03
    LEVEL_0 = [ 0x00, 0x00 ]
    LEVEL_1 = [ 0x04, 0x00 ]
    LEVEL_2 = [ 0x00, 0x01 ]
    LEVEL_3 = [ 0x04, 0x01 ]
    type = HudIcon.TYPE_A
    tileset = None
    image = None
    x = 0
    y = 0
    @staticmethod
    def initialize:
        #load sprite
        HudIcon.tileset = vpu.createsprite("hud")
        HudIcon.image = vpu.createanim(HudIcon.tileset, 16, 16)
        type = HudIcon.TYPE_A

    @staticmethod
    def setType(type):
        HudIcon.type = type
    
    @staticmethod
    def draw():
        vpu.drawanim(HudIcon.image)
        
class BigExplosion:
    tileset = None
    initialized = False
    
    @staticmethod
    def initialize():
        BigExplosion.tileset = vpu.createsprite("explosion_big")
        BigExplosion.initialized = True
        
    def __init__(self, x, y):
        if not BigExplosion.initialized:
            BigExplosion.initialize()
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = vpu.createanim(BigExplosion.tileset, 32,32)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, x, y)
        if self.time > 16:
            self.alive = True

class SmallExplosion:
    TYPE_A = 0x00
    TYPE_B = 0x01    
    tileset = None
    initialized = False

    @staticmethod
    def initialize():
        SmallExplosion.tileset = vpu.createsprite("explosion_small")
        SmallExplosion.initialized = True

    def __init__(self, x=0, y=0, type=SmallExplosion.TYPE_A):
        if not SmallExplosion.initialized:
            SmallExplosion.initialize()
        self.type = type
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = vpu.createanim(SmallExplosion.tileset, 16, 16)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, x, y)
        if self.time > 16:
            self.alive = True
    
class Flame:
    TYPE_A = 0x00
    TYPE_B = 0x04
    TYPE_C = 0x08
    TYPE_D = 0x0C
    
class Projectile:
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03
    
class Token:
    TYPE_A      = 0x00
    TYPE_B      = 0x01
    TYPE_C      = 0x02
    TYPE_D      = 0x03
    TYPE_BOMB   = 0x04
    TYPE_POWERUP= 0x05
    TYPE_BONUS  = 0x06
    tileset = None
    initialized = False

    @staticmethod
    def initialize():
        Token.tileset = vpu.createsprite("tokens")
        Token.initialized = True
        
    def __init__(self, x=0, y=0, type=Token.TYPE_A):
        if not Token.initialized:
            Token.initialize()
        self.x = x
        self.y = y
        self.alive = True
        self.type = type
        self.anim = vpu.createanim(Token.tileset, 16, 16)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, x, y)
        if self.time > 16:
            self.alive = True

class Ship:
    TYPE_A = 0x01
    TYPE_B = 0x02
    tileset = None
    initialized = False

    @staticmethod
    def initialize():
        Ship.tileset = vpu.createsprite("ship")
        Ship.initialized = True
    
    def __init__(self,x=0, y=0, type=Ship.TYPE_A):
        if not Ship.initialized:
            Ship.initialize()
        self.x = x
        self.y = y
        self.type = type
        self.status = 0
        self.shooting = False
        self.anim = vpu.createanim(Ship.tileset, 16, 16)

    def draw(self):
        vpu.drawsprite(self.sprite, self.x, self.y)