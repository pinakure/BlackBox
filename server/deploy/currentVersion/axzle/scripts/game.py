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
from scripts.main import menu
from random import random
import blackbox
import vpu
import joypad
from tiledmap   import TiledMap
from basicgame  import BasicGame
from animation  import Animation
from debug      import debug, error

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
from data.scripts.map import data, overlay, title_data, title_bg

class PuzzleSize:
    SMALL   = 0x00
    MEDIUM  = 0x01
    BIG     = 0x02

class Puzzle:
    
    game   = None
    width  = 16
    height = 16
    x = 0
    y = 0

    data = {}

    gfx = {}

    small = {}
    medium = {}
    big = {}

    sizes = {
        PuzzleSize.SMALL    : 8,
        PuzzleSize.MEDIUM   : 16,
        PuzzleSize.BIG      : 32,
    }

    @staticmethod 
    def loadlevels(sprite, levels, size):
        size = Puzzle.sizes[size]
        w = int(128 / size)
        h = int(128 / size)
        vpu.selectsprite(sprite)
        oy = 0
        ox = 0
        y  = 0
        x  = 0
        for level in range(0, w*h):
            levels[level] = {}
            for y in range(0, size):
                levels[level][y] = {}
                for x in range(0, size):
                    levels[level][y][x] = TileStatus.NORMAL_EMPTY if vpu.getpixel(ox+x, oy+y) == -0x1 else TileStatus.NORMAL_VALUE
            ox+=size
            if ox>=128:
                ox = 0
                oy += size
        
    def loadlevel(level, size):
        Puzzle.clear()
        Puzzle.setsize(Puzzle.sizes[size],Puzzle.sizes[size])
        i = 0
        levels = Puzzle.small if size == PuzzleSize.SMALL else Puzzle.medium if size == PuzzleSize.MEDIUM else Puzzle.big
        for y in range(0, Puzzle.height):
            for x in range(0, Puzzle.width):
                Puzzle.data[y][x] = levels[level][y][x]
        Puzzle.game.time_left = 600
        Puzzle.game.time_offset = blackbox.epoch()
        Puzzle.game.total_count = 1
        
    @staticmethod 
    def initialize(game):
        Puzzle.game   = game
        #gfx_small  = vpu.createsprite('small_color')
        gfx_medium = vpu.createsprite('medium_color')
        #gfx_big    = vpu.createsprite('big_color')
        #Puzzle.gfx[PuzzleSize.BIG   ] = vpu.createanim(gfx_small , Puzzle.sizes[PuzzleSize.SMALL ], Puzzle.sizes[PuzzleSize.SMALL ], 0, 0, int(128 / Puzzle.sizes[PuzzleSize.SMALL ]), 1, False, 0, False)
        Puzzle.gfx[PuzzleSize.MEDIUM] =  Animation(
            Puzzle.sizes[PuzzleSize.MEDIUM], Puzzle.sizes[PuzzleSize.MEDIUM],
            gfx_medium, 
            0, 0, 
            int(128 / Puzzle.sizes[PuzzleSize.MEDIUM]), 0, 
            False, 0, False
        )
        #Puzzle.gfx[PuzzleSize.SMALL ] = vpu.createanim(gfx_big   , Puzzle.sizes[PuzzleSize.BIG   ], Puzzle.sizes[PuzzleSize.BIG   ], 0, 0, int(128 / Puzzle.sizes[PuzzleSize.BIG   ]), 1, False, 0, False)
        small   = vpu.createsprite('small')
        medium  = vpu.createsprite('medium')
        big     = vpu.createsprite('big')
        debug("Puzzle", "Loading levels.......0%", end="\r")
        Puzzle.loadlevels(small  , Puzzle.small  , PuzzleSize.SMALL)
        debug("Puzzle", "Loading levels...33.33%", end="\r")
        Puzzle.loadlevels(medium , Puzzle.medium , PuzzleSize.MEDIUM)
        debug("Puzzle", "Loading levels...66.66%", end="\r")
        Puzzle.loadlevels(big    , Puzzle.big    , PuzzleSize.BIG)
        debug("Puzzle", "Loading levels.....100%   ")
        
    @staticmethod
    def clear():
        Puzzle.game.map.fill(0x00,0)
        Puzzle.game.map.fill(0x0f,1)
        Puzzle.game.map.fill(0x0f,2)
        Puzzle.game.loadmap(data, 0)
        Puzzle.game.map.setsurface(Puzzle.game.buffer[0])

    @staticmethod 
    def mark():
        if   Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.NORMAL_EMPTY: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.MARKED_EMPTY
        elif Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.NORMAL_VALUE: Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.MARKED_VALUE

    @staticmethod 
    def press():
        if   Puzzle.data[Puzzle.y][Puzzle.x] in (TileStatus.NORMAL_EMPTY, TileStatus.MARKED_EMPTY): 
            Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.PUSHED_EMPTY
            if Game.score > 0: 
                BasicGame.rumble(16)
                Game.score -= 10
            else: 
                BasicGame.rumble(32)
                Game.timepenalty()
        elif Puzzle.data[Puzzle.y][Puzzle.x] in (TileStatus.NORMAL_VALUE, TileStatus.MARKED_VALUE): 
            Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.PUSHED_VALUE
            Puzzle.game.correct()
            if Game.total_count == 0:
                Game.reveal()        
        
    @staticmethod 
    def unmark():
        if   Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.MARKED_EMPTY: 
            Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.NORMAL_EMPTY
        elif Puzzle.data[Puzzle.y][Puzzle.x] == TileStatus.MARKED_VALUE: 
            Puzzle.data[Puzzle.y][Puzzle.x] = TileStatus.NORMAL_VALUE
    
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
            #if Puzzle.data[y][column] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE, TileStatus.PUSHED_VALUE):
            if Puzzle.data[y][column] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE):
                sum+=1
                continue
            if sum > 0:
                count[len(count)] = sum
                sum = 0
        if sum > 0:
            count[len(count)] = sum
        return count if len(count) > 0 else { 0:0 }
    
    @staticmethod
    def counthorizontal(row):
        count = {}
        broken = False
        sum = 0
        for x in range(0, Puzzle.width):
            # if Puzzle.data[row][x] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE, TileStatus.PUSHED_VALUE):
            if Puzzle.data[row][x] in (TileStatus.MARKED_VALUE, TileStatus.NORMAL_VALUE):
                sum+=1
                continue
            if sum > 0:
                count[len(count)] = sum
                sum = 0
        if sum > 0:
            count[len(count)] = sum
        return count if len(count) > 0 else { 0:0 }
    
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
        Game.total_count = 0
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
                Game.total_count += c    
                c = 0x36 + c if c < 10 else 0x46+(c-10) if c < 20 else 0x53+(c-20)
                Game.map.set(ox + x, oy + y, c, 2)
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
                c = 0x36 + c if c < 10 else 0x46+(c-10) if c < 20 else 0x53+(c-20)
                Game.map.set(ox + x, oy + y, c, 2)
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
        if joypad.b()>0: Puzzle.press()
        
    @staticmethod
    def down():
        Puzzle.y+=1
        if Puzzle.y > Puzzle.height-1: Puzzle.y = 0
        if joypad.b()>0: Puzzle.press()

    @staticmethod
    def left():
        Puzzle.x-=1
        if Puzzle.x < 0: Puzzle.x = Puzzle.width-1
        if joypad.b()>0: Puzzle.press()
        
    @staticmethod
    def right():
        Puzzle.x+=1
        if Puzzle.x > Puzzle.width-1: Puzzle.x = 0        
        if joypad.b()>0: Puzzle.press()
    
class Game(BasicGame):
    
    time_offset = 0
    time_left   = 0
    total_count = 0

    reveal_time = 0

    area = None
    puzzle = None

    level = 0

    @staticmethod
    def timepenalty():
        Game.time_left -= 15
        #TODO: play SFX
        #TODO: state time loss (red print over clock?)
    
    @staticmethod
    def correct():
        #TODO: play sfx
        Game.score += 100
        Puzzle.updatemarkers()
    
    @staticmethod
    def setup():
        try:
            debug("Game", "Setting up")
            BasicGame.prepare()
            vpu.transition() 
            while not vpu.update():pass
            # change screen content here
            Game.buffer = [ vpu.createsurf(320, 240), vpu.createsurf(320, 240) ]
            # create display map
            Game.setmap(TiledMap(Game, 40, 30, 3, 8, 8))
            if not Game.map.load_tileset("tiles"):
                panic("Game", "Cannot load 'tilesets/tiles.png'", "Game could run perfectly, but we think it's better to abort current execution, as you wouldn't be able to see anything on the screen and that would be definitely bad.")
                
            Puzzle.initialize(Game)
            Puzzle.clear()
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
            panic(str(E))

    @staticmethod
    def destroy():
        pass

    @staticmethod
    def title():
        Game.map.setsurface(0)
        #Game.map.setactive()
        Game.map.setposition(
            (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1), 
            (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
        )
        vpu.select(0)
        vpu.fill(0,0,0)
        vpu.perlin(0, 160, 80,0)
        Puzzle.game.loadmap(title_bg, 0)
        Puzzle.game.loadmap(title_data, 1)        
        Game.map.draw()
        #vpu.setfont('magic')
        #vpu.setcolor(255,255,255)
        #vpu.textout("AXZLE", int(Game.width>>1)-64,  int(Game.height>>1)-64 )
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
        #Puzzle.clear()
        Puzzle.loadlevel(Game.level, PuzzleSize.MEDIUM)
        vpu.select(0);vpu.fill(0,0,0)
        vpu.select(1);vpu.fill(0,0,0,0)
        vpu.select(2);vpu.fill(0,0,0,0)        
        vpu.setfont('smk')

    @staticmethod
    def reveal():
        if Game.reveal_time <= 0:
            Game.reveal_time = 500

    @staticmethod
    def nextlevel():
        Game.level+=1
        Puzzle.loadlevel(Game.level, PuzzleSize.MEDIUM)        

    @staticmethod
    def destroy():
        debug("Game", "Destroying()")
        
    @staticmethod
    def loop():
        delta = 1.0
        Game.title()
        left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
        top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
        Game.map.setposition(0,0)
        Game.map.draw()
        for buffer in Game.buffer:
            if buffer:
                vpu.drawsurf(buffer, left, top)
        Game.map.setactive() # set this map to be autoredrawn when calling vpu.update()
        while Game.running:
            if Game.map.needsredraw():
                Game.map.fill(0xf, 1)
                Game.map.fill(0xf, 2)
                Puzzle.draw()
            # rasterize buffer onto bg layer
            
            vpu.select(Game.buffer[1])
            vpu.fill(0,0,0,0)
            vpu.setcolor(255,255,255,255)
            vpu.textout(f"SCORE   {Game.score}", 16, 16)
            vpu.textout(f" LEFT   {Game.total_count}", 16, 32)
            vpu.textout(f" TIME   {Game.time_left}", 16, 48)
            
            if Game.reveal_time > 0:
                vpu.select(2)
                vpu.setscale(2, 8, 8)
                vpu.enable(2)
                vpu.fill(0,0,0,0)
                Puzzle.gfx[PuzzleSize.MEDIUM].setframe(Game.level)
                Puzzle.gfx[PuzzleSize.MEDIUM].draw(322+(Puzzle.sizes[PuzzleSize.MEDIUM]>>1), 222+(Puzzle.sizes[PuzzleSize.MEDIUM]>>1))
                Game.reveal_time-=1
            elif Game.total_count==0:
                Game.nextlevel()

            vpu.select(0)
            left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)+(Game.rumble_x)
            top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)+(Game.rumble_y)
            for buffer in Game.buffer:
                if buffer:
                    vpu.drawsurf(buffer, left, top)
            
            BasicGame.draw()            
            
            now = blackbox.epoch()
            if Game.time_offset < now:
                Game.time_left-=1
                Game.time_offset = now
            if Game.time_left <= 0:
                Game.gameover()
            
            if joypad.b()==1:          Puzzle.press()
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
    return Game.destroy()