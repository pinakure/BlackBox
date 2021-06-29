from    menu                     import Menu
import  typewriter
import  vpu

class TypeWriterMenu:
    @staticmethod
    def open():
        menu = Menu("TypeWriter functions")
        menu.addchild("Test GetText Dlg"    , TypeWriterMenu.get_text       )
        menu.addchild('Set Color'           , TypeWriterMenu.set_color      )
        menu.addchild('Set Font'            , TypeWriterMenu.set_font       )
        menu.addchild('Set Position'        , TypeWriterMenu.set_position   )
        menu.addchild('Set Size'            , TypeWriterMenu.set_size       )
        menu.addchild("Back")
        menu.update()
    
    @staticmethod
    def get_text():
        typewriter.prompt('Unnamed')
        text = 0
        while not text:
            vpu.update()
            text = typewriter.gettext()
        typewriter.enqueue(text)
        while not typewriter.ready():
            vpu.update()

    @staticmethod
    def set_color():
        from scripts.menus.options import TypeWriterOptions
        TypeWriterOptions.set_color()
        
    @staticmethod
    def set_font():
        typewriter.setfont('mana', 'nk57')

    @staticmethod
    def set_position():
        typewriter.setposition(0,0)

    @staticmethod
    def set_size():
        typewriter.setsize(64,64)
