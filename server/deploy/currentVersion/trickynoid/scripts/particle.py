from animation  import Animation
from random     import random
from math       import cos, sin, atan2
from pixel      import Pixel

class Particle:

    def __init__(self, width, height, sprite):
        self.x          = 0             # Current position in the world
        self.y          = 0
        self.speed      = 0             # initial projectile speed    
        self.angle      = 0             # angle in degrees
        self.radians    = 0             # angle in radians (to avoid recalc)
        self.spread     = 0             # angle variation        
        self.gfx        = 0             # Graphic to display (can be animated)   
        self.isAlive    = True          # true if particle has to be processed
        self.gravity    = 0             # decay of deltaY (goes +gravity until collision)    
        self.origin     = Pixel(0,0)
        self.delta      = Pixel(0,0)    # movement vector  
        self.width      = width
        self.height     = height
        self.timeScale  = 1.0
        self.center     = Pixel(160, 120)
        self.rotation   = Pixel(self.center.x - (height / 2), self.center.y - (width / 2))
        self.sprite     = sprite
        self.range      = 256.0
        self.friction   = 1.0
        self.isAlive    = True
    
    def setTimeScale(self, timeScale):
        self.timeScale = timeScale
    
    def update(self):
        self.x += self.delta.x * self.timeScale
        self.y += self.delta.y * self.timeScale
        
        self.delta.y += self.gravity 
        self.delta.x *= self.friction
        
        radians = self.angle
        
        self.angle =  int(atan2(self.delta.y, self.delta.x)*(180/3.141519))
        
        if self.x > self.origin.x + self.range: self.isAlive = False
        if self.x < self.origin.x - self.range: self.isAlive = False
        if self.y > self.origin.y + self.range: self.isAlive = False
        if self.y < self.origin.y - self.range: self.isAlive = False
    
        return self.doLogic()
    
    def doLogic(self):
        # Collision check must be done here. Called automatically once per update
        return False
    
    def initialize(self, x, y, angle, graph):
        self.x = x
        self.y = y
        self.origin.x = self.x
        self.origin.y = self.y
        
        self.angle = angle
        
        # Randomize shot
        angle += (random() * (self.spread*2) )-self.spread
                
        # calculate direction vectors
        radians = (angle*3.141519)/180
        ty = self.y + ( (50) * sin(radians)) 
        tx = self.x + ( (50) * cos(radians)) 
        
        self.gravity = 0.15
        
        self.delta.x = self.angle/10    #( speed * Math.cos(radians) );
        self.delta.y = -2.0             #( speed * Math.sin(radians) );
        
        graph-=1
        gx = graph % 16
        gy = graph / 16
        self.gfx = Animation(2,2,self.sprite, gx, gy, gx, gy, False, 1, False)
    
    def draw(self, px, py):
        #g.rotate(x + centerX - px, y + centerY - py, angle);
        self.gfx.draw(self.rotation.x + self.x - px, self.rotation.y + self.y -py, self.angle)
        #g.rotate(x + centerX - px, y + centerY - py, -angle);    

    def isAlive(self):          return self.isAlive
    def setSpeed(self, speed):  self.speed = speed
    def setAngle(self, angle):  self.angle = angle    
    def getAngle(self):         return self.angle    
    def getX(self):             return int(self.center.x)    
    def getY(self):             return int(self.center.y)
