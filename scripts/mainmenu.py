from menu                    import Menu
from scripts.menus.debug     import DebugMenu
from scripts.menus.options   import OptionMenu
from scripts.menus.functions import FunctionsMenu
from scripts.menus.tests     import TestMenu
import blackbox

class MainMenu:
    @staticmethod
    def open():
        menu = Menu("BlackBox Menu")
        menu.addchild( "Debugging"  , DebugMenu.open        )
        menu.addchild( "Options"    , OptionMenu.open       )
        menu.addchild( "Functions"  , FunctionsMenu.open    )
        menu.addchild( "Tests"      , TestMenu.open         )
        menu.addchild( "Update"     , blackbox.update       )
        menu.addchild( "Close"      , None                  )
        menu.addchild( "Quit"       , quit                  )
        menu.update()
