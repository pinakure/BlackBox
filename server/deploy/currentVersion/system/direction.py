class DirectionNames:
    UP    = 'upwards'
    DOWN  = 'downwards'
    LEFT  = 'leftwards'
    RIGHT = 'rightwards'

class OppositeDirection:
    UP    = 0x02
    RIGHT = 0x03
    DOWN  = 0x00
    LEFT  = 0x01

class Direction:
    UP    = 0x00
    RIGHT = 0x01
    DOWN  = 0x02
    LEFT  = 0x03

    @staticmethod
    def string(t):
        if t  ==  Direction.UP    : return DirectionNames.UP
        elif t == Direction.DOWN  : return DirectionNames.DOWN
        elif t == Direction.LEFT  : return DirectionNames.LEFT
        elif t == Direction.RIGHT : return DirectionNames.RIGHT
    
    @staticmethod
    def opposite(t):
        if t  ==  Direction.UP    : return OppositeDirection.UP
        elif t == Direction.DOWN  : return OppositeDirection.DOWN
        elif t == Direction.LEFT  : return OppositeDirection.LEFT
        elif t == Direction.RIGHT : return OppositeDirection.RIGHT
