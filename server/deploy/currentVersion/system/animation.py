from vpu import createanim, updateanim, drawanim, setframe as SetFrame

class AnimationLoop:
    NONE     = 0x00
    FORWARD  = 0x01
    BACKWARD = 0x02
    PINGPONG = 0x03

class AnimationAutoUpdate:
    YES = True
    NO  = False

class Animation:
    def __init__(self, frame_width, frame_height, sprite_handle, initial_frame_x=0, initial_frame_y=0, end_frame_x=0, end_frame_y=0, scan_vertically=False, speed=1.0, autoupdate=AnimationAutoUpdate.YES, loop_type=AnimationLoop.FORWARD):
        self.width       = frame_width
        self.height      = frame_height
        self.sprite      = sprite_handle
        self._autoupdate = autoupdate
        self._loop       = loop_type
        self._speed      = speed
        self.handle      = createanim(
            frame_width     , frame_height, 
            sprite_handle, 
            initial_frame_x , initial_frame_y,
            end_frame_x     , end_frame_y,
            scan_vertically,
            speed,
            autoupdate
        )
        #vpu.setanimloop(self.handle, loop_type)

    def destroy(self):
        deleteanim(self.handle)
        self.handle = None

    def update(self, delta):
        if not self._autoupdate:
            updateanim(self.handle)

    def draw(self, x, y, rotation=0.0):
        drawanim(self.handle, x, y, rotation)

    def speed(self, speed=None):
        if speed is None: return self._speed
        self._speed = speed
        setanimspeed(self.handle, speed)

    def autoupdate(self, autoupdate=None):
        if autoupdate is None: return self._autoupdate
        self._autoupdate = autoupdate
        setanimautoupdate(self,handle, autoupdate)

    def loop(self, loop_type=None):
        if loop_type is None: return self._loop
        self._loop = loop_type
        setanimlooptype(self,handle, loop_type)

    def setframe(self, frame):
        SetFrame(self.handle, frame)
