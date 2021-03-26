import console
import joypad
import vpu

class Cursor:
    game    = None
    x       = 0
    y       = 0
    width   = 0
    height  = 0
    timer   = 0
    
    @staticmethod
    def initialize(game, width=40, height=30):
        Cursor.game     = game
        Cursor.width    = width
        Cursor.height   = height
    
    @staticmethod
    def draw():
        Cursor.timer+=1
        i = Cursor.timer*4 
        i = i % 255
        vpu.setcolor(i, 128-(i>>1) , 255-i, 200)
        x = Cursor.x * 8
        y = Cursor.y * 8
        vpu.drawsprite(Editor.tiles[Editor.current], x+4, y+4)
        vpu.rect(x,y,8,8)

    @staticmethod
    def destroy():
        pass

    @staticmethod
    def move_up():
        Cursor.y -= 1
        if Cursor.y < 0:
            Cursor.y = Cursor.height-1
        
    @staticmethod
    def move_down():
        Cursor.y += 1
        if Cursor.y > Cursor.height:
            Cursor.y = 0
        
    @staticmethod
    def move_left():
        Cursor.x -= 1
        if Cursor.x < 0:
            Cursor.x = Cursor.width-1
        
    @staticmethod
    def move_right():
        Cursor.x += 1
        if Cursor.x > Cursor.width:
            Cursor.x = 0
        
class Editor:
    game            = None
    cursor          = None       
    current         = 0x00
    tileset         = None
    tiles           = {}
    show_tileset    = False
    tilesetmode     = False

    @staticmethod
    def initialize(game):
        Editor.game = game
        Editor.cursor = Cursor
        Editor.cursor.initialize(game)
        Editor.tileset = vpu.createsprite('golf')
        i = 0
        for y in range(0,9):
            for x in range(0,32):
                Editor.tiles[i] = vpu.subsprite(Editor.tileset, x*8, y*8, (x*8)+8,(y*8)+8)
                i+=1
        console.exec("from data.scripts.editor import Editor")
        console.echo("Write down Editor.save(mapname) to save this map")
        
    @staticmethod
    def save(mapname):
        console.echo(f"Saving {mapname}")

    @staticmethod
    def destroy():
        Editor.cursor.destroy()

    @staticmethod
    def setvalue():
        Editor.game.map.set( Editor.cursor.x, Editor.cursor.y, Editor.current, 1)
        
    @staticmethod
    def erasevalue():
        Editor.game.map.set( Editor.cursor.x, Editor.cursor.y, 0x1FF, 1)
        
    @staticmethod
    def nextvalue(q=1):
        Editor.current+=q
        if Editor.current > 0x11f:
            Editor.current = 0
        
    @staticmethod
    def previousvalue(q=1):
        Editor.current-=q
        if Editor.current < 0: 
            Editor.current = 0x11f
        
    @staticmethod
    def tilesetmode():
        Editor.show_tileset = True
        if joypad.left()==1    : Editor.previousvalue()
        elif joypad.right()==1 : Editor.nextvalue()
        elif joypad.up() ==1   : Editor.previousvalue(32)
        elif joypad.down()==1  : Editor.nextvalue(32)
        

    @staticmethod
    def mapmode():
        Editor.show_tileset = False
        if joypad.left() == 1   : Editor.cursor.move_left()
        elif joypad.right()==1  : Editor.cursor.move_right()
        elif joypad.up()==1     : Editor.cursor.move_up()
        elif joypad.down()==1   : Editor.cursor.move_down()
        
        if joypad.a()>0 : Editor.setvalue()
        if joypad.b()>0 : Editor.erasevalue()
        
        # if joypad.l1() == 1     : Editor.previousvalue()
        # elif joypad.r1() == 1   : Editor.nextvalue()
        
    @staticmethod
    def update():
        Editor.tileset_mode = True if joypad.r1() else False
        if Editor.tileset_mode: Editor.tilesetmode()
        else: Editor.mapmode()
    
    @staticmethod
    def draw():
        if Editor.show_tileset:
            x = Editor.current % 32
            y = int(Editor.current / 32)
            vpu.setcolor(16,16,16,255)
            vpu.fillrect(160-129,120-37, 258,74)
            vpu.drawsprite(Editor.tileset, 160, 120)
            vpu.setcolor(255,0,255,128)
            vpu.rect((160-128)+ (x*8) ,(120-36)+(y*8), 8,8)
        else:
            Editor.cursor.draw()

    
        