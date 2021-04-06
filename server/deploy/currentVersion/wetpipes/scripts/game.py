import blackbox
import vpu
import joypad
import console
try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass

#from data.scripts.foo      import bar
from scripts.main           import menu
from random                 import random
from basicgame              import BasicGame
from tiledmap               import TiledMap
from debug                  import debug, error, panic, exception
class WaterCell:
    def __init__(self, x,y,value):
        self.x = x
        self.y = y
        self.falling = 0x0
        self.block   = False
        self.q       = 0x0
        self.empty   = False        
        self.value   = 0x0
        self.set(value)

    def setq(self, q):
        self.q = q
        self.empty = True if (q == 0 and self.falling == 0x0) else False
        self.value = (self.value & 0x2FF00) | (int(self.q) & 0xFF) | ((1 << 16) if self.empty else 0)

    @staticmethod
    def average(item_list = []):
        accumulated = 0
        t = 0
        for i in item_list:
            if i.block: continue
            t+=1
            accumulated += i.q            
        if t>0:
            return accumulated / t            
        return 0
        
    def set(self, value):
        self.falling = ((value&0xFF00)>>8) & 0xF
        self.q       = (value&0x00FF) &  0xF
        self.empty   = ((value&0x10000)>>16) 
        self.block   = ((value&0x20000)>>17) 
        self.value   = value

    def update(self, delta=1.0):
        # check  x , y+1; if no ground, lose 1 unit on x,y+1 (fall middle gfx)
        # check x-1, y+1; if no ground left, lose 1 on x-1, y+1 (fall right gfx)
        # check x+1, y+1; if no ground right, lose 1 on x+1, y+1 (fall left gfx)
        # check x-1,  y ; if q < this.q donate 1 unit on x-1,y (no fall gfx)
        # check x+1,  y ; if q < this.q donate 1 unit to x+1,y (no fall gfx)
        pass
        

class WaterGrid:
    def __init__(self, w,h):
        self.width = w
        self.height= h
        self.cells = {}
        for y in range(0, self.height):
            self.cells[y] = {}
            for x in range(0, self.width):
                self.cells[y][x] = WaterCell(x,y,Water.DRY)

    def fill(self, value):
        for y in range(0, self.height):
            for x in range(0, self.width):
                self.cells[y][x].set(value)
    
    def set(self, x, y, value):
        self.cells[y][x].set(value)

    def get(self, x, y):
        return self.cells[y][x].value

    def clone(self):
        ret  = {}
        for y in range(0, self.height):
            ret[y] = {}
            for x in range(0, self.width):
                ret[y][x] = WaterCell(x,y,self.cells[y][x].value)
        return ret
    
    def update(self, delta=1.0):
        data = self.clone()
        for y in range(0, self.height):
            for x in range(0, self.width):
                cell = self.cells[y][x]
                if cell.block: continue
                if x==0 or x == self.width-1:
                    cell.setq(WaterCell.average([data[y][x], data[y][(x+1) if x < (self.width-1) else (x-1)]]))
                else:
                    cell.setq(WaterCell.average([data[y][x-1],data[y][x],data[y][x+1]]))
        
    

class Water:
    DRY         = 0x10000
    FALL_LEFT   = 0x00100
    FALL_RIGHT  = 0x00200
    FALL_CENTER = 0x00400
    FALL_BOTH   = 0x00300
    EMPTY = 0x10
    BLOCK = 0x1A
    map = None
    game = None

    def initialize(game):
        Water.game = game
        Water.map = WaterGrid(20,15)
        Water.clear()

    def clear():
        Water.map.fill(Water.DRY)
        Water.game.map.fill(Water.EMPTY)
        
    def updatecell(x,y):
        c = Water.map.get(x,y,0) 
        c.update()
        return c.value

    def update():
        for y in range(0, 15):
            r = Water.map.cells[y]
            for x in range(0, 20):
                c = r[x]
                c.update()
                Water.game.map.set(x,y, Water.BLOCK if c.block else Water.EMPTY if c.empty else 0x00 + int(c.q)           , 0)
                Water.game.map.set(x,y, Water.EMPTY if c.empty else 0x10 + (c.falling & 0x7)  , 1)
                Water.game.map.set(x,y, Water.EMPTY if c.empty else 0x30 + c.falling          , 2)
                Water.game.map.set(x,y, Water.EMPTY if c.empty else 0x20 + int(c.q)           , 3)
        Water.map.update()
                

class Game(BasicGame):

    @staticmethod
    def setup():
        BasicGame.prepare()
        Game.setmap(TiledMap(Game, 20,15,4, 16,16))
        Game.map.load_tileset('water')
        Game.buffer = [ vpu.createsurf(320,240) ]
        BasicGame.autoredraw = False
        Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
        Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
        Game.map.setactive() # tell engine we want this map to be rendered automatically on vpu.update(), right after entities
        Game.map.setsurface(Game.buffer[0]) # tell engine which surface the map must be rendered onto
        Game.map.redraw()        
        Water.initialize(Game)
        Water.map.set( 5,7, 0x30000)
        Water.map.set(10,7, 0x0000F)
        Water.map.set(15,7, 0x30000)
        
        Water.map.set(0,7, 0x30000)
        Water.map.set(19,7, 0x30000)
        for i in range(0,20):
            Water.map.set(i,8, 0x30000)
        #Water.map.set(0x6,7, 0x00208)
        #Water.map.set(0x8,7, 0x00406)
        #Water.map.set(0xA,7, 0x00302)
        #Water.map.set(0xC,7, 0x00702)
        #Water.map.set(0xE,7, 0x00F02)
    
        
    @staticmethod
    def loop():
        delta = 1.0
        Game.map.need_redraw = True
        while Game.running:
            if Game.map.needsredraw() or Game.map.need_redraw:
                Water.update()            
                vpu.select(0)
                vpu.fill(16,0,0,255)        
                left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
                top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)                
                vpu.drawsurf(Game.buffer[0], left, top)
                vpu.select(Game.buffer[0])
                vpu.fill(32,32,48,255)        
            BasicGame.draw()
                
            #-----------------------------------------------
            time = blackbox.epoch()
            while not blackbox.epoch() > time:                
                vpu.update()
            Game.map.need_redraw = True
            Game.map.redraw()
            #-----------------------------------------------
                
            if joypad.a():           Water.map.set(10,7, 0x0000F)
            #elif joypad.right():    Game.map.scroll_right()
            #if joypad.up():         Game.map.scroll_up()
            #elif joypad.down():     Game.map.scroll_down()
            # required stuff
            BasicGame.update(delta)
