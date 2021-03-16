from blackbox import createentity, addcontroller, updateentity, drawentity, entitytarget

class Color:
    def __init__(self, r=0,g=0,b=0,a=0):
        self.r = r
        self.g = g
        self.b = b
        self.a = a

    def setval(self, color):
        self.r = color.r
        self.g = color.g
        self.b = color.b
        self.a = color.a

    def setrgba(self, r,g,b,a=255):
        self.r = r
        self.g = g
        self.b = b
        self.a = a


class Property:
    def __init__(self, name="UnnamedProperty", value=None):
        self.value = value
        self.name  = name

class IntegerProperty(Property):
    def __init__(self, name, value=0, default=0, min=0, max=0xFFFFFFFF, wrap=False, stepness=1):
        Property.__init__(self, value, name)
        self.default  = default
        self.min      = min
        self.max      = max
        self.wrap     = wrap 
        self.stepness = stepness

    def setval(self, value):
        if value > self.max: self.value = self.max
        elif value < self.min: self.value = self.max
        else: self.value = value

    def increase(self):
        fx = self.value + self.stepness
        if self.wrap:            
            if fx >= self.max: self.value = self.min
            else: self.value = fx
        elif fx < self.max: self.value = fx
        else: self.value = self.max
    
    def decrease(self):
        fx = self.value - self.stepness
        if self.wrap:            
            if fx <= self.min: self.value = self.max
            else: self.value = fx
        elif fx > self.min: self.value = fx
        else: self.value = self.min

class FloatProperty(Property):
    def __init__(self, name, value=0.0, default=0.0, min=0.0, max=float(0xFFFFFFFF), wrap=False, stepness=0.1):
        Property.__init__(self, value, name)
        self.default  = default
        self.min      = min
        self.max      = max
        self.wrap     = wrap 
        self.stepness = stepness

    def setval(self, value):
        if value > self.max: self.value = self.max
        elif value < self.min: self.value = self.max
        else: self.value = value

    def increase(self):
        fx = self.value + self.stepness
        if self.wrap:            
            if fx >= self.max: self.value = self.min
            else: self.value = fx
        elif fx < self.max: self.value = fx
        else: self.value = self.max
    
    def decrease(self):
        fx = self.value - self.stepness
        if self.wrap:            
            if fx <= self.min: self.value = self.max
            else: self.value = fx
        elif fx > self.min: self.value = fx
        else: self.value = self.min

class StringProperty(Property):
    def __init__(self, name, value="", default="", max_length=256):
        Property.__init__(self, value, name)
        self.default    = default
        self.max_length = max_length
        
    def setval(self, value):
        if (self.max_length>0) and (len(value) > self.max_length):
            value = value[0:self.max_length]
        self.value = value

    def increase(self):
        pass
    
    def decrease(self):
        pass

class ColorProperty(Property):
    def __init__(self, name, value=None, default=None):
        Property.__init__(self, value, name)
        self.value      = Color(0,0,0,0) if value is None else value
        self.default    = self.value if default is None else default
        
    def setrgba(self, r,g,b,a):
        self.value.setval(
            r & 255, 
            g & 255, 
            b & 255,
            a & 255
        )

    def setval(self, value):
        self.value = value

    def increasealpha(self):
        self.value.a = self.value.a + 1 if self.value.a < 255 else 255

    def decreasealpha(self):
        self.value.a = self.value.a - 1 if self.value.a > 0 else 0

    def increase(self):
        self.value.r = self.value.r + 1 if self.value.r < 255 else 255
        self.value.g = self.value.g + 1 if self.value.g < 255 else 255
        self.value.b = self.value.b + 1 if self.value.b < 255 else 255
    
    def decrease(self):
        self.value.r = self.value.r - 1 if self.value.r > 0 else 0
        self.value.g = self.value.g - 1 if self.value.g > 0 else 0
        self.value.b = self.value.b - 1 if self.value.b > 0 else 0

class Entity:
    @staticmethod
    def create(game, x=16, y=16, name="UnnamedEntity"):
        e = Entity(game, x, y, name)
        return e
    
    def __init__(self, game, width=16, height=16, name="UnnamedEntity"):
        self.game        = game
        self.name        = name
        self.x           = self.game.width  / 2 
        self.y           = self.game.height / 2 
        self.width       = width
        self.height      = height
        self.handle      = createentity(width, height, name)
        self.controllers = {}
        self.properties  = {}
        print("Entity initialized")

    def draw(self):
        import vpu
        vpu.select(1)
        vpu.setcolor(255,255,0,16)
        drawentity(self.handle)        
        
    def update(self, delta):
        updateentity(self.handle, delta)
        
    def addcontroller(self, type_index):
        self.controllers[ type_index ] = EntityController(self, type_index)
    
    def addproperty(name, **kwargs):
        # must check type and call internal _add<Type>Property
        if   is_instance(kwargs['value'], int  ): self.__addIntegerProperty(name, **kwargs)
        elif is_instance(kwargs['value'], float): self.__addFloatProperty(  name, **kwargs)
        elif is_instance(kwargs['value'], str  ): self.__addStringProperty( name, **kwargs)
        elif is_instance(kwargs['value'], Color): self.__addColorProperty(  name, **kwargs)
    
    def __addIntegerProperty(self, name='UnnamedInteger',value=0, default=0, min=int(-0xFFFFFFFF), max=int(0xFFFFFFFF), wrap=False, stepness=1):
        self.properties.append(IntegerProperty(name, value, default, min, max, wrap, stepness))

    def __addFloatProperty(self, name='UnnamedFloat', value=0.0, default=0.0, min=float(-0xFFFFFFFF), max=float(0xFFFFFFFF), wrap=False, stepness=0.1):
        self.properties.append(FloatProperty(name, value, default, min, max, wrap, stepness))

    def __addStringProperty(self, name='UnnamedString', value="", default="", max_length=256):
        self.properties.append(StringProperty(name, value, default, max_length))

    def __addColorProperty(self, name='UnnamedColor', value=0, default=0):
        self.properties.append(ColorProperty(name, value, default))

class EntityController:
    CONTROLLER_INPUT    = 0x00
    CONTROLLER_FOLLOW   = 0x01
    CONTROLLER_MOVE     = 0x02
    CONTROLLER_SHOOT    = 0x03
    CONTROLLER_AVOID    = 0x04
    CONTROLLER_MAX      = 0x05
    
    def set_target(self, target):
        entitytarget(self.parent.handle, target.handle, self.controller_type)
            
    def __init__(self, entity, controller_type):
        self.controller_type = controller_type
        self.parent = entity
        self.controller_type = controller_type
        self.handle = addcontroller(entity.handle, controller_type)
        
