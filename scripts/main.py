import vpu
import blackbox

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    vpu.fullscreen(False)

def main():
    return

class tests():
    @staticmethod
    def a():
        while(1):
            vpu.select(10)
            vpu.print("A", 330, 240);
            vpu.update()
        
    @staticmethod
    def b():
        while(1):
            vpu.select(10)
            vpu.print("B", 340, 240);
            vpu.update()
        
    @staticmethod
    def drawsurf():
        from random import random
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
        vpu.enable(0)
        vpu.select(0)
        img = vpu.createsurf(64,64)
        vpu.select(img)
        vpu.setcolor(255,255,0,255);
        vpu.fill()
        run = True

        while(run):
            vpu.select(img)
            vpu.setcolor(int(random()*255),int(random()*255),0,255);
            for i in range(32):
                vpu.pset(int(random()*64), int(random()*64))
            vpu.select(0)
            vpu.drawsurf(img, 640-32, 480-32);
            for i in range(32):
                vpu.pset(int(random()*1280), int(random()*1024))
            vpu.update()
        

    @staticmethod
    def fill():
        vpu.enable(1)
        while(1):
            vpu.select(1)
            vpu.setcolor(255,255,0,255)
            vpu.fill()
            vpu.update()
        

    @staticmethod
    def run():
        for e in range(0,1000):
            for i in range(0,512):
                c = i if i<256 else 512-i
                vpu.select(10)
                vpu.setrotation(10, (c/256)-0.5)    
                #vpu.setcolor(c,c,c)
                vpu.print(f"Iteration {e}", 160,120)
                vpu.update()
        # some tests to check vpu functionalities
        for i in range(0,11):
            vpu.disable(i)
        vpu.enable(4)
        #vpu.fadeout()
        then = blackbox.epoch()
        #vpu.rotate(11, 3.1495) 
        #vpu.scale(11, 2.0, 0.5)    
        #for i in range(0,314):
        #    vpu.rotate(11,i/100)
        #    vpu.update()
        # wait 10 seconds
        rrot = 0.0
        ssca = 1.0
        vpu.fadein()
        while((blackbox.epoch()-then) < 5):
            vpu.select(10)
            vpu.print("Hello", 220, 240);
            if vpu.frames() % 2:
                vpu.enable(0)
            else:
                vpu.disable(0)

            rot = rrot;
            sca = ssca;
            vpu.rotate(11, rot)
            vpu.setscale(11, -sca, sca)
            rot *= 2;
            sca *= 2;
            vpu.rotate(8, rot)
            vpu.setscale(8, sca, sca)
            rot *= 2;
            sca *= 2;
            vpu.rotate(9, rot)
            vpu.scale(9, sca, sca/2)
            rot *= 2;
            sca *= 2;
            vpu.rotate(7, rot)
            vpu.setscale(7, sca, sca)
            rot *= 2;
            sca *= 2;
            vpu.rotate(0, rot)
            vpu.setscale(0, sca/2, sca)    
            rot *= 2;
            sca *= 2;
            rrot += 0.001
            ssca += 0.001
            vpu.update()
        vpu.fadeout()
        then = blackbox.epoch()
        while((blackbox.epoch()-then) < 5):
            vpu.update()        
        vpu.fadein()
        while((blackbox.epoch()-then) < 15):
            vpu.select(10)
            vpu.print("Hello", 220, 140);
            if vpu.frames() % 2:
                vpu.enable(0)
            else:
                vpu.disable(0)
            vpu.update()
        print("Finished.")
        return True