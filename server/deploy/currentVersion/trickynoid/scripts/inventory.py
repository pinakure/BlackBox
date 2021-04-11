from data.scripts.token import Token, TokenType

class Inventory:
    game = None

    @staticmethod
    def initialize(game):
        Inventory.game = game

    def __init__(self):
        self.ammo        = 50
        self.grow        = 50
        self.shrink      = 50
        self.subdivide   = 1
        self.commodin    = 1
        self.shield      = 1
        self.stickyball  = 1
        self.ultraball   = 1
        self.explosive   = 1
        self.missile     = 1
        # Instantaneos
        self.divide      = False
        self.inverted    = False
        self.onedown     = False
        self.oneup       = False
        self.halfscore   = False
        self.doublescore = False
    
    def inheritToken(self, token_type):
        if   token_type == TokenType.GROW:        self.grow       += 10
        elif token_type == TokenType.SHRINK:      self.shrink     += 10
        elif token_type == TokenType.SUBDIVIDE:   self.subdivide  +=1
        elif token_type == TokenType.DIVIDE:      self.divide      = True
        elif token_type == TokenType.SHOOT:       self.ammo       += 50 
        elif token_type == TokenType.MISSILE:     self.missile    +=2
        elif token_type == TokenType.EXPLOSIVE:   self.explosive  +=1
        elif token_type == TokenType.ONEUP:       self.oneup       = True
        elif token_type == TokenType.ONEDOWN:     self.onedown     = True
        elif token_type == TokenType.ULTRABALL:   self.ultraball  +=1
        elif token_type == TokenType.STICKBALL:   self.stickyball +=1 
        elif token_type == TokenType.SHIELD:      self.shield     +=1
        elif token_type == TokenType.COMMODIN:    self.commodin   +=1
        elif token_type == TokenType.DELTASWAP:   self.inverted    = True
        elif token_type == TokenType.HALFSCORE:   self.halfscore   = True
        elif token_type == TokenType.DOUBLESCORE: self.doublescore = True
        self.update()
    
    def update(self):        
        if self.divide:
            Inventory.game.applyEffect(TokenType.DIVIDE)
            self.divide=False
        if self.inverted:
            Inventory.game.applyEffect(TokenType.DELTASWAP)
            self.inverted = False
        if self.oneup:
            Inventory.game.applyEffect(TokenType.ONEUP)
            self.oneup = False
        if self.onedown:
            Inventory.game.applyEffect(TokenType.ONEDOWN)
            self.onedown = False
        if self.doublescore:
            Inventory.game.applyEffect(TokenType.DOUBLESCORE)
            self.doublescore = False
        if self.halfscore:
            Inventory.game.applyEffect(TokenType.HALFSCORE)
            self.halfscore = False