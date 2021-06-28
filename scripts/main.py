from console    import echo as cout
from scripts.mainmenu   import MainMenu
import blackbox
import vpu

def configure():
    # in this moment, this is the only way to control fullscreen right before initializing vpu
    # bear with this while we dont bring a proper vpu.restart method
    vpu.fullscreen(False)

def menu():
    MainMenu.open()

def main():
    menu()

def wait(seconds = 1, callback = None):
    now = then = blackbox.epoch()
    while(then+seconds > now):
        if callback is not None:
            callback()
        vpu.update()
        now = blackbox.epoch()

cout('Write down "test.menu()" to discover available tests / benchmarks ')
cout('Use F1 to toggle console')