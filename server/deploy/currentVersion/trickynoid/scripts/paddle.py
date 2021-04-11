from data.scripts.ball  import Ball, BallStatus
from data.scripts.token import Token
from animation          import Animation, AnimationLoop, AnimationAutoUpdate
import vpu

class PaddleStatus:
    DEAD    = 0
    PLAYING = 1
    LONG    = 2
    READY   = 3
    STICKY  = 4
    SHORT   = 5
    INVERTED= 6

class Paddle:
    sprite = None
    gfx    = None
    game   = None
    
    @staticmethod
    def initialize(game):
        Paddle.game = game
        Paddle.sprite = vpu.createsprite('pad')
        Paddle.gfx = {}
        Paddle.gfx[PaddleStatus.DEAD      ] = Animation(20,4, Paddle.sprite, 0, 0, 0, 0, False,  0.0, AnimationAutoUpdate.NO)
        Paddle.gfx[PaddleStatus.PLAYING   ] = Animation(20,4, Paddle.sprite, 0, 1, 0, 5, False, 16.0, AnimationAutoUpdate.YES, AnimationLoop.PINGPONG)
        Paddle.gfx[PaddleStatus.LONG      ] = Animation(20,4, Paddle.sprite, 0, 6, 0, 6, False,  0.0, AnimationAutoUpdate.NO)
        Paddle.gfx[PaddleStatus.READY     ] = Animation(20,4, Paddle.sprite, 0, 7, 0, 7, False,  0.0, AnimationAutoUpdate.NO)
        Paddle.gfx[PaddleStatus.STICKY    ] = Animation(20,4, Paddle.sprite, 0, 8, 0, 8, False,  0.0, AnimationAutoUpdate.NO)
        
    def __init__(self, ball, game):
        Paddle.game = game
        
        self.sensitivity = 1.2
        self.width        = 20
        self.position     = 0.0
        self.delta        = 0.0
        self.sensitivity  = 1.0
        self.growSteps    = 0
        self.shrinkSteps  = 0
        self.lastPosition = 0.0
        self.status       = PaddleStatus.READY  # 3 waiting for ball active
        self.warmUp       = 1  #Prevents accidental initial fire 
        Paddle.game.getBalls().getTheBall().status=BallStatus.READY
        
    def getWidth(self):
        return self.width
    
    def grow(self):
        if self.width < 220: self.width += 2
    
    def shrink(self):
        if self.width > 10: self.width -= 2
    
    def setWidth(self, width):
        self.width = width
    
    def addDelta(self, position):
        self.delta += (self.lastPosition - self.position) / self.sensitivity
        self.lastPosition = int(position)
    
    def getDelta(self):
        return self.delta
    
    def setDelta(self, delta):
        self.delta += delta
    
    def getX(self):
        return int(self.position)
    
    def getY(self):
        return 224
    
    def setStatus(self, status):
        self.status = status
    
    def getStatus(self):
        return self.status
    
    def setX(self, position):
        self.position = position
    
    def stop(self, position):
        self.position = position
        self.delta = 0.0
    
    def shoot(self):
        #TODO
        pass
        """
        Inventory inv = game.getInventory();
        
        switch(game.getPowerupType()){
            case TOKEN_GROW:
                if(inv.grow > 0){
                    growSteps+=8;
                    inv.grow--;
                } 
                break;
            
            case TOKEN_SHRINK:
                if(inv.shrink > 0){
                    shrinkSteps+=8;
                    inv.shrink--;
                }
                break;
         
            case TOKEN_SUBDIVIDE:
                if(inv.subdivide > 0){
                    game.applyEffect(TOKEN_SUBDIVIDE);
                    inv.subdivide--;
                } 
                break;
            
            case TOKEN_ULTRABALL:
                if(inv.ultraball > 0) {
                    game.applyEffect(TOKEN_ULTRABALL);
                    inv.ultraball--;
                }
                break;
            
            case TOKEN_STICKBALL:
                if(inv.stickyball > 0) {
                    game.applyEffect(TOKEN_STICKBALL);
                    inv.stickyball--;
                }
                break;
            
            default:
                break;
        }  
        """
    
    def trigger(self, left=True):
        self.warmUp = 100        
        for b in Paddle.game.getBalls().getBalls():
            if b.status == BallStatus.STICKED:
                b.setDeltaX(-self.delta)
                b.setDeltaY(-1.0)
                b.status = BallStatus.STICKY
            elif b.status == BallStatus.READY:
                b.setDeltaX(-self.delta)                
                b.setDeltaY(-1.0)
                b.status = BallSatus.NORMAL
        # self.status = PADDLE_PLAYING
    
    def test(self, x, y):
        if (self.y >= 224) and (self.y <= 227):
            
            # left  = paddle.getX() - 4;
            # right = paddle.getX()+16 + 4;
            width   = (self.width / 2) #4 = grace zone
            px      = int(self.position)
            left    = px - (self.width)
            right   = px + (self.width)

            if (x > left-5) and (x < right+2):
                return True

        return False
    
    def render(self):
        halfwidth = self.width / 2
        Ball.gfx[BallStatus.NORMAL].draw( 1 + self.position - (self.halfwidth + 2), 224)
        Ball.gfx[BallStatus.NORMAL].draw( 1 + self.position + (self.halfwidth - 2), 224)
        Paddle.gfx[self.status].draw( 1 + self.position - self.halfwidth , 224, self.width, 4)
    
    def update(self):
        if self.delta < -0.01:
            self.delta *= 0.75
        else:
            if self.delta > 0.01:
                self.delta *= 0.75
            else:
                self.delta = 0.0
        # Move paddle
        self.position -= self.delta
        # Grow paddle
        if self.growSteps > 0:
            self.grow()
            self.growSteps-=1
        if self.shrinkSteps > 0:
            self.shrink()
            self.shrinkSteps-=1
        
        # Correct paddle position
        if self.position > (self.width/2)+4:
             if self.position > 223.99 - (self.width/2):
                 self.stop(224 - (self.width/2))
        else:
            self.stop((self.width/2)+4)
