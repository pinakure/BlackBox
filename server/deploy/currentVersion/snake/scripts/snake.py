from random import random
import joypad
import vpu

class Orientation:
    VERTICAL    = 0x10
    HORIZONTAL  = 0x20
    BOTH        = 0x40

class Direction:
    UP     = 0x00
    RIGHT  = 0x01
    DOWN   = 0x02
    LEFT   = 0x03

class CellType:
    TRANSPARENT = 0x00
    GROUND      = 0x01
    APPLE       = 0x03
    SNAKE       = 0x0F

class Space:
    game    = None
    cells   = {}
    x       = 0
    y       = 0
    width   = int(320/8)
    height  = int(240/8)

    @staticmethod
    def initialize(game):
        Space.game = game
        Space.width = int(320 / 8)
        Space.height = int(240 / 8)
        for y in range(0, Space.height):
            Space.cells[ y ] = {}
            for x in range(0, Space.width):
                Space.cells[ y ][ x ] = CellType.GROUND

    @staticmethod
    def draw():
        if not Snake.redraw: return
        vpu.select(1)
        vpu.fill(0,0,0,0)
        sx = 8
        sy = 8
        ix = (int(Snake.game.dims[1][0])>>1)-((Space.width>>1)*sx)
        iy = (int(Snake.game.dims[1][1])>>1)-((Space.height>>1)*sx)
        dy = iy
        vpu.select(1)
        for y in range(0, Space.height):
            dx = ix        
            for x in range(0, Space.width):
                value = Space.cells[y][x]
                if value == CellType.GROUND: vpu.setcolor(0,32,0,128)
                elif value == CellType.APPLE: vpu.setcolor(128,96,0,128)
                else: vpu.setcolor(0,128,0,128)
                vpu.fillrect(dx,dy,sx-1,sy-1)
                vpu.rect(dx+1,dy,sx-1,sy-1)
                dx += sx
            dy += sy
        Snake.redraw = False
    
    @staticmethod
    def destroy():
        Space.cells = {} # memory leak ?
        
        
    @staticmethod
    def clear():
        for y in range(0, Space.height):
            for x in range(0, Space.width):
                Space.cells[y][x] = CellType.GROUND        

    @staticmethod
    def randomdot():
        x = 1 + int( random() * Space.width  - 2 )
        y = 1 + int( random() * Space.height - 2 )
        for i in range(0, Snake.length):
            if (Snake.parts[i].x == x) and (Snake.parts[i].y == y):
                return Space.randomdot()
        Space.cells[y][x] = CellType.APPLE


class SnakePart:
    def __init__(self, x, y, orientation=Orientation.VERTICAL):
        self.x = x
        self.y = y
        self.orientation = orientation
        self.enabled = False

    def string(self):
        return f'X: {self.x} - Y: {self.y} - O: {self.orientation}'

class Snake:
    game            = None
    time            = 0
    speed           = 15
    initial_speed   = 15
    orientation     = Orientation.VERTICAL
    direction       = Direction.UP 
    next_direction  = Direction.UP 
    pause           = False
    moved           = False
    initial_length  = 0
    length          = 0
    parts           = []
    part_pool       = []
    redraw          = False

    @staticmethod
    def part(x, y, orientation=Orientation.VERTICAL):
        for p in Snake.part_pool:
            if p.enabled: continue
            p.x = x
            p.y = y
            p.enabled = True
            p.orientation = orientation
            return p

    @staticmethod
    def initialize(game):
        Snake.game = game
        Snake.time = 0
        Snake.speed = Snake.initial_speed
        Space.initialize(Snake.game)
        for i in range(Space.width * Space.height):
            Snake.part_pool.append(SnakePart(0,0))            
    
    @staticmethod
    def destroy():
        for p in Snake.part_pool:
            del p
        Snake.part_pool = []
        
    @staticmethod
    def spawn(length):
        # Reset Part Pool Status
        for p in Snake.part_pool:
            p.x = 0
            p.y = 0
            p.orientation = 0x00
            p.enabled = False
        # Compose Body
        Snake.initial_length = length
        Snake.parts = {}
        for i in range(0, Snake.initial_length):
            Snake.parts[i] = Snake.part(Space.width>>1 , (Space.height>>1) + (length - i) )
            Space.cells[Snake.parts[i].y][Snake.parts[i].x] = CellType.SNAKE
        Snake.length = Snake.initial_length
        Snake.next_direction = Direction.UP
        Snake.direction = Direction.UP
        Snake.orientation = Orientation.VERTICAL
        Snake.game.score = 0
        Space.clear()        
        Snake.redraw = True
        # load level here
        # ....
        Space.randomdot()
        
    @staticmethod
    def setscore(score):
        if score < 0: score = 0
        Snake.game.score = score

    @staticmethod
    def moveto(x, y):
        # check self collission
        for i in range(0, Snake.length):
            for t in range(0, Snake.length):
                #skip self check!
                if i==t: continue
                if (Snake.parts[t].x == Snake.parts[i].x) and (Snake.parts[t].y == Snake.parts[i].y):
                    Snake.spawn(4)
                    return
    
        # lose 'energy'
        if Snake.game.score > 0:
            Snake.setscore(Snake.game.score-1)
    
        # handle border wrap
        if y < 0: y = Space.height-1
        if y >= Space.height: y = 0
        if x < 0: x = Space.width-1
        if x >= Space.width: x = 0
    
        # erase from cell memory
        for i in range(0, Snake.length):
            Space.cells[Snake.parts[i].y][Snake.parts[i].x] = CellType.GROUND
    
        # move body
        for i in range(0, Snake.length-1):
            Snake.parts[i].x = Snake.parts[i+1].x
            Snake.parts[i].y = Snake.parts[i+1].y
            Snake.parts[i].orientation = Snake.parts[i+1].orientation
    
        # move head 
        Snake.parts[Snake.length-1].x = x
        Snake.parts[Snake.length-1].y = y
        if not (Snake.orientation & Orientation.VERTICAL) and not (Snake.orientation & Orientation.HORIZONTAL):
            Snake.parts[Snake.length-2].orientation = Snake.orientation
            Snake.orientation = Orientation.VERTICAL if ((Snake.direction == Direction.UP) or (Snake.direction == Direction.DOWN)) else Orientation.HORIZONTAL
        Snake.parts[Snake.length-1].orientation = Snake.orientation
        
        # grow
        if Space.cells[y][x]==CellType.APPLE:
            Snake.game.score+=100
            #--------------------            
            nx = Snake.parts[Snake.length-2].x - Snake.parts[Snake.length-1].x
            ny = Snake.parts[Snake.length-2].y - Snake.parts[Snake.length-1].y
            tx = Snake.parts[Snake.length-1].x - nx
            ty = Snake.parts[Snake.length-1].y - ny
            Snake.parts[Snake.length] = Snake.part(tx,ty, Snake.orientation)
            Snake.legnth+=1
            # gen new dot
            Space.randomdot()
        
        # draw on cell memory
        for i in range(0, Snake.length):
            Space.cells[Snake.parts[i].y][Snake.parts[i].x] = CellType.SNAKE | Snake.parts[i].orientation
        Snake.moved = False

    @staticmethod
    def walk():
        #Space.clear()
        if Snake.pause: return
        if Snake.direction == Direction.UP:   Snake.moveto(Snake.parts[ Snake.length-1 ].x  , Snake.parts[ Snake.length-1 ].y-1 )
        if Snake.direction == Direction.LEFT: Snake.moveto(Snake.parts[ Snake.length-1 ].x-1, Snake.parts[ Snake.length-1 ].y   )
        if Snake.direction == Direction.DOWN: Snake.moveto(Snake.parts[ Snake.length-1 ].x  , Snake.parts[ Snake.length-1 ].y+1 )
        if Snake.direction == Direction.RIGHT:Snake.moveto(Snake.parts[ Snake.length-1 ].x+1, Snake.parts[ Snake.length-1 ].y   )
        Snake.redraw = True
        
    @staticmethod    
    def keydown(direction):
        if Snake.moved: return
        Snake.moved = True
        Snake.next_direction = direction
            
        last_orientation = Orientation.VERTICAL if Snake.direction in (Direction.UP, Direction.DOWN) else Orientation.Horizontal
        last_direction   = Snake.direction
        
        if Snake.direction in (Direction.UP, Direction.DOWN):
            Snake.orientation = Orientation.VERTICAL
            Snake.direction = Direction.LEFT if Snake.next_direction==Direction.LEFT else Direction.RIGHT if  Snake.next_direction == Direction.RIGHT else Snake.direction
        if Snake.direction in (Direction.LEFT, Direction.RIGHT):
            Snake.orientation = Orientation.HORIZONTAL
            Snake.direction = Direction.UP if Snake.next_direction==Direction.UP else Direction.DOWN if  Snake.next_direction == Direction.DOWN else Snake.direction
        
        if Snake.orientation != last_orientation:
            #Snake.orientation = Orientation.BOTH
            #if((lastdir == 'u') && (snakedir == 'l'))snakeorient = 'nexus up-left' 
            #else if((lastdir == 'u') && (snakedir == 'r'))snakeorient = 'nexus up-right' 
            #else if((lastdir == 'd') && (snakedir == 'l'))snakeorient = 'nexus down-left' 
            #else if((lastdir == 'd') && (snakedir == 'r'))snakeorient = 'nexus down-right'
            #else if((lastdir == 'l') && (snakedir == 'd'))snakeorient = 'nexus left-down' 
            #else if((lastdir == 'l') && (snakedir == 'u'))snakeorient = 'nexus left-up' 
            #else if((lastdir == 'r') && (snakedir == 'd'))snakeorient = 'nexus right-down' 
            #else if((lastdir == 'r') && (snakedir == 'u'))snakeorient = 'nexus right-up' 
            pass
    
    @staticmethod
    def print():
        print("----------------------------")
        for p in Snake.parts:
            print(Snake.parts[p].string())
        
    @staticmethod
    def update(delta):
        Snake.time+=1
        if Snake.time>=Snake.speed:
            Snake.time=0
            Snake.walk()
        """
        if joypad.start():  
            Snake.moved = False
            Snake.pause ^= 1
        """
        if joypad.up():     Snake.keydown(Direction.UP)
        if joypad.right():  Snake.keydown(Direction.RIGHT)
        if joypad.down():   Snake.keydown(Direction.DOWN)
        if joypad.left():   Snake.keydown(Direction.LEFT)