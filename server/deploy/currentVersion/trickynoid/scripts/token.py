from random import random
import console
import vpu
from animation import Animation

class TokenType:

    NONE        = 0x10
    GROW        = 0x00
    SHRINK      = 0x01
    SUBDIVIDE   = 0x02
    DIVIDE      = 0x03
    SHOOT       = 0x04
    MISSILE     = 0x05
    EXPLOSIVE   = 0x06
    ONEUP       = 0x07
    ONEDOWN     = 0x08
    ULTRABALL   = 0x09
    STICKBALL   = 0x0A
    SHIELD      = 0x0B
    COMMODIN    = 0x0C
    DELTASWAP   = 0x0D
    HALFSCORE   = 0x0E
    DOUBLESCORE = 0x0F

    names = {
        NONE        : "              ",
        GROW        : "     Grow     ",
        SHRINK      : "    Shrink    ",
        SUBDIVIDE   : "  Multi-Ball  ",
        DIVIDE      : " Divide Balls ",
        SHOOT       : "     Shot     ",
        MISSILE     : "    Rocket    ",
        EXPLOSIVE   : "Explosive Ball",
        ONEUP       : "  Extra Live  ",
        ONEDOWN     : "  Live  Lost  ",
        ULTRABALL   : "  Ultra Ball  ",
        STICKBALL   : " Sticky  Ball ",
        SHIELD      : "    Shield    ",
        COMMODIN    : "   Commodin   ",
        DELTASWAP   : "  Delta Swap  ",
        HALFSCORE   : "  Half Score  ",
        DOUBLESCORE : " Double Score ",
    }

class Token:

    gfx    = None
    game   = None
    width  = 11
    height = 7
    
    
    @staticmethod
    def initialize(game):
        Token.game = game
        Token.gfx  = {}
        """
        try {
            SpriteSheet tokenSheet = new SpriteSheet("data/gfx/tokens.png", TOKEN_WIDTH, TOKEN_HEIGHT, new Color(255,0,255));
            gfx = new Animation[0x10];
            for(int i=0; i < 0x10; i++) {
                gfx[i] = new Animation(tokenSheet, 0, i, 5, i,false, 60, false);            
            }
        } catch (SlickException ex) {
            gfx = null;
            Logger.getLogger(Token.class.getName()).log(Level.SEVERE, null, ex);
        }
        """

    def __init__(self, x, y, token_type):
        self.x     = x+6 # Current position, in pixels
        self.y     = y
        self.frame = 0
        if token_type > 0xF:
            token_type = int(random()*16)
        self.type = token_type
        
    def getFrame(self):
        return int(self.frame)
    
    def getType(self):
        return self.type
    
    def collides(self, paddle):
        pWidth  = paddle.getWidth()/2
        pLeft   = paddle.getX() - (pWidth)
        pRight  = paddle.getX()+ (pWidth)
        pTop    = paddle.getY()-8
        pBottom = pTop + 10
        
        if ( self.y >= pTop ) and ( self.y <= pBottom ):
            if ( self.x >= pLeft -14 ) and ( self.x <= pRight):
                return True
        return False
    
    def getY(self):
        return int(self.y)
                
    def getX(self):
        return int(self.x)
    
    def update(timeScale):
        self.y += 0.25 * timeScale
        self.frame += 0.35 * timeScale
        if self.frame >= 6.00: self.frame = 0.00
        