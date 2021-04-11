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
from data.scripts.ball              import Ball
from data.scripts.paddle            import Paddle
from data.scripts.ballsystem        import BallSystem
from data.scripts.explosion         import Explosion
from data.scripts.hitreaction       import HitReaction
from data.scripts.particle          import Particle
from data.scripts.particlesystem    import ParticleSystem
from Vpu                            import Vpu
        
class Game(BasicGame):

    particles   = None
    balls       = None
    
    @staticmethod
    def setup():
        BasicGame.prepare()
        BallSystem.initialize(Game)
        Explosion.initialize(Game)
        Paddle.initialize(Game)
        ParticleSystem.initialize(Game)
        Game.particles  = ParticleSystem()
        Game.balls      = BallSystem()
        
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
    def update(delta):
        # do stuff
        # ...

        BasicGame.update(delta)
        
    @staticmethod
    def draw():
        # clear buffer
        vpu.select(1)
        vpu.fill(0,0,0,0)
        
        #draw stuff
        # ...

        BasicGame.draw()
