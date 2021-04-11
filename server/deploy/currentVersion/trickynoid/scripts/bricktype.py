#isExplosive   ─────> True if brick blows up when hit
#isSticked     ─────> True if brick is inmune to gravity
#isInverter    ─────> True if causes delta switching
#causesBounce  ─────> Causes the ball to bounce when hit
#blowable      ─────> Can be destroyed by an explosion splash
#shootable     ─────> Can be destroyed by a shooted projectile
#shootAngle    ─────> 0  - none, 1-Up, 2-Right, 3-Bottom, 4-Left
#shootsMissile ─────> true if this brick shoots a missile when hit
#noise         ─────> Noise index casted when hit
#score         ─────> Score earned when this brick is destroyed
#graphic       ─────> Graphic used by this brick
#health        ─────> Brick health (0 or less causes a call to destroy() )
#destructable  


class BrickType:
    
    game = None

    NONE            = 0x00
    GOO             = 0x01
    CLAY            = 0x02
    CLAY_STICKED    = 0x03
    STONE           = 0x04
    STONE_STICKED   = 0x05
    IRON            = 0x06
    IRON_STICKED    = 0x07
    SUP             = 0x08
    SRIGHT          = 0x09
    SDOWN           = 0x0A
    SLEFT           = 0x0B
    SUP_STICKED     = 0x0C
    SRIGHT_STICKED  = 0x0D
    SDOWN_STICKED   = 0x0E
    SLEFT_STICKED   = 0x0F
    MUP             = 0x10
    MRIGHT          = 0x11
    MDOWN           = 0x12
    MLEFT           = 0x13
    MUP_STICKED     = 0x14
    MRIGHT_STICKED  = 0x15
    MDOWN_STICKED   = 0x16
    MLEFT_STICKED   = 0x17
    BOOM            = 0x18
    BOOM_STICKED    = 0x19
    INVERTER        = 0x1A
    DIAMOND         = 0x1B
    DIAMOND_STICKED =0x1C
    CHANNEL_H       = 0x1D
    CHANNEL_V       = 0x1E
    CHANNEL_HV      = 0x1F
    INVERTER_A      = 0x20
    INVERTER_B      = 0x21
    INVERTER_C      = 0x22
    INVERTER_D      = 0x23

    def __init__(self, index, bounce=True, explosive=False, invert=False, stick=False, blowable=False, shootable=False, shootangle=False, missile=False, noise=False, score=False, graphic=False, health=False, destructable=False):
       self.index         = index
       self.isExplosive   = explosive
       self.isInverter    = invert
       self.isSticked     = stick
       self.causesBounce  = bounce
       self.blowable      = blowable
       self.shootable     = shootable
       self.shootAngle    = shootangle
       self.shootsMissile = missile
       self.noise         = noise
       self.score         = score
       self.graphic       = graphic
       self.health        = health
       self.destructable  = destructable

    @staticmethod
    def initialize(game):
        from data.scripts.brick import Brick
        BrickType.game = game
        Brick.types = {
            BrickType.NONE            : BrickType(BrickType.NONE            ),
            BrickType.GOO             : BrickType(BrickType.GOO             , bounce=False, stick=True),
            BrickType.CLAY            : BrickType(BrickType.CLAY            , bounce=True , stick=False, health=1),
            BrickType.CLAY_STICKED    : BrickType(BrickType.CLAY_STICKED    , bounce=True , stick=True , health=2),
            BrickType.STONE           : BrickType(BrickType.STONE           , bounce=True , stick=False, health=3),
            BrickType.STONE_STICKED   : BrickType(BrickType.STONE_STICKED   , bounce=True , stick=True , health=4),
            BrickType.IRON            : BrickType(BrickType.IRON            , bounce=True , stick=False, destructable=False),
            BrickType.IRON_STICKED    : BrickType(BrickType.IRON_STICKED    , bounce=True , stick=True , destructable=False),
            BrickType.SUP             : BrickType(BrickType.SUP             , bounce=True , stick=False, destructable=False, shootangle=1),
            BrickType.SRIGHT          : BrickType(BrickType.SRIGHT          , bounce=True , stick=False, destructable=False, shootangle=2),
            BrickType.SDOWN           : BrickType(BrickType.SDOWN           , bounce=True , stick=False, destructable=False, shootangle=3),
            BrickType.SLEFT           : BrickType(BrickType.SLEFT           , bounce=True , stick=False, destructable=False, shootangle=4),
            BrickType.SUP_STICKED     : BrickType(BrickType.SUP_STICKED     , bounce=True , stick=True , destructable=False, shootangle=1),
            BrickType.SRIGHT_STICKED  : BrickType(BrickType.SRIGHT_STICKED  , bounce=True , stick=True , destructable=False, shootangle=2),
            BrickType.SDOWN_STICKED   : BrickType(BrickType.SDOWN_STICKED   , bounce=True , stick=True , destructable=False, shootangle=3),
            BrickType.SLEFT_STICKED   : BrickType(BrickType.SLEFT_STICKED   , bounce=True , stick=True , destructable=False, shootangle=4),
            BrickType.MUP             : BrickType(BrickType.MUP             , bounce=True , stick=False, destructable=False, shootangle=1, missile=True),
            BrickType.MRIGHT          : BrickType(BrickType.MRIGHT          , bounce=True , stick=False, destructable=False, shootangle=2, missile=True),
            BrickType.MDOWN           : BrickType(BrickType.MDOWN           , bounce=True , stick=False, destructable=False, shootangle=3, missile=True),
            BrickType.MLEFT           : BrickType(BrickType.MLEFT           , bounce=True , stick=False, destructable=False, shootangle=4, missile=True),
            BrickType.MUP_STICKED     : BrickType(BrickType.MUP_STICKED     , bounce=True , stick=True , destructable=False, shootangle=1, missile=True),
            BrickType.MRIGHT_STICKED  : BrickType(BrickType.MRIGHT_STICKED  , bounce=True , stick=True , destructable=False, shootangle=2, missile=True),
            BrickType.MDOWN_STICKED   : BrickType(BrickType.MDOWN_STICKED   , bounce=True , stick=True , destructable=False, shootangle=3, missile=True),
            BrickType.MLEFT_STICKED   : BrickType(BrickType.MLEFT_STICKED   , bounce=True , stick=True , destructable=False, shootangle=4, missile=True),
            BrickType.BOOM            : BrickType(BrickType.BOOM            , bounce=True , stick=False, explosive=True),
            BrickType.BOOM_STICKED    : BrickType(BrickType.BOOM_STICKED    , bounce=True , stick=True , explosive=True),
            BrickType.INVERTER        : BrickType(BrickType.INVERTER        , bounce=True , stick=False, invert=True ),#, explosive=True
            BrickType.DIAMOND         : BrickType(BrickType.DIAMOND         , bounce=True , stick=False, destructable=False),
            BrickType.DIAMOND_STICKED : BrickType(BrickType.DIAMOND_STICKED , bounce=True , stick=True , destructable=False),
            BrickType.CHANNEL_H       : BrickType(BrickType.CHANNEL_H       , bounce=True , stick=True , destructable=False),
            BrickType.CHANNEL_V       : BrickType(BrickType.CHANNEL_V       , bounce=True , stick=True , destructable=False),
            BrickType.CHANNEL_HV      : BrickType(BrickType.CHANNEL_HV      , bounce=True , stick=True , destructable=False),
            BrickType.INVERTER_A      : BrickType(BrickType.INVERTER_A      , bounce=True , stick=True , invert=True),
            BrickType.INVERTER_B      : BrickType(BrickType.INVERTER_B      , bounce=True , stick=True , invert=True),
            BrickType.INVERTER_C      : BrickType(BrickType.INVERTER_C      , bounce=True , stick=True , invert=True),
            BrickType.INVERTER_D      : BrickType(BrickType.INVERTER_D      , bounce=True , stick=True , invert=True),
        }

"""
final boolean EXPLOSIVE= true;
final boolean STICKED  = true;
final boolean BOUNCE   = true;
final boolean BLOWABLE = true;
final boolean SHOOTABLE= true;
final byte SHOOT_NONE  = 0x00;
final byte SHOOT_UP    = 0x01;
final byte SHOOT_RIGHT = 0x02;
final byte SHOOT_DOWN  = 0x03;
final byte SHOOT_LEFT  = 0x04;
final boolean MISSILE  = true;
final byte DEFAULTNOISE= 0x01;
final boolean DESTRUCTABLE = true;
final int DEFAULTGRAPH = 1;
final int HEALTH_ONE   = 1;
final int HEALTH_TWO   = 2;
final int HEALTH_THREE = 3;

addType(BrickType.NONE,         false,  false,   STICKED, false , false,    false,     SHOOT_NONE, false,   (byte)0x00,       0, 0x00, 0, false);
addType(BrickType.GOO,          false,  false,   STICKED, false , BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   150, 0x01, HEALTH_ONE,    DESTRUCTABLE);
addType(BrickType.CLAY,         false,  false,   false,   BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   200, 0x11, HEALTH_ONE,    DESTRUCTABLE);
addType(BrickType.CLAY_STICKED, false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   200, 0x21, HEALTH_TWO,    DESTRUCTABLE);
addType(BrickType.STONE,        false,  false,   false,   BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   300, 0x31, HEALTH_THREE,  DESTRUCTABLE);
addType(BrickType.STONE_STICKED,false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   300, 0x41, HEALTH_THREE,  DESTRUCTABLE);
addType(BrickType.IRON,         false,  false,   false,   BOUNCE, BLOWABLE,     false, SHOOT_NONE, false,   DEFAULTNOISE,   300, 0x31, HEALTH_THREE,  DESTRUCTABLE);
addType(BrickType.IRON_STICKED, false,  false,   STICKED, BOUNCE, BLOWABLE,     false, SHOOT_NONE, false,   DEFAULTNOISE,   300, 0x41, HEALTH_THREE,  false);

addType(BrickType.SUP_STICKED,  false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_UP,   false,   DEFAULTNOISE,     5, 0x71, HEALTH_ONE,    false);
addType(BrickType.SRIGHT_STICKED,false, false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_RIGHT,false,   DEFAULTNOISE,     5, 0x72, HEALTH_ONE,    false);
addType(BrickType.SDOWN_STICKED,false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_DOWN, false,   DEFAULTNOISE,     5, 0x73, HEALTH_ONE,    false);
addType(BrickType.SLEFT_STICKED,false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_LEFT, false,   DEFAULTNOISE,     5, 0x74, HEALTH_ONE,    false);
addType(BrickType.SUP,          false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_UP,   false,   DEFAULTNOISE,     5, 0x75, HEALTH_ONE,    false);
addType(BrickType.SRIGHT,       false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_RIGHT,false,   DEFAULTNOISE,     5, 0x76, HEALTH_ONE,    false);
addType(BrickType.SDOWN,        false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_DOWN, false,   DEFAULTNOISE,     5, 0x77, HEALTH_ONE,    false);
addType(BrickType.SLEFT,        false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_LEFT, false,   DEFAULTNOISE,     5, 0x78, HEALTH_ONE,    false);
addType(BrickType.MUP_STICKED,  false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_UP,   MISSILE, DEFAULTNOISE,     5, 0x79, HEALTH_ONE,    false);
addType(BrickType.MRIGHT_STICKED,false, false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_RIGHT,MISSILE, DEFAULTNOISE,     5, 0x7A, HEALTH_ONE,    false);
addType(BrickType.MDOWN_STICKED,false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_DOWN, MISSILE, DEFAULTNOISE,     5, 0x7B, HEALTH_ONE,    false);
addType(BrickType.MLEFT_STICKED,false,  false,   STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_LEFT, MISSILE, DEFAULTNOISE,     5, 0x7C, HEALTH_ONE,    false);
addType(BrickType.MUP,          false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_UP,   MISSILE, DEFAULTNOISE,     5, 0x7D, HEALTH_ONE,    false);
addType(BrickType.MRIGHT,       false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_RIGHT,MISSILE, DEFAULTNOISE,     5, 0x7E, HEALTH_ONE,    false);
addType(BrickType.MDOWN,        false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_DOWN, MISSILE, DEFAULTNOISE,     5, 0x7F, HEALTH_ONE,    false);
addType(BrickType.MLEFT,        false,  false,     false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_LEFT, MISSILE, DEFAULTNOISE,     5, 0x80, HEALTH_ONE,    false);

addType(BrickType.BOOM,         EXPLOSIVE,false,   false, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   250, 0x20, HEALTH_ONE,    DESTRUCTABLE);
addType(BrickType.BOOM_STICKED, EXPLOSIVE,false, STICKED, BOUNCE, BLOWABLE, SHOOTABLE, SHOOT_NONE, false,   DEFAULTNOISE,   250, 0x30, HEALTH_ONE,    DESTRUCTABLE);
addType(BrickType.INVERTER,     false,    true, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,   350, 0x40, HEALTH_ONE,    false);

addType(BrickType.DIAMOND,      false,    false,   false, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,    15, 0x51, HEALTH_ONE,    false);
addType(BrickType.DIAMOND_STICKED, false, false, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,    15, 0x61, HEALTH_ONE,    false);

addType(BrickType.CHANNEL_H,    false,   false,    false, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,    15, 0x50, HEALTH_ONE,    false);
addType(BrickType.CHANNEL_V,    false,   false,    false, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,    15, 0x60, HEALTH_ONE,    false);
addType(BrickType.CHANNEL_HV,   false,   false,    false, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,    15, 0x70, HEALTH_ONE,    false);

addType(BrickType.INVERTER_A,   false,    true, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,   350, 0x9C, HEALTH_ONE,    false);
addType(BrickType.INVERTER_B,   false,    true, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,   350, 0x9D, HEALTH_ONE,    false);
addType(BrickType.INVERTER_C,   false,    true, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,   350, 0x9E, HEALTH_ONE,    false);
addType(BrickType.INVERTER_D,   false,    true, STICKED, BOUNCE,    false,     false, SHOOT_NONE, false,   DEFAULTNOISE,   350, 0x9F, HEALTH_ONE,    false);
"""    
