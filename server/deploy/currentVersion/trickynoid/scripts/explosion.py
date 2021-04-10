from animation import Animation

class Explosion:
    
    gfx     = None    
    game    = None

    def initialize(game):
        Explosion.game = game
        Explosion.gfx = {}
        """
        try {
            // Load explosion graphics
            SpriteSheet explosionSheet = new SpriteSheet("data/gfx/explosion.png", 48, 24, new Color(255,0,255));
            gfx = new Animation(explosionSheet, 0, 0, 7, 0, true, 25, false);
            gfx.setLooping(false);
        } catch (SlickException ex) {
            gfx = null;
            Logger.getLogger(Explosion.class.getName()).log(Level.SEVERE, null, ex);
        }
        """
