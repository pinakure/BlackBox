import                          vpu
from random                     import random
from entity                     import Entity
from animation                  import *
from direction                  import *
from qsort                      import quick_sort
from data.scripts.pacmanstatus  import PacmanStatus
from data.scripts.ghoststatus   import GhostStatus
from data.scripts.ghosttype     import GhostType
from data.scripts.target        import Target


class Ghost(Entity):
    spawn = {
        GhostType.BLINKY : [(13*8)+4, (11*8)],
        GhostType.INKY   : [(11*8)+4, (14*8)],
        GhostType.PINKY  : [(13*8)+4, (14*8)],
        GhostType.CLYDE  : [(15*8)+4, (14*8)],
    }
    default_target = {
        GhostType.BLINKY : [23  ,   3],
        GhostType.INKY   : [20  ,  27],
        GhostType.PINKY  : [4   ,   3],
        GhostType.CLYDE  : [7   ,  27],
    }    
    delay = {
        GhostType.BLINKY :  0,
        GhostType.INKY   : 30,
        GhostType.PINKY  :  2,
        GhostType.CLYDE  : 60,
    }
    colors = {
        GhostType.BLINKY : 0xff0000,
        GhostType.INKY   : 0x00ffff,
        GhostType.PINKY  : 0xffaaaa,
        GhostType.CLYDE  : 0xffaa00,
    }
    # Set time limits to switch between modes 
    # Note: When ghosts are in frightnened mode their timers WONT INCREASE!! 
    chase_time     = {
        0 : [ 20, 20,   20, 9999], #Level = 0
        1 : [ 20, 20, 1033, 9999], #Level >0 & <5
        2 : [ 20, 20, 1037, 9999], #Level >= 5
    }
    scatter_time = {
        0 : [ 7, 7, 5, 5], #
        1 : [ 7, 7, 5, 1], # Last value is  1/60, not 1
        2 : [ 5, 5, 5, 1], # Last value is  1/60, not 1
    }
    
    game    = None
    gfx     = {}
    initialized = False

    @staticmethod
    def is_solid(x,y):
        return Ghost.game.map.get(x,y,0) < 0x1F;# 0x1F is the door
    
    @staticmethod
    def destroy():
        pass

    @staticmethod
    def initialize(game):
        Ghost.game = game
        Ghost.sprite = vpu.createsprite("ghosts")       
        Ghost.gfx = {}        
        for i in range(0,4):
            Ghost.gfx[i] = {
                Direction.UP     : Animation(16, 16, Ghost.sprite, 0, i,  1, i, False, 0.25, False, AnimationLoop.PINGPONG),
                Direction.DOWN   : Animation(16, 16, Ghost.sprite, 2, i,  3, i, False, 0.25, False, AnimationLoop.PINGPONG),
                Direction.LEFT   : Animation(16, 16, Ghost.sprite, 4, i,  5, i, False, 0.25, False, AnimationLoop.PINGPONG),
                Direction.RIGHT  : Animation(16, 16, Ghost.sprite, 6, i,  7, i, False, 0.25, False, AnimationLoop.PINGPONG),
            }
        # Eyes animation
        Ghost.gfx[GhostType.DEAD] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.DEAD, 1, GhostType.DEAD , False, 0.25, False, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 2, GhostType.DEAD, 3, GhostType.DEAD , False, 0.25, False, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 4, GhostType.DEAD, 5, GhostType.DEAD , False, 0.25, False, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 6, GhostType.DEAD, 7, GhostType.DEAD , False, 0.25, False, AnimationLoop.PINGPONG),
        }        
        # Scared and Scared blinking 
        Ghost.gfx[GhostType.PREY] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, False, AnimationLoop.PINGPONG),
        }
        Ghost.gfx[GhostType.BLINK] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, False, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, False, AnimationLoop.PINGPONG),
        }
        Ghost.initialized = True

    def __init__(self, ghost_type=GhostType.BLINKY):
        Entity.__init__(self, Ghost.game)
        self.ghost_type     = ghost_type
        
        # Positioning registers    
        self.x              = Ghost.spawn[self.ghost_type][0];        #Real X (with decimals)
        self.y              = Ghost.spawn[self.ghost_type][1];        #Real Y (with decimals)
        self.rx             = int(self.x)
        self.ry             = int(self.y)
        self.tx             = int(self.rx / 8);        #Tile X
        self.ty             = int(self.ry / 8);        #Tile Y 

        self.inIntersection = False
        self.in_house       = False
        self.in_door        = False
        self.behindDoor     = False
        self.inFrontOfDoor  = False
        self.inFrontOfHouse = False
        self.mustLeave      = False
        self.inTunnel       = False
        self.speed          = 0.125
        self.timer          = 0
        self.gfx            = self.ghost_type #current displayed animation
        self.direction      = Direction.LEFT
        self.mode           = GhostStatus.SCATTER
        self.frame          = 0
        self.chaseTime      = 0
        self.scaterTime     = 0
        self.targetX        = Ghost.default_target[ghost_type][0]
        self.targetY        = Ghost.default_target[ghost_type][1]
        self.targets        = [ Target(self), Target(self), Target(self), Target(self) ]
        self.sprite         = False
        self.updateflags()

    def reset(self):
        # Positioning registers    
        self.x              = Ghost.spawn[self.ghost_type][0];        #Real X (with decimals)
        self.y              = Ghost.spawn[self.ghost_type][1];        #Real Y (with decimals)
        self.rx             = int(self.x)
        self.ry             = int(self.y)
        self.tx             = int(self.rx / 8);        #Tile X
        self.ty             = int(self.ry / 8);        #Tile Y 
        self.inIntersection = False
        self.in_house       = False
        self.in_door        = False
        self.behindDoor     = False
        self.inFrontOfDoor  = False
        self.inFrontOfHouse = False
        self.mustLeave      = False
        self.inTunnel       = False
        self.speed          = 0.125
        self.timer          = 0
        self.gfx            = self.ghost_type
        self.direction      = Direction.LEFT
        self.mode           = GhostStatus.SCATTER
        self.frame          = 0
        self.chaseTime      = 0
        self.scaterTime     = 0
        self.targetX        = Ghost.default_target[self.ghost_type][0]
        self.targetY        = Ghost.default_target[self.ghost_type][1]        
        
    def handle_house(self, delta):
        if self.in_house:
            if self.mustLeave:
                
                self.targetY = Ghost.game.door.y - 1
                self.targetX = Ghost.game.door.x[0]
                
                # This ghost must leave the house
                if self.in_door:self.direction = Direction.UP
                if self.behindDoor:
                    self.x = self.tx * 8
                    self.direction = Direction.UP
            elif self.behindDoor:
                self.direction = Direction.LEFT

            if self.inIntersection:
                self.set_targets([self.tx, self.tx, self.tx, self.tx], [self.ty, self.ty, self.ty, self.ty])
                onTop    = (self.ty <= Ghost.game.door.y+1) and ((self.y%8)==0)
                onBottom =  self.ty >= Ghost.game.door.y+2
                onLeft   =  self.tx <= Ghost.game.door.x[0]-2
                onRight  =  self.tx >= Ghost.game.door.x[1]+1
                """
                if onTop        : self.direction = Direction.DOWN; 
                if onBottom     : self.direction = Direction.UP; 
                """
                if onLeft  and onBottom : self.direction = Direction.RIGHT
                if onRight and onBottom : self.direction = Direction.UP
                if onRight and onTop    : self.direction = Direction.LEFT
                if onLeft  and onTop    : self.direction = Direction.DOWN
            return True
        return False

    def nearest_target(self):
        # Update targets before doing anything
        self.calc_next()
        quick_sort(self.targets, 0, 3, lambda x, y: x.distance > y.distance)
        
        # discard target in opposite direction
        for i in range(0,4):
            if self.targets[i].direction == Direction.opposite(self.direction):
                self.targets[i].viable = False
        # return first viable target if any
        for i in range(0,4):
            if self.targets[i].viable: 
                self.targets[i].color = 0xffff00
                return self.targets[i]
        # return first non-solid target if any
        for i in range(0,4):
            if not self.targets[i].solid:
                self.targets[i].color = 0xff00ff
                return self.targets[i]
        return None

    def can_walk(self, direction):
        # Straight line over the ghost house
        if self.inFrontOfHouse:
            if direction == Direction.UP: return False
            if (direction == Direction.RIGHT) or (direction == Direction.LEFT): return True
            # Ignore IA If not on RETURNING status when being in front of ghost house
            if self.mode != GhostStatus.RETURNING: return False
            return True
        
        if self.in_house: return True
        
        row     = Ghost.game.map.width
        r       = (self.ty * row) + self.tx

        hInt = (int(self.x) % 8) == 0
        vInt = (int(self.y) % 8) == 0
        cx = self.tx
        cy = self.ty
        if direction == Direction.RIGHT : return ((Ghost.game.map.get(cx+1  , cy    ,0) > 0x1E) and (vInt)) or self.inTunnel
        elif direction==Direction.DOWN  : return ((Ghost.game.map.get(cx    , cy+1  ,0) > 0x1E) and (hInt)) or self.inTunnel
        elif direction==Direction.LEFT  : return ((Ghost.game.map.get(cx-1  , cy    ,0) > 0x1E) and (vInt)) or (not hInt) or self.inTunnel
        elif direction==Direction.UP    : return ((Ghost.game.map.get(cx    , cy-1  ,0) > 0x1E) and (hInt)) or (not vInt) or self.inTunnel
        return False
        
    def updateflags(self):
        # Updates Ghost internal flags. TBC initially and each time the ghost moves
        tx = self.tx 
        ty = self.ty
        x  = self.x
        y  = self.y
        cx = ((self.rx - self.x) + (self.x%8))
        cy = ((self.ry - self.y) + (self.y%8))
        
        self.inIntersection  = ( ((cx>8-self.speed) or (cx<0+self.speed)) and ((cy>8-self.speed) or (cy< 0+self.speed)) )
        self.in_house        = (tx >= 11) and (ty >= 12) and (tx <= 16) and (ty <= 15)
        self.in_door         = (ty == Ghost.game.door.y) and (tx >= Ghost.game.door.x[0]) and (tx <= Ghost.game.door.x[1]) and ((self.x % 8)==0)
        self.behindDoor      = ((((y % 8)==0)) and ((ty == Ghost.game.door.y+1) and ((tx >= Ghost.game.door.x[0]) and (tx <= Ghost.game.door.x[1]))))
        self.inFrontOfDoor   = ((ty == Ghost.game.door.y-1) and ((tx >= Ghost.game.door.x[0]) and (tx <= Ghost.game.door.x[1])))
        self.inFrontOfHouse  = ((y % 8 == 0) and ((ty == Ghost.game.door.y - 1) and (tx >= Ghost.game.door.x[0]-1) and (tx <= Ghost.game.door.x[1]+1)))
        self.inTunnel        = ( (self.tx <= 0) or (self.tx >= Ghost.game.map.width-1) or (self.ty >= Ghost.game.map.height-1) or (self.ty <= 0) )
        self.mustLeave       = True if (self.timer >= Ghost.delay[self.ghost_type]) else False

    def set_speed(self):
        if self.mode == GhostStatus.RETURNING: return
        # Adjust speed 
        if self.game.level == 0:
            if self.mode == GhostStatus.FRIGHTENED: self.speed = 0.5
            elif self.inTunnel: self.speed = 0.4
            else: self.speed = 0.75
        elif self.game.level < 4:
            if self.mode == GhostStatus.FRIGHTENED: self.speed = 0.55
            elif self.inTunnel: self.speed = 0.45
            else: self.speed = 0.85
        elif self.game.level < 19:
            if self.mode == GhostStatus.FRIGHTENED: self.speed = 0.6
            elif self.inTunnel: self.speed = 0.5
            else: self.speed = 0.95
        else:
            if self.inTunnel: self.speed = 0.5
            else: self.speed = 0.95

    def set_chase_target(self):
        if self.ghost_type   == GhostType.BLINKY:
            self.targetX = self.game.pacman.tx
            self.targetY = self.game.pacman.ty                
        elif self.ghost_type == GhostType.PINKY:
            #Must return bx-px*2 , by-py*2, the position on double vector of blinky -> pacman
            self.targetX = self.targetX
            self.targetY = self.targetY                
        elif self.ghost_type == GhostType.INKY:
            #Compute up to several vars, the direction pacman is looking, target of blinky
            self.targetX = self.targetX
            self.targetY = self.targetY
        elif self.ghost_type == GhostType.CLYDE:
            self.targetX = self.targetX
            self.targetY = self.targetY
    
    def set_targets(self, x, y, ox, oy):
        self.targets[0].lock(x[0], y[0])
        self.targets[1].lock(x[1], y[1])
        self.targets[2].lock(x[2], y[2])
        self.targets[3].lock(x[3], y[3])
        
    def calc_next(self):
        self.targets[0].lock(self.tx+1,  self.ty)
        self.targets[1].lock(self.tx-1,  self.ty)
        self.targets[2].lock(self.tx,    self.ty-1)
        self.targets[3].lock(self.tx,    self.ty+1)
        
    def switch_mode(self, mode):
        if self.mode == GhostStatus.CHASE:
            self.chaseTime = 0
            self.gfx     = self.ghost_type
            self.reverse()
        elif self.mode == GhostStatus.SCATTER:
            self.scaterTime = 0
            self.gfx    = self.ghost_type
            self.reverse()            
        elif self.mode == GhostStatus.FRIGHTENED:
            self.gfx    = GhostType.PREY
            self.reverse()            
        elif self.mode == GhostStatus.RETURNING:
            self.gfx    = GhostType.DEAD
            self.reverse()
        self.mode = mode

    def reverse(self):
        if self.direction ==  Direction.RIGHT: self.direction = Direction.LEFT
        elif self.direction == Direction.DOWN: self.direction = Direction.UP
        elif self.direction == Direction.LEFT: self.direction = Direction.RIGHT
        elif self.direction == Direction.UP  : self.direction = Direction.DOWN
     
    def move(self):
        # Animate - not needed since autoupdate is on
        
        self.set_speed()
        #self.speed *= 0.125

        if not self.can_walk(self.direction):
            self.move_to(self.x, self.y)
        
        if self.can_walk(self.direction):
            if self.direction  ==  Direction.LEFT  : self.move_to( self.rx - self.speed , int(( self.ry + self.speed ) / 8 ) * 8 )
            elif self.direction == Direction.RIGHT : self.move_to( self.rx + self.speed , int(( self.ry + self.speed ) / 8 ) * 8 ) 
            elif self.direction == Direction.UP    : self.move_to( int(( self.rx + self.speed ) / 8 ) * 8 , self.ry - self.speed ) 
            elif self.direction == Direction.DOWN  : self.move_to( int(( self.rx + self.speed ) / 8 ) * 8 , self.ry + self.speed ) 
        elif Ghost.game.debug.GHOSTS:
                print(f"Cant go {Direction.string(self.direction)}")
        
        if self.inTunnel: return
            
        if self.inIntersection:
            #self.centerInTile()
            nearest = self.nearest_target()
            
            #special cases
            if self.tx == 18 and self.ty == 14:
                if self.targetX > self.tx and self.targetY < self.ty:
                    if self.direction != Direction.LEFT: nearest.direction =  Direction.RIGHT
            if self.tx == 11 and self.ty == 14:
                if self.targetX < self.tx and self.targetY < self.ty:
                    if self.direction != Direction.RIGHT: nearest.direction =  Direction.LEFT
            
            if Ghost.game.debug.GHOSTS:
                if nearest.direction != self.direction:
                    print(f"Turning {Direction.string(self.direction)}")
                       
            if self.can_walk(nearest.direction):
                self.direction = nearest.direction;            
            elif Ghost.game.debug.GHOSTS:
                    print(f"Going {Direction.string(self.direction)}: can't go {Direction.string(nearest.direction)} from ({self.tx},{self.ty}) to ({nearest.x},{nearest.y})")

    def move_to(self, x,y):
        ix = int(x)
        iy = int(y)
        # Clamp and set real X and Y
        if iy   > Ghost.game.map_height : self.ry = iy - Ghost.game.map_height - 8
        elif iy < -8                    : self.ry = iy - Ghost.game.map_height + 8
        else                            : self.ry = y
        if ix   > Ghost.game.map_width  : self.rx = ix - Ghost.game.map_width - 8
        elif ix < -8                    : self.rx = ix + Ghost.game.map_width + 8    
        else                            : self.rx = x
        # Set rounded position (for use in drawing and calculations)
        self.x      = int(self.rx);    
        self.y      = int(self.ry);        
        # Set relative position (for use in IA)
        self.tx     = int(self.x / 8)
        self.ty     = int(self.y / 8)
        self.updateflags()

    def update(self, delta):
        # Perform specific actions depending on current status
        self.timer += (1 / 120)*delta
        if   self.mode == GhostStatus.PAUSE:
            if Ghost.game.pacman.status != PacmanStatus.DEAD: 
                #print("\nPAUSE >> SCATTER")
                self.switch_mode(GhostStatus.SCATTER)
        elif self.mode == GhostStatus.SCATTER:            
            if not self.handle_house(delta):
                # if pacman gets energized, jump to frightened mode
                if Ghost.game.pacman.energized == Ghost.game.pacman.energyQ - 1:
                    self.switch_mode(GhostStatus.FRIGHTENED)
                    #print("\nSCATTER >> FRIGHTEN")
                    self.targetX = Ghost.default_target[self.ghost_type][0]
                    self.targetY = Ghost.default_target[self.ghost_type][1]
                # when scaterTime timer is over, switch to chase mode!
                if self.scaterTime > 400: 
                    #print("\nSCATTER >> CHASE")
                    self.switch_mode(GhostStatus.CHASE)
                else: self.scaterTime+=1
                self.move()
            else: self.updateflags()
                #print(f"SCATTER[{self.scaterTime}]", end="\r")
        elif self.mode == GhostStatus.CHASE:
            if Ghost.game.pacman.energized == Ghost.game.pacman.energyQ - 1:
                self.switch_mode(GhostStatus.FRIGHTENED)
                #print("\nCHASE >> FRIGHTEN")
            #when chase timer is over, switch to scatter mode!
            self.set_chase_target()
            if self.ghost_type != GhostType.BLINKY:
                if self.chaseTime > 400: 
                    #print("\nCHASE >> SCATTER")
                    self.switch_mode(GhostStatus.SCATTER)
                else: self.chaseTime+=1
            self.move()
            #print(f"CHASE[{self.chaseTime}]", end="\r")
        elif self.mode == GhostStatus.RETURNING:
            x = self.tx
            y = self.ty

            # Help eyes to reach home in certain checkpoints
            if self.inIntersection:
                self.x = self.tx<<3
                self.y = self.ty<<3
                if ((x == 6) and (y == 14)) and (self.inIntersection): self.direction = Direction.RIGHT
                elif (x ==21) and (y ==14): self.direction = Direction.LEFT
                elif (x == 9) and (y == 5): self.direction = Direction.DOWN
                elif (x == 6) and (y ==20): self.direction = Direction.UP
                elif (x ==21) and (y ==20): self.direction = Direction.UP
                elif (x ==18) and (y == 5): self.direction = Direction.DOWN
                elif (x == 9) and (y ==23): self.direction = Direction.RIGHT
                elif (x ==12) and (y ==23): self.direction = Direction.UP
                elif (x ==15) and (y ==23): self.direction = Direction.UP
                elif (x ==18) and (y ==23): self.direction = Direction.LEFT
            self.speed = 2
            self.targetX = Ghost.game.door.x[1]
            self.targetY = Ghost.game.door.y + 1
            if (self.tx == self.targetX) and (self.ty == self.targetY): 
                #print("\nRETURN >> SCATTER")
                self.switch_mode(GhostStatus.SCATTER)
            self.move()
        elif self.mode == GhostStatus.FRIGHTENED:
            r = [ int(random()*3) , int(random()*3) ]
            self.targetX = self.targets[r[0]].x + (-1 + r[1])
            self.targetY = self.targets[r[0]].y + (-1 + r[0])
            
            if Ghost.game.pacman.energized  <  Ghost.game.pacman.energyQ/2:self.gfx = GhostType.BLINK
            else: self.gfx = GhostType.PREY
            if Ghost.game.pacman.energized  <= 0:
                #print("\nFRIGHT >> SCATTER")
                self.switch_mode(GhostStatus.SCATTER)
            self.move()
        elif self.mode == GhostStatus.DEAD:
            #print("\nDEAD >> PAUSE")
            self.switch_mode(GhostStatus.PAUSE)

    def draw_box(self, x, y):
        color = Ghost.colors[self.ghost_type]
        vpu.setcolor((color&0xff0000)>>16, (color&0xff00)>>8, (color&0xff))
        vpu.fillrect(x, y, 8, 8)
        
    def draw(self):
        if Ghost.game.debug.GHOSTS:
            # Draw target
            Target.draw_static(self.targetX<<3, self.targetY<<3, Ghost.colors[self.ghost_type], True)
            
            # Draw Next possible Positions
            self.targets[0].draw()
            self.targets[1].draw()
            self.targets[2].draw()
            self.targets[3].draw()
            
            # Draw Relative Position
            Target.draw_static(self.x, self.y, Ghost.colors[self.ghost_type], False)
            
            # Draw absolute Position
            self.draw_box(self.tx<<3, self.ty<<3)
        else:
            self.frame += .05
            Ghost.gfx[self.gfx][self.direction].setframe(int(self.frame))
            Ghost.gfx[self.gfx][self.direction].draw(self.x+4, self.y+4)