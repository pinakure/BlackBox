import wmgr

class WidgetType:
    BUTTON = 0x00
    LABEL = 0x01
    TEXTBOX = 0x02
    MAX = 0x03

class Widget:
    
    def __init__(self, parent, widgettype, x, y, width, height, caption="Untitled", callback="", flags=0x00):
        self.type     = widgettype
        self.x        = x
        self.y        = y
        self.width    = width
        self.height   = height
        self.caption  = caption
        self.callback = callback
        self.flags    = flags
        self.handle   = wmgr.addcomponent(parent.handle, widgettype, x, y, width, height, caption, callback, flags)
        print(f"Added widget => {str(self)}")

    def __str__(self):
        return f"HND:{self.handle}, TYPE: {self.type}, POSITION: ({self.x},{self.y}), SIZE: ({self.width},{self.height}), CAPTION:'{self.caption}' "

class Window:
    def __init__(self, handle, x=0, y=0, width=64, height=64, caption="Untitled", flags=0x00):
        self.handle = handle
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.caption = caption
        self.flags = flags
        self.children = {}

    def setposition(self, x, y):
        [ self.x, self.y] = wmgr.setposition(self.handle, x, y)
    
    def setsize(self, width, height):
        [ self.width, self.height] = wmgr.setsize(self.handle, width, height)
    
    def setcaption(self, caption):
        self.caption = caption
        wmgr.setcaption(self.handle, caption)

    def addcomponent(self, widgettype, x, y, width, height, caption="UnnamedWidget", callback="", flags=0x00):
        w = Widget(self, widgettype, x, y, width, height, caption, callback, flags)
        self.children[w.handle] = w
        return w

    def __str__(self):
        return f"Handle: {self.handle}, Caption: '{self.caption}',  Position: ({self.x},{self.y}), Size: ({self.width},{self.height}), Flags: 0x{hex(self.flags)}"

class WindowManager:
    windows = {}

    @staticmethod
    def initialize():
        # Dont call this function twice, or modify it 
        # to delete every window in WindowManager.windows
        # before cleaning up the dict, so no windows be left uncontrolled
        WindowManager.windows = {}
        w = WindowManager.createwindow(30,60,64,64,"Hello",0x0F)
        WindowManager.list()
        w.setcaption("Modified Caption")
        w.setposition(600, 400)
        w.setsize(10, 200)
        button = w.addcomponent(WidgetType.BUTTON, 0,0,32,32,"Ok", "print('hello')", 0xF0)

    @staticmethod
    def list():
        print("Current script side Window instances:")
        for w in WindowManager.windows:
            print(f"\t{str(WindowManager.windows[w])}")

    @staticmethod
    def createwindow(x, y, width, height, caption, flags):
        handle = wmgr.createwindow(x, y, width, height, caption, flags)
        window = Window(handle, x, y, width, height, caption, flags)
        WindowManager.windows[handle] = window
        return window