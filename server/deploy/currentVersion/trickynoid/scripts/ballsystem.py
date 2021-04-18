from debug                  import debug, exception, error, deprecate, panic
from data.scripts.ball      import Ball, BallStatus
from data.scripts.paddle    import PaddleStatus
from animation              import Animation
import vpu

class BallSystem:
    game    = None
    gfx     = None
    sprite  = None
    
    @staticmethod
    def initialize(game):
        try:
            BallSystem.game = game
            BallSystem.sprite  = vpu.createsprite('balls')
            BallSystem.gfx  = {}
            for i in range(0, BallStatus.MAGIC):
                BallSystem.gfx[i]  = Animation(4,4, BallSystem.sprite, i, 0, i, 0, False, 1, False);        
            BallSystem.gfx[BallStatus.MAGIC] = Animation(4,4, BallSystem.sprite, BallStatus.MAGIC, 0, BallStatus.MAGIC+3, 0, False, 1, True)
        except Exception as E:
            exception(E)

    def __init__(self):
        self.balls          = []
        self.gfx            = {}
        self.balls.append(Ball(0))
        self.theBall        = self.balls[0]
        self.theBall.status = BallStatus.READY
        self.drawTrail      = False
        self.time_scale     = 1.0
        self.paddle         = BallSystem.game.paddle
        self.stillAlive     = True
    
    def setPaddle(self, paddle):        self.paddle = paddle
    def setTimeScale(self, time_scale): self.time_scale = time_scale
    def getBalls(self):                 return self.balls
    def getTheBall(self):               return self.theBall
    def size(self):                     return len(self.balls)
    def get(self, index):               return self.balls[index]
    
    def alter(self, x, y):      
        for b in self.balls:
            if b.status > 0:
                b.addDelta(x/2, y/2)
            
    
    def multiply(self):
        newBall = Ball(self.theBall.status)
        newBall.x = self.theBall.x
        newBall.y = self.theBall.y
        newBall.setDeltaX(-self.theBall.delta_x)
        newBall.setDeltaY( self.theBall.delta_y)
        self.balls.append(newBall)
    
    def divide(self):
        oldBalls = []
        newBalls = []   
        ballCount = 0        
        oldBalls = self.balls
        
        if oldBalls.size() > 1:
            ballCount = oldBalls.size() / 2
            newBalls = []
            for i in range(0, ballCount):
                newBalls.append(oldBalls[i])
            self.balls = newBalls
    
    def subdivide(self):
        newBalls = {}
        lastBall = self.theBall
        for b in self.balls:
            newBalls.append(b)
        for b in newBalls:
            self.theBall = b
            if balls.size()<512: multiply()            
        self.theBall = lastBall
    
    def setStatus(self, status):
        for b in self.balls:
            b.status = status
    
    def render(self, ix, iy):
        ix += 5
        iy += 2
        if self.drawTrail:
            for b in self.balls:
                for i in range(0, len(b.trail)):
                    BallSystem.gfx[BallStatus.ULTRA].draw(ix + b.trail[i].x, iy + b.trail[i].y)
        
        if (BallSystem.game.time_scale < 1.0) and (BallSystem.game.getBulletTime() > 0.1):            
            self.drawTrail = not self.drawTrail
        else:
            self.drawTrail = False
        
        for b in self.balls:
            if b.status >= 0:
                BallSystem.gfx[b.status%10].draw(ix + b.x, iy + b.y )
    
    def swapDeltas(self, ball):
        up          = ball.delta_y < 0.00
        left        = ball.delta_x < 0.00
        tempDeltaY  = 0
        tempDeltaX  = 0

        if up: # Y < 0
            if left: # X < 0
                tempDeltaY =  ball.delta_x # -1 = -1 
                tempDeltaX =  ball.delta_y # -1 = -1
            else: # X > 0
                tempDeltaY = -ball.delta_x # -1 = -(1)
                tempDeltaX = -ball.delta_y #  1 = -(-1)
        else: # Y > 0
            if left: # X < 0
                tempDeltaY = -ball.delta_x #  1 = -(-1)
                tempDeltaX = -ball.delta_y # -1 = -(1)
            else: # X > 0
                tempDeltaY =  ball.delta_x #  1 = 1
                tempDeltaX =  ball.delta_y #  1 = 1

        ball.setDeltaX(tempDeltaX)
        ball.setDeltaY(tempDeltaY)
        
        BallSystem.game.reaction.invertDeltas = False
    
    def update(self, delta):
        map     = BallSystem.game.getMap()
        bricks  = BallSystem.game.getBricks()
        
        self.stillAlive = False
        # remove dead balls 
        balls = []
        for b in self.balls:
            if b.status == -1: continue                
            balls.append(b)
        self.balls = balls

        bx = 0      # Ball X position
        by = 0      # Ball Y position
        
        dx = 0      # Ball delta X
        dy = 0      # Ball delta Y
        
        idx = 0     # Int converted delta X
        idy = 0     # Int converted delta Y
                
        testY = 0   # X Point to be tested
        testX = 0   # Y Point to be tested
        for b in self.balls: 
            msg = None
            # Apply timescale
            if self.time_scale > 1.0:
                b.time_scale = self.time_scale
                b.setTrailStart(b.x, b.y)
            else:
                b.time_scale = self.time_scale
            
            ballStatus = b.status
            
            # Operate balls
            if b.status == BallStatus.DEAD: 
                continue
            elif b.status == BallStatus.STICKED:
                msg = "Sticked"
                self.stillAlive = True      # If at least one ball is alive, the player is still alive
                self.theBall = b            # Select one of them as the ultimate ball, or next to be spawn
                b.x = int(self.paddle.position) + b.getStickOffset()
                b.y = 219
            elif b.status == BallStatus.READY:
                msg = "Ready"
                self.stillAlive = True      # If at least one ball is alive, the player is still alive
                self.theBall = b            # Select one of them as the ultimate ball, or next to be spawn
                b.x = int(self.paddle.position) - 3
                b.y = 219
            else:                           # Moving balls
                self.stillAlive = True      # If at least one ball is alive, the player is still alive
                self.theBall = b            # Select one of them as the ultimate ball, or next to be spawn
                if b.isStopped():
                    b.launch(1.0, -1.0)
                # UPDATE HORIZONTAL MOVEMENT ------------------------------- 

                # Move ball horizontally and get updated position
                b.update(delta, False)
                bx = b.x
                by = b.y

                dx = b.delta_x; 
                idx = int(dx)

                # Set test points for horizontal bounce
                testY = by

                if dx < 0.0:
                    testX = bx - 2 + idx
                else: 
                    testX = bx + 2 + idx

                # Check if ball bounces with something
                if bricks.test(testX, testY):
                    self.game.reaction.target = b
                    if bricks.hit(testX, testY):
                        b.bounceX()
                
                # UPDATE VERTICAL MOVEMENT --------------------------------- 

                # Move ball vertically and get updated position
                b.update(delta, True)
                #bx = b.x
                by = b.y

                # Update delta 
                dy = b.delta_y
                idy = int(dy)

                # Set test point for vertical check
                testX = bx

                # Check vertical ball bounce
                if dy < 0.0:
                    testY = by - 1 + idy
                else: 
                    testY = by + 2 + idy

                # Check if ball bounces with something vertically, or if the paddle hits the ball
                if bricks.test(testX, testY):
                    self.game.reaction.target = b
                    if bricks.hit(testX, testY):
                        b.bounceY()
                elif (self.paddle.test(testX, testY+2)) and (dy > 0.0):
                    # Paddle bounce
                    if ballStatus == BallStatus.STICKY:
                        msg = "Sticky ball @paddle "
                        # Stop ball at paddle                        
                        b.status = BallStatus.STICKED
                        b.setDeltaX(0.0)
                        b.setDeltaY(0.0)
                        b.setStickOffset(b.x() - self.paddle.position)
                    else:
                        msg = "Bouncing @paddle "
                        pd = self.paddle.getDelta() * -0.25 # Paddle delta, to add on bounce 
                        b.bounceY();                        
                        b.addDelta(pd, 0.0)

                if self.game.reaction.invertDeltas: 
                    self.swapDeltas(b)
                self.game.map.need_redraw = True
            #if msg is not None:
            #    print(msg)
                
        # Check if balSystem is fully dead
        if not self.stillAlive:
            self.game.paddle.status = PaddleStatus.DEAD
            self.game.oneDown(1)
            self.game.ready()
    