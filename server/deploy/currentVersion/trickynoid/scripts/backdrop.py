from animation  import Animation, AnimationLoop
from pixel      import Pixel
from random     import random
import vpu

class Color:
    CYAN     = 0
    DARKCYAN = 1
    BLUE     = 2
    LYLA     = 3
    DARKPINK = 4
    PINK     = 5
    MAGENTA  = 6
    RED      = 7
    ORANGE   = 8
    GOLDEN   = 9
    LEMON    = 10
    LIMA     = 11
    GREEN    = 12
    SKY      = 13
    WHITE    = 14
    GRAY     = 15
 
class Backdrop:

    game   = None
    sprite = None
    gfx    = None
    total  = 0x40 
    
    @staticmethod
    def initialize(game):
        Backdrop.game = game
        try:
            Backdrop.sprite = vpu.createsprite('backdrop')
            Backdrop.gfx = {}
            for i in range(0, Backdrop.total):
                Backdrop.gfx[i] = Animation(16,16, Backdrop.sprite, 0, i, 7, i, False, 0.5, True, AnimationLoop.PINGPONG if (i%3 != 2) else AnimationLoop.FORWARD)
        except Exception as E:
            exception(E)
            Backdrop.gfx = None

    def __init__(self, tempo):
        self.color      = Pixel(0, 0, 255, 0, 255)
        self.active     = 0
        self.speed      = tempo
        self.alternate  = 0
        self.alteration = 0
        self.timer      = 0
        self.lastFrame  = 0
            
    def getTypeColor(self, color_type):
        color_type %= 16
        if color_type == Color.CYAN:    color = Pixel(0,0,  0,252,248)
        elif color_type==Color.DARKCYAN:color = Pixel(0,0,  0,200,248)
        elif color_type==Color.BLUE:    color = Pixel(0,0,  0, 24,255)
        elif color_type==Color.LYLA:    color = Pixel(0,0,248,  0,200)
        elif color_type==Color.DARKPINK:color = Pixel(0,0,128, 24,255)
        elif color_type==Color.PINK:    color = Pixel(0,0,192,  0,248)
        elif color_type==Color.MAGENTA: color = Pixel(0,0,255,  0,255)
        elif color_type==Color.RED:     color = Pixel(0,0,255,  0,  0)
        elif color_type==Color.ORANGE:  color = Pixel(0,0,248, 24,  0)
        elif color_type==Color.GOLDEN:  color = Pixel(0,0,248,196,  0)
        elif color_type==Color.LEMON:   color = Pixel(0,0,200,248,  0)
        elif color_type==Color.LIMA:    color = Pixel(0,0, 96,248,  0)
        elif color_type==Color.GREEN:   color = Pixel(0,0,  0,248,  0)
        elif color_type==Color.SKY:     color = Pixel(0,0,  8,250,204)
        elif color_type==Color.WHITE:   color = Pixel(0,0,200,200,216)
        elif color_type==Color.GRAY:    color = Pixel(0,0,192,212,216)
        else: color = Pixel(0,0, 64, 64, 96)
        return color
    
    def setSpeed(self, speed):  self.speed = speed; self.timer = 0
    def setColor(self, index):  self.color = self.getTypeColor(index)
    def update(self, delta, musicplayer, time_scale):
        if musicplayer.isPlaying():
            self.timer += delta
            if self.timer > self.speed:
                self.timer  = 0
                self.active = int(random()*(Backdrop.total-1))
                if int(random()*3)==0: self.alternate = int(random()*(Backdrop.total-1))
                else: self.alternate = self.active
                self.active %= Backdrop.total
                self.alteration = int(random()*6)+1
                Backdrop.gfx[self.active].restart()
        else:
            self.timer = 0
        Backdrop.gfx[self.active].update(int(delta*time_scale))
    
    def render(self, ix, iy):
        b1 = Backdrop.gfx[self.active]
        b2 = Backdrop.gfx[self.alternate]
        self.lastFrame = 0 #Backdrop.gfx[self.active].getFrame()
        i=0
        ox = ix
        for y in range(0, 15):
            i+=1
            ix = ox
            for x in range(0, 14):
                b = b1 if i&1 else b2
                b.setframe(0)
                b.draw(ix, iy)
                ix+=16
                i+=1
            iy+=16
        #g.setDrawMode(Graphics.MODE_COLOR_MULTIPLY);
        #g.setColor(backdropColor);
        #g.fillRect(3, 3, 224, 240);
        #    g.setLineWidth(2);
        #    for(int e=0; e<225; e++){
        #        g.drawGradientLine(3 + e, 3, backdropColor, 3 + e, 240, backdropColor.scaleCopy(0.025f));
        #    }
        #g.setDrawMode(Graphics.MODE_NORMAL);        
        
        b1.setframe(self.lastFrame)
        b2.setframe(self.lastFrame)
