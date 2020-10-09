var con = {
    name: 'console',

    'print': {
        usage: 'text',
        brief: 'Dump given text over console',
        example: '',
    },
    'cls': {
        usage: '',
        brief: 'Clean console buffer',
        example: '',
    },
};

var blackbox = {
    name: 'blackbox',

    "ctrlc": {
        usage: '',
        brief: 'Returns TRUE if CTRL+C was pressed',
        example: '',
    },
    "version": {
        usage: '',
        brief: 'Return current BlackBox engine version',
        example: '',
    },
    "epoch": {
        usage: '',
        brief: 'Return current engine epoch uptime',
        example: '',
    },
};

var vpu = {
    name: 'vpu',

    'createanim': {
        usage: 'width, height, sprite_handle',
        brief: 'Return Handle to new Animation object',
        example: '',

    },
    'createsprite': {
        usage: 'width, height, filename',
        brief: 'Returns Handle to Sprite object create upon given filename',
        example: '',

    },
    'createsurf': {
        usage: 'width, height',
        brief: 'Returns Handle to Surface object to be drawn arbitrarily to screen',
        example: '',

    },
    'dimensions': {
        usage: '',
        brief: 'Returns selected bitmap [ width, height ] ',
        example: '',

    },
    'disable': {
        usage: 'layer',
        brief: 'Toggle off given vpu layer',
        example: '',

    },
    'deleteanim': {
        usage: 'handle',
        brief: 'Delete Animation identified by given Handle"',
        example: '',

    },
    'deletesprite': {
        usage: 'handle',
        brief: 'Delete Sprite identified by given Handle"',
        example: '',

    },
    'deletesurf': {
        usage: 'handle',
        brief: 'Delete Surface identified by given Handle"',
        example: '',

    },
    'drawanim': {
        usage: 'handle, x, y',
        brief: 'Draw animation identified by given handle onto given coordinates"',
        example: '',

    },
    'drawsprite': {
        usage: 'handle, x, y',
        brief: 'Draw Sprite identified by given handle onto given coordinates"',
        example: '',

    },
    'drawsurf': {
        usage: 'handle, x, y',
        brief: 'Draw surface identified by given handle onto given coordinates"',
        example: '',

    },
    'enable': {
        usage: 'layer',
        brief: 'Toggle on  given vpu layer',
        example: '',

    },
    'fadein': {
        usage: '',
        brief: 'Fade screen from black',
        example: '',

    },
    'fadeout': {
        usage: '',
        brief: 'Fade screen to black',
        example: '',

    },
    'fill': {
        usage: 'r, g, b, a',
        brief: 'Fill selected bitmap with given color or default color',
        example: '',

    },
    'frames': {
        usage: '',
        brief: 'Return actual frame count',
        example: '',

    },
    'fullscreen': {
        usage: 'enabled',
        brief: 'Toggle fullscreen mode',
        example: '',

    },
    'print': {
        usage: 'text, x, y',
        brief: 'Print given text at given coordinates',
        example: '',

    },
    'pset': {
        usage: 'x, y',
        brief: 'Draw a pixel onto selected surface onto given coordinates"',
        example: '',

    },
    'restart': {
        usage: '',
        brief: 'Restart Video Processing Unit',
        example: '',

    },
    'rotate': {
        usage: 'layer, angle',
        brief: 'Rotate specified layer (0-11) given degrees',
        example: '',

    },
    'scale': {
        usage: 'layer, scale_x, scale_y',
        brief: 'Change specified layer [0-11] given horizontal and vertical scale factor',
        example: '',

    },
    'select': {
        usage: 'layer',
        brief: 'Select given layer to perform next graphic operations onto it',
        example: '',

    },
    'setcolor': {
        usage: 'r, g, b, a',
        brief: 'Sets current painting color',
        example: '',

    },
    'setrotation': {
        usage: 'layer, angle',
        brief: 'Sets rotation for specified layer (0-11) at given degrees',
        example: '',

    },
    'setscale': {
        usage: 'layer, scale_x, scale_y',
        brief: 'Sets scale for specified layer [0-11] given horizontal and vertical values',
        example: '',

    },
    'update': {
        usage: '',
        brief: 'Allow blackbox engine to perform its rendering based input and output operations',
        example: '',
    },
};