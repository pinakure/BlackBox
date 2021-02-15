from math       import sin, cos, tan
from random     import random
from time       import sleep
from vpu        import *
from blackbox   import *
from console    import echo as cout
from vpu        import textout
import blackbox
import typewriter 
import vpu

VARIABLE_TYPEWRITER_R=0,
VARIABLE_TYPEWRITER_G=1,
VARIABLE_TYPEWRITER_B=2,
VARIABLE_TYPEWRITER_A=3,
VARIABLE_TYPEWRITER_X=4,
VARIABLE_TYPEWRITER_Y=5,
	

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    fullscreen(False)

def main():
    menu()
    return

def wait(seconds = 1, callback = None):
    now = then = epoch()
    while(then+seconds > now):
        if callback is not None:
            callback()
        update()
        now = epoch()

def menu():
    while(1):
        typewriter.addchoice(' Options '        , 'o')
        typewriter.addchoice(' Functions '      , 'f')
        typewriter.addchoice(' Tests Menu '     , 't')
        typewriter.addchoice('Close Menu'       , 'c')
        typewriter.addchoice('Quit'             , 'x')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice("BlackBox Menu")
        if   choice == 'c': return
        elif choice == 'x': quit()
        elif choice == 'f': functions.menu()
        elif choice == 'o': options.menu()
        elif choice == 't': test.menu()
        if not typewriter.ready():
                vpu.update()

class options():
    @staticmethod
    def menu():
        while(1):
            typewriter.addchoice(' TypeWriter '     , 't')
            typewriter.addchoice('Back'             , 'x')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice("Option menu")
            if   choice == 'x': return
            elif choice == 't': options.typewriter_color()
            if not typewriter.ready():
                    vpu.update()

    @staticmethod
    def typewriter_color():
        typewriter.addoption('1. R '      , 0)#VARIABLE_TYPEWRITER_R
        typewriter.addoption('2. G '      , 1)#VARIABLE_TYPEWRITER_G
        typewriter.addoption('3. B '      , 2)#VARIABLE_TYPEWRITER_B
        typewriter.addchoice('OK'             , 'x')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice("TypeWriter Background Color")
        if   choice == 'x': return
        if not typewriter.ready():
                vpu.update()


class functions():
    @staticmethod
    def menu():
        while(1):
            typewriter.addchoice(' Engine '     , 'b')
            typewriter.addchoice(' TypeWriter ' , 't')
            typewriter.addchoice(' Vpu '        , 'v')
            typewriter.addchoice('Back'         , 'x')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice("Functions")
            if   choice == 'x': return
            elif choice == 'b': functions.blackbox_menu()
            elif choice == 't': functions.typewriter_menu()
            elif choice == 'v': functions.vpu_menu()
            if not typewriter.ready():
                vpu.update()
    
    @staticmethod
    def vpu_menu():
        while(1):
            typewriter.addchoice(' enable '     , 'e')
            typewriter.addchoice(' rotate '     , 'r')
            typewriter.addchoice(' scale '      , 's')
            typewriter.addchoice('Back'         , 'x')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice("VPU")
            if   choice == 'x': return
            elif choice == 'e': vpu.enable(1)
            elif choice == 'r': vpu.rotate(45)
            elif choice == 's': vpu.scale(0.5, 0.5, 0.5)
            if not typewriter.ready():
                vpu.update()
    
    @staticmethod
    def blackbox_menu():
        while(1):
            typewriter.addchoice(' epoch'       , 'e')
            typewriter.addchoice(' ctrlc '      , 'c')
            typewriter.addchoice(' version '    , 'v')
            typewriter.addchoice('Back'         , 'x')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice("Engine")
            if   choice == 'x': return
            elif choice == 'e': typewriter.enqueue(f'Epoch is {blackbox.epoch()}')
            elif choice == 'v': typewriter.enqueue(f'Engine Version is {blackbox.version()}')
            elif choice == 'c': blackbox.ctrlc()
            if not typewriter.ready():
                vpu.update()
        
    @staticmethod
    def typewriter_menu():
        while(1):
            typewriter.addchoice('setcolor'     , 'c')
            typewriter.addchoice('setfont'      , 'f')
            typewriter.addchoice('setposition ' , 'p')
            typewriter.addchoice('setsize'      , 's')
            typewriter.addchoice('Back'         , 'x')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice("TypeWriter Functions")
            if   choice == 'x': return
            elif choice == 'c': options.typewriter_color()
            elif choice == 'f': typewriter.setfont(1)
            elif choice == 'p': typewriter.setposition(0,0)
            elif choice == 's': typewriter.setsize(64,64)
            if not typewriter.ready():
                vpu.update()
                 

class test():
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
                x += (0.3141596/16)
                px = int(cx+(sin(-x)*100))
                py = int(cy+(cos(-x)*100))
                drawanim(ani, px, py)
                update()                
            e+=1
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
            fill()
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
            choice = typewriter.getchoice('Choose A or B')
        if choice == '2':
            typewriter.enqueue('You chose A')
            while not typewriter.ready():
                vpu.update()
        elif choice == '3':
            typewriter.enqueue('You chose B')
            while not typewriter.ready():
                vpu.update()
            typewriter.addchoice('C', '5')
            typewriter.addchoice('D', '6')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice('Choose C or D')
            if choice == '5':
                typewriter.enqueue('You chose C')
                while not typewriter.ready():
                    vpu.update()
            elif choice == '6':
                typewriter.enqueue('You chose D')
                while not typewriter.ready():
                    vpu.update()


    @staticmethod
    def menu():
        typewriter.addchoice('drawanim()'   , '1')
        typewriter.addchoice('drawsprite()' , '2')
        typewriter.addchoice('drawsurf()'   , '3')
        typewriter.addchoice('fade()'       , '4')
        typewriter.addchoice('benchmark()'  , '5')
        typewriter.addchoice('dialog()'     , '6')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice("Available Tests")
        if   choice == '1': test.drawanim()
        elif choice == '2': test.drawsprite()
        elif choice == '3': test.drawsurf()
        elif choice == '4': test.fade()
        elif choice == '5': test.benchmark()
        elif choice == '6': test.dialog()

cout('Write down "test.menu()" to discover available tests / benchmarks ')
cout('Use F1 to toggle console')