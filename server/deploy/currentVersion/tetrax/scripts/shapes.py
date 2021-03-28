data_L = [ 
    [ 
        1, 0, 0, 
        1, 0, 0, 
        1, 1, 0, 
    ],[ 
        1, 1, 1, 
        1, 0, 0, 
        0, 0, 0, 
    ],[ 
        0, 1, 1, 
        0, 0, 1, 
        0, 0, 1, 
    ],[ 
        0, 0, 0, 
        0, 0, 1, 
        1, 1, 1, 
    ],
]

data_LF = [ 
    [ 
        1, 1, 0, 
        1, 0, 0, 
        1, 0, 0, 
    ],[ 
        1, 1, 1, 
        0, 0, 1, 
        0, 0, 0, 
    ],[ 
        0, 0, 1, 
        0, 0, 1, 
        0, 1, 1, 
    ],[ 
        0, 0, 0, 
        1, 0, 0, 
        1, 1, 1, 
    ],
]

data_Z = [ 
    [ 
        1, 1, 0, 
        0, 1, 1, 
        0, 0, 0  
    ],[ 
        0, 0, 1, 
        0, 1, 1, 
        0, 1, 0, 
    ],[ 
        0, 0, 0, 
        1, 1, 0, 
        0, 1, 1, 
    ],[ 
        0, 1, 0, 
        1, 1, 0, 
        1, 0, 0, 
    ],
]

data_ZF = [ 
    [ 
        0, 1, 1, 
        1, 1, 0, 
        0, 0, 0, 
    ],[ 
        0, 1, 0, 
        0, 1, 1, 
        0, 0, 1, 
    ],[ 
        0, 0, 0, 
        0, 1, 1, 
        1, 1, 0, 
    ],[ 
        1, 0, 0, 
        1, 1, 0, 
        0, 1, 0, 
    ],
]

data_I = [ 
    [ 
        1, 1, 1, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
    ],[ 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
    ],[ 
        1, 1, 1, 1, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
        0, 0, 0, 0, 
    ],[ 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
        1, 0, 0, 0, 
    ],
]

data_B = [ 
    [ 
        1, 1, 
        1, 1
    ],[ 
        1, 1, 
        1, 1
    ],[ 
        1, 1, 
        1, 1
    ],[ 
        1, 1, 
        1, 1
    ],
]

data_T = [ 
    [ 
        1, 1, 1, 
        0, 1, 0, 
        0, 0, 0, 
    ],[ 
        0, 0, 1, 
        0, 1, 1, 
        0, 0, 1, 
    ],[ 
        0, 0, 0, 
        0, 1, 0, 
        1, 1, 1, 
    ],[ 
        1, 0, 0, 
        1, 1, 0, 
        1, 0, 0, 
    ],
]

class ShapeType:
    L   = 0x00
    LF  = 0x01
    Z   = 0x02
    ZF  = 0x03
    I   = 0x04
    B   = 0x05
    T   = 0x06

    def __init__(self, shape_type, type_name, allow_rotation, center_point, width, height, data={}, index=0):
        self.type_name      = type_name
        self.allow_rotation = allow_rotation
        self.center_point   = center_point
        self.width          = width
        self.height         = height
        self.data           = data 
        self.index          = index
        
class ShapeTypes:
    L   = ShapeType(ShapeType.L , 'L'           , True, [1,1], 3, 3, data_L , 0)
    LF  = ShapeType(ShapeType.LF, 'L Flipped'   , True, [1,1], 3, 3, data_LF, 1)
    Z   = ShapeType(ShapeType.Z , 'Z'           , True, [1,1], 3, 3, data_Z , 2)
    ZF  = ShapeType(ShapeType.ZF, 'Z Flipped'   , True, [1,1], 3, 3, data_ZF, 3)
    I   = ShapeType(ShapeType.I , 'I'           , True, [1,1], 4, 4, data_I , 4)
    B   = ShapeType(ShapeType.B , '2x2 Cube'    , False,[0,1], 2, 2, data_B , 5)
    T   = ShapeType(ShapeType.T , 'T'           , True, [1,1], 3, 3, data_T , 6)    
    
    @staticmethod
    def get(index):
        if index==ShapeType.L   : return ShapeTypes.L
        elif index==ShapeType.LF: return ShapeTypes.LF
        elif index==ShapeType.Z : return ShapeTypes.Z
        elif index==ShapeType.ZF: return ShapeTypes.ZF
        elif index==ShapeType.I : return ShapeTypes.I
        elif index==ShapeType.B : return ShapeTypes.B
        elif index==ShapeType.T : return ShapeTypes.T
        else: return ShapeTypes.B