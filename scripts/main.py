import vpu

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    vpu.fullscreen(False)

def main():
    #some tests to check vpu functionalities
    vpu.rotate(11, 3.1495) 
    vpu.scale(11, 2.0, 0.5)    
    