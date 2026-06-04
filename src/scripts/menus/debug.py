from menu import Menu

class DebugMenu:
    @staticmethod
    def open():
        menu = Menu("Debugging Menu")    
        menu.addchild("General" , DebugGeneral.menu)
        menu.addchild("Curtain" , DebugCurtain.menu)
        menu.addchild("Back"    )
        menu.update()

class DebugGeneral:
    @staticmethod
    def menu():
        menu = Menu("General Debugging")
        menu.addchild( "Show FPS"       , DebugGeneral.showfps)
        menu.addchild( "Toggle Console" , DebugGeneral.toggle_console)
        menu.addchild( "Back" )
        menu.update()

    @staticmethod
    def showfps():
        pass

    @staticmethod
    def toggle_console():
        import console
        console.toggle()

class DebugCurtain:
    @staticmethod
    def menu():
        menu = Menu("Curtain Menu")
        menu.addchild( "Status"         , DebugCurtain.status)
        menu.addchild( "Open"           , DebugCurtain.open)
        menu.addchild( "Close"          , DebugCurtain.close)
        menu.addchild( "Back" )
        menu.update()
        
    @staticmethod
    def close():
        pass
    
    @staticmethod
    def open():
        pass
    
    @staticmethod
    def status():
        pass
    
