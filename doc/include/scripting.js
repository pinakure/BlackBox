var con = {
    name: 'console',

    'echo': {
        usage: 'text',
        brief: 'Dump given text over console',
        example: `<b>console.echo('Hello world!')</b>;\n\nplayer = 'Neo'\n<b>console.echo(f'Welcome home, { player }, nice to see you around.')</b>\n<i>#Draws the same as before, but text and name colored separately:</i>\n<b>console.echo(f'~7Welcome home, ~4{ player }~7, nice to see you around.')</b>`,
    },
    'cls': {
        usage: '',
        brief: 'Clean console buffer',
        example: '<b>console.cls()</b>',
    },
};

var typewriter = {
    name: 'typewriter',

    
    'setfont': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'setcolor': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'setsize': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'setposition': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'answer': {
        usage: '',
        brief: `Use this to inspect which choice was selected after calling choice().</br>Once read, this value will set to ''.`,
        return: `Key corresponding to value selected or '' if no choice was made yet`,
        example: `typewriter.question('Which color?', ['red', 'green', 'blue'], ['Red Wine', 'Green cabbage', 'Blue sky'])\na = ''\nwhile(a==''):\n\t<b>a = typewriter.answer()</b>\n\tif(a == 'red'): console.echo('Selected Red')\n\tvpu.update()`,
    },
    'choice': {
        usage: 'question, keys[], answers[]',
        brief: 'Enqueue a question message along its answers into typewriter buffer.',
        return: 'Key corresponding to answer chosen. This allows multiple choices to be stored and wait for any answer in no particular order.',
        example: `<b>typewriter.question('Which color?', ['red', 'green', 'blue'], ['Red Wine', 'Green cabbage', 'Blue sky'])</b>`,
    },
    'type': {
        usage: 'text',
        brief: 'Enqueue message into typewriter buffer and open if if closed',
        example: `<b>typewriter.type('Hello world')</b>\n<b>typewriter.type('How are you?')</b>`,
    },
    'loadpic': {
        usage: 'picfilename, x=0, y=0, w=0, h=0',
        brief: 'Load picture from file to typewriter overlay',
        example: `<b>typewriter.loadpic('bottle')</b>`,
    },
    'clearpic': {
        usage: '',
        brief: 'Remove picture from typewriter overlay',
        example: `<b>typewriter.clearpic()</b>`,
    },
};

var blackbox = {
    name: 'blackbox',

    "ctrlc": {
        usage: '',
        brief: 'Useful to debug the code. Once it returns True, it will lock until CTRL keyup is detected.',
        return: 'True if CTRL+C was pressed',
        example: '<i>#Will increase by one until CTRL+C is pressed</i>\ni=0\n<b>while(not blackbox.ctrlc()):</b>\n\ti=i+1\n\tconsole.echo(str(i))\n\tvpu.update()',
    },
    "version": {
        usage: '',
        brief: 'Useful to check BlackBox version in runtime',
        return: 'Version number',
        example: '<b>console.echo(blackbox.version())</b>',
    },
    "epoch": {
        usage: '',
        brief: 'Useful to check time pass. Must call update for the value it returns to change.',
        return: 'current execution time, in seconds',
        example: '<b>before = blackbox.epoch()</b>\n<b>while(before > blackbox.epoch() - 5):</b> <i>#Wait 5 seconds</i>\n\tvpu.update()',
    },
};

var vpu = {
    name: 'vpu',

    'loadfont': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'setfont': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'deletefont': {
        usage: '',
        brief: ``,
        return: ``,
        example: ``,
    },
    'createanim': {
        usage: 'width, height, sprite_handle',
        brief: 'Create Animation object and get Handle. Requires handle to initialized Sprite ',
        return: 'Handle to Initialized Animation or Negative on error',
        example: `a = []\nspr = vpu.createsprite('bottle') <i>#Returns sprite handle</i>\n<b>a[0] = vpu.createanim(spr, 32, 32, 0, 0, 0, 7, False)</b> <i>#Create animation, each frame 32x32, selecting from 0,0 to 0,7 sweeping horizontally\n</i><b>a[2] = vpu.createanim(spr,  8, 16, 0, 0, 3, 9,  True)</b><i> #Create animation, each frame  8x16, selecting from 0,0 to 3,9 sweeping vertically</i>`,
    },
    'createsprite': {
        usage: 'filename',
        brief: `Create Sprite object taking data from 'data/sprites/&lt;filename&gt;.png'`,
        return: 'Handle to Initialized Sprite or Negative on error',
        example: `<b>spr = vpu.createsprite('bottle') <i>#Load sprite 'data/sprites/bottle.png'</i></b>`,
    },
    'createsurf': {
        usage: 'width, height',
        brief: 'Returns Handle to Surface object to be drawn arbitrarily to screen',
        return: 'Handle to Initialized Surface or Negative on error',
        example: `<b>srf = vpu.createsurf(32, 32) <i>#Create 32x32 video surface</i></b>`,
    },
    'dimensions': {
        usage: '',
        brief: 'Get currently selected target surface geometry values',
        return: 'array(2) containing selected surface Width and Height',
        example: 'vpu.select(0)\n<b>width, height = vpu.dimensions()</b> <i>#Returns [width, height] object</i>',
    },
    'disable': {
        usage: 'layer',
        brief: 'Toggle off given vpu layer',
        example: '<b>vpu.disable(0)</b> <i>#disable layer 0 (background)</i>',
    },
    'deleteanim': {
        usage: 'handle',
        brief: 'Delete Animation identified by given Handle',
        example: `spr = vpu.createsprite('bottle') <i>#Returns sprite handle</i>\nani = vpu.createanim(spr, 0, 0, 1, 1, False)\n<b>vpu.deleteanim(spr) <i>#Accepts sprite handle</i></b>`,
    },
    'deletesprite': {
        usage: 'handle',
        brief: 'Delete Sprite identified by given Handle',
        example: `spr = vpu.createsprite('bottle')\n<b>vpu.deletesprite(spr)</b>`,

    },
    'deletesurf': {
        usage: 'handle',
        brief: 'Delete Surface identified by given Handle',
        example: `srf = vpu.createsurf(32, 32)\n<b>vpu.deletesurf(srf)</b>`,

    },
    'drawanim': {
        usage: 'handle, x, y',
        brief: 'Draw animation identified by given handle onto given coordinates',
        example: `spr = vpu.createsprite('bottle')\nani = vpu.createanim(spr, 32, 32, 0, 0, 0, 7, False)\nvpu.select(0)\n<b>vpu.drawanim(ani, 32, 32)</b>`,
    },
    'drawsprite': {
        usage: 'handle, x, y',
        brief: 'Draw Sprite identified by given handle onto given coordinates',
        example: '',
        example: `spr = vpu.createsprite('bottle')\nvpu.select(0)\n<b>vpu.drawsprite(spr, 32, 32)</b>`,
    },
    'drawsurf': {
        usage: 'handle, x, y',
        brief: 'Draw surface identified by given handle onto given coordinates',
        example: `srf = vpu.createsurf(32, 32)\nvpu.select(0)\n<b>vpu.drawsurf(srf, 32, 32)</b>`,
    },
    'enable': {
        usage: 'layer',
        brief: 'Toggle on specified surface handle. Handles 0-2 belong to BG, FG and Overlay respectively.',
        example: '<b>vpu.enable(1) <i>#Foreground layer ON</i></b>',
    },
    'fadein': {
        usage: 'r=-1, g=-1, b=-1',
        brief: 'Fade screen from given color to fully transparent. If no color is specified, last color will be used.<br/>Engine will not wait until fade ends, it is required to wait 2 seconds before changing screen contents if intended use is to perform transitions.',
        example: '<b>vpu.fadein(0, 0, 0) <i>#Prepare fade settings</i></b>\nwhile(vpu.fading()):\n\tvpu.update() <i>#Update fade and paint screen</i>',
    },
    'fadeout': {
        usage: 'r=-1, g=-1, b=-1',
        brief: 'Fade screen from fully transparent to given color. If no color is specified, last color will be used.<br/>Engine will not wait until fade ends, it is required to wait 2 seconds before changing screen contents if intended use is to perform transitions.',
        example: '<b>vpu.fadeout(0, 0, 0) <i>#Prepare fade settings</i></b>\nwhile(vpu.fading()):\n\tvpu.update() <i>#Update fade and paint screen</i>',
    },
    'fading': {
        usage: '',
        brief: 'Return <blue>True</blue> if fadein or fadeout is pending completion.',
        return: 'True if fade is ocurring, False otherwise.',        
        example: '<b>console.echo(vpu.fading()) <i>#Will output: False</i></b>\nvpu.fadein()\n<b>console.echo(vpu.fading()) <i>#Will output: True</i></b>\n',
    },
    'fill': {
        usage: 'r=-1, g=-1, b=-1, a=-1',
        brief: 'Fill selected bitmap with given color if specified or default color otherwise.<br/>Specifying does not change selected color.',
        example: 'vpu.select(0)\nvpu.setcolor(64,0,0)\n<b>vpu.fill() <i>#Fill with dark red</i></b>\n<b>vpu.fill(255,255,0) <i>#Fill with bright yellow</i></b>\n<b>vpu.fill(-1,-1,-1,128)</b> <i>#Fill with dark red but half the alpha</i>',
    },
    'fillrect': {
        usage: 'x,y,dx,dy',
        brief: 'Draw a filled rectangle onto selected surface from x,y to dx,dy using selected color as fill color.',
        example: 'vpu.select(0)\nvpu.setcolor(0,255,0)\nw, h = vpu.dimensions()\n<b>vpu.fillrect(0,0,w,h) <i>#Fill layer fully</i></b> ',
    },
    'frames': {
        usage: '',
        brief: 'Return actual frame count',
        return : `Actual frame count.<br/>It will not change if the script does not call vpu.update() periodically.`,
        example: `<b>frame = vpu.frames()</b>\n<b>while(frame+120 < vpu.frames()):</b>\n\t<i>#Do stuff...</i>\n\tvpu.update() <i>#Always call update at least once or loop will become infinite</i>`,
    },
    'fullscreen': {
        usage: 'enabled',
        brief: 'Toggle fullscreen mode. Useless if vpu.restart() is not called somewhere.',
        example: '<b>vpu.fullscreen(1)</b>\nvpu.restart() <i>#Call this to make settings efective</i>',
    },    
    'line': {
        usage: 'x, y, dx, dy',
        brief: 'Draw a line onto selected surface from x, y to dx, dy',
        example: 'vpu.select(0)\nvpu.setcolor(0,255,0)\n<b>vpu.line(0, 0, 100, 100)</b>\nvpu.update()',
    },
    'textout': {
        usage: 'text, x, y',
        brief: 'Draw given <b>text</b> at given <b>x, y</b> coordinates',
        example: 'vpu.select(0)\n<b>vpu.textout("hello", 16, 16)</b>\nvpu.update()',
    },
    'pset': {
        usage: 'x, y',
        brief: 'Draw a pixel onto selected surface onto given coordinates',
        example: '<i>#Draws a diagonal line 16 pixel long</i>\nvpu.select(0)\nfor x in range(0, 16):\n\t<b>vpu.pset(x, x)</b>\nvpu.update()',
    },
    'rect': {
        usage: 'x,y,dx,dy',
        brief: 'Draw a rectangle onto selected surface from x,y to dx,dy',
        example: 'vpu.select(0)\nvpu.setcolor(0,255,0)\nw, h = vpu.dimensions()\n<b>vpu.rect(0,0,0+w,0+h) <i>#Draw a rectangle around the whole selected layer</i></b> ',
    },
    'restart': {
        usage: '',
        brief: 'Restart Video Processing Unit. Delayed video settings will apply when this is called.',
        example: 'vpu.fullscreen(1)\n<b>vpu.restart()</b>',
    },
    'rotate': {
        usage: 'layer_handle, angle',
        brief: 'Rotate specified layer (0-11) given degrees',
        example: '<b>vpu.rotate(0, 3.141596)</b> <i>#Add 180 degree of rotation to layer 0</i>',
    },
    'scale': {
        usage: 'layer_handle, scale_x, scale_y',
        brief: 'Change specified layer given horizontal and vertical scale factor',
        example: '<b>vpu.scale(0, 1.0, 1.0)</b> <i>#Set scale 1:1 to layer 0</i>',
    },
    'select': {
        usage: 'layer_handle',
        brief: 'Select given layer to perform next graphic operations onto it',
        example: '<b>vpu.select(0) </b><i>#Select background layer</i>\n<b>vpu.select(1) </b><i>#Select foreground layer</i>\n<b>vpu.select(2) </b><i>#Select overlay layer</i>\nsrf = vpu.createsurf(128,128)\n<b>vpu.select(srf)</b> <i>#Select "srf" surface </i>',
    },
    'setcolor': {
        usage: 'r, g, b, a',
        brief: 'Sets current painting color',
        example: '<b>vpu.setcolor(0,255,0)</b>     <i>#Select bright green as painting color</i>\n<b>vpu.setcolor(0,255,0,128)</b> <i>#Select same color, but half the alpha</i>',
    },
    'setrotation': {
        usage: 'layer_handle, angle',
        brief: 'Sets rotation for specified layer at given degrees',
        example: '<b>vpu.setrotation(0, 3.141596)</b> <i>#Set rotation to 180 degree for layer 0</i>',
    },
    'setscale': {
        usage: 'layer_handle, scale_x, scale_y',
        brief: 'Sets scale for specified layer given horizontal and vertical values',
        example: '<b>vpu.setscale(0, 1.0, 1.0)</b>',
    },
    'update': {
        usage: '',
        brief: 'Allow blackbox engine to perform its rendering based input and output operations',
        example: '<b>vpu.update()</b>',
    },
};