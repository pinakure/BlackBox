from math       import sin, cos, tan
from random     import random
from time       import sleep
from vpu        import *
from blackbox   import *
from console    import echo as cout
from vpu        import textout
import typewriter 
import vpu

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    fullscreen(False)

def main():
    return

def wait(seconds = 1, callback = None):
    now = then = epoch()
    while(then+seconds > now):
        if callback is not None:
            callback()
        update()
        now = epoch()

class test():
    @staticmethod
    def menu():
        setcolor(255,255,255);
        cout('~f------------------------')
        cout('~f    Available tests')
        cout('~f------------------------')
        setcolor(200,200,200);
        cout('~7drawanim~8()')
        cout('~7drawsprite~8()')
        cout('~7drawsurf~8()')
        cout('~ffade~8()')
        cout('~7benchmark~8()')

    @staticmethod
    def drawanim():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        spr = createsprite('down', 8);
        ani = createanim(32,32, spr, 0, 0, 3, 0, False)
        x = 0
        cy = int(1024/2)
        cx = int(1280/2)
        while(not ctrlc()):
            select(0)
            setcolor(0,0,0)
            fill()
            x += (0.3141596/16);
            px = cx+int(sin(-x)*100)
            py = cy+int(cos(-x)*100)
            drawanim(ani, px, py)
            update()

    @staticmethod
    def drawsprite():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        spr = createsprite('bottle', 8);
        while(not ctrlc()):
            select(0)
            drawsprite(spr, int(random()*1280), int(random()*1024))
            update()
    
    @staticmethod
    def drawsurf():
        disable(0);
        disable(1);
        disable(2)
        enable(0);
        select(0)
        img = createsurf(64,64)
        select(img)
        setcolor(255,255,0,255);
        fill()
        run = True

        while(not ctrlc()):
            select(img)
            setcolor(int(random()*255),int(random()*255),0,255);
            for i in range(32):
                pset(int(random()*64), int(random()*64))
            select(0)
            drawsurf(img, 640-32, 480-32);
            for i in range(32):
                pset(int(random()*1280), int(random()*1024))
            update()            
        
    @staticmethod
    def benchmark():
        spr = createsprite(32,32, 'down', 8);
        ani = createanim(32,32, spr, 0, 0, 3, 0, False)
        x = 0
        select(0)
        cx, cy = dimensions();
        cy /= 2
        cx /= 2
        e=1
        while(not ctrlc()):
            for i in range(0,512):
                c = i if i<256 else 512-i
                select(0)
                setrotation(0, (c/256)-0.5)    
                textout(f"Iteration {e}", 512,512)
                setcolor(64,32,0)
                fill()
                x += (0.3141596/16);
                px = int(cx+(sin(-x)*100))
                py = int(cy+(cos(-x)*100))
                drawanim(ani, px, py)
                update()                
            e+=1;
    @staticmethod 
    def fade():
        while(not ctrlc()):
            wait(1)
            fadeout()
            wait(1)
            select(0)
            r = int(random()*255)
            g = int(random()*255)
            b = int(random()*255)
            setcolor(r,g,b)
            fill()
            fadein()
            wait(1)

    @staticmethod
    def discard():
        fadeout()
        wait(2)        
        def callback():
            select(0)
            r = int(random()*255)
            g = int(random()*255)
            b = int(random()*255)
            fill();
            setcolor(r,g,b)          
        while(not ctrlc()):        
            fadein()
            wait(1, callback)
            fadeout()
            wait(1, callback)
        cout("Script Finished.")
        return True

    @staticmethod
    def dialog():
        typewriter.enqueue('Hello world!')
        while not typewriter.ready():
            vpu.update()
        typewriter.addchoice('A', '2')
        typewriter.addchoice('B', '3')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice()
        if choice == '2':
            typewriter.enqueue('CHOSEN A')
            while not typewriter.ready():
                vpu.update()
        elif choice == '3':
            typewriter.enqueue('CHOSEN B')
            while not typewriter.ready():
                vpu.update()
            typewriter.addchoice('C', '5')
            typewriter.addchoice('D', '6')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice()
            if choice == '5':
                typewriter.enqueue('CHOSEN C')
                while not typewriter.ready():
                    vpu.update()
            elif choice == '6':
                typewriter.enqueue('CHOSEN D')
                while not typewriter.ready():
                    vpu.update()


cout('Write down "test.menu()" to discover available tests / benchmarks ')