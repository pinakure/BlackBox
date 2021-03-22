import vpu
from math import sqrt
from direction import *

class Target:
    def __init__(self, ghost):
        self.ghost     = ghost
        self.x         = 0
        self.y         = 0
        self.distance  = 0
        self.direction = Direction.UP
        self.solid     = False
        self.viable    = False
        self.color     = 0xffff00

    @staticmethod
    def compare(a, b):
        return a.distance - b.distance 
        
    def lock(self, x, y): #settarget!
        self.viable    = True
        self.x         = x
        self.y         = y
        # Calculate distance to target (ox, oy)
        ox = self.ghost.targetX
        oy = self.ghost.targetY
        self.distance  = sqrt( ((self.x - ox) * (self.x - ox)) + ((self.y - oy) * (self.y - oy)) )
        
        # Forgive me about this monstruosity, seems I can't make work this sentence splitted in several lines.
        self.direction = Direction.RIGHT if self.x > self.ghost.tx else Direction.LEFT  if self.x < self.ghost.tx else Direction.DOWN  if self.y > self.ghost.ty else Direction.UP    if self.y < self.ghost.ty else Direction.DOWN
        
        # Set viability and color
        self.solid = self.ghost.is_solid(self.x, self.y)
        if self.solid:
            self.viable = False
            self.color  = 0x0000ff
        else:
            self.viable = True
            self.color  = 0x666000

    @staticmethod
    def draw_static(x, y, color, is_target):
        if is_target:
            vpu.setcolor((color&0xff0000)>>16, (color&0xff00)>>8, (color&0xff))
            vpu.line(x  , y , x+8  , y+8 )
            vpu.line(x  , y+8 , x+8  , y )
        else:
            vpu.setcolor((color&0xff0000)>>16, (color&0xff00)>>8, (color&0xff))
            vpu.pset(x  , y  )
            vpu.pset(x+1, y  )
            vpu.pset(x  , y+1)
            vpu.pset(x+7, y  )
            vpu.pset(x+6, y  )
            vpu.pset(x+7, y+1)
            vpu.pset(x+7, y+6)
            vpu.pset(x+7, y+7)
            vpu.pset(x+6, y+7)
            vpu.pset(x+1, y+7)
            vpu.pset(x  , y+7)
            vpu.pset(x  , y+6)

    def draw(self):
        x = self.x<<3
        y = self.y<<3
        vpu.setcolor((self.color&0xff0000)>>16, (self.color&0xff00)>>8, (self.color&0xff))
        def cross(x,y,distance):
            #vpu.line(x  , y   , x+8  , y+8 )
            #vpu.line(x  , y+8 , x+8  , y   )
            vpu.textout(f"{int(distance)}",x-4,y)

        if self.direction == Direction.LEFT     : cross(x-8, y,self.distance)
        elif self.direction == Direction.RIGHT  : cross(x+8, y,self.distance)
        elif self.direction == Direction.UP     : cross(x, y-8,self.distance)
        elif self.direction == Direction.DOWN   : cross(x, y+8,self.distance)
            
    def __str__(self):
        status = "SOLID" if self.viable and self.solid else "VIABLE" if self.viable else "FORBID"
        return     f"[ {Direction.string(self.direction)} { status } D: {self.distance} ]"
