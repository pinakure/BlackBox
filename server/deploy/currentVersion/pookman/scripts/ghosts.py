import          vpu
from entity     import Entity
from animation  import *

DOOR_X = [13, 14]
DOOR_Y = 12


class Direction:
    UP    = 0x00
    RIGHT = 0x01
    DOWN  = 0x02
    LEFT  = 0x03

    @staticmethod
    def string(direction):
        if direction == Direction.UP:       return "UP"
        if direction == Direction.RIGHT:    return "RIGHT"
        if direction == Direction.DOWN:     return "DOWN"
        if direction == Direction.LEFT:     return "LEFT"

class Target:
    def __init__(self, ghost):
        self.ghost     = ghost
        self.x         = 0
        self.y         = 0
        self.distance  = 0
        self.direction = Direction.UP
        self.solid     = False
        self.viable    = False
        self.color     = 0xffff00

    @staticmethod
    def compare(a, b):
        return a.distance > b.distance

    def lock(self, x, y): #settarget!
        self.viable    = True
        self.x         = x
        self.y         = y
        # Calculate distance to target (ox, oy)
        ox = self.ghost.targetX
        oy = self.ghost.targetY
        self.distance  = sqrt( ((self.x - ox) * (self.x - ox)) + ((self.y - oy) * (self.y - oy)) )
        
        # Forgive me about this monstruosity, seems I can't make work this sentence splitted in several lines.
        self.direction = Direction.RIGHT if self.x > self.ghost.tx else Direction.LEFT  if self.x < self.ghost.tx else Direction.DOWN  if self.y > self.ghost.ty else Direction.UP    if self.y < self.ghost.ty else Direction.DOWN
        
        # Set viability and color
        self.solid = self.ghost.is_solid(self.x, self.y)
        if self.solid:
            self.viable = False
            self.color  = 0x0000ff
        else:
            self.viable = True
            self.color  = 0x666000

    def __drawstatictarget(x, y, color, is_target):
        #ctx = game.graphics.context[game.graphics.defaultContext]
        if is_target:
            pass
            """
            ctx.strokeStyle = color
            ctx.beginPath()
            ctx.moveTo(x,     y+24)
            ctx.lineTo(x+8, y+8+24)
            ctx.moveTo(x, y+8+24)
            ctx.lineTo(x+8, y+24)
            ctx.stroke()
            """
        else:
            pass
            """
            ctx.fillStyle = color
            ctx.fillRect(x,     y + 24, 8,8)
            ctx.clearRect(x+1,     y + 25, 6,6)
            ctx.clearRect(x,     y + 26, 8,4)
            ctx.clearRect(x+2,    y + 24, 4,8)
            """

    def draw(self):
        x = self.x<<3
        y = self.y<<3
        """
        ctx = g.context[g.defaultContext];    
        ctx.fillStyle = self.color
        ctx.fillRect(x,     y + 24, 8,8)
        ctx.clearRect(x+1,    y + 25, 6,6)
        ctx.clearRect(x,     y + 26, 8,4)
        ctx.clearRect(x+2,    y + 24, 4,8);    
        
        ctx.strokeStyle = self.color
        ctx.beginPath()
        switch(self.direction){
            case DIR_LEFT:
                ctx.moveTo(x,y+28)
                ctx.lineTo(x+8, y+24)
                ctx.moveTo(x,y+28)
                ctx.lineTo(x+8, y+32)
                break
            case DIR_DOWN:
                ctx.moveTo(x+4,y+32)
                ctx.lineTo(x, y+24)
                ctx.moveTo(x+4,y+32)
                ctx.lineTo(x+8, y+24)
                break
            case DIR_UP:
                ctx.moveTo(x+4,y+24)
                ctx.lineTo(x, y+32)
                ctx.moveTo(x+4,y+24)
                ctx.lineTo(x+8, y+32)
                break
            case DIR_RIGHT:
                ctx.moveTo(x+8,y+28)
                ctx.lineTo(x, y+24)
                ctx.moveTo(x+8,y+28)
                ctx.lineTo(x, y+32)
                break;            
                break
        }
        ctx.stroke();            
        """
        
    def __str__(self):
        status = "SOLID" if self.viable and self.solid else "VIABLE" if self.viable else "FORBID"
        return     f"[ {Direction.string(self.direction)} { status } D: {self.distance} ]"
    
class GhostStatus:
    CHASE        = 0x00
    SCATTER        = 0x01
    FRIGHTENED    = 0x02
    RETURNING    = 0x03 # Lone Eyes
    LAST        = 0x04
    PAUSE        = 0x05

class GhostType:
    BLINKY    = 0x00
    INKY     = 0x01
    PINKY     = 0x02
    CLYDE     = 0x03
    PREY    = 0x04
    DEAD    = 0x05
    BLINK     = 0x06

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
        w = Ghost.game.map_width
        c = Ghost.game.map[parseInt( ( y * w) + x)]
        return c < 31;#31 is the door!
        
    @staticmethod
    def initialize(game):
        Ghost.game = game
        Ghost.sprite = vpu.createsprite("ghosts")       
        Ghost.gfx = {}        
        for i in range(0,4):
            Ghost.gfx[i] = {
                Direction.UP     : Animation(16, 16, Ghost.sprite, 0, i,  1, i, False, 0.25, True, AnimationLoop.PINGPONG),
                Direction.DOWN   : Animation(16, 16, Ghost.sprite, 2, i,  3, i, False, 0.25, True, AnimationLoop.PINGPONG),
                Direction.LEFT   : Animation(16, 16, Ghost.sprite, 4, i,  5, i, False, 0.25, True, AnimationLoop.PINGPONG),
                Direction.RIGHT  : Animation(16, 16, Ghost.sprite, 6, i,  7, i, False, 0.25, True, AnimationLoop.PINGPONG),
            }
        # Eyes animation
        Ghost.gfx[GhostType.DEAD] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.DEAD, 1, GhostType.DEAD , False, 0.25, True, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 2, GhostType.DEAD, 3, GhostType.DEAD , False, 0.25, True, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 4, GhostType.DEAD, 5, GhostType.DEAD , False, 0.25, True, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 6, GhostType.DEAD, 7, GhostType.DEAD , False, 0.25, True, AnimationLoop.PINGPONG),
        }        
        # Scared and Scared blinking 
        Ghost.gfx[GhostType.PREY] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 0, GhostType.PREY, 3, GhostType.PREY , False, 0.125, True, AnimationLoop.PINGPONG),
        }
        Ghost.gfx[GhostType.BLINK] = {
            Direction.RIGHT  : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.LEFT   : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.DOWN   : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, True, AnimationLoop.PINGPONG),
            Direction.UP     : Animation(16,16, Ghost.sprite, 0, GhostType.BLINK,7, GhostType.BLINK, False, 0.125, True, AnimationLoop.PINGPONG),
        }
        Ghost.initialized = True

    def __init__(self, ghost_type=GhostType.BLINKY):
        self.ghost_type     = ghost_type
        
        # Positioning registers    
        self.x              = Ghost.spawn[self.ghost_type][0];        #Real X (with decimals)
        self.y              = Ghost.spawn[self.ghost_type][1];        #Real Y (with decimals)
        self.rx             = int(self.x)
        self.ry             = int(self.y)
        self.tx             = int(self.rx / 8);        #Tile X
        self.ty             = int(self.ry / 8);        #Tile Y 

        self.inIntersection = False
        self.inHouse        = False
        self.inDoor         = False
        self.behindDoor     = False
        self.inFrontOfDoor  = False
        self.inFrontOfHouse = False
        self.mustLeave      = False
        self.inTunnel       = False
        self.speed          = 1
        self.timer          = 0
        self.gfx            = self.ghost_type #current displayed animation
        self.direction      = Direction.LEFT
        self.mode           = GhostStatus.SCATTER
        self.frame          = 0
        self.chaseTime      = 0
        self.scaterTime     = 0
        self.targetX        = GhostTarget[type][0]
        self.targetY        = GhostTarget[type][1]
        self.targets        = [ Target(self), Target(self), Target(self), Target(self) ]
        self.sprite         = False

    def reset(self):
        # Positioning registers    
        self.x              = Ghost.spawn[self.ghost_type][0];        #Real X (with decimals)
        self.y              = Ghost.spawn[self.ghost_type][1];        #Real Y (with decimals)
        self.rx             = int(self.x)
        self.ry             = int(self.y)
        self.tx             = int(self.rx / 8);        #Tile X
        self.ty             = int(self.ry / 8);        #Tile Y 
        self.inIntersection = False
        self.inHouse        = False
        self.inDoor         = False
        self.behindDoor     = False
        self.inFrontOfDoor  = False
        self.inFrontOfHouse = False
        self.mustLeave      = False
        self.inTunnel       = False
        self.speed          = 1
        self.timer          = 0
        self.gfx            = self.ghost_type
        self.direction      = Direction.LEFT
        self.mode           = GhostStatus.SCATTER
        self.frame          = 0
        self.chaseTime      = 0
        self.scaterTime     = 0
        self.targetX        = Ghost.default_target[self.ghost_type][0]
        self.targetY        = Ghost.default_target[self.ghost_type][1]
        #self.updateFlags()
        
    def draw_box(self, x, y):
        ctx = self.game.engine.graphics.context[self.game.engine.graphics.defaultContext];    
        ctx.fillStyle = GhostColors[self.ghost_type]
        ctx.fillRect(x, y + 24, 8,8)
    
    def handle_house(self, delta):
        if self.inHouse:
            if self.mustLeave:
                
                self.targetY = DOOR_Y - 1
                self.targetX = DOOR_X[0]
                
                # This ghost must leave the house
                if self.inDoor:self.direction = Direction.UP
                if self.behindDoor:
                    self.x = self.tx * 8
                    self.direction = Direction.UP
            elif self.behindDoor:
                self.direction = Direction.LEFT

            if self.inIntersection:
                self.setTargets([self.tx, self.tx, self.tx, self.tx], [self.ty, self.ty, self.ty, self.ty])
                onTop    = (self.ty <= DOOR_Y+1) and ((self.y%8)==0)
                onBottom =  self.ty >= DOOR_Y+2
                onLeft   =  self.tx <= DOOR_X[0]-2
                onRight  =  self.tx >= DOOR_X[1]+1
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
        self.calcNext()
        self.targets.sort(Target.compare)
        for i in range(1,4):
            if self.targets[i].direction == DirOpposite[self.direction]:
                self.targets[i].viable = False
        for i in range(1,4):
            if self.targets[i].viable: 
                self.targets[i].color = 0xfff000
                return self.targets[i]
        for i in range(1,4):
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
        
        if self.inHouse: return True
        
        map     = self.game.map.map.layers[0].data
        row     = Ghost.game.map_width
        r       = (self.ty * row) + self.tx

        hInt = (self.x % 8) == 0
        vInt = (self.y % 8) == 0
        
        if direction == Direction.RIGHT : return ((map[r + 1]                           > 30) and (vInt)) or self.inTunnel
        elif direction==Direction.DOWN  : return ((map[((self.ty+1) * row) + self.tx]   > 30) and (hInt)) or self.inTunnel
        elif direction==Direction.LEFT  : return ((map[r - 1]                           > 30) and (vInt)) or (not hInt) or self.inTunnel
        elif direction==Direction.UP    : return ((map[((self.ty-1) * row) + self.tx]   > 30) and (hInt)) or (not vInt) or self.inTunnel
        return False
        
    def update_flags(self):
        # Updates Ghost internal flags. TBC initially and each time the ghost moves
        tx = self.tx 
        ty = self.ty
        x  = self.x
        y  = self.y
        cx = ((self.rx - self.x) + (self.x%8))
        cy = ((self.ry - self.y) + (self.y%8))
        
        self.inIntersection  = ( ((cx>8-self.speed) or (cx<0+self.speed)) and ((cy>8-self.speed) or (cy< 0+self.speed)) )
        self.inHouse         = (((tx >= 11) and (ty >= 12)) and ((tx <= 16) and (ty <= 15)))
        self.inDoor          = ((ty == DOOR_Y) and ((tx >= DOOR_X[0]) and (tx <= DOOR_X[1]))) and ((self.x % 8)==0)
        self.behindDoor      = ((((y % 8)==0)) and ((ty == DOOR_Y+1) and ((tx >= DOOR_X[0]) and (tx <= DOOR_X[1]))))
        self.inFrontOfDoor   = ((ty == DOOR_Y-1) and ((tx >= DOOR_X[0]) and (tx <= DOOR_X[1])))
        self.inFrontOfHouse  = ((y % 8 == 0) and ((ty == DOOR_Y - 1) and (tx >= DOOR_X[0]-1) and (tx <= DOOR_X[1]+1)))
        self.inTunnel        = ( (self.tx <= 0) or (self.tx >= Ghost.game.map_width-1) or (self.ty >= Ghost.game.map_height-1) or (self.ty <= 0) )
        self.mustLeave       = (self.timer >= Ghost.delay[self.ghost_type])

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
        self.targets[0].lock(self.tx,    self.ty-1)
        self.targets[1].lock(self.tx-1,  self.ty)
        self.targets[2].lock(self.tx,    self.ty+1)
        self.targets[3].lock(self.tx+1,  self.ty)
        
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
            self.gfx    = GHOST_PREY
            self.reverse()            
        elif self.mode == GhostStatus.RETURNING:
            self.gfx    = GHOST_DEAD
            self.reverse()
        self.mode = mode

    def reverse(self):
        if self.direction == Direction.RIGHT :self.direction = DIR_LEFT
        elif self.direction == Direction.DOWN:self.direction = DIR_UP
        elif self.direction == Direction.LEFT:self.direction = DIR_RIGHT
        elif self.direction == Direction.UP  :self.direction = DIR_DOWN
     
    def move(self):
        # Animate - not needed since autoupdate is on
        #Ghost.gfx[self.gfx][self.direction].update()
        
        self.set_speed()

        if not self.can_walk(self.direction):
            self.move_to(self.x, self.y)
        
        if self.can_walk(self.direction):
            # switch(self.direction){
                # case DIR_LEFT:    self.moveTo(self.rx-self.speed,self.ty<<3); break
                # case DIR_RIGHT:    self.moveTo(self.rx+self.speed,self.ty<<3); break
                # case DIR_UP:     self.moveTo(self.tx<<3,self.ry-self.speed); break
                # case DIR_DOWN:    self.moveTo(self.tx<<3,self.ry+self.speed); break;            
            # }
            if self.direction  ==  Direction.LEFT  : self.move_to( self.rx - self.speed , int(( self.ry + self.speed ) / 8 ) * 8 )
            elif self.direction == Direction.RIGHT : self.move_to( self.rx + self.speed , int(( self.ry + self.speed ) / 8 ) * 8 ) 
            elif self.direction == Direction.UP    : self.move_to( int(( self.rx + self.speed ) / 8 ) * 8 , self.ry - self.speed ) 
            elif self.direction == Direction.DOWN  : self.move_to( int(( self.rx + self.speed ) / 8 ) * 8 , self.ry + self.speed ) 
        else:
            print(f" Cant go {self.direction}")
        
        if self.inTunnel: return
            
        if self.inIntersection:
            #self.centerInTile()
            nearest = self.nearest_target()
            # if nearest.direction != self.direction:
            #     print(f"Turning {self.direction}")
                       
            if self.can_walk(nearest.direction):
                self.direction = nearest.direction;            
            else:
                print(f"Going {self.direction} cant turn {nearest.direction} from {self.tx},{self.ty} to {nearest.x},{nearest.y}")

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
        self.updateFlags()

    def update(self, delta):
        """                
        # Perform specific actions depending on current status
        switch(self.mode){
            case STATUS_PAUSE:
                if(self.game.pacman.status != PACMAN_DEAD) self.switchMode(STATUS_SCATTER)
                return
                
            case STATUS_SCATTER:
                if(!self.handleHouse()){
                    if(self.game.pacman.energized == self.game.pacman.energyQ - 1) {
                        self.switchMode(STATUS_FRIGHTENED);        
                        break
                    }
                
                    self.targetX = GhostTarget[self.ghost_type][0]
                    self.targetY = GhostTarget[self.ghost_type][1]
                    
                    #when timer is over, jump to chase mode!
                    if(self.scaterTime > 400) self.switchMode(STATUS_CHASE)
                    else self.scaterTime++
                }
                self.move()
                break
                
            case STATUS_CHASE: 
                if(self.game.pacman.energized == self.game.pacman.energyQ - 1) {
                    self.switchMode(STATUS_FRIGHTENED);        
                    break
                }
                
                self.setChaseTarget()
                
                #when timer is over, jump to scatter mode!
                if(self.ghost_type != GHOST_BLINKY){
                    if(self.chaseTime > 400) self.switchMode(STATUS_SCATTER)
                    else self.chaseTime++
                }
                self.move()
                break
            
            case STATUS_RETURNING:
                x = self.tx
                y = self.ty

                # Help eyes to reach home in certain checkpoints
                if(self.inIntersection){
                    self.x = self.tx<<3
                    self.y = self.ty<<3
                    if(((x == 6)&&(y == 14))&&(self.inIntersection))self.direction = DIR_RIGHT
                    else if((x ==21)&&(y ==14)) self.direction = DIR_LEFT
                    else if((x == 9)&&(y == 5)) self.direction = DIR_DOWN
                    else if((x == 6)&&(y ==20)) self.direction = DIR_UP
                    else if((x ==21)&&(y ==20)) self.direction = DIR_UP
                    else if((x ==18)&&(y == 5)) self.direction = DIR_DOWN
                    else if((x == 9)&&(y ==23)) self.direction = DIR_RIGHT
                    else if((x ==12)&&(y ==23)) self.direction = DIR_UP
                    else if((x ==15)&&(y ==23)) self.direction = DIR_UP
                    else if((x ==18)&&(y ==23)) self.direction = DIR_LEFT
                }
                self.speed = 2
                self.targetX = DOOR_X[1]
                self.targetY = DOOR_Y + 1
                if((self.tx == self.targetX)&&(self.ty == self.targetY)) self.switchMode(STATUS_SCATTER)
                self.move()
                break
                
            case STATUS_FRIGHTENED:
                r = [parseInt(Math.random()*3),parseInt(Math.random()*3)]
                self.targetX = self.targets[r[0]].x + (-1 + r[1])
                self.targetY = self.targets[r[0]].y + (-1 + r[0])
                
                if(self.game.pacman.energized < self.game.pacman.energyQ/2) self.gfx = GHOST_BLINK; else self.gfx = GHOST_PREY
                if(self.game.pacman.energized <= 0) self.switchMode(STATUS_SCATTER)
                self.move()
                break
        }

        if(self.game.pacman.status == PACMAN_DEAD) self.switchMode(STATUS_PAUSE)
        """
        pass
    
    def draw(self):
        """
        self.game.engine.graphics.setAlpha(1)
        #$('#debug_l').text(self.
        
        if(debug[DEBUG_GHOSTS]){
            # Draw target
            Target.prototype.drawStaticTarget(self.targetX<<3,     self.targetY<<3, GhostColors[self.ghost_type], True)
            
            self.game.engine.graphics.setAlpha(0.5);        
            
            
            # Draw Next possible Positions
            self.targets[0].draw(self.game.graphics)
            self.targets[1].draw(self.game.graphics)
            self.targets[2].draw(self.game.graphics)
            self.targets[3].draw(self.game.graphics)
            
            # Draw Relative Position
            Target.prototype.drawStaticTarget(self.x,                 self.y,            GhostColors[self.ghost_type], False)
            
            # Draw absolute Position
            self.game.engine.graphics.setAlpha(.4)
            self.drawBox(self.tx<<3, self.ty<<3)
            
            # Set opacity for sprite almost transparent
            self.game.engine.graphics.setAlpha(.05)
        } else self.game.engine.graphics.setAlpha(.65)
        
        Ghost.gfx[self.gfx][self.direction].currentFrame = parseInt(self.frame)
        Ghost.gfx[self.gfx][self.direction].draw(self.x - 4, self.y + 20)
        
        self.game.engine.graphics.setAlpha(1)
        
        #if(self.inIntersection)debugger
        """
        pass