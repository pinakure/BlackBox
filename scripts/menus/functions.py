from menu                       import Menu
from scripts.menus.blackbox     import BlackBoxMenu
from scripts.menus.typewriter   import TypeWriterMenu
from scripts.menus.vpu          import VpuMenu

class FunctionsMenu:
    @staticmethod
    def open():
        menu = Menu("Functions")
        menu.addchild("Engine"      , BlackBoxMenu.open)
        menu.addchild("TypeWriter"  , TypeWriterMenu.open)
        menu.addchild("VPU"         , VpuMenu.open)
        menu.addchild("Back")
        menu.update()
        