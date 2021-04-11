#################################################################################################################
#                                                                                                               #
#  888888888888  88888888ba   88    ,ad8888ba,   88      a8P   888b      88    ,ad8888ba,    88  88888888ba,    #
#       88       88      "8b  88   d8"'    `"8b  88    ,88'    8888b     88   d8"'    `"8b   88  88      `"8b   #
#       88       88      ,8P  88  d8'            88  ,88"      88 `8b    88  d8'        `8b  88  88        `8b  #
#       88       88aaaaaa8P'  88  88             88,d88'       88  `8b   88  88          88  88  88         88  #
#       88       88""""88'    88  88             8888"88,      88   `8b  88  88          88  88  88         88  #
#       88       88    `8b    88  Y8,            88P   Y8b     88    `8b 88  Y8,        ,8P  88  88         8P  #
#       88       88     `8b   88   Y8a.    .a8P  88     "88,   88     `8888   Y8a.    .a8P   88  88      .a8P   #
#       88       88      `8b  88    `"Y8888Y"'   88       Y8b  88      `888    `"Y8888Y"'    88  88888888Y"'    #
#                                                                                                               #
#################################################################################################################
import blackbox
import vpu
import joypad
from scripts.main                   import menu
from random                         import random
from basicgame                      import BasicGame
from data.scripts.ball              import Ball, BallStatus
from data.scripts.paddle            import Paddle, PaddleStatus
from data.scripts.ballsystem        import BallSystem
from data.scripts.explosion         import Explosion
from data.scripts.hitreaction       import HitReaction
from data.scripts.particle          import Particle
from data.scripts.particlesystem    import ParticleSystem
from data.scripts.bricktype         import BrickType
from data.scripts.brick             import Brick
from data.scripts.bricksystem       import BrickSystem
from data.scripts.backdrop          import Backdrop
from data.scripts.hud               import Hud
from data.scripts.token             import Token, TokenType
from data.scripts.inventory         import Inventory
from data.scripts.musicplayer       import MusicPlayer
from tiledmap                       import TiledMap
from Vpu                            import Vpu

class DebugAction:
    GROW             = False
    SHRINK           = False
    MULTIPLY         = False
    ERASE_SONG       = False
    NEXT_SONG        = False
    HIGLIGHT_BALL    = False
    ABORT            = False
    FULLSCREEN       = False
    TIMESCALE_DOUBLE = False
    
class Action:
    debug            = DebugAction
    BALLTIME         = False
    TILT_UP          = False
    TILT_DOWN        = False
    TILT_LEFT        = False
    TILT_RIGHT       = False
    ACTIVATE         = False
    NEXT_POWERUP     = False
    PREV_POWERUP     = False
    MOVE_LEFT        = False
    MOVE_RIGHT       = False
    MOVE_UP          = False
    MOVE_DOWN        = False        
    PAUSE            = False

class Game(BasicGame):

    nextPowerup = [ 
        TokenType.SHRINK, 
        TokenType.SUBDIVIDE,                                         
        TokenType.SHOOT,
        0,
        TokenType.MISSILE,
        TokenType.EXPLOSIVE,
        TokenType.ULTRABALL,
        0,
        0,
        TokenType.STICKBALL,
        TokenType.SHIELD,
        TokenType.COMMODIN,
        TokenType.GROW
    ]
    
    prevPowerup = [
        TokenType.COMMODIN,  #grow
        TokenType.GROW,      #shrink
        TokenType.SHRINK,    #sub
        0,                   #div
        TokenType.SUBDIVIDE, #shot                                         
        TokenType.SHOOT,     #missile
        TokenType.MISSILE,   #explosive
        0,
        0,
        TokenType.EXPLOSIVE, #ultra 
        TokenType.ULTRABALL, #stick
        TokenType.STICKBALL, #shield
        TokenType.SHIELD     #commo
    ]

    action              = Action
    lives               = 6
    bricks              = None
    particles           = None
    balls               = None
    hud                 = None
    tokens              = None
    backdrop            = None
    inventory           = None
    name                = None
    reaction            = None
    timeScale           = 1.0            
    scoreMultiplier     = 1
    accel               = 0.0
    maxAccel            = 1.5
    buffer              = None
    musicplayer         = None
    map                 = None
    powerupType         = 0x00

    @staticmethod
    def setup(name="TrickyNoid"):
        Game.name       = name
        Game.buffer     = vpu.createsurf(320,240)
        BasicGame.prepare()
        BallSystem.initialize(Game)
        Token.initialize(Game)
        Explosion.initialize(Game)
        Paddle.initialize(Game)
        ParticleSystem.initialize(Game)
        BrickType.initialize(Game)
        Hud.initialize(Game)
        Backdrop.initialize(Game)
        Inventory.initialize(Game)
        BrickSystem.initialize(Game)
        Game.bricks     = BrickSystem() 
        Game.particles  = ParticleSystem()
        Game.balls      = BallSystem()
        Game.hud        = Hud
        Game.musicplayer= MusicPlayer(Game.hud)
        Game.paddle     = Paddle(Game.balls.getTheBall(), Game)
        Game.backdrop   = Backdrop(140)
        Game.inventory  = Inventory()
        Game.reaction   = HitReaction()
        Game.map        = TiledMap(Game, 20, 30, 3)
        Game.tokens     = {}
        Game.map.fill(0x9F)
        Game.map.setactive()
        Game.map.setposition(0,0)
        Game.map.setsurface(Game.buffer)
        """
        loadGraphics();
        Brick.initProperties();
        
        /* Create game instance */
        game = new Game("Smiker");
        
        initInput(gc);
        
        /* Create hud object, passing tokenGfx to be able to show active powerup */
        hud = new Hud(Token.gfx, game);
                
        /* Give game a reference to which hud is monitoring the game */
        game.setHud(hud);
        
        /* Create music player */
        musicplayer = new MusicPlayer(hud.getMap());

        /* Give game a reference to which object holds the map which is loaded */
        game.setMusicplayer(musicplayer);
        
        /* Create backdrop object, which manages the background tiles */
        backdrop = new Backdrop(musicplayer.getTempo());
        
        /* Give game a reference to which object manages the backdrop, 
           to be able to alter color depending on which block is stroke */        
        game.setBackdrop(backdrop);
        
        /* New game */
        game.New();
        """

    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)

    @staticmethod
    def destroy():
        pass

    @staticmethod
    def readKeyboard():
        pass
        """
        Input input = inputManager.input;
        if(input.isKeyDown(Input.KEY_F1)) ACTION_DEBUG_SHRINK = true;
        if(input.isKeyDown(Input.KEY_F2)) ACTION_DEBUG_GROW = true;
        if(input.isKeyDown(Input.KEY_F3)) ACTION_DEBUG_HIGLIGHT_BALL = true;
        if(input.isKeyPressed(Input.KEY_F4)) ACTION_DEBUG_MULTIPLY = true;
        if(input.isKeyPressed(Input.KEY_DELETE)) ACTION_DEBUG_ERASE_SONG = true;
        if(input.isKeyDown(Input.KEY_ESCAPE)) ACTION_DEBUG_ABORT = true;
        if(input.isKeyPressed(Input.KEY_F11)) ACTION_DEBUG_FULLSCREEN = true;
        if(input.isKeyPressed(Input.KEY_F12)) ACTION_DEBUG_NEXT_SONG = true;
        if(input.isKeyDown(Input.KEY_ADD)) ACTION_DEBUG_TIMESCALE_DOUBLE = true;
        //if(input.isMouseButtonDown(1)) ACTION_BALLTIME = true;
            if(input.isKeyDown(Input.KEY_RIGHT)) ACTION_TILT_RIGHT = true;
        else if(input.isKeyDown(Input.KEY_LEFT)) ACTION_TILT_LEFT = true;
           if(input.isKeyDown(Input.KEY_DOWN)) ACTION_TILT_DOWN = true;
        else if(input.isKeyDown(Input.KEY_UP)) ACTION_TILT_UP = true;
        if(input.isKeyPressed(Input.KEY_Q)) ACTION_PREV_POWERUP = true;
        if(input.isKeyPressed(Input.KEY_E)) ACTION_NEXT_POWERUP = true;
        if(input.isKeyPressed(Input.KEY_SPACE)) ACTION_ACTIVATE = true;
        // Handle movement
        if(input.isKeyDown(Input.KEY_A)) game.moveLeft();
        else if(input.isKeyDown(Input.KEY_D)) game.moveRight();
        """

    @staticmethod
    def update(delta):
        Game.delta = delta
        Paddle.gfx[Game.paddle.getStatus()].update(int(delta * Game.timeScale))
        Game.updateMap()
        Game.updateTokens()
        Game.updatePaddle(input)
        Game.balls.update(delta)
        Game.particles.update(delta)
        Game.bricks.update(delta)
        if Game.powerupType < 0x10:
            Token.gfx[Game.powerupType].update(int(delta*Game.timeScale))
        """
        inputManager.setInput(gc.getInput());
        
        Paddle paddle = game.getPaddle();
        Ball theBall = game.getTheBall();
        BallSystem balls = game.getBalls();
        
        resetInput();
        
        readKeyboard();
        
        // Move paddle or brake it
        if(ACTION_MOVE_LEFT) game.moveLeft();
        else if(ACTION_MOVE_RIGHT) game.moveRight();
        else if(!ACTION_MOVE_LEFT) game.brake();
        
        //readJoystick();
        
        if(ACTION_NEXT_POWERUP) {
            game.setPowerupType(nextPowerup[game.getPowerupType()]);
            ACTION_NEXT_POWERUP=false;
        } else if(ACTION_PREV_POWERUP) {
            game.setPowerupType(prevPowerup[game.getPowerupType()]);
            ACTION_PREV_POWERUP=false;
        }
        
        
        if(ACTION_DEBUG_GROW) {
            paddle.grow();
        }
        
        if(ACTION_DEBUG_SHRINK) {
            paddle.shrink();            
        }
        
        if(ACTION_DEBUG_HIGLIGHT_BALL) {
            game.getParticles().generate(0x01, theBall.getX()+2, theBall.getY()+2);
        }
        
        if(ACTION_DEBUG_MULTIPLY) {
            balls.subdivide();
        }
        
        // Handle Music player
        if(ACTION_DEBUG_NEXT_SONG) {
            musicplayer.nextSong();
            backdrop.setSpeed(musicplayer.getTempo());
        } else if(ACTION_DEBUG_ERASE_SONG) {
            musicplayer.deleteSong();
        } 
        musicplayer.update(Delta);
        
        if(ACTION_DEBUG_ABORT) {
            throw new SlickException("Aborted by User");
        }
        
        if(ACTION_DEBUG_FULLSCREEN) {
            gc.setFullscreen(!gc.isFullscreen());
        }
        
        if(ACTION_ACTIVATE) {
            boolean ballsReady = false;
            for(Ball b : game.getBalls().getBalls()) {
                if((b.getStatus() == BALL_STICKED) || (b.getStatus() == BALL_READY)) {
                    ballsReady = true;
                    break;
                } else {
                    
                }
            }
            
            //if(paddle.getStatus() == PADDLE_STICKY | paddle.getStatus() == PADDLE_READY ){
            if(ballsReady) {
               paddle.trigger(false);
               if(paddle.getStatus() != PADDLE_STICKY) paddle.setStatus(PADDLE_PLAYING);
                
            } else if(paddle.getStatus() != PADDLE_READY){
                
                paddle.shoot();
            }  
            
            // Switch off
            ACTION_ACTIVATE = false;
        }
        
        // Handle timescale
        if(ACTION_DEBUG_TIMESCALE_DOUBLE) {
            game.setTimeScale(2.0f);
        } else {
            // Handle bullet time
            
            if((ACTION_BALLTIME)&&(game.getBulletTime() > 0.0000f)) {
                game.setTimeScale(0.125f);
                game.setBulletTime(game.getBulletTime()-0.0125f);
            
                float bulletTime = game.getBulletTime();            
                
                if(bulletTime > 0.02f) {
                    if(ACTION_TILT_LEFT) {                        
                        balls.alter(-bulletTime, 0);
                    } else if(ACTION_TILT_RIGHT) {
                        balls.alter(bulletTime, 0);
                    }

                    if(ACTION_TILT_UP) {
                        balls.alter(0, -bulletTime);
                    } else if(ACTION_TILT_DOWN) {
                        balls.alter(0, bulletTime);
                    }
                }
                
            } else {                 
                game.setTimeScale(1.0f);
                
                // Regenerate bullet Time
                float bulletTime = game.getBulletTime();
                
                if(game.getBulletTime() < 1.000f)bulletTime += 0.0012f;
                else bulletTime = 1.0000f;
                
                game.setBulletTime(bulletTime);
            }
        }
        
        backdrop.update(Delta, musicplayer, game.getTimeScale());
        
        game.update(Delta, inputManager.input);
        
        hud.updateLcd(game.getBulletTime());
        
        Explosion.gfx.update((int)(Delta*game.getTimeScale()));
        
        
        if(game.isGameOver()){
            /* Call Gameover process */
            game.Over();
            /* Restart game */ 
            game.New();
        }    
        """
        #required stuff
        BasicGame.update(delta)

    @staticmethod
    def draw():
        # clear buffer
        vpu.select(1)
        vpu.fill(0,0,0,0)
        
        #draw stuff
        """
        g.scale(screenScale, screenScale);
        g.translate(0, 0);        
        
        backdrop.render(g);
        game.getMap().render(g);
        game.getBricks().render(g);
        
        drawTokens();
        
        game.getPaddle().render();
        
        game.getBalls().render(g);
        
        game.getParticles().render(g);
                
        //Explosion.gfx.draw(32, 32); 
        
        hud.render(game);
        
        renderInventary(g);
        """


        # ...
        BasicGame.draw()
    
    @staticmethod
    def setHud(hud):                
        Game.hud = hud   

    @staticmethod
    def isGameOver():               
        return Game.gameOver

    @staticmethod   
    def setGameOver(gameOver):      
        Game.gameOver = gameOver

    @staticmethod
    def getScore():                 
        return Game.score

    @staticmethod
    def setScore(score):            
        Game.score = score

    @staticmethod
    def getLives():                 
        return Game.lives    

    @staticmethod
    def setLives(lives):            
        Game.lives = lives    

    @staticmethod
    def getLevel():                 
        return Game.level    

    @staticmethod
    def setLevel(level):            
        Game.level = level    

    @staticmethod
    def getPowerupType():           
        return Game.powerupType    

    @staticmethod
    def setPowerupType(powerupType):
        Game.powerupType = powerupType    

    @staticmethod
    def getTimeScale():             
        return Game.timeScale    

    @staticmethod
    def setTimeScale(timeScale):
        Game.timeScale = timeScale
        Game.balls.setTimeScale(timeScale)    

    @staticmethod
    def getBulletTime():            
        return Game.bulletTime    

    @staticmethod
    def setBulletTime(bulletTime):  
        Game.bulletTime = bulletTime    

    @staticmethod
    def setBackdrop(backdrop):      
        Game.backdrop = backdrop    

    @staticmethod
    def getBackdrop():              
        return Game.backdrop

    @staticmethod
    def setMusicplayer(musicplayer):
        Game.musicplayer = musicplayer    

    @staticmethod
    def getSerialVersionUID():      
        return Game.serialVersionUID    

    @staticmethod
    def getName():                  
        return Game.name    

    @staticmethod
    def getRand():                  
        return Game.rand    

    @staticmethod
    def getPaddle():                
        return Game.paddle    

    @staticmethod
    def getMap():                   
        return Game.map    

    @staticmethod
    def getTheBall():               
        return Game.balls.getTheBall()    

    @staticmethod
    def getBricks():                
        return Game.bricks    

    @staticmethod
    def getTime():                  
        return Game.time    

    @staticmethod
    def getTokens():                
        return Game.tokens    

    @staticmethod
    def getBalls():                 
        return Game.balls    

    @staticmethod
    def getExplosions():            
        return Game.explosions    

    @staticmethod
    def getParticles():             
        return Game.particles

    @staticmethod
    def getInventory():
        return Game.inventory
    @staticmethod
    def addScore(points):
        Game.score += points
    
    @staticmethod
    def New():
        if Game.hud == None:
            print("Hud is not loaded. Cannot start game")
            return        
        Game.score          = 0
        Game.lives          = 5
        Game.gameOver       = False
        Game.level          = 0
        Game.lives          = 3
        Game.gameOver       = False
        Game.timeScale      = 1.0       
        Game.powerupType    = 0x00
        Game.level          = 0
        Game.score          = 0
        Game.bulletTime     = 1.0
        Game.balls          = BallSystem()  # Create balls 
        Game.newPaddle()                        # Create paddle
        Game.tokens         = {}                # Create token list 
        Game.particles      = ParticleSystem(Game)
        Game.particles.init()
        Game.bricks         = BrickSystem() # Create brick system 
        Game.map = Map("sample", Game)          # Load map (must be done after creating bricksystem 
        Game.musicplayer.nextSong()
        
    
    @staticmethod
    def newPaddle():
        Game.paddle = Paddle(Game.balls.getTheBall(), Game)
        Game.paddle.setStatus(PaddleStatus.READY)                 
        Game.balls.setPaddle(Game.paddle)   # Update ballSystem paddle reference 
    
    @staticmethod
    def Over():
        # Game over process 
        pass
    
    @staticmethod
    def oneUp(lives):
        Game.lives += lives
    
    @staticmethod
    def oneDown(lives):
        Game.lives -= lives
        if Game.lives <= 0: 
            Game.gameOver = True
    
    @staticmethod
    def ready():
        # Create new ball system and assign it to the paddle        
        Game.balls = BallSystem()
        Game.newPaddle()
        Game.bulletTime = 1.0
        Game.musicplayer.quiet()
    
    @staticmethod
    def brake():
        Game.paddle.setDelta(Game.accel)
        if Game.accel < 0.0: Game.accel += 0.6
        if Game.accel > 0.0: Game.accel -= 0.6
        if Game.accel < 0.7 and Game.accel > -0.7: Game.accel = 0.0
    
    @staticmethod
    def moveLeft():
        if Game.accel < 0.0: Game.accel =- Game.accel/2
        Game.paddle.setDelta(Game.accel + 1.0)
        Game.accel += 0.2
        if Game.accel > Game.maxAccel: Game.accel = Game.accel * 0.75
    
    @staticmethod
    def moveRight():
        if Game.accel > 0.0: Game.accel =- Game.accel / 2
        Game.paddle.setDelta( Game.accel - 1.0 )
        Game.accel -= 0.2
        if Game.accel < -Game.maxAccel: Game.accel = Game.accel * 0.5
    
    @staticmethod
    def doubleScore():
        if Game.scoreMultiplier < 32:
            Game.scoreMultiplier *= 2
    
    @staticmethod
    def halfScore():
        if Game.scoreMultiplier > 1:
            Game.scoreMultiplier /= 2
    
    @staticmethod
    def enableShield():
        #Enables game shield
        pass
    
    @staticmethod
    def applyEffect(effectIndex):
        Game.balls.setStatus(BallStatus.NORMAL)
        Game.paddle.setStatus(PaddleStatus.PLAYING)        
        if   effectIndex == TokenType.SUBDIVIDE:   Game.balls.subdivide()
        elif effectIndex == TokenType.DIVIDE:      Game.balls.divide()
        elif effectIndex == TokenType.SHOOT:       Game.paddle.shoot()
        elif effectIndex == TokenType.MISSILE:     Game.paddle.shoot()
        elif effectIndex == TokenType.EXPLOSIVE:   Game.balls.setStatus(BallStatus.EXPLOSIVE)
        elif effectIndex == TokenType.ONEUP:       Game.oneUp(1)
        elif effectIndex == TokenType.ONEDOWN:     Game.oneDown(1)
        elif effectIndex == TokenType.ULTRABALL:   Game.balls.setStatus(BallStatus.ULTRA)
        elif effectIndex == TokenType.STICKBALL:   Game.balls.setStatus(BallStatus.STICKY); Game.paddle.setStatus(PaddleStatus.STICKY)
        elif effectIndex == TokenType.COMMODIN:    Game.balls.setStatus(BallStatus.MAGIC)
        elif effectIndex == TokenType.SHIELD:      Game.enableShield()
        elif effectIndex == TokenType.HALFSCORE:   Game.halfScore()
        elif effectIndex == TokenType.DOUBLESCORE: Game.doubleScore()        
        elif effectIndex == TokenType.DELTASWAP:
            # paddle.setInverted(True)
            pass
        else:
            #paddle.setInverted(False)
            pass   
    
    @staticmethod
    def addToken(token):
        Game.tokens.add(token)
    
    @staticmethod
    def updateMap():
        i=0        
        for y in range(0, 26):
            for x in range(0, 14):
                b = Game.bricks.get(i)
                if b.getType() != BrickType.NONE:
                    # Put shadow
                    Game.map.set(x, y, 3, 0x91)
                    
                    if b.getDamage() > 8:
                        Game.bricks.hit(x*16, y*8)
                        Game.map.set(x, y, 1, 0x00) #Remove damage picture
                    else:
                        #Set damage graphic
                        Game.map.set(x, y, 1, 0x91 + (b.getDamage()*4))
                else:
                    Game.map.set(x, y, 3, 0x00)
                    Game.map.set(x, y, 1, 0x00) #Remove damage picture
                Game.map.set(x, y, 0, b.getGraph())
                i+=1
    
    @staticmethod
    def updatePaddle(input):
        if Game.paddle.getStatus() != PaddleStatus.DEAD:
            Game.paddle.update()
        if Game.action.ACTIVATE:
            Game.action.ACTIVATE = False
            if Game.paddle.getStatus() == PaddleStatus.READY:
                Game.paddle.trigger(False)
                Game.musicplayer.loud()

    @staticmethod 
    def loadMap(filename):        
        try:
            Game.map = TiledMap("data/map/"+ filename + ".tmx", "data/gfx")
            Game.getBricks().loadBricks(map)
        except Exception as E:
            exception(E)

    @staticmethod
    def test(x,y):
        if x < 0     : return False
        elif y < 0   : return False
        elif x > 220 : return False
        elif y > 208 : return False
        return Game.map.get((x/16), (y/8), 0) > 0x00

    @staticmethod
    def updateTokens():
        for t in Game.tokens:
            t.update(Game.timeScale)
            if t.collides(Game.paddle):
                #Execute token effect and unlink it from list
                #TODO: Do something
                Game.bulletTime = 1.0
                #Game.powerupType = t.getType();
                Game.inventory.inheritToken(t.getType())
                Game.tokens.remove(t)
                continue
            if t.getY() >= 240:
                Game.tokens.remove(t)

    @staticmethod
    def drawTokens():
        for t in Game.getTokens():
            tokentype = t.getType()
            tok = Token.gfx[tokentype]
            tok.setframe(t.getFrame())
            tok.draw(3 + t.getX(), 3 + t.getY())

    @staticmethod
    def drawToken(x, y, tokentype):
        last = Token.gfx[tokentype].getFrame()
        Token.gfx[tokentype].setCurrentFrame(0)
        Token.gfx[tokentype].draw(x, y)
        Token.gfx[tokentype].setCurrentFrame(last)
    
    @staticmethod
    def renderInventary():
        Game.drawToken(241,  50, TokenType.GROW)
        Game.drawToken(241,  58, TokenType.SHRINK)
        Game.drawToken(241,  66, TokenType.SUBDIVIDE)
        Game.drawToken(241,  74, TokenType.SHOOT)
        Game.drawToken(241,  82, TokenType.MISSILE)        
        Game.drawToken(281,  50, TokenType.EXPLOSIVE)
        Game.drawToken(281,  58, TokenType.ULTRABALL)
        Game.drawToken(281,  66, TokenType.STICKBALL)
        Game.drawToken(281,  74, TokenType.SHIELD)
        Game.drawToken(281,  82, TokenType.COMMODIN)        
        Token.gfx[Game.getPowerupType()].draw(269, 90)
    
    @staticmethod
    def resetInput():
        Game.action.debug.GROW             = False
        Game.action.debug.SHRINK           = False
        Game.action.debug.HIGLIGHT_BALL    = False
        Game.action.debug.MULTIPLY         = False
        Game.action.debug.ERASE_SONG       = False
        Game.action.debug.ABORT            = False
        Game.action.debug.FULLSCREEN       = False
        Game.action.debug.NEXT_SONG        = False
        Game.action.debug.TIMESCALE_DOUBLE = False
        Game.action.PAUSE = False                    
        #Game.action.debug.BALLTIME = False        