from vpu import *
from random import random

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
    TYPE_BOMB_A = 0x04
    TYPE_BOMB_B = 0x05
    TYPE_BOMB_C = 0x06
    TYPE_BOMB_D = 0x07
    TYPE_POWERUP= 0x08
    TYPE_BONUS  = 0x09
    TYPE_MAX    = 0x0A
    width       = 16
    height      = 16
    game = None
    tileset = {}
    gfx = {}
    initialized = False

    @staticmethod
    def initialize(game):
        Token.game = game
        print("Initializing Tokens...")
        sprite = createsprite("tokens",15)
        if not sprite:
            print("ERROR: Cannot load Token tileset!")        
        print(f"Creating subsprites...")        
        Token.tileset[ Token.TYPE_A      ] = subsprite(sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_B      ] = subsprite(sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_C      ] = subsprite(sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_D      ] = subsprite(sprite, 0,  0, 128, 16)
        Token.tileset[ Token.TYPE_BOMB_A ] = subsprite(sprite, 0, 16, 128, 32)
        Token.tileset[ Token.TYPE_BOMB_B ] = subsprite(sprite, 0, 16, 128, 32)
        Token.tileset[ Token.TYPE_BOMB_C ] = subsprite(sprite, 0, 16, 128, 32)
        Token.tileset[ Token.TYPE_BOMB_D ] = subsprite(sprite, 0, 16, 128, 32)
        Token.tileset[ Token.TYPE_POWERUP] = subsprite(sprite, 0, 32, 128, 48)
        Token.tileset[ Token.TYPE_BONUS  ] = subsprite(sprite, 0, 48, 128, 64)
        tintsprite(Token.tileset[ Token.TYPE_A ], original_colors, palettes[0])
        tintsprite(Token.tileset[ Token.TYPE_B ], original_colors, palettes[1])
        tintsprite(Token.tileset[ Token.TYPE_C ], original_colors, palettes[2])
        tintsprite(Token.tileset[ Token.TYPE_D ], original_colors, palettes[3])
        tintsprite(Token.tileset[ Token.TYPE_BOMB_A ], original_colors, palettes[0])
        tintsprite(Token.tileset[ Token.TYPE_BOMB_B ], original_colors, palettes[1])
        tintsprite(Token.tileset[ Token.TYPE_BOMB_C ], original_colors, palettes[2])
        tintsprite(Token.tileset[ Token.TYPE_BOMB_D ], original_colors, palettes[3])
        Token.gfx = {}
        for i in range(0, Token.TYPE_MAX):  
            Token.gfx[ i ] = createanim(16, 16, Token.tileset[ i ], 0, 0, 7, 0, False,0.125)        
        Token.initialized = True

    @staticmethod
    def destroy():
        for t in Token.gfx: 
            deleteanim(t)
        Token.gfx = {}
        
        for t in Token.tileset: 
            deletesprite(t)
        Token.tileset = {}
        
        if Token.sprite: 
            deletesprite(Token.sprite)
        Token.sprite = None
        
        Token.initialized = False
        
    def __init__(self, x=0, y=0, token_type=TYPE_A):
        self.time = 0
        self.x = x
        self.y = y
        self.delta_x = ((random()*200)/200)-1.0
        self.delta_y = ((random()*200)/200)-1.0
        self.alive = True
        self.token_type = token_type
        
    def __del__(self):
        pass

    def update(self, delta):
        if not self.alive: 
            return
        self.x += self.delta_x
        self.y += self.delta_y
        if self.x < -Token.width: self.alive = False
        elif self.y < -Token.height: self.alive = False
        elif self.x > int(Token.game.dims[1][0]): self.alive = False
        elif self.y > int(Token.game.dims[1][1]): self.alive = False

    def spawn(self):
        self.delta_x = ((random()*250)/250)*(-1 if int(random()*2)==1 else 1)
        self.delta_y = ((random()*250)/250)*(-1 if int(random()*2)==1 else 1)
        self.x = int(random()*Token.game.dims[1][0])-16
        self.y = int(random()*Token.game.dims[1][1])-16
        self.token_type = int(random() * Token.TYPE_MAX)
        self.alive = True

    def draw(self):
        if not self.alive: 
            return
        drawanim(Token.gfx[self.token_type], int(self.x), int(self.y))
        