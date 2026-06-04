from   menu         import Menu
import typewriter
import blackbox

class BlackBoxMenu:
    @staticmethod
    def open():
        menu = Menu("Engine Functions")
        menu.addchild("epoch"   , BlackBoxMenu.epoch   )
        menu.addchild("version" , BlackBoxMenu.version )
        menu.addchild("ctrlc"   , BlackBoxMenu.ctrlc   )
        menu.addchild("Back")
        menu.update()

    @staticmethod
    def epoch():
        typewriter.enqueue(f'Epoch is {blackbox.epoch()}')
        
    @staticmethod
    def version():
        typewriter.enqueue(f'Engine Version is {blackbox.version()}')
    
    @staticmethod
    def ctrlc():
        blackbox.ctrlc()
