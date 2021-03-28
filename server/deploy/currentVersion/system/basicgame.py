from scripts.main import menu
import blackbox
import vpu
import joypad

try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass

class BasicGame:
    running = True
    dims = {}
    width = 0
    height = 0
    scale = 2.0
    score = 0
    time = 0
    autoredraw = True
    map = None
    

    @staticmethod
    def getdimensions():
        vpu.select(0); BasicGame.dims[0] = vpu.dimensions()
        vpu.select(1); BasicGame.dims[1] = vpu.dimensions()
        vpu.select(2); BasicGame.dims[2] = vpu.dimensions()
        print(f"GAME: BG Resolution: {BasicGame.dims[0][0]} x {BasicGame.dims[0][1]}")
        BasicGame.width  = int (BasicGame.dims[0][0])
        BasicGame.height = int (BasicGame.dims[0][1])
        print(f"GAME: FG Resolution: {BasicGame.dims[1][0]} x {BasicGame.dims[1][1]}")
        print(f"GAME: OL Resolution: {BasicGame.dims[2][0]} x {BasicGame.dims[2][1]}")
        
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
        vpu.select(0); vpu.fill(0,0,0,0)
        vpu.select(1); vpu.fill(0,0,0,0)
        vpu.select(2); vpu.fill(0,0,0,0)

    @staticmethod
    def setscale():
        vpu.setscale(0, BasicGame.scale  , BasicGame.scale)
        vpu.setscale(1, BasicGame.scale  , BasicGame.scale)
        vpu.setscale(2, BasicGame.scale/2, BasicGame.scale/2)

    @staticmethod
    def loadmap(data=[], layer=0):
        #BasicGame.map.fill(0x00)
        #load map...
        i=0
        try:
            for y in range(0, BasicGame.map.height):
                for x in range(0,  BasicGame.map.width):
                    BasicGame.map.setvalue(x,y, data[i], layer)
                    i+=1
        except:
            print("WARNING: Insufficient data provided to fill map!")

    @staticmethod
    def prepare():
        print("GAME: Prepare...")
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
    def update(delta):
        if blackbox.ctrlc():
            print("Control+C pressed.")
            BasicGame.destroy()
            BasicGame.running = False
            
        if joypad.menu():
            menu()

        if BasicGame.time % 16==0 and BasicGame.autoredraw:
            BasicGame.map.redraw()

        BasicGame.time += 1

    @staticmethod
    def destroy():
        pass