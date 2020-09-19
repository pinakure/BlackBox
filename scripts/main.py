import vpu
import blackbox

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    vpu.fullscreen(False)

def main():
    # some tests to check vpu functionalities
    #for i in range(0,11):
    #    vpu.disable(i)
    #vpu.enable(4)
    then = blackbox.epoch()
    #vpu.rotate(11, 3.1495) 
    #vpu.scale(11, 2.0, 0.5)    
    #for i in range(0,314):
    #    vpu.rotate(11,i/100)
    #    vpu.update()
    # wait 10 seconds
    rrot = 0.0
    ssca = 1.0
    while((blackbox.epoch()-then) < 60):
        vpu.select(10)
        vpu.print("Hello", 320, 240);
        if vpu.frames() % 2:
            vpu.enable(0)
        else:
            vpu.disable(0)

        rot = rrot;
        sca = ssca;
        vpu.rotate(11, rot)
        vpu.scale(11, sca, sca)
        rot *= 2;
        sca *= 2;
        vpu.rotate(8, rot)
        vpu.scale(8, sca, sca)
        rot *= 2;
        sca *= 2;
        vpu.rotate(9, rot)
        vpu.scale(9, sca, sca)
        rot *= 2;
        sca *= 2;
        vpu.rotate(7, rot)
        vpu.scale(7, sca, sca)
        rot *= 2;
        sca *= 2;
        vpu.rotate(0, rot)
        vpu.scale(0, sca, sca)    
        rot *= 2;
        sca *= 2;
        rrot += 0.001
        ssca += 0.001
        vpu.update()
    then = blackbox.epoch()
    while((blackbox.epoch()-then) < 60):
        vpu.select(10)
        vpu.print("Hello", 320, 240);
        if vpu.frames() % 2:
            vpu.enable(0)
        else:
            vpu.disable(0)
        vpu.update()