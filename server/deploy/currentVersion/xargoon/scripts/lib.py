import blackbox
import vpu

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
        HudIcon.tileset = vpu.createsprite("hud",16)
        if not HudIcon.tileset:
            print("Cannot load HudIcon tileset!")
        else:
            HudIcon.gfx[HudIcon.TYPE_A] = {}
            HudIcon.gfx[HudIcon.TYPE_B] = {}
            HudIcon.gfx[HudIcon.TYPE_C] = {}
            HudIcon.gfx[HudIcon.TYPE_D] = {}
            HudIcon.gfx[HudIcon.TYPE_A][0] = vpu.createanim(16,16, HudIcon.tileset, 0, 0, 0, 0, False)
            HudIcon.gfx[HudIcon.TYPE_A][1] = vpu.createanim(16,16, HudIcon.tileset, 4, 0, 4, 0, False)
            HudIcon.gfx[HudIcon.TYPE_A][2] = vpu.createanim(16,16, HudIcon.tileset, 0, 1, 0, 1, False)
            HudIcon.gfx[HudIcon.TYPE_A][3] = vpu.createanim(16,16, HudIcon.tileset, 4, 1, 4, 1, False)
            HudIcon.gfx[HudIcon.TYPE_B][0] = vpu.createanim(16,16, HudIcon.tileset, 1, 0, 1, 0, False)
            HudIcon.gfx[HudIcon.TYPE_B][1] = vpu.createanim(16,16, HudIcon.tileset, 5, 0, 5, 0, False)
            HudIcon.gfx[HudIcon.TYPE_B][2] = vpu.createanim(16,16, HudIcon.tileset, 1, 1, 1, 1, False)
            HudIcon.gfx[HudIcon.TYPE_B][3] = vpu.createanim(16,16, HudIcon.tileset, 5, 1, 5, 1, False)
            HudIcon.gfx[HudIcon.TYPE_C][0] = vpu.createanim(16,16, HudIcon.tileset, 2, 0, 2, 0, False)
            HudIcon.gfx[HudIcon.TYPE_C][1] = vpu.createanim(16,16, HudIcon.tileset, 6, 0, 6, 0, False)
            HudIcon.gfx[HudIcon.TYPE_C][2] = vpu.createanim(16,16, HudIcon.tileset, 2, 1, 2, 1, False)
            HudIcon.gfx[HudIcon.TYPE_C][3] = vpu.createanim(16,16, HudIcon.tileset, 6, 1, 6, 1, False)
            HudIcon.gfx[HudIcon.TYPE_D][0] = vpu.createanim(16,16, HudIcon.tileset, 3, 0, 3, 0, False)
            HudIcon.gfx[HudIcon.TYPE_D][1] = vpu.createanim(16,16, HudIcon.tileset, 7, 0, 7, 0, False)
            HudIcon.gfx[HudIcon.TYPE_D][2] = vpu.createanim(16,16, HudIcon.tileset, 3, 1, 3, 1, False)
            HudIcon.gfx[HudIcon.TYPE_D][3] = vpu.createanim(16,16, HudIcon.tileset, 7, 1, 7, 1, False)
        HudIcon.weapon_type = HudIcon.TYPE_A
        HudIcon.x = int( HudIcon.game.dims[HudIcon.layer_index ][0] / 2 )-(16 + 8)
        HudIcon.y = int( HudIcon.game.dims[HudIcon.layer_index ][1] / 2 )-(16 + 8)

    @staticmethod
    def setType(weapon_type):
        HudIcon.weapon_type = weapon_type
    
    @staticmethod
    def draw():
        vpu.select( HudIcon.layer_index )
        vpu.drawanim( 
            HudIcon.gfx[HudIcon.weapon_type][HudIcon.level], 
            HudIcon.x+160, HudIcon.y
        )        
        
class BigExplosion:
    game = None
    tileset = None
    initialized = False
    
    @staticmethod
    def initialize(game):
        BigExplosion.game = game
        print("Initializing BigExplosion...")
        BigExplosion.tileset = vpu.createsprite("explosion_big")
        BigExplosion.initialized = True
        
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = vpu.createanim(BigExplosion.tileset, 32,32)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, x, y)
        if self.time > 16:
            self.alive = True

class SmallExplosion:
    TYPE_A = 0x00
    TYPE_B = 0x01    
    game = None
    tileset = None
    initialized = False

    @staticmethod
    def initialize(game):
        SmallExplosion.game = game
        print("Initializing SmallExplosion...")
        SmallExplosion.tileset = vpu.createsprite("explosion_small")
        SmallExplosion.initialized = True

    def __init__(self, x=0, y=0, type=TYPE_A):
        self.type = type
        self.x = x
        self.y = y
        self.time = 0
        self.alive = True
        self.anim = vpu.createanim(SmallExplosion.tileset, 16, 16)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, x, y)
        if self.time > 16:
            self.alive = True
    
class Flame:
    TYPE_A = 0x00
    TYPE_B = 0x04
    TYPE_C = 0x08
    TYPE_D = 0x0C
    
class Projectile:
    TYPE_A = 0x00
    TYPE_B = 0x01
    TYPE_C = 0x02
    TYPE_D = 0x03

original_colors = [
    [0, 0, 0],
    [36, 36, 36],
    [48, 48, 48],
    [26, 26, 26],
    [76, 76, 76],
    [145, 145, 145],
    [0, 0, 0],
    [0, 0, 0],
    [0, 0, 0],
    [0, 0, 0],
    [255, 122, 122],
    [255, 92, 92],
    [255, 71, 71],
    [202, 73, 73],
    [125, 44, 44],
    [87, 30, 30],
]

palettes = [
    [
        [24, 30, 40],
        [42, 53, 71],
        [58, 73, 98],
        [78, 98, 132],
        [112, 141, 191],
        [149, 188, 255],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [255, 211, 204],
        [255, 162, 154],
        [255, 112, 112],
        [227, 96, 91],
        [219, 59, 79],
        [162, 52, 65],
    ],[
        [24, 30, 40],
        [42, 53, 71],
        [58, 73, 98],
        [78, 98, 132],
        [112, 141, 191],
        [149, 188, 255],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [190, 255, 255],
        [94, 255, 255],
        [35, 228, 216],
        [71, 202, 228],
        [43, 123, 218],
        [78, 40, 255],
    ],[
        [24, 30, 0],
        [42, 53, 0],
        [58, 73, 0],
        [78, 98, 0],
        [112, 141, 0],
        [149, 188, 0],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [255, 255, 180],
        [247, 255, 127],
        [221, 202, 24],
        [205, 123, 14],
        [196, 80, 9],
        [165, 75, 18],
    ],[
        [24, 30, 0],
        [42, 53, 0],
        [58, 73, 0],
        [78, 98, 0],
        [112, 141, 12],
        [149, 188, 101],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [0, 0, 0],
        [190, 255, 101],
        [94, 255, 101],
        [35, 228, 55],
        [71, 202, 71],
        [43, 123, 58],
        [30, 87, 41]
    ]
]

class Token:
    TYPE_A      = 0x00
    TYPE_B      = 0x01
    TYPE_C      = 0x02
    TYPE_D      = 0x03
    TYPE_BOMB   = 0x04
    TYPE_POWERUP= 0x05
    TYPE_BONUS  = 0x06
    TYPE_MAX    = 0x07
    game = None
    tileset = {}
    sprite = None
    initialized = False

    @staticmethod
    def initialize(game):
        Token.game = game
        print("Initializing Tokens...")
        Token.sprite = vpu.createsprite("tokens",15)
        if not Token.sprite:
            print("ERROR: Cannot load Token tileset!")        
        print(f"Creating subsprites...")
        Token.tileset[ Token.TYPE_A      ] = vpu.subsprite(Token.sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_B      ] = vpu.subsprite(Token.sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_C      ] = vpu.subsprite(Token.sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_D      ] = vpu.subsprite(Token.sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_BOMB   ] = vpu.subsprite(Token.sprite, 0, 16, 128, 32)
        Token.tileset[ Token.TYPE_POWERUP] = vpu.subsprite(Token.sprite, 0, 32, 128, 48)
        Token.tileset[ Token.TYPE_BONUS  ] = vpu.subsprite(Token.sprite, 0, 48, 128, 64)
        print("Tinting Tokens SpriteSheet 0...", end="\r")
        vpu.tintsprite(Token.tileset[ Token.TYPE_A ], original_colors, palettes[0])
        print("Tinting Tokens SpriteSheet 1...", end="\r")
        vpu.tintsprite(Token.tileset[ Token.TYPE_B ], original_colors, palettes[1])
        print("Tinting Tokens SpriteSheet 2...", end="\r")
        vpu.tintsprite(Token.tileset[ Token.TYPE_C ], original_colors, palettes[2])
        print("Tinting Tokens SpriteSheet 3...", end="\n")
        vpu.tintsprite(Token.tileset[ Token.TYPE_D ], original_colors, palettes[3])
        Token.initialized = True
        
    def __init__(self, x=0, y=0, token_type=TYPE_A):
        self.time = 0
        self.x = x
        self.y = y
        self.alive = True
        self.token_type = token_type
        self.anim = vpu.createanim(16, 16, Token.tileset[self.token_type], 0, 0, 7, 0, False)

    def draw(self):
        if not self.alive: 
            return
        self.time+=1
        vpu.drawanim(self.anim, self.x, self.y)
        if self.time > 16:
            self.alive = True

class Ship:
    TYPE_A = 0x01
    TYPE_B = 0x02
    game = None
    tileset = None
    initialized = False

    @staticmethod
    def initialize(game):
        Ship.game = game
        print("Initializing Ship...")
        Ship.tileset = vpu.createsprite("ship01",32)
        Ship.initialized = True
    
    def __init__(self,x=0, y=0, type=TYPE_A):
        self.x = x
        self.y = y
        self.type = type
        self.status = 0
        self.shooting = False
        self.anim = vpu.createanim(16, 16,Ship.tileset)

    def draw(self):
        vpu.drawsprite(self.anim, self.x, self.y)