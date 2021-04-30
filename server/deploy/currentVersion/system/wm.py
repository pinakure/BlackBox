import wmgr

class Window:
    def __init__(self, handle, x=0, y=0, width=64, height=64, caption="Untitled", flags=0x00):
        self.handle = handle
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.caption = caption
        self.flags = flags

    def setposition(self, x, y):
        [ self.x, self.y] = wmgr.setposition(self.handle, x, y)
    
    def setsize(self, width, height):
        [ self.width, self.height] = wmgr.setsize(self.handle, width, height)
    
    def setcaption(self, caption):
        self.caption = caption
        wmgr.setcaption(self.handle, caption)

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