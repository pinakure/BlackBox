from animation import Animation
import vpu

class ParticleSystem:
    sprite  = None
    gfx     = None
    game    = None
    
    @staticmethod
    def initialize(game):
        ParticleSystem.game = game
        try:
            ParticleSystem.sprite = vpu.createsprite('particles')
            ParticleSystem.gfx = {}
            # Load particle graphics 
            i=0
            for y in range(0, 0x5):
                for x in range(0, 16):
                    ParticleSystem.gfx[i] = Animation(2, 2, ParticleSystem.sprite, x, y, x, y, False, 1, False)
                    i+=1
        except Exception as E:
            exception(E)

    def __init__(self):
        self.debris  = {}
        
    def getParticleGfx(self):
        return ParticleSystem.gfx

    def getParticleSheet(self):
        return ParticleSystem.sprite
    
    def render(self):
        for p in debris:
            p.draw(p.getX(), p.getY())
    
    def update(self, delta):
        timeScale = ParticleSystem.game.getTimeScale()
        for p in self.debris:
            p = debris.get(i)
            p.setTimeScale(timeScale)
            p.setAngle(p.getAngle()+1)
            p.update()
            if not p.isAlive():
                self.debris.remove(p)
    
    def generate(self, particle_type, x, y):
        for i in range(0, 8):
            self.debris.add(self.newParticle(particle_type, x, y))
    
    def newParticle(particle_type, x, y):
        p = Particle(2, 2, ParticleSystem.sprite)
        p.initialize(x, y, ParticleSystem.game.randInt(-20, 20), particle_type)
        p.setSpeed(1.4)
        return p
