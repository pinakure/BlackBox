# 
# Holds reaction data from last ball which hit a brick
# 

class  HitReaction: 
    # invertDeltas;              //If true, ball deltas swap   
    # causeExplosion;            //If true, hit brick explodes  
    # causeFall;                 //If true, hit brick begins to fall
    # causeSelectiveDestruction; //If true, bricks with same id as hit brick dissappear
    
    def __init__(self):
        self.causeBounce    = False
        self.invertDeltas   = False
        self.causeExplosion = False
        self.causeFall      = False
        self.target         = None
        self.causeSelectiveDestruction = False
