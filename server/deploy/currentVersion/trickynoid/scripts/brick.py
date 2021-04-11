from data.scripts.token         import Token
from data.scripts.bricktype     import BrickType
from debug                      import debug, panic, error, exception, deprecate

class Brick:

    types  = {}
    width  = 16
    height = 8    
        
    def __init__(self, x, y, brick_type, token):
        self.offset = 0
        self.isFalling = False
        self.brick_type = brick_type
        self.damage = 0
        self.x = x
        self.y = y
        self.token = token
        self.offset = 0
        self.setType(brick_type)
        self.variation = 0
        self.isFalling = False
       
    def getOffset(self):        return self.offset
    def causesBounce(self):     return Brick.types[self.brick_type].causesBounce
    def isDestructable(self):   return Brick.types[self.brick_type].destructable
    def getX(self):             return self.x
    def getY(self):             return self.y
    def setX(self, x):          self.x = x
    def setY(self, y):          self.y = y
    def isSticked(self):        return Brick.types[self.brick_type].isSticked
    def isInverter(self):       return Brick.types[self.brick_type].isInverter    
    def getType(self):          return self.brick_type
    def getGraph(self):         return Brick.types[self.brick_type].graphic + self.variation
    def getScore(self):         return Brick.types[self.brick_type].score
    def getDamage(self):        return self.damage
    def getHealth(self):        return Brick.types[self.brick_type].health
    def getTokenType(self):     return self.token
    def isAlive(self):          return self.brick_type != BrickType.NONE
    def hasToken(self):         return self.token > 0x00
    
    def applyDamage(self, appliedDamage):
        self.damage += appliedDamage
        if self.damage > Brick.types[self.brick_type].health: self.damage = 9
    
    def kill(self):
        self.variation = 0
        self.brick_type = BrickType.NONE
    
    def fall(self):
        return
        # self.isFalling = True        
        if self.offset < 8: self.offset += 1
        else: self.y += 1; self.offset = 0

    def generateToken(self):
        t = Token(self.token, self.x*16, self.y*8)
        debug("Brick", f"Generating token type: {t.getType()}")
        return t
    
    def test(self, x, y):
        bx = (self.x * Brick.width)
        by = (self.y * Brick.height) + self.offset
        return (self.x >= bx) and (self.x <  bx + Brick.width ) and (self.y >= by) and (self.y <  by + Brick.height)
    
    def setType(self, brick_type):
        # Note: the type sent by here is the GRAPHIC representation used, so real type must be deduced from there 
        self.variation = 0
        if   brick_type == 0: self.brick_type = 0; return                
        if   brick_type % 0x10 != 0: self.variation = (brick_type-1) & 0xF
        if   brick_type > 0x00 & brick_type < 0x10: self.brick_type = BrickType.GOO;            
        elif brick_type > 0x10 & brick_type < 0x20: self.brick_type = BrickType.CLAY
        elif brick_type > 0x20 & brick_type < 0x30: self.brick_type = BrickType.CLAY_STICKED
        elif brick_type > 0x30 & brick_type < 0x40: self.brick_type = BrickType.IRON
        elif brick_type > 0x40 & brick_type < 0x50: self.brick_type = BrickType.IRON_STICKED
        elif brick_type > 0x50 & brick_type < 0x60: self.brick_type = BrickType.DIAMOND
        elif brick_type > 0x60 & brick_type < 0x70: self.brick_type = BrickType.DIAMOND_STICKED
        elif brick_type == 0x10: self.brick_type = BrickType.CLAY
        elif brick_type == 0x20: self.brick_type = BrickType.BOOM 
        elif brick_type == 0x30: self.brick_type = BrickType.BOOM_STICKED
        elif brick_type == 0x40: self.brick_type = BrickType.INVERTER
        elif brick_type == 0x50: self.brick_type = BrickType.CHANNEL_H
        elif brick_type == 0x60: self.brick_type = BrickType.CHANNEL_V
        elif brick_type == 0x70: self.brick_type = BrickType.CHANNEL_HV
        elif brick_type > 0x70 and brick_type <= 0x80: self.brick_type = BrickType.SUP + (brick_type - 0x71)
        elif brick_type > 0x9B and brick_type <  0xA0: self.brick_type = BrickType.INVERTER_A + brick_type-0x9C
        else: self.brick_type = 0x00
