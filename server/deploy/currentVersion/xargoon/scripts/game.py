from data.scripts.ship              import Ship
from data.scripts.flame             import Flame
from data.scripts.projectile        import Projectile
from data.scripts.bigexplosion      import BigExplosion
from data.scripts.smallexplosion    import SmallExplosion
from data.scripts.token             import Token
from data.scripts.hudicon           import HudIcon
from random                         import random
import blackbox
import vpu

class Game:
    running = True
    width = 0
    height = 0
    dims = {}
    tokens = []
    explosions = []
    mini_explosions = []
    projectiles = []
    foes = []
    bigfoes = []
    ship = None

    @staticmethod
    def setup():
        print("GAME: Setting up...")
        Game.running = True
        # disable rendering
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
        # get layer dimensions
        vpu.select(0); Game.dims[0] = vpu.dimensions()
        vpu.select(1); Game.dims[1] = vpu.dimensions()
        vpu.select(2); Game.dims[2] = vpu.dimensions()
        print(f"GAME: BG Resolution: {Game.dims[0][0]} x {Game.dims[0][1]}")
        print(f"GAME: FG Resolution: {Game.dims[1][0]} x {Game.dims[1][1]}")
        print(f"GAME: OL Resolution: {Game.dims[2][0]} x {Game.dims[2][1]}")
        # prepare initial layer state
        vpu.select(0); vpu.fill(128,64,0,255)
        vpu.select(1); vpu.fill(0,0,0,0)
        vpu.select(2); vpu.fill(255,255,0,255)
        # enable rendering back
        vpu.enable(0)
        vpu.enable(1)
        vpu.enable(2)
        # initialize subcomponents
        print("GAME: Initializing subcomponents...")
        HudIcon.initialize(Game)
        Token.initialize(Game)
        BigExplosion.initialize(Game)
        SmallExplosion.initialize(Game)
        Flame.initialize(Game)
        Projectile.initialize(Game)
        Ship.initialize(Game)
        #Foe::initialize(Game)
        #BigFoe::initialize(Game)
        print("GAME: Ready!")
        #################################################
        ### Create test tokens
        for i in range(0, Token.TYPE_MAX):
            Game.tokens.append(Token(int(Game.dims[1][0]/2)-(i*16),int(Game.dims[1][1]/2), i))
        #################################################
        ### Create test explosions
        for i in range(0, 32):
            x = int(Game.dims[1][0]/4) + int(random()*(Game.dims[1][0]/2))-16
            y = int(Game.dims[1][1]/4) + int(random()*(Game.dims[1][1]/2))-16
            Game.explosions.append(BigExplosion(x,y))
        #################################################
        ### Create test small explosions
        for i in range(0, 32):
            x = int(Game.dims[1][0]/4) + int(random()*(Game.dims[1][0]/2))-16
            y = int(Game.dims[1][1]/4) + int(random()*(Game.dims[1][1]/2))-16
            Game.mini_explosions.append(SmallExplosion(x,y))
        #################################################
        ### Create test projectiles
        w = int(Game.dims[1][0]/2)
        h = int(Game.dims[1][1]/2)
        for i in range(0, 64):
            x = int(Game.dims[1][0]/2) + int(random()*(Game.dims[1][0]/4))-16
            y = int(Game.dims[1][1]/2) + int(random()*(Game.dims[1][1]/4))-16
            t = int(random()*1)
            l = int(random()*1)
            Game.projectiles.append(Projectile(x,y,t,l))
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)
        # Spawn the player
        Game.spawn()
        
    @staticmethod
    def spawn():
        w = int(Game.dims[1][0]/2)-16
        h = int(Game.dims[1][1]/2)
        Game.ship = Ship(w, h, Ship.TYPE_A)

    @staticmethod
    def destroy():
        Game.tokens = []
        Game.explosions = []
        Game.mini_explosions = []
        Game.projectiles = []
        Game.foes = []
        Game.bigfoes = []
        Game.ship = None

        Token.destroy()
        HudIcon.destroy()
        BigExplosion.destroy()
        SmallExplosion.destroy()
        Projectile.destroy()
        Flame.destroy()
        Ship.destroy()
        #Foe.destroy()
        #BigFoe.destroy()
        
    @staticmethod
    def loop():
        while Game.running:
            Game.draw()
            if blackbox.ctrlc():
                print("Control+C pressed.")
                Game.destroy()
                Game.running = False
        
    @staticmethod
    def drawTokens():
        for i in range(0, Token.TYPE_MAX):
            Game.tokens[i].draw()
    
    @staticmethod
    def drawExplosions():
        for explosion in Game.explosions:
            explosion.draw()
        for explosion in Game.mini_explosions:
            explosion.draw()
    
    @staticmethod
    def drawProjectiles():
        for projectile in Game.projectiles:
            projectile.draw()

    @staticmethod
    def draw():
        # Draw hud (drawn on self-managed layer)
        HudIcon.draw()
        # pre-clean foreground layer
        vpu.select(1)
        vpu.fill(0,0,0,0)
        # Draw tokens (drawn on foreground layer)        
        Game.drawTokens()
        # Draw explosions (drawn on foreground layer)       
        Game.drawExplosions()
        # Draw Particles (drawn on foreground layer)       
        Game.drawProjectiles()
        # Draw Ship (drawn on foreground layer)       
        Game.ship.draw()
        # raster screen and update input
        vpu.update()