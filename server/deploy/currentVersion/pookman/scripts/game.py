#################################################################################################################
#                                                                                                               #
# 88888888ba     ,ad8888ba,      ,ad8888ba,    88      a8P   88b           d88         db         888b      88  #
# 88      "8b   d8"'    `"8b    d8"'    `"8b   88    ,88'    888b         d888        d88b        8888b     88  #
# 88      ,8P  d8'        `8b  d8'        `8b  88  ,88"      88`8b       d8'88       d8'`8b       88 `8b    88  #
# 88aaaaaa8P'  88          88  88          88  88,d88'       88 `8b     d8' 88      d8'  `8b      88  `8b   88  #
# 88""""""'    88          88  88          88  8888"88,      88  `8b   d8'  88     d8YaaaaY8b     88   `8b  88  #
# 88           Y8,        ,8P  Y8,        ,8P  88P   Y8b     88   `8b d8'   88    d8""""""""8b    88    `8b 88  #
# 88            Y8a.    .a8P    Y8a.    .a8P   88     "88,   88    `888'    88   d8'        `8b   88     `8888  #
# 88             `"Y8888Y"'      `"Y8888Y"'    88       Y8b  88     `8'     88  d8'          `8b  88      `888  #
#                                                                                                               #
#################################################################################################################
from data.scripts.map           import map
from data.scripts.pacmanstatus  import PacmanStatus
from data.scripts.ghoststatus   import GhostStatus
from data.scripts.ghosttype     import GhostType
from data.scripts.gamemode      import GameMode
from data.scripts.pacman        import Pacman
from data.scripts.ghosts        import Ghost
from data.scripts.bonus         import Bonus
from scripts.main               import menu
from tiledmap                   import TiledMap
from random                     import random
from direction                  import *
import blackbox
import vpu
import joypad

class Debug:
    PACMAN  = False
    GHOSTS  = False
    GAME    = True
    SLOWMO  = False

class Action:
    DEBUG_TEST_INVENTORY= False
    DEBUG_NEXT_SONG     = False
    DEBUG_ABORT         = False
    ACTIVATE            = False
    NEXT_POWERUP        = False
    PREV_POWERUP        = False
    MOVE_LEFT           = False
    MOVE_RIGHT          = False
    MOVE_UP             = False
    MOVE_DOWN           = False
    CANCEL              = False
    PAUSE               = False

class Door:
    x       = [13, 14]
    y       = 12
    open    = False

class Game:
    debug   = Debug
    action  = Action
    running = True
    width = 0
    height = 0
    dims = {}
    scale = 2.0
    score = 0
    time = 0
    
    level   = 0
    lives   = 0
    mode    = 0
    status  = 0
    timeout = 0
    gameOver=False
    cheats  =False
    gameMode=False
    freeze = False
    
    map_width  = 28 * 8 
    map_height = 31 * 8

    blinky = None
    pinky  = None
    inky   = None
    clyde  = None
    pacman = None
    bonus  = None

    buffer = [ None, None ]
    
    map = None

    door = Door

    def resetInput(): 
        Game.action.DEBUG_ABORT = False
        Game.action.NEXT_SONG     = False
        Game.action.CANCEL         = False
        Game.action.PAUSE         = False

    @staticmethod
    def resetSystems():
        Game.blinky.reset()
        Game.pinky.reset()
        Game.inky.reset()
        Game.clyde.reset()
        Game.pacman.ready()    

    @staticmethod
    def closeHouse():
        print("Closing House")
        Game.door.open = False
        Game.map.set(Game.door.x[0], Game.door.y, 0x1F)
        Game.map.set(Game.door.x[1], Game.door.y, 0x1F)
        Game.map.redraw()

    @staticmethod
    def openHouse():
        print("Opening House")
        Game.door.open = True
        Game.map.set(Game.door.x[0], Game.door.y, 0x3F)
        Game.map.set(Game.door.x[1], Game.door.y, 0x3F)
        Game.map.redraw()
    
    @staticmethod
    def nextMap():
        Game.resetSystems()
        Game.level = Game.level + 1
        #reload map!
        Game.pacman.spawn()
        Game.blinky.reset()
        Game.inky.reset()
        Game.pinky.reset()
        Game.clyde.reset()
        Game.closeHouse()
        Game.bonus.reset()
        Game.reload_map()

    @staticmethod
    def setMode(mode):
        Game.mode = mode
        
        
    @staticmethod
    def oneUp(lives):
        Game.lives += lives

    @staticmethod
    def oneDown(lives):
        Game.lives -= lives
        if Game.lives <= 0 :
            Game.lives = 0
            Game.gameOver = True
        
    @staticmethod
    def is_pill(i):
        x = i % 28
        y = int(i / 28)
        return Game.map.get(x,y,0) == 0x20
    
    @staticmethod
    def is_energizer(i):
        x = i % 28
        y = int(i / 28)
        return Game.map.get(x,y,0) == 0x21

    @staticmethod 
    def countPills():
        Game.pillsLeft = Game.map.count(0x20)
        if Game.pillsLeft==0:
            Game.pillsLeft = -1
            Game.nextMap()

    @staticmethod 
    def New():
        print("^6------------------- Starting new game --------------------")
        Game.gameOver             = False
        Game.cheats             = False
        Game.map                 = False    
        Game.menu                 = False    
        Game.lives                 = 0
        Game.level                 = 0
        Game.score                 = 0
        Game.gameMode             = 0
        Game.resetSystems()
        Game.countPills()
        Game.bonus.reset()
        Game.snapshot()
        Game.reload_map()        
        print("New game started")

    @staticmethod
    def is_ghost(i):
        if Game.pacman.status == PacmanStatus.DIEING: return False
        elif Game.pacman.status == PacmanStatus.DEAD: return False
        
        w = Game.map.width
        h = Game.map.height
        pos = {
            GhostType.BLINKY    : (Game.blinky.ty * w) + Game.blinky.tx,
            GhostType.PINKY     : (Game.pinky.ty  * w) + Game.pinky.tx,
            GhostType.INKY      : (Game.inky.ty   * w) + Game.inky.tx,
            GhostType.CLYDE     : (Game.clyde.ty  * w) + Game.clyde.tx,
        }
        return ((Game.blinky.mode    < GhostStatus.RETURNING) and (pos[GhostType.BLINKY] == i)) or ((Game.pinky.mode    < GhostStatus.RETURNING) and (pos[GhostType.PINKY ] == i)) or ((Game.inky.mode     < GhostStatus.RETURNING) and (pos[GhostType.INKY  ] == i)) or ((Game.clyde.mode    < GhostStatus.RETURNING) and (pos[GhostType.CLYDE ] == i))

    @staticmethod
    def reload_map():
        Game.map.fill(0x6f)
        #load map...
        i=0
        for y in range(0, 31):
            for x in range(0, 28):
                Game.map.setvalue(x,y, map[i], 0)
                i+=1

    @staticmethod
    def setup():
        print("GAME: Setting up...")
        Game.running = True
        Game.map = map
        # disable rendering
        vpu.disable(0)
        vpu.disable(1)
        vpu.disable(2)
        # set video scale to 2x
        vpu.setscale(0, 2.0, 2.0)
        vpu.setscale(1, 2.0, 2.0)        
        vpu.setscale(2, 1.0, 1.0) # Overlay is always drawn at 2x zomm
        vpu.setrotation(0, 0.0)
        vpu.setrotation(1, 0.0)        
        vpu.setrotation(2, 0.0)  
        vpu.setfont('tiny')
        # get layer dimensions
        vpu.select(0); Game.dims[0] = vpu.dimensions()
        Game.width  = int(Game.dims[0][0])
        Game.height = int(Game.dims[0][1])
        scale_y = Game.height / (31*8)
        vpu.setscale(0, 2, scale_y/2)
        vpu.select(1); Game.dims[1] = vpu.dimensions()
        vpu.select(2); Game.dims[2] = vpu.dimensions()
        
        #allocate custom video buffers
        Game.buffer[0] = vpu.createsurf(Game.map_width, Game.map_height)
        Game.buffer[1] = vpu.createsurf(Game.map_width, Game.map_height)
        
        # prepare initial layer state
        vpu.select(0); vpu.fill(0,0,0,255)
        vpu.select(1); vpu.fill(0,0,0,255)
        vpu.select(2); vpu.fill(0,0,0,0)
        vpu.select(Game.buffer[0]); vpu.fill(0,0,0,0)
        vpu.select(Game.buffer[1]); vpu.fill(0,0,0,0)
        
        print("GAME: Initializing classes...")
        Game.map = TiledMap(Game, 28, 31)
        if not Game.map.load_tileset("walls"):
            print("Cannot load tileset!")
                

        Game.reload_map()
        
        Game.pacman = Pacman
        Game.bonus  = Bonus
        Game.ghost  = Ghost
        #Game.demo     = Demo
        #Game.music = MusicPlayer
        
        Game.pacman.initialize(Game)
        Game.ghost.initialize(Game)
        Game.bonus.initialize(Game)
        #Game.demo.initialize(Game)
        #Game.music.initialize(Game)

        Game.blinky     = Ghost(GhostType.BLINKY)
        Game.inky       = Ghost(GhostType.INKY  )
        Game.pinky      = Ghost(GhostType.PINKY )
        Game.clyde      = Ghost(GhostType.CLYDE )

        Game.gameOver   = False
        Game.cheats     = False
        Game.lives      = 3
        Game.level      = 0
        Game.score      = 0
        Game.gameMode   = 0
        Game.pills      = 0
        Game.pillsLeft  = 0

        Game.freeze     = 0
        
        Game.mode       = GameMode.ARCADE
        
        
        # enable rendering back
        vpu.enable(0)
        #vpu.enable(1)
        vpu.enable(2)
        
    @staticmethod
    def destroy():
        Ghost.destroy()
        Pacman.destroy()
        Bonus.destroy()

        vpu.select(0); vpu.fill(0,0,0,255)
        vpu.select(1); vpu.fill(0,0,0,255)
        vpu.select(2); vpu.fill(0,0,0,0)        

    @staticmethod
    def loop():
        delta = 1.0
        while Game.running:
            Game.draw()
            Game.update(delta)            
            
    @staticmethod
    def update_input(delta):
        #if Game.demo.playback: return
        
        # Handle movement
        if joypad.left()    : Game.pacman.go(Direction.LEFT)
        elif joypad.right() : Game.pacman.go(Direction.RIGHT) 
        elif joypad.up()    : Game.pacman.go(Direction.UP)      
        elif joypad.down()  : Game.pacman.go(Direction.DOWN)    
                
        Game.debug.SLOWMO = True if joypad.select() else False
        
        if joypad.start()   : Game.pacman.spawn()    
        
        #if(input.keyDown[input.KEY_P]        ) debug[DEBUG_PACMAN] ^= 1; else     
        #if(input.keyDown[input.KEY_O]        ) debug[DEBUG_GAME] ^= 1;   else     
        #if(input.keyDown[input.KEY_I]        ) debug[DEBUG_GHOSTS] ^= 1; else     
        
    @staticmethod
    def update(delta):
        Game.update_input(delta)
        # Freeze game while kill sound is playing
        if Game.freeze <= 0:
            Game.bonus.update(delta)
            # Delay pacman a frame when it eats 
            if not Game.pacman.eating:
                Game.pacman.update(delta)
            else:
                Game.countPills()
                Game.pacman.eating = False
            
            Game.blinky.update(delta)
            Game.inky.update(delta)
            Game.pinky.update(delta)
            Game.clyde.update(delta)
            
            if Game.pinky.mustLeave:
                if not Game.door.open: 
                    Game.openHouse()

            Game.map.redraw()
            if Game.debug.SLOWMO:
                Game.freeze = 10
        else:
            Game.freeze-=1
        
        #Game.map.update(delta)

        Game.time+=1
        if Game.time % 16==0:
            Game.map.redraw()

        # required stuff
        if blackbox.ctrlc():
            print("Control+C pressed.")
            Game.destroy()
            Game.running = False
            
        if joypad.menu():
            menu()
        
    @staticmethod
    def draw():
        # clear sprite buffer
        vpu.select(Game.buffer[1])
        vpu.fill(0,0,0,0)
        
        if Game.map.need_redraw:
            #clear bg buffer
            vpu.select(Game.buffer[0])
            vpu.fill(0,0,0,0)
            
            # Draw lives
            for i in range(0, Game.lives):
                Game.pacman.gfx[ Direction.RIGHT ].draw(i<<4, 272)
            
            # Draw current level (bonus)
            li = 3 if (Game.level > 2) else (Game.level+1)
            for i in range(0, li):                
                Bonus.gfx[Game.level-((li-1)-i)].draw(208 - (i<<4), 272)

            
            # draw map here
            # --------------------------
            Game.map.x = ( Game.width   >> 1 ) - (( Game.map.width  * Game.map.tile_width  ) >> 1 )
            Game.map.y = ( Game.height  >> 1 ) - (( Game.map.height * Game.map.tile_height ) >> 1 )
            Game.map.draw()
            # --------------------------
            if Bonus.isAlive:
                i = Bonus.game.level % (32*8)
                Bonus.gfx[i].draw(Bonus.x, Bonus.y+24)
        # draw sprites 
        vpu.select(Game.buffer[1])
        vpu.fill(0,0,0,0)
        Pacman.gfx[Direction.RIGHT].setframe(9)
        Game.pacman.draw()
        
        if Game.pacman.status != PacmanStatus.DIEING: 
            Game.blinky.draw()    
            Game.inky.draw()
            Game.pinky.draw()
            Game.clyde.draw()

        vpu.select(0)
        left = (Game.width  >> 1) - (Game.map_width>>1)
        top  = (Game.height >> 1) - (Game.map_height>>1)
        vpu.drawsurf(Game.buffer[0], left, top)
        vpu.drawsurf(Game.buffer[1], left, top)
        vpu.update()

def setup():
    return Game.setup()

def loop():
    return Game.loop()

def destroy():
    return Game.destroy()


