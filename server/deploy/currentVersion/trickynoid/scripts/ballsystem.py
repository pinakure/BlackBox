from data.scripts.ball import Ball


class BallSystem:
    game = None
    gfx  = None
    
    @staticmethod
    def initialize(game):
        BallSystem.game = game
        BallSystem.gfx  = {}
        """
        try {
            ballGfx = new Animation[6];
            
            SpriteSheet ballSheet = new SpriteSheet("data/gfx/balls.png", 4, 4, new Color(255,0,255));
            for(int i=0; i<BALL_MAGIC; i++){
                ballGfx[i] = new Animation(ballSheet, i, 0, i, 0, false, 1, false);        
            }
            ballGfx[BALL_MAGIC] = new Animation(ballSheet, BALL_MAGIC, 0, BALL_MAGIC+3, 0, true, 1, false);
        } catch (SlickException ex) {
            ballGfx = null;
            Logger.getLogger(BrickSystem.class.getName()).log(Level.SEVERE, null, ex);
        }
        """
    
    def BallSystem(self):
        self.balls = {}
        self.gfx   = {}
        self.balls.add(Ball(0))
        self.theBall = self.balls.get(0)
        
        self.theBall.setStatus(BallStatus.READY)
        self.drawTrail = False
        
        self.timeScale = 1.0
    
    def setTimeScale(self, timeScale):
        self.timeScale = timeScale

    def getBalls(self):
        return self.balls

    def getTheBall(self):
        return self.theBall
    
    def alter(self, x, y):
        for b in self.balls:
            if b.getStatus() > 0:
                b.addDelta(x/2, y/2)
            
    def size(self):
        return self.balls.size()
    
    def get(self, index):
        return self.balls.get(index)
    
    def multiply(self):
        newBall = Ball(self.theBall.getStatus())
        newBall.setX(self.theBall.getX())
        newBall.setY(self.theBall.getY())
        newBall.setDeltaX(-self.theBall.getDeltaX())
        newBall.setDeltaY( self.theBall.getDeltaY())
        self.balls.add(newBall)
    
    def divide(self):
        oldBalls = {}
        newBalls = {}   
        ballCount = 0        
        oldBalls = self.balls
        
        if oldBalls.size() > 1:
            ballCount = oldBalls.size() / 2
            newBalls = {}
            for i in range(0, ballCount):
                newBalls.add(oldBalls.get(i))
            self.balls = newBalls
    
    def subdivide(self):
        newBalls = {}
        lastBall = self.theBall
        for b in self.balls:
            newBalls.add(b)
        for b in newBalls:
            self.theBall = b
            if balls.size()<512: multiply()            
        self.theBall = lastBall
    
    def setStatus(self, status):
        for b in self.balls:
            b.setStatus(status)
    
    def render(self):
        if self.drawTrail:
            for b in self.balls:
                for i in range(0, b.getTrailLength()):
                    BallSystem.gfx[BallStatus.ULTRA].draw(2 + b.getTrailX(i), 2+ b.getTrailY(i))
        
        if (BallSystem.game.getTimeScale() < 1.0) and (BallSystem.game.getBulletTime() > 0.1):            
            self.drawTrail = not self.drawTrail
        else:
            self.drawTrail = False
        
        for b in self.balls:
            if b.getStatus() >= 0:
                BallSystem.gfx[b.getStatus()%10].draw(2 + b.getX(), 2 + b.getY() )
    
    def swapDeltas(self, ball):
        up          = ball.getDeltaY() < 0.00
        left        = ball.getDeltaX() < 0.00
        tempDeltaY  = 0
        tempDeltaX  = 0

        if up: # Y < 0
            if left: # X < 0
                tempDeltaY =  ball.getDeltaX(); # -1 = -1 
                tempDeltaX =  ball.getDeltaY(); # -1 = -1
            else: # X > 0
                tempDeltaY = -ball.getDeltaX();# -1 = -(1)
                tempDeltaX = -ball.getDeltaY();#  1 = -(-1)
        else: # Y > 0
            if left: # X < 0
                tempDeltaY = -ball.getDeltaX();#  1 = -(-1)
                tempDeltaX = -ball.getDeltaY();# -1 = -(1)
            else: # X > 0
                tempDeltaY =  ball.getDeltaX(); #  1 = 1
                tempDeltaX =  ball.getDeltaY(); #  1 = 1

        ball.setDeltaX(tempDeltaX)
        ball.setDeltaY(tempDeltaY)
        
        BallSystem.game.reaction.invertDeltas = False
    
    def update(self, delta):
        map     = BallSystem.game.getMap()
        bricks  = BallSystem.game.getBricks()
        
        stillAlive = false
        for i in range(0, self.balls.size()):
            if self.balls.get(i).getStatus() == -1:
                self.balls.remove(i)

        bx = 0;     # Ball X position
        by = 0;     # Ball Y position
        
        dx = 0;     # Ball delta X
        dy = 0;     # Ball delta Y
        
        idx = 0;    # Int converted delta X
        idy = 0;    # Int converted delta Y
                
        testY = 0;  # X Point to be tested
        testX = 0;  # Y Point to be tested
                
        for b in self.balls:
            # Apply timescale
            if timeScale > 1.0:
                b.setTimeScale(timeScale)
                b.setTrailStart(b.getX(), b.getY())
            else:
                b.setTimeScale(timeScale)
            
            ballStatus = b.getStatus()
            
            # Operate balls
            """
            switch(ballStatus) {
                case BALL_DEAD:
                    break;
                 
                case BALL_STICKED:
                    stillAlive = true;// If at least one ball is alive, the player is still alive
                    theBall = b;// Select one of them as the ultimate ball, or next to be spawn
                    
                    b.setX(paddle.getX() + b.getStickOffset());                    
                    b.setY(219);
                    break;
                    
                case BALL_READY:
                    stillAlive = true;// If at least one ball is alive, the player is still alive
                    theBall = b;// Select one of them as the ultimate ball, or next to be spawn
                    
                    b.setX((int)paddle.getX()-3);
                    b.setY(219);
                    break;
                    
                default: //Moving balls
                    stillAlive = true;// If at least one ball is alive, the player is still alive
                    theBall = b;// Select one of them as the ultimate ball, or next to be spawn
                    
                    
                    if(b.isStopped()){
                        b.launch(1.0f, -1.0f);
                    };
                    /* UPDATE HORIZONTAL MOVEMENT ------------------------------- */

                    // Move ball horizontally and get updated position
                    b.update(Delta, false);
                    bx = b.getX(); 
                    by = b.getY();

                    dx = b.getDeltaX(); 
                    idx = (int)dx;

                    // Set test points for horizontal bounce
                    testY = by;

                    if(dx < 0.0f) 
                        testX = bx - 2 + idx;
                    else 
                        testX = bx + 2 + idx;

                    // Check if ball bounces with something
                    if( bricks.test(testX, testY) ) {
                        game.reaction.target = b;
                        if( bricks.hit(testX, testY) ) b.bounceX();
                    }
                    /* UPDATE VERTICAL MOVEMENT --------------------------------- */

                    // Move ball vertically and get updated position
                    b.update(Delta, true);
                    //bx = b.getX(); 
                    by = b.getY();

                    // Update delta (
                    dy = b.getDeltaY();
                    idy = (int)dy;

                    // Set test point for vertical check
                    testX = bx;

                    // Check vertical ball bounce
                    if(dy < 0.0f) 
                        testY = by - 1 + idy;
                    else 
                        testY = by + 2 + idy;

                    // Check if ball bounces with something vertically, or if the paddle hits the ball
                    if( bricks.test(testX, testY) ) {
                        game.reaction.target = b; 
                        if( bricks.hit(testX, testY) ){ b.bounceY();}
                    }
                    else if(paddle.test(testX, testY+2) & dy > 0.0f) {
                        // Paddle bounce
                        switch(ballStatus) {
                            case BALL_STICKY:
                                // Stop ball at paddle                        
                                b.setStatus(BALL_STICKED);
                                b.setDeltaX(0.0f);
                                b.setDeltaY(0.0f);
                                b.setStickOffset(b.getX() - paddle.getX());
                                break;
                                
                            default:
                                float pd = paddle.getDelta() * -0.25f;   // Paddle delta, to add on bounce 
                                b.bounceY();                        
                                b.addDelta(pd, 0.0f);
                                break;
                        }
                    }

                    if(game.reaction.invertDeltas)swapDeltas(b);
                 // End Of if(paddle.getStatus() == PADDLE_READY) ...
                    break;
            } // EndSwitch
            """

        # Check if balSystem is fully dead
        if not stillAlive:
            self.game.paddle.setStatus(PaddleStatus.DEAD)
            self.game.oneDown(1)
            self.game.ready()
    