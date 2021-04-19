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
from debug                          import debug, deprecate, error, exception, panic
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
    time_scale          = 1.0
    scoreMultiplier     = 1
    accel               = 0.0
    maxAccel            = 1.5
    musicplayer         = None
    powerupType         = 0x00
    game_over           = False
    left                = 0
    top                 = 0

    @staticmethod
    def setup(name="TrickyNoid"):
        Game.name       = name
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
        Game.paddle     = None
        Game.balls      = BallSystem()
        Game.hud        = Hud
        Game.musicplayer= MusicPlayer(Game.hud)
        Game.buffer     = [ vpu.createsurf(Game.width, Game.height), vpu.createsurf(Game.width, Game.height)]
        Game.paddle     = Paddle(Game.balls.getTheBall(), Game)
        Game.backdrop   = Backdrop(140)
        Game.inventory  = Inventory()
        Game.reaction   = HitReaction()
        Game.map        = TiledMap(Game, 20, 30, 4, 16, 8)
        Game.explosion  = Explosion
        Game.tokens     = {}
        Game.map.load_tileset('tileset')
        Game.map.fill(0x9F)
        Game.map.setactive()
        Game.map.setposition(0,0)
        Game.map.setposition(
            ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 ),
            ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )
        )
        Game.map.setsurface(Game.buffer[1])
        #draw stuff
        #Game.hud.draw()
        for x in range(0, 20):
            for y in range(0, 30):
                Game.map.set(x,y, Hud.data[y][x], 3)
        Game.map.setoffset(3,3,0)
        Game.map.setoffset(3,3,1)        
        Game.left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
        Game.top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
        Game.New()
        Game.draw()
        Game.bricks.update(1.0)
        Game.map.redraw()
        Game.draw()
        
        

    @staticmethod
    def loop():
        Game.map.need_redraw = True
        vpu.select(Game.buffer[0])
        vpu.fill(0,0,0,0)
        vpu.gradient(64,0,0,255,192,192,1,255)
        Game.backdrop.render(Game.left+11,Game.top+2)

        delta = 1.0
        BasicGame.autoredraw = True
        Game.autoredraw = True
        while Game.running:
            #Game.map.need_redraw = 1 if Game.map.need_redraw == 2 else 0
            Game.draw()
            Game.update(delta)

    @staticmethod
    def destroy():
        pass

    @staticmethod
    def readKeyboard():
        #Game.action.debug.SHRINK           = (keyboard.f1() == 1)
        #Game.action.debug.GROW             = (keyboard.f2() == 1)
        #Game.action.debug.HIGLIGHT_BALL    = (keyboard.f3() == 1)
        #Game.action.debug.MULTIPLY         = (keyboard.f4() == 1)
        #Game.action.debug.ERASE_SONG       = (keyboard.delete() == 1)
        #Game.action.debug.ABORT            = (keyboard.escape() == 1)
        #Game.action.debug.FULLSCREEN       = (keyboard.f11() == 1)
        #Game.action.debug.NEXT_SONG        = (keyboard.f12() == 1)
        #Game.action.debug.TIMESCALE_DOUBLE = (keyboard.plus() == 1)
        Game.action.ACTIVATE      = (joypad.b() == 1)
        Game.action.BALLTIME      = joypad.a()
        #Game.action.TILT_LEFT     = joypad.l1()
        #Game.action.TILT_RIGHT    = joypad.r1()
        #Game.action.TILT_UP       = joypad.l2()
        #Game.action.TILT_DOWN     = joypad.r2()
        Game.action.NEXT_POWERUP  = (joypad.x() == 1)
        Game.action.PREV_POWERUP  = (joypad.y() == 1)
        # Handle movement
        if   joypad.left() : Game.moveLeft()
        elif joypad.right(): Game.moveRight()

    @staticmethod
    def update(delta):
        Game.delta = delta
        Paddle.gfx[Game.paddle.getStatus()].update(int(delta * Game.time_scale))
        Game.updateMap()
        Game.updateTokens()
        Game.updatePaddle(input)
        Game.balls.update(delta)
        Game.particles.update(delta)
        Game.bricks.update(delta)
        if Game.powerupType < 0x10:
            Token.gfx[Game.powerupType].update(int(delta*Game.time_scale))
        
        paddle  = Game.getPaddle()
        theBall = Game.getTheBall()
        balls   = Game.getBalls()
        
        Game.resetInput()
        
        Game.readKeyboard()

        
        # Move paddle or brake it
        if Game.action.MOVE_LEFT:           Game.moveLeft()
        elif Game.action.MOVE_RIGHT:        Game.moveRight()
        elif not Game.action.MOVE_LEFT:     Game.brake()
        
        # readJoystick();
        
        if Game.action.NEXT_POWERUP:
            Game.setPowerupType( Game.nextPowerup[ Game.getPowerupType() ] )
            Game.action.NEXT_POWERUP = False
        elif Game.action.PREV_POWERUP:
            Game.setPowerupType( Game.prevPowerup[ Game.getPowerupType() ] )
            Game.action.PREV_POWERUP = False
        
        if Game.action.debug.GROW:          Game.paddle.grow()        
        if Game.action.debug.SHRINK:        Game.paddle.shrink()        
        if Game.action.debug.HIGLIGHT_BALL: Game.getParticles().generate(0x01, Game.theBall.x+2, Game.theBall.y+2)        
        if Game.action.debug.MULTIPLY:      Game.balls.subdivide()
        
        # Handle Music player
        if Game.action.debug.NEXT_SONG:
            Game.musicplayer.nextSong()
            Game.backdrop.setSpeed(Game.musicplayer.getTempo())
        else: 
            if Game.action.debug.ERASE_SONG:
                Game.musicplayer.deleteSong()
        Game.musicplayer.update(delta)
        
        if Game.action.debug.ABORT:
            raise Exception("Aborted by User")
        
        if Game.action.debug.FULLSCREEN:
            gc.setFullscreen(not gc.isFullscreen())
        
        if Game.action.ACTIVATE:
            ballsReady = False
            for b in Game.balls.getBalls():
                if b.status in [BallStatus.STICKED, BallStatus.READY]:
                    ballsReady = True
            
            #if paddle.getStatus() in [PaddleStatus.STICKY, PaddleStatus.READY]:
            if ballsReady:
               paddle.trigger(False)
               if paddle.getStatus() != PaddleStatus.STICKY:
                   paddle.status = PaddleStatus.PLAYING
            elif paddle.getStatus() != PaddleStatus.READY:                
                paddle.shoot()
            
            # Switch off
            Game.action.ACTIVATE = False
        
        # Handle timescale
        if Game.action.debug.TIMESCALE_DOUBLE:  
            Game.time_scale = 2.0
        else: 
            # Handle bullet time
            if (Game.action.BALLTIME) and (Game.getBulletTime() > 0.0000):
                Game.time_scale = 0.125
                Game.setBulletTime(Game.getBulletTime()-0.0125)
                bulletTime = Game.getBulletTime()
                if bulletTime > 0.02:
                    if Game.action.TILT_LEFT:
                        balls.alter(-bulletTime, 0)
                    elif Game.action.TILT_RIGHT:
                        balls.alter(bulletTime, 0)
                    if Game.action.TILT_UP:
                        balls.alter(0, -bulletTime)
                    elif Game.action.TILT_DOWN:
                        balls.alter(0, bulletTime)
            else:
                Game.time_scale = 1.0
                # Regenerate bullet Time
                bulletTime = Game.getBulletTime()
                if Game.getBulletTime() < 1.000: bulletTime += 0.0012
                else: bulletTime = 1.0000
                Game.setBulletTime(bulletTime)
        
        Game.backdrop.update(delta, Game.musicplayer, Game.time_scale)        
        #Game.update(delta, inputManager.input)        
        Game.hud.updateLcd(Game.getBulletTime())        
        Game.explosion.gfx.update(int(delta*Game.time_scale))

        
        if Game.isGameOver():
            # Call Gameover process 
            Game.Over()
            # Restart game 
            Game.New()

        #required stuff
        BasicGame.update(delta)

    @staticmethod
    def draw():
        Game.map.need_redraw = True  # remove this as you rewrite the whole fucking redraw system. >:(
        """
                
        //Explosion.gfx.draw(32, 32); 
        
        hud.render(game);
        
        """
        if Game.map.need_redraw:
            vpu.select(0)
            vpu.drawsurf(Game.buffer[0], 0, 0)
            vpu.drawsurf(Game.buffer[1], 0, 0)
            vpu.select(1)
            vpu.fill(0,0,0,0)
            Game.getBricks().render()
            Game.getBalls().render(Game.left, Game.top)
            Game.getPaddle().render(Game.left, Game.top)
            Game.drawTokens(Game.left, Game.top)
            Game.getParticles().render(Game.left, Game.top)
            Game.renderInventary(Game.left, Game.top)
            Game.map.need_redraw = False
        BasicGame.draw()
        # ...
        
    @staticmethod
    def setHud(hud):                
        Game.hud = hud   

    @staticmethod
    def isGameOver():               
        return Game.game_over

    @staticmethod   
    def setGameOver(game_over):      
        Game.game_over = game_over

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
        return Game.time_scale    

    @staticmethod
    def setTimeScale(time_scale):
        Game.time_scale = time_scale
        Game.balls.setTimeScale(time_scale)    

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
        Game.game_over      = False
        Game.level          = 0
        Game.lives          = 3
        Game.time_scale     = 1.0       
        Game.powerupType    = 0x00
        Game.level          = 0
        Game.score          = 0
        Game.bulletTime     = 1.0
        Game.balls          = BallSystem()  # Create balls 
        Game.newPaddle()                        # Create paddle
        Game.tokens         = {}                # Create token list 
        Game.particles      = ParticleSystem()
        Game.bricks         = BrickSystem() # Create brick system 
        Game.musicplayer.nextSong()
        Game.loadMap("sample") # Load map (must be done after creating bricksystem 
        debug("Game", "Game setup finished")
    
    @staticmethod
    def newPaddle():
        Game.paddle = Paddle(Game.balls.getTheBall(), Game)
        Game.paddle.status = PaddleStatus.READY
        Game.balls.setPaddle(Game.paddle)   # Update ballSystem paddle reference 
    
    @staticmethod
    def Over():
        # Game over process 
        Game.game_over = False
    
    @staticmethod
    def oneUp(lives):
        Game.lives += lives
    
    @staticmethod
    def oneDown(lives):
        Game.lives -= lives
        if Game.lives <= 0: 
            Game.game_over = True
        debug("Game", f"OneDown : lives = {Game.lives}")
    
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
        t = Game.accel
        if Game.accel < 0.0: Game.accel += 0.6
        if Game.accel > 0.0: Game.accel -= 0.6
        if Game.accel < 0.7 and Game.accel > -0.7: Game.accel = 0.0
        if Game.accel != t:
            Game.map.need_redraw = True
    
    @staticmethod
    def moveLeft():
        if Game.accel < 0.0: Game.accel =- Game.accel/2
        Game.paddle.setDelta(Game.accel + 1.0)
        Game.accel += 0.2
        if Game.accel > Game.maxAccel: Game.accel = Game.accel * 0.75
        Game.map.need_redraw = True
    
    @staticmethod
    def moveRight():
        if Game.accel > 0.0: Game.accel =- Game.accel / 2
        Game.paddle.setDelta( Game.accel - 1.0 )
        Game.accel -= 0.2
        if Game.accel < -Game.maxAccel: Game.accel = Game.accel * 0.5
        Game.map.need_redraw = True
    
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
        Game.paddle.status = PaddleStatus.PLAYING
        if   effectIndex == TokenType.SUBDIVIDE:   Game.balls.subdivide()
        elif effectIndex == TokenType.DIVIDE:      Game.balls.divide()
        elif effectIndex == TokenType.SHOOT:       Game.paddle.shoot()
        elif effectIndex == TokenType.MISSILE:     Game.paddle.shoot()
        elif effectIndex == TokenType.EXPLOSIVE:   Game.balls.setStatus(BallStatus.EXPLOSIVE)
        elif effectIndex == TokenType.ONEUP:       Game.oneUp(1)
        elif effectIndex == TokenType.ONEDOWN:     Game.oneDown(1)
        elif effectIndex == TokenType.ULTRABALL:   Game.balls.setStatus(BallStatus.ULTRA)
        elif effectIndex == TokenType.STICKBALL:   Game.balls.setStatus(BallStatus.STICKY); Game.paddle.status = PaddleStatus.STICKY
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
        return
        
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
            #Game.map = TiledMap(Game, 20, 30, 3)
            #Game.map.loadtmx(filename)
            Game.bricks.load(Game.map)
            debug("Game", "loadMap : Map loaded")
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
            t.update(Game.time_scale)
            if t.collides(Game.paddle):
                #Execute token effect and unlink it from list
                #TODO: Do something
                Game.bulletTime = 1.0
                #Game.powerupType = t.getType();
                Game.inventory.inheritToken(t.getType())
                Game.tokens.remove(t)
                continue
            if t.y >= 240:
                Game.tokens.remove(t)

    @staticmethod
    def drawTokens(left, top):
        for t in Game.getTokens():
            tokentype = t.getType()
            tok = Token.gfx[tokentype]
            tok.setframe(t.getFrame())
            tok.draw(left+3 + t.x, top+3 + t.y)

    @staticmethod
    def drawToken(x, y, tokentype):
        #last = Token.gfx[tokentype].getFrame()
        Token.gfx[tokentype].setframe(0)
        Token.gfx[tokentype].draw(x, y)
        #Token.gfx[tokentype].setCurrentFrame(last)
    
    @staticmethod
    def renderInventary(x, y):
        x+=6
        y+=8
        Game.drawToken(x+241,  y+50, TokenType.GROW)
        Game.drawToken(x+241,  y+58, TokenType.SHRINK)
        Game.drawToken(x+241,  y+66, TokenType.SUBDIVIDE)
        Game.drawToken(x+241,  y+74, TokenType.SHOOT)
        Game.drawToken(x+241,  y+82, TokenType.MISSILE)        
        Game.drawToken(x+281,  y+50, TokenType.EXPLOSIVE)
        Game.drawToken(x+281,  y+58, TokenType.ULTRABALL)
        Game.drawToken(x+281,  y+66, TokenType.STICKBALL)
        Game.drawToken(x+281,  y+74, TokenType.SHIELD)
        Game.drawToken(x+281,  y+82, TokenType.COMMODIN)        
        Token.gfx[Game.getPowerupType()].draw(x+268, y+112)
    
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