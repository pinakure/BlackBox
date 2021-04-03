from random         import random
from scripts.main   import menu
import blackbox
import vpu
import joypad

from Vpu import Vpu

try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass

from pixel                  import Pixel        
from debug                  import debug

class BasicGame:
    running = True
    width = 0
    height = 0
    scale = 2.0
    score = 0
    time = 0
    autoredraw = True
    map = None
    buffer = []    
    rumble_x = 0
    rumble_y = 0
    rumble_x_time = 0
    rumble_y_time = 0
    scroll = Pixel(0,0)

    @staticmethod
    def getdimensions():
        BasicGame.width  = Vpu.bground.width
        BasicGame.height = Vpu.bground.height
        debug('BasicGame', f"Resolution {BasicGame.width}x{BasicGame.height}")

    @staticmethod
    def disable_all():
        # disable rendering
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
    
    @staticmethod
    def enable_all():
        # disable rendering
        vpu.enable(0)
        vpu.enable(1)
        vpu.enable(2)
        
    @staticmethod
    def clear():
        vpu.select(Vpu.bground.handle); vpu.fill(0,0,0,0)
        vpu.select(Vpu.fground.handle); vpu.fill(0,0,0,0)
        vpu.select(Vpu.overlay.handle); vpu.fill(0,0,0,0)

    @staticmethod
    def setscale():
        vpu.setscale(Vpu.bground.handle, BasicGame.scale  , BasicGame.scale)
        vpu.setscale(Vpu.fground.handle, BasicGame.scale  , BasicGame.scale)
        vpu.setscale(Vpu.overlay.handle, BasicGame.scale/2, BasicGame.scale/2)

    @staticmethod
    def loadmap(data=[], layer=0):
        BasicGame.map.load(data,layer)
        """
        #BasicGame.map.fill(0x00)
        #load map...
        i=0
        try:
            for y in range(0, BasicGame.map.height):
                for x in range(0,  BasicGame.map.width):
                    BasicGame.map.setvalue(x,y, data[i], layer)
                    i+=1
        except:
            debug('BasicGame', "WARNING: Insufficient data provided to fill map!")
        """

    @staticmethod
    def prepare():
        BasicGame.autoredraw = True
        Vpu.initialize()
        BasicGame.running = True        
        # disable rendering 
        BasicGame.disable_all()
        # get layer dimensions
        BasicGame.getdimensions()
        # prepare initial layer state
        BasicGame.clear()
        # enable rendering back
        BasicGame.enable_all()
        # set scale        
        BasicGame.setscale()

    @staticmethod
    def draw():
        # raster screen and update input
        vpu.update()
        # the only moment to take a snapshot is here
        #BasicGame.clear()

    @staticmethod
    def setmap(map):
        BasicGame.map = map

    @staticmethod
    def rumble(q):
        BasicGame.rumble_x_time += q
        BasicGame.rumble_y_time += q
    
    @staticmethod
    def update(delta):
        if blackbox.ctrlc():
            debug("BasicGame", "Control+C pressed.")
            BasicGame.destroy()
            BasicGame.running = False
            return
            
        if joypad.menu():
            menu()

        if BasicGame.time % 4==0 and BasicGame.autoredraw:
            BasicGame.map.redraw()

        BasicGame.rumble_x_time = BasicGame.rumble_x_time -1 if BasicGame.rumble_x_time > 0 else BasicGame.rumble_x_time
        BasicGame.rumble_y_time = BasicGame.rumble_y_time -1 if BasicGame.rumble_y_time > 0 else BasicGame.rumble_y_time
        BasicGame.rumble_x = (int(random()*3)-1) if BasicGame.rumble_x_time > 0 else 0
        BasicGame.rumble_y = (int(random()*3)-1) if BasicGame.rumble_y_time > 0 else 0
            
        BasicGame.time += 1

    @staticmethod
    def destroy():
        debug("BasicGame", "Destroying")
        Vpu.bground.restore(255)
        Vpu.fground.restore()
        Vpu.overlay.restore()
        Vpu.destroy()
        
        vpu.select(0)
        vpu.perlin(255,0,128)
        
        if BasicGame.map is not None:
            debug("BasicGame", "Destroying Map")
            BasicGame.map.setactive(True)
            BasicGame.map.setsurface(-1)
            BasicGame.map.settarget(-1)
            del BasicGame.map

        for buffer in BasicGame.buffer:
            debug("BasicGame", "Destroying buffer")
            vpu.deletesurf(buffer)
            buffer = None
        BasicGame.buffer = []
        vpu.setfont('ibm')
        
            