import vpu
from debug import debug
from pixel import Pixel

class PseudoSurface:
    def __init__(self, handle, default_scale_x=1.0,  default_scale_y=1.0):
        self.handle = handle
        vpu.select(handle)
        self.default_scale  = Pixel(default_scale_x, default_scale_y)
        self.scale          = Pixel(default_scale_x, default_scale_y)
        self.enabled        = True
        self.rotation       = 0
        [self.width,self.height] = vpu.dimensions()
        self.width               = int(self.width)
        self.height              = int(self.height)
        debug('Vpu', f"Created Surface #{handle} : {self.width} x {self.height}")

    def setscale(self, x, y):
        self.scale.x = x
        self.scale.y = y
        vpu.setscale(self.handle, x, y)
        
    def setrotation(self, rotation):
        self.rotation = rotation
        vpu.setrotation(self.handle, rotation)

    def enable(self):           
        self.enabled = True
        vpu.enable(self.handle)

    def disable(self):          
        self.enabled = False
        vpu.disable(self.handle)

    def select(self):           
        Vpu.target = self
        vpu.select(self.handle)

    def restore(self, alpha=0):
        self.setscale(self.default_scale.x, self.default_scale.y)
        self.setrotation(0)
        self.select()
        vpu.fill(0,0,0,alpha) 
        self.enable()
                
class Vpu:
    width    = 320
    height   = 480
    bground  = None
    fground  = None
    overlay  = None
    target   = None

    @staticmethod
    def initialize():
        debug("Vpu", "Initializing")
        Vpu.bground = PseudoSurface(0, 2, 2)
        Vpu.fground = PseudoSurface(1, 2, 2)
        Vpu.overlay = PseudoSurface(2, 1, 1)

    @staticmethod
    def destroy():
        debug("Vpu", "Destroy")
        try:
            del Vpu.bground
            del Vpu.fground
            del Vpu.overlay
        except:
            error("Vpu", "Called destroy before initialize()")

    @staticmethod
    def update():
        vpu.update()