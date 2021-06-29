from    menu           import Menu
from    blackbox       import *
import  typewriter 
import  vpu

class OptionMenu:
    @staticmethod
    def open():
        menu = Menu("Options Menu")
        menu.addchild('TypeWriter', TypeWriterOptions.set_color)
        menu.addchild('Back')
        menu.update()


class TypeWriterOptions:
    @staticmethod
    def set_color():
        typewriter.addoption(findvar('ui.r'))
        typewriter.addoption(findvar('ui.g'))
        typewriter.addoption(findvar('ui.b'))
        typewriter.addchoice('x', 'Back')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice("TypeWriter Background Color")
        if   choice == 'x': return
        if not typewriter.ready():
                vpu.update()

