import blackbox
import data.scripts.lib
import vpu

terminate = False
def main():
    vpu.disable(0)
    vpu.disable(1)
    vpu.disable(2)
    vpu.disable(3)        
    vpu.enable(0)
    while not blackbox.ctrlc():
        vpu.update()