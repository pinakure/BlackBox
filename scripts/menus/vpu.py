from    scripts.menus.transition import TransitionMenu
from    menu                     import Menu
import  vpu

class VpuMenu:
    @staticmethod
    def open():
        menu = Menu("Functions")
        menu.addchild("Enable"      , VpuMenu.enable)
        menu.addchild("Rotate"      , VpuMenu.rotate)
        menu.addchild("Scale"       , VpuMenu.scale)
        menu.addchild("Transition"  , TransitionMenu.open)
        menu.addchild("Back")
        menu.update()

    @staticmethod
    def enable():
        vpu.enable(1)

    @staticmethod
    def rotate():
        vpu.rotate(1, 45)

    @staticmethod
    def scale():
        vpu.scale(0.5, 0.5, 0.5)

