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
from debug                  import debug, error, panic

class Water:
    DRY         = 0x10000
    FALL_LEFT   = 0x00100
    FALL_RIGHT  = 0x00200
    FALL_CENTER = 0x00400
    FALL_BOTH   = 0x00300
    EMPTY = 0x10
    map = None
    game = None

    def initialize(game):
        Water.game = game
        Water.map = TiledMap(game, 20,15,1,1,1)
        Water.clear()

    def clear():
        Water.map.fill(Water.DRY)
        Water.game.map.fill(Water.EMPTY)
        
    def update():
        for y in range(0, 15):
            for x in range(0, 20):
                c = Water.map.get(x,y,0) 
                falling_status = ((c&0xFF00)>>8) & 0xF
                wet_status = (c&0x00FF) &  0xF
                empty = ((c&0xF0000)>>16) & 0x1
                Water.game.map.set(x,y, Water.EMPTY if empty else 0x00 + wet_status     , 0)
                Water.game.map.set(x,y, Water.EMPTY if empty else 0x10 + (falling_status& 0x7) , 1)
                Water.game.map.set(x,y, Water.EMPTY if empty else 0x30 + falling_status , 2)
                Water.game.map.set(x,y, Water.EMPTY if empty else 0x20 + wet_status     , 3)

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
        Water.map.set(0x2,7, 0x00002)
        Water.map.set(0x4,7, 0x00106)
        Water.map.set(0x6,7, 0x00208)
        Water.map.set(0x8,7, 0x00406)
        Water.map.set(0xA,7, 0x00302)
        Water.map.set(0xC,7, 0x00702)
        Water.map.set(0xE,7, 0x00F02)
        
        
    @staticmethod
    def loop():
        delta = 1.0
        Game.map.need_redraw = True
        while Game.running:
            Water.update()
            if Game.map.needsredraw() or Game.map.need_redraw:
                # draw to buffer[1]
                #vpu.select(Game.buffer[1])
                #vpu.setcolor(255,0,0)
                #vpu.fill(0,0,0,0)
                #scroll = Game.map.getscroll()
                #vpu.textout(f"{scroll.x},{scroll.y}", 0, 0)
                # rasterize buffer onto bg layer
                vpu.select(0)
                left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
                top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
                for buffer in Game.buffer:
                    if buffer:
                        vpu.drawsurf(buffer, left, top)
                # required stuff
            BasicGame.draw()
                
            #if joypad.left():       Game.map.scroll_left()
            #elif joypad.right():    Game.map.scroll_right()
            #if joypad.up():         Game.map.scroll_up()
            #elif joypad.down():     Game.map.scroll_down()
            # required stuff
            BasicGame.update(delta)
            