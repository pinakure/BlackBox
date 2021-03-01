from vpu import *

class HudIcon:
    game = None
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03
    LEVEL_0 = [ 0x00, 0x00 ]
    LEVEL_1 = [ 0x04, 0x00 ]
    LEVEL_2 = [ 0x00, 0x01 ]
    LEVEL_3 = [ 0x04, 0x01 ]
    gfx = {}
    weapon_type = TYPE_A
    level = 0
    tileset = None
    x = 0
    y = 0
    layer_index = 2
    @staticmethod
    def initialize(game):
        HudIcon.game = game
        print("Initializing HudIcon...")
        HudIcon.tileset = createsprite("hud",16)
        if not HudIcon.tileset:
            print("Cannot load HudIcon tileset!")
        else:
            HudIcon.gfx[HudIcon.TYPE_A] = {}
            HudIcon.gfx[HudIcon.TYPE_B] = {}
            HudIcon.gfx[HudIcon.TYPE_C] = {}
            HudIcon.gfx[HudIcon.TYPE_D] = {}
            HudIcon.gfx[HudIcon.TYPE_A][0] = createanim(16,16, HudIcon.tileset, 0, 0, 0, 0, False)
            HudIcon.gfx[HudIcon.TYPE_A][1] = createanim(16,16, HudIcon.tileset, 4, 0, 4, 0, False)
            HudIcon.gfx[HudIcon.TYPE_A][2] = createanim(16,16, HudIcon.tileset, 0, 1, 0, 1, False)
            HudIcon.gfx[HudIcon.TYPE_A][3] = createanim(16,16, HudIcon.tileset, 4, 1, 4, 1, False)
            HudIcon.gfx[HudIcon.TYPE_B][0] = createanim(16,16, HudIcon.tileset, 1, 0, 1, 0, False)
            HudIcon.gfx[HudIcon.TYPE_B][1] = createanim(16,16, HudIcon.tileset, 5, 0, 5, 0, False)
            HudIcon.gfx[HudIcon.TYPE_B][2] = createanim(16,16, HudIcon.tileset, 1, 1, 1, 1, False)
            HudIcon.gfx[HudIcon.TYPE_B][3] = createanim(16,16, HudIcon.tileset, 5, 1, 5, 1, False)
            HudIcon.gfx[HudIcon.TYPE_C][0] = createanim(16,16, HudIcon.tileset, 2, 0, 2, 0, False)
            HudIcon.gfx[HudIcon.TYPE_C][1] = createanim(16,16, HudIcon.tileset, 6, 0, 6, 0, False)
            HudIcon.gfx[HudIcon.TYPE_C][2] = createanim(16,16, HudIcon.tileset, 2, 1, 2, 1, False)
            HudIcon.gfx[HudIcon.TYPE_C][3] = createanim(16,16, HudIcon.tileset, 6, 1, 6, 1, False)
            HudIcon.gfx[HudIcon.TYPE_D][0] = createanim(16,16, HudIcon.tileset, 3, 0, 3, 0, False)
            HudIcon.gfx[HudIcon.TYPE_D][1] = createanim(16,16, HudIcon.tileset, 7, 0, 7, 0, False)
            HudIcon.gfx[HudIcon.TYPE_D][2] = createanim(16,16, HudIcon.tileset, 3, 1, 3, 1, False)
            HudIcon.gfx[HudIcon.TYPE_D][3] = createanim(16,16, HudIcon.tileset, 7, 1, 7, 1, False)
        HudIcon.weapon_type = HudIcon.TYPE_A
        HudIcon.x = int( HudIcon.game.dims[HudIcon.layer_index ][0] / 2 )-(16 + 8)
        HudIcon.y = int( HudIcon.game.dims[HudIcon.layer_index ][1] / 2 )-(16 + 8)

    @staticmethod
    def destroy():
        if len(HudIcon.gfx) > 0:        
            for o in range(0,4):
                for i in range(0,4):
                    if HudIcon.gfx[o][i]:
                        deleteanim(HudIcon.gfx[o][i])
                        HudIcon.gfx[o][i] = None
                HudIcon.gfx[o] = {}
            HudIcon.gfx = {}
        if HudIcon.tileset:
            deletesprite(HudIcon.tileset)
            HudIcon.tileset = None
        HudIcon.initialized = False

    @staticmethod
    def setType(weapon_type):
        HudIcon.weapon_type = weapon_type
    
    @staticmethod
    def draw():
        select( HudIcon.layer_index )
        drawanim( 
            HudIcon.gfx[HudIcon.weapon_type][HudIcon.level], 
            HudIcon.x+160, HudIcon.y
        )        
