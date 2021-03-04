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
    projectiles = []
    foes = []
    bigfoes = []
    ship = None
    last_recycled_projectile = 0

    @staticmethod
    def setup():
        #gc.disable()
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
        for i in range(0, 16):
            Game.randomToken()
        #################################################
        ### Create test explosions
        for i in range(0, 32):
            Game.randomExplosion(BigExplosion)
        #################################################
        ### Create test small explosions
        for i in range(0, 32):
            Game.randomExplosion(SmallExplosion)
        #################################################
        ### Create projectile pool
        w = int(Game.dims[1][0]/2)
        h = int(Game.dims[1][1]/2)
        for i in range(0, 512):
            Game.projectiles.append(Projectile(0,0,0,0))
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)
        # Spawn the player
        Game.spawn()
        
    @staticmethod
    def recycle_projectile(x,y,t,l,angle=0):
        if Game.last_recycled_projectile == len(Game.projectiles)-1: Game.last_recycled_projectile = 0
        for key, projectile in enumerate(Game.projectiles, Game.last_recycled_projectile):
            if not projectile.alive:
                projectile.alive = True
                projectile.x = x
                projectile.y = y
                projectile.t = t
                projectile.l = l
                #calculate delta based on original position & angle
                projectile.delta_x = 0.0
                projectile.delta_y = 1.0
                Game.last_recycled_projectile = key
                return

    @staticmethod
    def randomExplosion(baseclass):
        x = int(Game.dims[1][0]/4) + int(random()*(Game.dims[1][0]/2))-16
        y = int(Game.dims[1][1]/4) + int(random()*(Game.dims[1][1]/2))-16
        s = baseclass(x,y)
        s.time += int(random()*30)
        Game.explosions.append(s)
    
    @staticmethod
    def randomToken():
        x = int(Game.dims[1][0]/4) + int(random()*(Game.dims[1][0]/2))-16
        y = int(Game.dims[1][1]/4) + int(random()*(Game.dims[1][1]/2))-16
        token_type = int(random()*(Token.TYPE_MAX))
        Game.tokens.append(Token(x, y, token_type))        

    @staticmethod
    def spawn():
        w = int(Game.dims[1][0]/2)-16
        h = int(Game.dims[1][1]/2)
        Game.ship = Ship(w, h, Ship.TYPE_A)

    @staticmethod
    def destroy():
        #gc.enable()
        Game.tokens = []
        Game.explosions = []
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
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)
            if blackbox.ctrlc():
                print("Control+C pressed.")
                Game.destroy()
                Game.running = False
            
    @staticmethod
    def update(delta):
        HudIcon.update(delta)
        for token in Game.tokens:
            if token.alive: token.update(delta)
            else: token.spawn()
        for explosion in Game.explosions:
            if explosion.alive: explosion.update(delta)
            else: explosion.spawn()
        for projectile in Game.projectiles:
            if projectile.alive: projectile.update(delta)
            #else: Game.recycle_projectile(x,y,t,l)
        Game.ship.update(delta)
        
    @staticmethod
    def draw():
        # Draw hud (drawn on self-managed layer)
        HudIcon.draw()
        # pre-clean foreground layer
        vpu.select(1)
        vpu.fill(0,0,0,0)
        for token in Game.tokens:            
            if token.alive: token.draw()
        for explosion in Game.explosions:            
            if explosion.alive:explosion.draw()
        for projectile in Game.projectiles:
            projectile.draw()        
        # Draw Ship (drawn on foreground layer)       
        Game.ship.draw()
        # raster screen and update input
        vpu.update()


def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()