from math       import sin, cos, tan
from random     import random
from vpu        import *
from blackbox   import *
from console    import print as cout
from vpu        import print as textout

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    fullscreen(False)

def main():
    return

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
        cout('~7benchmark~8()')

    @staticmethod
    def drawanim():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        spr = createsprite(32,32, 'down', 8);
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
        spr = createsprite(32,32, 'bottle', 8);
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
    def discard():
        enable(0)
        then = epoch()
        rrot = 0.0
        ssca = 1.0
        fadein()
        while((epoch()-then) < 5):
            select(0)
            textout("Hello", 220, 240);
            if frames() % 2:
                enable(0)
            else:
                disable(0)

            rot = rrot;
            sca = ssca;
            rotate(0, rot)
            setscale(0, -sca, sca)
            rot *= 2;
            sca *= 2;
            rotate(1, rot)
            setscale(1, sca, sca)
            rot *= 2;
            sca *= 2;
            rotate(2, rot)
            scale(2, sca, sca/2)
            rot *= 2;
            sca *= 2;
            rrot += 0.001
            ssca += 0.001
            update()
        fadeout()
        then = epoch()
        while((epoch()-then) < 5):
            update()        
        fadein()
        while((epoch()-then) < 15):
            select(0)
            textout("Hello", 220, 140);
            if frames() % 2:
                enable(0)
            else:
                disable(0)
            update()
        cout("Script Finished.")
        return True

cout('Write down "test.menu()" to discover available tests / benchmarks ')