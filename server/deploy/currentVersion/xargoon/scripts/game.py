import blackbox
import data.scripts.lib
from data.scripts.lib import HudIcon, BigExplosion, SmallExplosion, Token, Projectile, Flame, Ship
import vpu

class Game:
    running = True
    width = 0
    height = 0
    dims = {}
    tokens = {}

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
        vpu.select(0); vpu.fill(0,0,0,255)
        vpu.select(1); vpu.fill(0,0,0,255)
        vpu.select(2); vpu.fill(255,255,0,255)
        # enable rendering back
        #vpu.enable(0)
        #vpu.enable(1)
        vpu.enable(2)
        # initialize subcomponents
        print("GAME: Initializing subcomponents...")
        HudIcon.initialize(Game)
        Token.initialize(Game)
        BigExplosion.initialize(Game)
        SmallExplosion.initialize(Game)
        #Flame.initialize(Game)
        #Projectile.initialize(Game)
        Ship.initialize(Game)
        print("GAME: Ready!")
        ###
        for i in range(0, Token.TYPE_MAX):
            Game.tokens[i] = Token(320-(i*16),120, i)
        
    @staticmethod
    def update():
        if blackbox.ctrlc():
            print("Control+C pressed.")
            Game.running = False
        
    @staticmethod
    def draw():
        HudIcon.draw()
        for i in range(0, Token.TYPE_MAX):
            Game.tokens[i].draw()
        vpu.update()
        
def main():
    Game.setup()
    
    while Game.running:
        Game.update()
        Game.draw()
