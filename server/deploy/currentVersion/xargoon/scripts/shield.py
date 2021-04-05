from vpu import *
from debug      import debug, error, panic
from random import random

class Shield:
    ORIENTATION_NORTH = 0
    ORIENTATION_NE    = 1
    ORIENTATION_EAST  = 2
    ORIENTATION_SE    = 3
    ORIENTATION_SOUTH = 4
    ORIENTATION_SW    = 5
    ORIENTATION_WEST  = 6
    ORIENTATION_NW    = 7
    TYPE_MAX = 0x03
    # ----------- #    
    gfx = {}
    game = None
    tileset = None
    # ------------#
    initialized = False
    alive = True
    orientation = ORIENTATION_NORTH
    width = 64
    height = 64
    x = 0
    y = 0
    energy = 100
    time = 0

    @staticmethod
    def initialize(game):
        Shield.game = game
        debug("Shield", "Initializing")
        Shield.tileset = createsprite("shield")
        if Shield.tileset:
            for i in range(0,8):
                Shield.gfx[ i ] =  createanim(Shield.width, Shield.height, Shield.tileset, 0, 0, 7, 0, False,1/1.5,False)
            Shield.initialized = True
        else:
            panic("Shield", "Cannot load tileset!")

    @staticmethod
    def destroy():
        for i in Shield.gfx: 
            deleteanim(i)
        Shield.gfx = {}        
        
        if Shield.tileset:
            deletesprite(Shield.tileset)
        Shield.tileset = None

        Shield.initialized = False

    @staticmethod
    def spawn(x, y, orientation):
        Shield.x = x
        Shield.y = y
        Shield.time = 0
        Shield.alive = True
        Shield.orientation = orientation
        #resetanim(Shield.gfx[Shield.orientation])
        
    @staticmethod
    def sync_position(entity):
        Shield.x = entity.x
        Shield.y = entity.y + 12

    @staticmethod
    def activate(x, y):
        #TODO: guess which direction the collision comes from
        center_x = Shield.game.ship.x
        center_y = Shield.game.ship.y
        left  = int(center_x) - (Shield.game.ship.width>>1)
        top   = int(center_y) - (Shield.game.ship.height>>1)
        right = left + Shield.game.ship.width
        bottom= top  + Shield.game.ship.height
        east = False
        west = False
        north = False
        south = False
        if   x >= left   and x < center_x: west =  True
        elif x <= right  and x > center_x: east =  True
        if   y >= top    and y < center_y: north = True
        elif y <= bottom and y > center_y: south = True
        Shield.orientation = 0
        Shield.orientation = Shield.ORIENTATION_NORTH if north and not west  and not east  else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_NE    if north and east else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_NW    if north and west else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_SOUTH if south and not west  and not east  else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_SE    if south and east else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_SW    if south and west else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_WEST  if west  and not north and not south else Shield.orientation
        Shield.orientation = Shield.ORIENTATION_EAST  if east  and not north and not south else Shield.orientation
        Shield.time = 0
        Shield.alive = True
        
    @staticmethod
    def update(delta):
        Shield.sync_position( Shield.game.ship )
        if not Shield.alive: 
            return
        frame_time = 8 * 1.5
        if (Shield.time % frame_time) == frame_time-1:
            Shield.alive = False        
            
    @staticmethod
    def draw():
        if not Shield.alive: 
            return
        drawanim(Shield.gfx[Shield.orientation], int(Shield.x), int(Shield.y), Shield.orientation*(3.141591/4))
        updateanim(Shield.gfx[Shield.orientation])
        Shield.time+=1#*delta
   