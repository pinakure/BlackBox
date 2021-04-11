from debug      import debug, deprecate, error, exception, panic
from animation  import Animation, AnimationLoop
import vpu 

class Explosion:
    sprite  = None
    gfx     = None    
    game    = None

    def initialize(game):
        Explosion.game = game
        try:
            Explosion.sprite = vpu.createsprite('explosion')
            Explosion.gfx = Animation(48,24, Explosion.sprite, 0, 0, 7, 0, False, .25, True, AnimationLoop.NONE)
        except Exception as E:
            exception(E)
