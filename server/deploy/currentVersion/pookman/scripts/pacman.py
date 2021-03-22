import                          vpu
from entity                     import Entity
from animation                  import * 
from direction                  import * 
from data.scripts.pacmanstatus  import PacmanStatus
from data.scripts.ghoststatus   import GhostStatus
from data.scripts.ghosttype     import GhostType
from data.scripts.ghosts        import Ghost


class Pacman(Entity):
    initialized = False
    game        = None
    sprite      = None
    gfx         = {}

    rx 		    = 0
    ry	 		= 0
    x 			= 0
    y 			= 0
    tx 		    = 0
    ty	 		= 0
    steady		= True

    status		= PacmanStatus.DEAD
    energized	= 0
    energyQ	    = 500

    inIntersection = False

    speed 		= 1.25
    frame 		= 0
    frameDir 	= 1
    frameSpd 	= 0.75
    eating		= False
    multiplier	= 1	

    direction	= Direction.RIGHT
    nextDir	    = Direction.RIGHT

    @staticmethod
    def initialize(game):
        Pacman.game = game
        Entity.__init__(Pacman, Pacman.game)
        Pacman.sprite = vpu.createsprite("pacman")
        if Pacman.sprite:            
            Pacman.gfx = {}
            Pacman.gfx[Direction.RIGHT] = Animation(16,16, Pacman.sprite, 0,  0, 15, 0, False, 0, False)
            Pacman.gfx[Direction.DOWN ] = Animation(16,16, Pacman.sprite, 0,  1, 15, 1, False, 0, False)
            Pacman.gfx[Direction.LEFT ] = Animation(16,16, Pacman.sprite, 0,  2, 15, 2, False, 0, False)
            Pacman.gfx[Direction.UP   ] = Animation(16,16, Pacman.sprite, 0,  3, 15, 3, False, 0, False)
            Pacman.initialized = True
        else:
            Pacman.initialized = False
            print('Failed to load graphics');                
        
        Pacman.rx 		    = (13*8)+4
        Pacman.ry	 		= (23*8)
        Pacman.x 			= int(Pacman.rx)
        Pacman.y 			= int(Pacman.ry)
        Pacman.tx 		    = 1
        Pacman.ty	 		= 1
        Pacman.steady		= True

        Pacman.status		= PacmanStatus.DEAD
        Pacman.energized	= 0
        Pacman.energyQ	    = 500

        Pacman.inIntersection = False

        Pacman.speed 		= 1.25 #.8;
        Pacman.frame 		= 0
        Pacman.eating		= False
        Pacman.multiplier	= 1	

        Pacman.direction	= Direction.RIGHT
        Pacman.nextDir	    = Direction.RIGHT

        Pacman.ready()

    @staticmethod
    def destroy():
        pass
    
    def ready():
        Pacman.steady	    = True
        Pacman.ry	 	    = (23*8)
        Pacman.rx 		    = (13*8)+4
        Pacman.x 		    = Pacman.rx
        Pacman.y		    = Pacman.ry
        Pacman.tx 		    = int(Pacman.x / 8)
        Pacman.ty	 	    = int(Pacman.y / 8)
        Pacman.game.pills   = 0
        Pacman.spawn()  

    def draw():
        if Pacman.status == PacmanStatus.DEAD: return

        if Pacman.status not in (PacmanStatus.DIEING,PacmanStatus.DEAD):
            Pacman.gfx[Pacman.direction].setframe(9 + int(Pacman.frame))
        else:
            Pacman.gfx[Pacman.direction].setframe(int(Pacman.frame))
        print(f"{Direction.string(Pacman.direction)}  -> {Pacman.frame}", end="      \r")
            
        Pacman.gfx[Pacman.direction].draw(Pacman.rx+4, Pacman.ry + 4)

        if Pacman.game.debug.PACMAN:
            vpu.setcolor(64,64,0, 64)
            x = Pacman.tx * 8
            y = Pacman.ty * 8
            vpu.fillrect(x,y,8,8)
            
            x = Pacman.x
            y = Pacman.y            
            vpu.setcolor(128,128,0,128)            
            vpu.fillrect(x, y , 8, 8)
            vpu.setcolor(255, 255, 0, 128)
            vpu.fillrect(x+1,y+1, 6 , 6 )
            vpu.fillrect( x ,y+2, 8 , 4 )
            vpu.fillrect(x+2, y , 4 , 8 )            
            

    def spawn():
        Pacman.tx         = 13
        Pacman.ty         = 23
        Pacman.rx         = (Pacman.tx * 8) + 4
        Pacman.ry         = (Pacman.ty * 8)
        Pacman.x          = Pacman.rx
        Pacman.y          = Pacman.ry
        Pacman.status     = PacmanStatus.READY
        Pacman.direction  = Direction.RIGHT
        if Pacman.game.blinky: 
            Pacman.game.blinky.reset()
            Pacman.game.inky.reset()
            Pacman.game.pinky.reset()
            Pacman.game.clyde.reset()        
    def die():
        Pacman.status     = PacmanStatus.DIEING
        Pacman.frame      = 15

    def set_speed():
        # Adjust settings for level > 19  
        energy_level = 1
        eating_speed = 0.79 if Pacman.energized > 0 else 0.79
        normal_speed =  0.9 if Pacman.energized > 0 else  0.9

        if Pacman.game.level == 0:
            energy_level = 500
            eating_speed = 0.79 if Pacman.energized > 0 else 0.71
            normal_speed =  0.9 if Pacman.energized > 0 else  0.8
        elif Pacman.game.level < 4:
            energy_level = 250
            eating_speed = 0.83 if Pacman.energized > 0 else 0.79
            normal_speed = 0.95 if Pacman.energized > 0 else  0.9            
        elif Pacman.game.level < 19:
            energy_level = 125
            eating_speed = 0.87 if Pacman.energized > 0 else 0.87
            normal_speed = 1.00 if Pacman.energized > 0 else 1.00
        
        Pacman.energyQ = energy_level
        Pacman.speed = eating_speed if Pacman.eating else normal_speed
        

    def update(delta):
        i = (Pacman.ty * 28) + Pacman.tx
        if Pacman.energized > 0:
            if Pacman.game.is_ghost(i):
                Pacman.eat_ghost(i)
            Pacman.energized -= 1
        else:
            if Pacman.game.is_ghost(i):
                Pacman.die()
                return
        
        Pacman.set_speed()

        if Pacman.status  ==  PacmanStatus.DEAD:
            Pacman.game.resetSystems()
            return        
        elif Pacman.status == PacmanStatus.DIEING:
            Pacman.game.freeze = 5
            if Pacman.frame > 0:
                Pacman.frame-=1
            else: 
                Pacman.status = PacmanStatus.DEAD
            Pacman.gfx[Pacman.direction].setframe(Pacman.frame)
            return
        elif Pacman.status == PacmanStatus.READY:
            #Pacman.energized = 0;
            if not Pacman.steady: 
                if Pacman.can_walk(Pacman.nextDir):
                    Pacman.direction  = Pacman.nextDir
                    Pacman.status     = PacmanStatus.WALKING            
        elif Pacman.status == PacmanStatus.WALKING:
            if not Pacman.can_walk(Pacman.direction):
                Pacman.status     = PacmanStatus.READY
                Pacman.x          = (Pacman.x>>1)<<1
                Pacman.y          = (Pacman.y>>1)<<1
                Pacman.rx         = Pacman.x 
                Pacman.ry         = Pacman.y
            
            if Pacman.can_walk(Pacman.nextDir):
                if Pacman.direction != Pacman.nextDir:
                    Pacman.rx = Pacman.x
                    Pacman.ry = Pacman.y
                Pacman.direction = Pacman.nextDir
                    
            if Pacman.can_walk(Pacman.direction):
                if Pacman.direction  ==  Direction.RIGHT: Pacman.move_right()	
                elif Pacman.direction == Direction.DOWN : Pacman.move_down()
                elif Pacman.direction == Direction.LEFT : Pacman.move_left()
                elif Pacman.direction == Direction.UP   : Pacman.move_up()
        
                # Animate Pacman
                Pacman.frame += Pacman.frameSpd * Pacman.frameDir;			
                if Pacman.frame > 6: 
                    Pacman.frame      = 6
                    Pacman.frameDir   = -1
                if Pacman.frame < 0: 
                    Pacman.frame      = 0
                    Pacman.frameDir   =  1
                
        
            # Exit if pacman is outside the map 
            if Pacman.tx > Pacman.game.map.width : return
            if Pacman.ty > Pacman.game.map.height: return
            
            # Check if there is food in the middle of Pacman position 
            if Pacman.inIntersection:
                i = (Pacman.ty * Pacman.game.map.width) + Pacman.tx
                if Pacman.game.is_pill(i)	    : Pacman.eat_pill(i)		
                elif Pacman.game.is_energizer(i): Pacman.eat_energizer(i)
        
    def eat_ghost(position):
        # Set ghost status to scared (eyes returning to the zone)

        Pacman.game.score   += (200 * Pacman.multiplier)
        Pacman.multiplier	*= 2
        Pacman.eating		= True
        Pacman.game.freeze	= 25

        w = Pacman.game.map.width
        h = Pacman.game.map.height
        
        pos = {
            GhostType.BLINKY : (Pacman.game.blinky.ty	* w) + Pacman.game.blinky.tx,
            GhostType.PINKY  : (Pacman.game.pinky.ty 	* w) + Pacman.game.pinky.tx ,
            GhostType.INKY   : (Pacman.game.inky.ty 	* w) + Pacman.game.inky.tx  ,
            GhostType.CLYDE  : (Pacman.game.clyde.ty 	* w) + Pacman.game.clyde.tx , 
        }

        if (pos[ GhostType.BLINKY ] == position) and (Pacman.game.blinky.mode == GhostStatus.FRIGHTENED): return Pacman.game.blinky.switch_mode(GhostStatus.RETURNING)
        if (pos[ GhostType.PINKY  ] == position) and (Pacman.game.pinky.mode  == GhostStatus.FRIGHTENED): return Pacman.game.pinky.switch_mode(GhostStatus.RETURNING)
        if (pos[ GhostType.INKY   ] == position) and (Pacman.game.inky.mode   == GhostStatus.FRIGHTENED): return Pacman.game.inky.switch_mode(GhostStatus.RETURNING)
        if (pos[ GhostType.CLYDE  ] == position) and (Pacman.game.clyde.mode  == GhostStatus.FRIGHTENED): return Pacman.game.clyde.switch_mode(GhostStatus.RETURNING)

    def eat_pill(i):
        x = i % Pacman.game.map.width
        y = int(i / Pacman.game.map.width)
        Pacman.game.map.set(x, y, 40, 0)
        Pacman.game.score += 10
        Pacman.game.map.redraw()
        Pacman.eating = True
        if Pacman.game.blinky.timer	 < Ghost.delay[GhostType.BLINKY]: Pacman.game.blinky.timer +=1
        elif Pacman.game.pinky.timer < Ghost.delay[GhostType.PINKY ]: Pacman.game.pinky.timer  +=1
        elif Pacman.game.inky.timer	 < Ghost.delay[GhostType.INKY  ]: Pacman.game.inky.timer   +=1	
        elif Pacman.game.clyde.timer < Ghost.delay[GhostType.CLYDE ]: Pacman.game.clyde.timer  +=1

    def eat_energizer(i):
        x = i % Pacman.game.map.width
        y = int(i / Pacman.game.map.width)
        Pacman.game.map.set(x, y, 40, 0)
        Pacman.energized += Pacman.energyQ
        Pacman.game.score += 50
        Pacman.multiplier	= 1
        Pacman.game.map.redraw()
        Pacman.eating = True

    def can_walk(direction):
        w = Pacman.game.map.width
        h = Pacman.game.map_height
           
        right	= Pacman.tx + 1
        down	= Pacman.ty + 1
        left 	= Pacman.tx - 1
        up 		= Pacman.ty - 1

        outside	= (Pacman.tx < 1) or (Pacman.ty < 1) or (Pacman.tx >= w-1) or (Pacman.ty >= h-1)
        hInt    = (Pacman.x % 8) == 0
        vInt    = (Pacman.y % 8) == 0

        if direction  ==  Direction.RIGHT: return ((Pacman.game.map.get(right, Pacman.ty) > 30) and (vInt)) or (outside)
        elif direction == Direction.DOWN : return ((Pacman.game.map.get(Pacman.tx,  down) > 30) and (hInt)) or (outside)
        elif direction == Direction.LEFT : return ((Pacman.game.map.get(left,  Pacman.ty) > 30) and (vInt)) or (not hInt) or (outside)
        elif direction == Direction.UP   : return ((Pacman.game.map.get(Pacman.tx,    up) > 30) and (hInt)) or (not vInt) or (outside)

    def go(next_direction):
        Pacman.steady = False
        # Avoid entering the ghost house!
        if Pacman.ty == Pacman.game.door.y - 1:
            if next_direction == Direction.DOWN:
                if (Pacman.tx >= Pacman.game.door.x[0]-2) and (Pacman.tx <= Pacman.game.door.x[1]+2): return
        Pacman.nextDir = Pacman.direction
        if Pacman.x > (Pacman.game.map.width  * 8) - 16: return
        if Pacman.y > (Pacman.game.map.height * 8) - 16: return
        if Pacman.y < 8: return
        if Pacman.x < 8: return
        Pacman.nextDir = next_direction

    def clamp():
        w       = Pacman.game.map.width
        h       = Pacman.game.map.height
        tw      = 8
        th      = 8
        height	= h * th
        width 	= w * tw

        if Pacman.ry > height	+ 16: Pacman.ry -= height + 24
        elif Pacman.ry   <   -  16: Pacman.ry += height + 24
            
        if Pacman.rx > width +  16: Pacman.rx -= width  + 24
        elif Pacman.rx   <   -  16: Pacman.rx += width  + 24

        Pacman.x  = int(Pacman.rx)
        Pacman.y  = int(Pacman.ry)

        Pacman.ty = int(Pacman.y / 8)
        Pacman.tx = int(Pacman.x / 8)

        #Pacman.inIntersection = (((Pacman.y % 8) == 0) and ((Pacman.x % 8) == 0))
        corner = 4.5
        Pacman.inIntersection  = (((((Pacman.rx % 8) > 8-(corner+Pacman.speed) )or((Pacman.rx % 8)< 1 + (corner + Pacman.speed) )))and(((Pacman.ry % 8) > 8-(corner + Pacman.speed) )or((Pacman.ry % 8)< 0 + (corner + Pacman.speed) )));

    def move_right():
        Pacman.rx += Pacman.speed
        Pacman.clamp()

    def move_down():
        Pacman.ry += Pacman.speed
        Pacman.clamp()

    def move_left():
        Pacman.rx -= Pacman.speed
        Pacman.clamp()

    def move_up():
        Pacman.ry -= Pacman.speed
        Pacman.clamp()
