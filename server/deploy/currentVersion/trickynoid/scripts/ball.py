from pixel import Pixel
from debug import deprecate

class BallStatus:
    DEAD      = -1
    READY     = 0
    NORMAL    = 1
    ULTRA     = 2
    STICKY    = 3
    EXPLOSIVE = 4
    MAGIC     = 5 #Commodin ball
    STICKED   = 13
    
class Ball:

    trail = {}

    @staticmethod
    def initializeTrail(length):
        if len(Ball.trail)==0:
            for i in range(0, length):
                Ball.trail[i] = Pixel(0,0)
        else:
            for i in range(0, len(Ball.trail)):
                Ball.trail[i].x, Ball.trail[i].y = 0,0
        return Ball.trail

    def __init__(self, status=BallStatus.DEAD):
        self.x            = 0
        self.y            = 0
        self.delta_x      = 0
        self.delta_y      = 0
        self.time_scale   = 1.0
        self.max_speed    = 2.00
        self.trail_timer  = 0
        self.stick_offset = 0
        self.trail        = Ball.initializeTrail(32)
        self.status       = status
        
    def getStickOffset(self):
        deprecate("getStickOffset", "Use direct access to stick_offset instead")
        return self.stick_offset

    def setStickOffset(self, offset):
        deprecate("setStickOffset", "Use direct access to stick_offset instead")
        self.stick_offset = value

    def getTrailLength(self):
        deprecate("getTrailLength", "Use direct access to trail instead")
        return len(self.trail)

    def getTrailX(self, index):
        deprecate("getTrailX", "Use direct access to trail instead")
        return self.trail[index].x
    
    def getTrailY(self, index):
        deprecate("getTrailY", "Use direct access to trail instead")
        return self.trail[index].y
    
    def setTimeScale(self, time_scale):
        deprecate("setTimeScale", "Use direct access to time_scale instead")
        self.time_scale = time_scale

    def getDeltaX(self):
        deprecate("getDeltaX", "Use direct access to delta_x instead")
        return self.delta_x

    def getDeltaY(self):
        deprecate("getDeltaY", "Use direct access to delta_y instead")
        return self.delta_y
    
    def getX(self):
        deprecate("getX", "Use direct access to x instead")
        return int(self.x)
    
    def getY(self):
        deprecate("getY", "Use direct access to y instead")
        return int(self.y)
    
    def setX(self, x):
        deprecate("setX", "Use direct access to x instead")        
        self.x = float(x)

    def setY(self, y):
        deprecate("setY", "Use direct access to y instead")
        self.y = float(y)

    def setStatus(self, status):
        deprecate("setStatus", "Use direct access to status instead")
        self.status = status
    
    def getStatus(self):
        deprecate("getStatus", "Use direct access to status instead")
        return self.status

    def isStopped(self):
        return (self.delta_x == 0.00) and (self.delta_y == 0.00)
    
    def launch(self, delta_x, delta_y):
        self.delta_x,self.delta_y = delta_x, delta_y
    
    def setTrailStart(self, x, y):
        for i in self.trail:
            i.x, i.y = x, y
    
    def addTrailStep(self, x,  y):
        for i in range(1, len(self.trail)):
            self.trail[i].x, self.trail[i].y = self.trail[i-1].x, self.trail[i-1].y
        self.trail[0].x,self.trail[0].y = x, y
    
    def nextX(self):
        return self.x + (self.delta_x * self.time_scale)
    
    def nextY(self):
        return self.y + (self.delta_y * self.time_scale)
    
    def moveX(self, delta):
        self.x = self.nextX()
        if self.trail_timer < 200:
            self.trail_timer += delta
        else:
            self.trail_timer = 0
            self.addTrailStep(int(self.x),int(self.y))
    
    def moveY(self, delta):
        self.y = self.nextY()
        if self.trail_timer < 200:
            self.trail_timer += delta
        else:
            self.trail_timer = 0
            self.addTrailStep(int(self.x),int(self.y))
    
    def addDelta(self, deltaX, deltaY):
        self.delta_y += deltaY
        self.delta_x += deltaX
        
        # Clamp delta...
        if   self.delta_x >  self.max_speed: self.delta_x =  self.max_speed
        elif self.delta_x < -self.max_speed: self.delta_x = -self.max_speed
        if   self.delta_y >  self.max_speed: self.delta_y =  self.max_speed
        elif self.delta_y < -self.max_speed: self.delta_y = -self.max_speed
    
    def getDamage(self):
        return 9 if self.status == BallStatus.ULTRA else 1
    
    def setDeltaX(self, dx):
        if   dx >  self.max_speed: dx =  self.max_speed
        elif dx < -self.max_speed: dx = -self.max_speed
        self.delta_x = dx

    def setDeltaY(self, dy):
        #if   dy >  self.max_speed: dy =  self.max_speed
        #elif dy < -self.max_speed: dy = -self.max_speed
        self.delta_y = dy

    def bounceX(self):
        self.delta_x *= -1.0

    def bounceY(self):
        self.delta_y *= -1.0
    
    def getDirection(self):
        if   (self.delta_x < 0.0) and (self.delta_y < 0.0): return 0
        elif (self.delta_x > 0.0) and (self.delta_y < 0.0): return 1
        elif (self.delta_x < 0.0) and (self.delta_y > 0.0): return 2
        elif (self.delta_x > 0.0) and (self.delta_y > 0.0): return 3
        else: return -1
    
    def update(self, delta, vertical=False):
        if vertical:            
            self.moveY(delta)
            if self.y < 0.01:
                self.bounceY()
                self.y = 0.0
            elif self.y > 238.0:
                # self.bounceY()
                self.y = 240.0
                self.status = -1
        else:
            self.moveX(delta)
            if self.x < 0.01:
                self.bounceX()
                self.x = 0.0
            elif self.x > 220.99:
                if self.delta_x > 0.0:
                    self.bounceX()
                self.x = 220.0
