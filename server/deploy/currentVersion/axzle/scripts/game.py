##############################################################################################################
#                                                                                                            #
#                        db         8b        d8  888888888888  88           88888888888                     #
#                       d88b         Y8,    ,8P            ,88  88           88                              #
#                      d8'`8b         `8b  d8'           ,88"   88           88                              #
#                     d8'  `8b          Y88P           ,88"     88           88aaaaa                         #
#                    d8YaaaaY8b         d88b         ,88"       88           88"""""                         #
#                   d8""""""""8b      ,8P  Y8,     ,88"         88           88                              #
#                  d8'        `8b    d8'    `8b   88"           88           88                              #
#                 d8'          `8b  8P        Y8  888888888888  88888888888  88888888888                     #
#                                                                                                            #
##############################################################################################################
# to import classes in this folder: 
# from data.scripts.ship              import Ship 
from scripts.main import menu
from random import random
import blackbox
import vpu
import joypad
from tiledmap import TiledMap
from basicgame import BasicGame

class TileStatus:
    NORMAL_VALUE = 0x00
    NORMAL_EMPTY = 0x01
    MARKED_VALUE = 0x02
    MARKED_EMPTY = 0x03
    PUSHED_VALUE = 0x04
    PUSHED_EMPTY = 0x05

try:
    import importlib 
    import data.scripts.map
    importlib.reload(data.scripts.map) 
except:
    print("It seems Map is not loaded; no need to reload it")
    pass
from data.scripts.map import data, overlay

class Puzzle:
    
    game   = None
    width  = 16
    height = 16
    x = 0
    y = 0

    data = {}

    @staticmethod 
    def initialize(game):
        Puzzle.game = game

    @staticmethod 
    def mark():
        if   Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.NORMAL_EMPTY: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.MARKED_EMPTY
        elif Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.NORMAL_VALUE: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.MARKED_VALUE

    @staticmethod 
    def press():
        if   Puzzle.data[Puzzle.y][Puzzle.x] in (TileStatus.NORMAL_EMPTY, TileStatus.MARKED_EMPTY): Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.PUSHED_EMPTY
        elif Puzzle.data[Puzzle.y][Puzzle.x] in (TileStatus.NORMAL_VALUE, TileStatus.MARKED_VALUE): Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.PUSHED_VALUE
        
    @staticmethod 
    def unmark():
        if   Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.MARKED_EMPTY: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.NORMAL_EMPTY
        elif Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.MARKED_VALUE: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.NORMAL_VALUE
        
    

    @staticmethod
    def setsize(width, height):
        Puzzle.width  = width
        Puzzle.height = height
        Puzzle.data = {}
        for y in range(0,height):
            Puzzle.data[y] = {}
            for x in range(0,width):
                Puzzle.data[y][x] = TileStatus.NORMAL_EMPTY

    @staticmethod
    def randomize():
        i = 0
        for y in range(0, Puzzle.height):
            for x in range(0, Puzzle.width):
                Puzzle.data[y][x] = TileStatus.NORMAL_EMPTY if int(random()*2) else TileStatus.NORMAL_VALUE

    @staticmethod
    def countvertical(column):
        count = {}
        broken = False
        sum = 0
        for y in range(0, Puzzle.height):
            if Puzzle.data[y][column] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE, TileStatus.PUSHED_VALUE):
                sum+=1
                continue
            if sum > 0:
                count[len(count)] = sum
                sum = 0
        if sum > 0:
            count[len(count)] = sum
        return count
    
    @staticmethod
    def counthorizontal(row):
        count = {}
        broken = False
        sum = 0
        for x in range(0, Puzzle.width):
            if Puzzle.data[row][x] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE, TileStatus.PUSHED_VALUE):
                sum+=1
                continue
            if sum > 0:
                count[len(count)] = sum
                sum = 0
        if sum > 0:
            count[len(count)] = sum
        return count
    
    @staticmethod
    def updategrid():
        ox = 22
        oy = 12
        i = 0
        for y in range(0, 16):
            for x in range(0, 16):
                Game.map.set(ox + x, oy + y, 0x10+(((Puzzle.data[y][x])%0x10)), 1)
                i+=1
        Game.map.set(ox + Puzzle.x, oy + Puzzle.y, 0x0D, 2)
                
    
    @staticmethod
    def updatehorizontalmarkers():
        ox      = 12
        oy      = 12
        c       = 0
        i       = True
        for y in range(0, 16):
            i = False if i else True
            for x in range(0, 10):
                active  = True if Puzzle.y == y else False
                if x == 0: c = 0x20 if active else 0x30 if i else 0x40
                elif x==9: c = 0x22 if active else 0x32 if i else 0x42            
                else: c = 0x21 if active else 0x31 if i else 0x41
                Game.map.set(ox + x, oy + y, c, 1)
            count = Puzzle.counthorizontal(y)
            x = 8
            for ci in count.__reversed__():
                c = count[ci]
                Game.map.set(ox + x, oy + y, (0x50 + c) if (c<10) else (0x60+c), 2)
                x-= 1
        
    @staticmethod
    def updateverticalmarkers():
        ox      = 22
        oy      = 2
        c       = 0
        i       = True
        for x in range(0, 16):
            i = False if i else True            
            for y in range(0, 10):
                active  = True if Puzzle.x == x else False
                if y == 0   : c = 0x23 if active else 0x25 if i else 0x24
                elif y==9   : c = 0x43 if active else 0x45 if i else 0x44
                else: c = 0x33 if active else 0x35 if i else 0x34
                Game.map.set(ox + x, oy + y, c, 1)
            count = Puzzle.countvertical(x)
            y = 8
            for ci in count.__reversed__():
                c = count[ci]
                Game.map.set(ox + x, oy + y, (0x50 + c) if (c<10) else (0x60+c), 2)
                y-= 1
    
    @staticmethod
    def updatemarkers():
        Puzzle.updatehorizontalmarkers()
        Puzzle.updateverticalmarkers()

    @staticmethod
    def draw():
        
        Puzzle.updategrid()
        Puzzle.updatemarkers()

    @staticmethod
    def up():
        Puzzle.y-=1
        if Puzzle.y < 0: Puzzle.y = Puzzle.height-1
        
    @staticmethod
    def down():
        Puzzle.y+=1
        if Puzzle.y > Puzzle.height-1: Puzzle.y = 0

    @staticmethod
    def left():
        Puzzle.x-=1
        if Puzzle.x < 0: Puzzle.x = Puzzle.width-1
        
    @staticmethod
    def right():
        Puzzle.x+=1
        if Puzzle.x > Puzzle.width-1: Puzzle.x = 0        
    
class Game(BasicGame):
    
    buffer = []
    area = None
    puzzle = None

    @staticmethod
    def setup():
        try:
            print("GAME: Setup...")
            BasicGame.prepare()
            vpu.setrotation(0,0)
            vpu.transition() 
            while not vpu.update():pass
            # change screen content here
            Game.buffer = [ vpu.createsurf(320, 240), vpu.createsurf(320, 240) ]
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 3, 8, 8))
            if not Game.map.load_tileset("tiles"):
                print("\n---------------------------------------------------------\nERROR: Cannot load 'tilesets/tiles.png'\n\tGame could run perfectly, but we think it's better to\n\tabort current execution, as you wouldn't be\n\table to see anything on the screen and\n\tthat would be definitely bad.\n---------------------------------------------------------\n")
                quit()
            Game.map.fill(0x00,0)
            Game.map.fill(0x0f,1)
            Game.map.fill(0x0f,2)
            Game.loadmap(data       , 0)
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )+160
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )+120
            Game.map.setsurface(Game.buffer[0])
            
            Puzzle.initialize(Game)
            Puzzle.setsize(16, 16)
            Puzzle.randomize()
            
            # enable rendering back
            vpu.enable(0)
            vpu.enable(1)
            vpu.enable(2)
            
            # set video scale to 2x
            vpu.setscale(0, 2.0, 2.0)
            vpu.setscale(1, 2.0, 2.0)
        except Exception as E:
            print(str(E))
            quit()

    @staticmethod
    def destroy():
        pass

    @staticmethod
    def title():
        vpu.select(0)
        vpu.fill(0,0,0)
        vpu.perlin(0, 160, 80,0)
        vpu.setfont('magic')
        vpu.setcolor(255,255,255)
        vpu.textout("AXZLE", int(Game.width>>1)-64,  int(Game.height>>1)-64 )
        while not vpu.update():pass        
        while not joypad.start():
            vpu.select(1)
            vpu.fill(0,0,0,0)
            if vpu.frames()%20>10:
                vpu.setfont('mana')        
                vpu.setcolor(255,255,255)
                vpu.textout("PRESS START BUTTON", int(Game.width>>1)-72,  int(Game.height>>1)+64 )
            vpu.update()
        vpu.transition(int(random()*20)) 
        while not vpu.update():pass
        vpu.select(0);vpu.fill(0,0,0)
        vpu.select(1);vpu.fill(0,0,0,0)
        vpu.select(2);vpu.fill(0,0,0,0)        
        vpu.setfont('ibm')

    @staticmethod
    def loop():
        delta = 1.0
        Game.title()
        Game.map.setactive()
        while Game.running:
            if Game.map.needsredraw():
                Game.map.fill(0xf, 1)
                Game.map.fill(0xf, 2)
                Puzzle.draw()
            # rasterize buffer onto bg layer
            vpu.select(0)
            left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
            top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
            for buffer in Game.buffer:
                if buffer:
                    vpu.drawsurf(buffer, left, top)
            
            BasicGame.draw()
            
            
            if joypad.b():             Puzzle.press()
            elif joypad.a():           Puzzle.unmark()
            elif joypad.y():           Puzzle.mark()
            if joypad.left()==1:       Puzzle.left()
            elif joypad.right()==1:    Puzzle.right()
            if joypad.up()==1:         Puzzle.up()
            elif joypad.down()==1:     Puzzle.down()
            #required stuff
            BasicGame.update(delta)
            
def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    Game.map.setactive(True)
    Game.map.setsurface(-1)
    return Game.destroy()