from menu           import Menu
from blackbox       import *
from vpu            import *
from math           import sin, cos, tan
from console        import echo as cout
from random         import random
from time           import sleep

class TestMenu:
    @staticmethod
    def open():
        menu = Menu("Options Menu")
        menu.addchild('drawanim()'      , test.drawanim     )
        menu.addchild('drawsprite()'    , test.drawsprite   )
        menu.addchild('drawsurf()'      , test.drawsurf     )
        menu.addchild('fade()'          , test.fade         )
        menu.addchild('benchmark()'     , test.benchmark    )
        menu.addchild('dialog()'        , test.dialog       )
        menu.addchild('Back')
        menu.update()

class test():
    @staticmethod
    def drawanim():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        spr = createsprite('down', 8);
        ani = createanim(32,32, spr, 0, 0, 3, 0, False)
        x = 0
        cy = int(1024/2)
        cx = int(1280/2)
        while(not ctrlc()):
            select(0)
            setcolor(0,0,0)
            fill()
            x += (0.3141596/16);
            px = cx+int(sin(-x)*100)
            py = cy+int(cos(-x)*100)
            drawanim(ani, px, py)
            update()

    @staticmethod
    def drawsprite():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        spr = createsprite('bottle', 8);
        while(not ctrlc()):
            select(0)
            drawsprite(spr, int(random()*1280), int(random()*1024))
            update()
    
    @staticmethod
    def drawsurf():
        disable(0)
        disable(1)
        disable(2)
        enable(0)
        select(0)
        img = createsurf(64,64)
        select(img)
        setcolor(255,255,0,255)
        fill()
        run = True

        while(not ctrlc()):
            select(img)
            setcolor(int(random()*255),int(random()*255),0,255);
            for i in range(32):
                pset(int(random()*64), int(random()*64))
            select(0)
            drawsurf(img, 640-32, 480-32);
            for i in range(32):
                pset(int(random()*1280), int(random()*1024))
            update()            
        
    @staticmethod
    def benchmark():
        spr = createsprite(32,32, 'down', 8);
        ani = createanim(32,32, spr, 0, 0, 3, 0, False)
        x = 0
        select(0)
        cx, cy = dimensions();
        cy /= 2
        cx /= 2
        e=1
        while(not ctrlc()):
            for i in range(0,512):
                c = i if i<256 else 512-i
                select(0)
                setrotation(0, (c/256)-0.5)    
                textout(f"Iteration {e}", 512,512)
                setcolor(64,32,0)
                fill()
                x += (0.3141596/16)
                px = int(cx+(sin(-x)*100))
                py = int(cy+(cos(-x)*100))
                drawanim(ani, px, py)
                update()                
            e+=1
    @staticmethod 
    def fade():
        while(not ctrlc()):
            wait(1)
            fadeout()
            wait(1)
            vpu.enable(1)
            vpu.fill(255,0,128)        
            select(0)
            r = int(random()*255)
            g = int(random()*255)
            b = int(random()*255)
            setcolor(r,g,b)
            fill()
            fadein()
            wait(1)

    @staticmethod
    def discard():
        fadeout()
        wait(2)        
        def callback():
            select(0)
            r = int(random()*255)
            g = int(random()*255)
            b = int(random()*255)
            fill()
            setcolor(r,g,b)          
        while(not ctrlc()):        
            fadein()
            wait(1, callback)
            fadeout()
            wait(1, callback)
        cout("Script Finished.")
        return True

    @staticmethod
    def dialog():
        typewriter.enqueue('Hello world!')
        while not typewriter.ready():
            vpu.update()
        typewriter.addchoice('A', '2')
        typewriter.addchoice('B', '3')
        choice = ''
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice('Choose A or B')
        if choice == '2':
            typewriter.enqueue('You chose A')
            while not typewriter.ready():
                vpu.update()
        elif choice == '3':
            typewriter.enqueue('You chose B')
            while not typewriter.ready():
                vpu.update()
            typewriter.addchoice('C', '5')
            typewriter.addchoice('D', '6')
            choice = ''
            while choice=='':
                vpu.update()
                choice = typewriter.getchoice('Choose C or D')
            if choice == '5':
                typewriter.enqueue('You chose C')
                while not typewriter.ready():
                    vpu.update()
            elif choice == '6':
                typewriter.enqueue('You chose D')
                while not typewriter.ready():
                    vpu.update()


    
    @staticmethod
    def variable_test():
        # create a variable (should return its internal handle)    
        foo = createstring('foo', 'bar')
        print(f'CreateString:{foo}')
        # find the var_handle (should return its internal handle, matching handle given @ createstring)    
        foo = findvar('foo')
        print(f'FindVar:{foo}')
        # read the variable value (should return 'bar')
        v = getvar(foo)
        print(f'GetVar:{v}')
        # override the variable value (should return True)
        v = setvar(foo, 'baz')
        print(f'SetVar:{v}')
        # read the variable value again (should return 'baz')
        v = getvar(foo)
        print(f'GetVar:{v}')
        # delete variable (should return True)
        v = deletevar(foo)
        print(f'DeleteVar:{v}')
        # try to delete variable again (should return False)
        v = deletevar(foo)
        print(f'DeleteVar:{v}')
        # try to get the value of the deleted variable (should return False)
        v = getvar(foo)
        print(f'GetVar:{v}')
        # try to set the value of the deleted variable (should return False)
        v = setvar(foo, 'baz')
        print(f'SetVar:{v}')
        # try to find the deleted variable by its name (should return -1)
        foo = findvar('foo')
        print(f'FindVar:{foo}')
        # now the variable is pointing to a null variable, repeat the test. 
        # try to set the value of the deleted variable (should return False)
        v = setvar(foo, 'baz')
        print(f'SetVar:{v}')
        # try to get the value of the deleted variable (should return False)
        v = getvar(foo)
        print(f'GetVar:{v}')
        # try to delete variable again (should return False)
        v = deletevar(foo)
        print(f'DeleteVar:{v}')
    
