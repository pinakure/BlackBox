#############################################################################################################
#                                                                                                           #
#                                     ,ad8888ba,     ,ad8888ba,    88           88888888888                 #
#                                    d8"'    `"8b   d8"'    `"8b   88           88                          #
#                                   d8'            d8'        `8b  88           88                          #
#                      88       88  88             88          88  88           88aaaaa                     #
#                      88       88  88      88888  88          88  88           88"""""                     #
#                      "8a,   ,a88  Y8,        88  Y8,        ,8P  88           88                          #
#                       `"YbbdP'Y8   Y8a.    .a88   Y8a.    .a8P   88           88                          #
#                               88    `"Y88888P"     `"Y8888Y"'    88888888888  88                          #
#                                                                                                           #
#############################################################################################################
import importlib 
import data.scripts.map
importlib.reload(data.scripts.map) 

from data.scripts.map           import map
from data.scripts.editor        import Editor
from scripts.main               import menu
from tiledmap                   import TiledMap
from random                     import random
from animation                  import Animation
from direction                  import *
from basicgame                  import BasicGame
from debug                      import debug, error, panic
from math                       import cos, sin
import                          vpu
import                          joypad
        
class Terrain:
    game            = None
    width           = 40
    height          = 30
    invert          = False
    depth_range     = 32
    data            = None
    seed            = 0x1337DEAD
    scale_factor    = 2.0
    octave          = 7

    @staticmethod
    def initialize(game):
        Terrain.data = vpu.createsurf(Terrain.width, Terrain.height)            
        Terrain.game = game
        x, y = Terrain.randomize()
        Terrain.sethole(x, y)
        Terrain.setpole(x, y)
        Terrain.setflag(x, y)

    @staticmethod
    def randomize():
        #make noise
        Terrain.seed = int(random() * 0xFFFFFF)
        Terrain.scale_factor = 1+(random()*3.0)
        Terrain.octave = 5+int(random()*15)
        vpu.perlin(Terrain.data, Terrain.depth_range,0,0, Terrain.scale_factor, Terrain.octave, Terrain.seed) # Will produce up to 5 valid values...
        #vpu.perlin(Game.terrain, 32, 32, 32, 2.0, 7)
        vpu.select(Terrain.data)
        for y in range(0,30):
            for x in range(0,40):
                c = vpu.getpixel(x,y) & 0xFF
                c = int((c / Terrain.depth_range) * 5) # 5 is the number of different turfs
                if c == 0x00: Terrain.game.map.set(x,y, 0x00 if not Terrain.invert else 0x80, 0)
                elif c==0x01: Terrain.game.map.set(x,y, 0x20 if not Terrain.invert else 0x60, 0)
                elif c==0x02: Terrain.game.map.set(x,y, 0x40 if not Terrain.invert else 0x40, 0)
                elif c==0x03: Terrain.game.map.set(x,y, 0x60 if not Terrain.invert else 0x20, 0)
                elif c==0x04: Terrain.game.map.set(x,y, 0x80 if not Terrain.invert else 0x00, 0)
                else: print(hex(c))
        return int(random()*Terrain.width), int(random()*Terrain.height)

    @staticmethod
    def setpole(x,y):
        if y<2:y=2
        Terrain.game.map.set(x,y-2, 0x4F, 2)
        Terrain.game.map.set(x,y-1, 0x6F, 2)
        Terrain.game.map.set(x,  y, 0x8F, 2)
    
    @staticmethod
    def setflag(x,y):
        if y<2:y=2
        Terrain.game.map.set(x,y-2, 0x4E, 2)
    
    @staticmethod
    def sethole(x,y):
        if y<2:y=2
        Terrain.game.map.set(x, y, 0x4D, 1)
        

from pixel import Pixel

class Ball:
    anim     = None
    gfx      = None
    gfx_anim = None
    game     = None
    size     = Pixel(4,4)
    angle    = 0
    force    = 8

    @staticmethod
    def initialize(game):
        Ball.game = game
        sprite         = vpu.createsprite('ball')
        Ball.gfx       = Animation(8,8, sprite, 0, 0, 3, 0, False, 0, False)
        Ball.gfx_anim  = vpu.createsprite('ballanim')
        Ball.anim      = Animation(4,4, Ball.gfx_anim, 0, 0, 3, 0, False, 0.125)

    @staticmethod
    def increaseangle():
        Ball.angle += 1
        Ball.angle %= 200

    @staticmethod
    def decreaseangle():
        Ball.angle -= 1
        Ball.angle %= 200

    def __init__(self, x, y):
        self.position   = Pixel(x,y)
        self.position.r = 0
        self.delta      = Pixel(0,0)
        self.delta.r    = 0
        self.ground     = True
        self.ready      = True

    def preparehit(self):
        if Ball.force < 128:
            Ball.force+=0.125

    def hit(self):
        distance = 32
        Ball.force /= 32
        dst = Pixel(self.position.x+(sin((Ball.angle)/distance)*Ball.force  ),  self.position.y+(cos((Ball.angle)/distance)*Ball.force))
        src = Pixel(self.position.x, self.position.y)
        self.delta = Pixel(src.x-dst.x ,src.y- dst.y)
        self.delta.r = 1.0*(Ball.force/16)
        self.ground = False
        self.ready  = False
        Ball.force = 8

    def update(self, delta):
        self.position.x += self.delta.x
        self.position.y += self.delta.y
        self.position.r += self.delta.r
        if self.position.r < 0:
            self.position.r = 0
            self.delta.r = abs(self.delta.r)* 0.5            
        self.ground = self.position.r == 0
        if not self.ground:
            self.delta.r = self.delta.r - 0.00097 if self.delta.r > -1.0 else -1.0        
        self.delta.x *= 0.987 if self.ground else 0.996
        self.delta.y *= 0.987 if self.ground else 0.996
        
        if (abs(self.delta.x) + abs(self.delta.y)) <0.015: 
            self.delta.x = 0
            self.delta.y = 0
            self.delta.r = 0
            self.ground  = True
            self.ready   = True

        if self.position.x < 0: 
            self.position.x = 0
            self.delta.x = abs(self.delta.x)
        if self.position.y < 0: 
            self.position.y = 0
            self.delta.y = abs(self.delta.y)
        if self.position.x >= 320: 
            self.position.x = 319
            self.delta.x = -abs(self.delta.x)
        if self.position.y >= 240: 
            self.position.y = 239
            self.delta.y = -abs(self.delta.y)
        
    def draw(self, x=0, y=0):
        if (abs(self.delta.x) > 0) or (abs(self.delta.y) > 0):
            frame = int((Game.ball.position.r / 32) * 4)
            Ball.gfx.setframe(3 if frame > 3 else frame)
            Ball.gfx.draw(self.position.x+x, self.position.y+y)
        else:
            vpu.setcolor(255,0,0)
            #vpu.pset(self.position.x + fx, self.position.y + fy)
            size = 12
            distance = 32
            poly = [
                Pixel(self.position.x+(sin((Ball.angle-size)/distance) * Ball.force ), self.position.y+(cos((Ball.angle-size)/distance) * Ball.force ) ),
                Pixel(self.position.x+(sin((Ball.angle     )/distance) * Ball.force ), self.position.y+(cos((Ball.angle     )/distance) * Ball.force ) ),
                Pixel(self.position.x+(sin((Ball.angle+size)/distance) * Ball.force ), self.position.y+(cos((Ball.angle+size)/distance) * Ball.force ) ),
                Pixel(self.position.x, self.position.y)
            ]
            vpu.line(poly[3].x, poly[3].y, poly[0].x, poly[0].y)
            vpu.line(poly[3].x, poly[3].y, poly[1].x, poly[1].y)
            vpu.line(poly[3].x, poly[3].y, poly[2].x, poly[2].y)
            vpu.line(poly[0].x, poly[0].y, poly[1].x, poly[1].y)
            vpu.line(poly[1].x, poly[1].y, poly[2].x, poly[2].y)
            Ball.anim.draw(self.position.x+x, self.position.y+y)
        
class Game(BasicGame):    

    ball = None

    @staticmethod
    def setup():
        try:
            debug("Game", "Setting up")
            BasicGame.prepare()

            #allocate custom video buffers
            Game.buffer = [vpu.createsurf(320, 240) , vpu.createsurf(320, 240) , vpu.createsurf(320, 240) ]
    
            # create map
            Game.setmap(TiledMap(Game, 40, 30, 3))
            if not Game.map.load_tileset("golf"):
                panic("Cannot load 'tilesets/golf.png'", "Game could run perfectly, but we think it's better to abort current execution, as you wouldn't be able to see anything on the screen and that would be definitely bad.")
            
            Ball.initialize(Game)
            Game.ball = Ball(160,120)

            Game.map.load(map)
            Game.map.redraw()
            Game.map.fill(0x11F, 0)
            Game.map.fill(0x11F, 1)
            Game.map.fill(0x11F, 2)
            Game.map.setactive()
            Game.map.setsurface(Game.buffer[0])
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )
            vpu.setfont('tiny')
            Terrain.initialize(Game)
            Editor.initialize(Game)

            Game.ball.x = 0
            Game.ball.y = 0            

        except Exception as E:
            print("\n---------------------------------------------------------\nERROR: Setup Failed\n\tGame cannot run.\n---------------------------------------------------------\n")
            print(str(E))
            quit()
        
    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)            
            
    @staticmethod
    def update(delta):
        # do stuff

        if joypad.left()     : Ball.decreaseangle()
        if joypad.right()    : Ball.increaseangle()
        if Game.ball.ready:
            if joypad.a()>0      : Game.ball.preparehit()
            elif joypad.a()==-1  : Game.ball.hit()
        if joypad.select()==1: Terrain.randomize()

        Game.ball.update(delta)

        # required stuff
        BasicGame.update(delta)
        
    @staticmethod
    def destroy(): 
        debug("Game", "Destroy")
        
    @staticmethod
    def draw():
        if Game.map.need_redraw:
            #clear map buffer
            #vpu.select(Game.buffer[0])
            #vpu.fill(0,0,0,0)
            
            # draw map
            #Game.map.draw()

            # draw things over map layer

        
            
            # clear sprite buffers
            vpu.select(Game.buffer[1])
            vpu.fill(0,0,0,0)
            Game.ball.draw()

            vpu.select(Game.buffer[2])
            vpu.fill(0,0,0,0)
            vpu.setcolor(255,255,255,255)            
            vpu.textout(f"Ball Z Delta: {str(abs(Game.ball.delta.r))[0:4]}" ,2, 180)
            vpu.textout(f" Ball Height: {str(Game.ball.position.r)[0:4]}"   ,2, 190)
            vpu.textout(f"Editor Value: {hex(Editor.current)}"              ,2, 200)
            vpu.textout(f"Scale Factor: {str(Terrain.scale_factor)[0:4]}"   ,2, 210)
            vpu.textout(f"Noise Octave: {Terrain.octave}"                   ,2, 220)
            vpu.textout(f"Random  Seed: 0x{hex(Terrain.seed).upper()[2:]}"  ,2, 230)
            
            # draw editor
            Editor.draw()
            Editor.update()
            
            vpu.select(0)
            left = (Game.width  >> 1) - ((Game.map.width * Game.map.tile_width)>>1)
            top  = (Game.height >> 1) - ((Game.map.height * Game.map.tile_height)>>1)
            for buffer in Game.buffer:
                if buffer:
                    vpu.drawsurf(buffer, left, top)
        
        # required stuff
        BasicGame.draw()        
