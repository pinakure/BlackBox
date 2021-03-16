

# Makes the entity aware of an input control signal, whether is human or automatically driven
class EntityInputController(EntityController):
    def __init__(self, parent, input=None):
        EntityController.__init__(self, parent)
        self.a      = False # ()
        self.b      = False # ><
        self.x      = False # /\
        self.y      = False # []
        self.l1     = False
        self.l2     = False
        self.l3     = False
        self.r1     = False
        self.r2     = False
        self.r3     = False
        self.start  = False
        self.select = False
        self.menu   = False
        self.up        = False
        self.down   = False
        self.left   = False
        self.right  = False
        self.lhat   = [ 0, 0 ]
        self.rhat   = [ 0, 0 ]
        self.input  = input
    
    def update(self, delta):
        EntityController.update(self, delta)        
        self.a      = self.input.a()
        self.b      = self.input.b()
        self.x      = self.input.x()
        self.y      = self.input.y()
        self.l1     = self.input.l1()
        self.l2     = self.input.l2()
        self.l3     = self.input.l3()    
        self.r1     = self.input.r1() 
        self.r2     = self.input.r2()     
        self.r3     = self.input.r3()     
        self.start  = self.input.start()
        self.select = self.input.select()
        self.menu   = self.input.menu()  
        self.up     = self.input.up()  
        self.down   = self.input.down()  
        self.left   = self.input.left()  
        self.right  = self.input.right()  
        self.lhat   = self.input.lhat()  
        self.rhat   = self.input.rhat()  

# Makes the entity follow a route, generating it towards given target, if appliable
class EntityFollowController(EntityController):
    def __init__(self, parent):
        EntityController.__init__(self, parent)
        self.path        = []
        self.target_x        = 0     # Target position to move to (x coor)
        self.target_y        = 0    # Target position to move to (y coord)
        self.target_entity    = None  # If set, entity's towards the path will converge to
        self.acceleration    = 1.0
    
    def generatePath(self):
        pass

    def nextTarget(self):
        # set as target next point in path and discard current one
        if len(self.path)==0:
            self.generatePath()
        point = self.path[0]
        del self.path[0]
        self.target_x = point[0]
        self.target_y = point[1]
        
    def update(self, delta):
        EntityController.update(self, delta)        
        dx = ((self.target_x - self.parent.x) / 10) * delta
        dy = ((self.target_y - self.parent.y) / 10) * delta
        if abs(dx) < 8 and abs(dy) < 8:
            self.nextTarget()
        self.parent.controller[EntityController.CONTROLLER_MOVE].delta_x += dx * self.acceleration
        self.parent.controller[EntityController.CONTROLLER_MOVE].delta_y += dy * self.acceleration
    

# Moves the entity using its delta and manipulates its delta
class EntityMoveController(EntityController):
    def __init__(self, parent):
        EntityController.__init__(self, parent)
        self.delta_x = 0.0
        self.delta_y = 0.0
        self.decay_x = 1.0
        self.decay_y = 1.0
        self.wrap_x  = False
        self.wrap_y  = False
        self.top     = 0            # Movement space top boundary
        self.left    = 0            # Movement space left boundary
        self.right   = parent.game.width    # Movement space right boundary
        self.bottom  = parent.game.height    # Movement space bottom boundary

    def update(self, delta):
        EntityController.update(self, delta)        
        # scale delta with engine's delta value
        dx = self.delta_x * delta
        dy = self.delta_y * delta
        # calculate next position
        fx = self.parent.x + dx
        fy = self.parent.x + dy
        # delta decay
        self.delta_x *= self.decay_x
        self.delta_y *= self.decay_y
        # boundary check
        w = self.parent.width>>1
        h = self.parent.width>>1
        if   fx-w  < self.left:   fx = (w)       if not self.wrap_x else fx+(self.right  - self.left)
        elif fx+w  > self.right:  fx = (right-w)  if not self.wrap_x else fx-(self.right  - self.left)
        if   fy-h  < self.top:       fy = fy = (h)   if not self.wrap_y else fy+(self.bottom - self.top )
        elif fy+h  > self.bottom: fy = (bottom-h) if not self.wrap_y else fy-(self.bottom - self.top )
        self.parent.x = fx
        self.parent.y = fy

# Shoots if shoot signal is received 
class EntityShootController(EntityController):
    def __init__(self, parent):
        EntityController.__init__(self, parent)
        self.reload_time    = 16    # Time between shot bursts
        self.reload_timer    = 0     # Time elapsed since last bust
        self.burst_size        = 5     # Rounds shot in each burst
        self.burst_counter    = 0     # Rounds shots since burst start
        self.burst_time        = 8    # Time between shots
        self.burst_timer    = 0    # Time elapsed since last shot
        self.target_x        = 0     # Target position to shoot at (x coor)
        self.target_y        = 0    # Target position to shoot at (y coord)
        self.target_entity    = None  # If set, entity's coordinates override target_x and target_y

    def update(self, delta):
        EntityController.update(self, delta)
        # aim and shoot...

"""


"""
class ExampleEntity(Entity):
    width  = 32
    height = 32

    def __init__(self, game, name="UnnamedExampleEntity"):
        Entity.__init__(self, game, ExampleEntity.width, ExampleEntity.height, name)
        self.addController(EntityController.CONTROLLER_INPUT    , EntityInputController(self, joypad))
        self.addController(EntityController.CONTROLLER_MOVE    , EntityMoveController(self))
        self.addController(EntityController.CONTROLLER_FOLLOW    , EntityFollowController(self))
        self.addController(EntityController.CONTROLLER_SHOOT    , EntityShootController(self))

    def update(self, delta):
        Entity.update(self, delta)
        if self.controllers[EntityController.CONTROLLER_INPUT].menu:
            print("Pressed [A]")
            quit()
        
# usage:
from data.scripts.game import  Game
ent = ExampleEntity(Game, "MyEntity")
ent.update()

#
#* Game.width   = visible width (Surface::width * Surface::scale[0])
#** Game.height = visible height (Surface::width * Surface::scale[1])
#*** type_index -> must be one amongst EntityController.CONTROLLER_XXXXXX enum values
#
"""
'''