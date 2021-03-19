import vpu

class Color:
    def __init__(self, color):        
        self.r = int(color&0x000000FF)
        self.g = int(color&0x0000FF00)>>8
        self.b = int(color&0x00FF0000)>>16
        self.a = int(color&0xFF000000)>>24

    def __str__(self):
        return f'[{self.r},{self.g},{self.b},{self.a}]'

    def integer(self):
        return ((int(self.a)&0xFF)<<24) | ((int(self.b)&0xFF)<<16) | ((int(self.g)&0xFF)<<8) |((int(self.r)&0xFF))

class ReplaceSet:
    def __init__(self, real, day, dawn, nite):
        print(f'{real}, {day}, {dawn}, {nite}')
        self.real   = real
        self.day    = day
        self.dawn   = dawn
        self.nite   = nite

def extract_palette(filename='nite'):
    palette = {}
    real_bg = vpu.createsprite('bg_set')
    if not real_bg:
        print("Cannot load bg_set.png")
    repl_bg = vpu.createsprite(f'bg_{filename}')
    if not repl_bg:
        print(f"Cannot load bg_{filename}.png")
    print("Extracting palettes...")
    with open(f"{filename}.pal", "w") as f:
        f.write("JASC-PAL\n0100\n256\n")                    
    vpu.selectsprite(real_bg); data_real = vpu.getsurfacedata()
    vpu.selectsprite(repl_bg); data_repl = vpu.getsurfacedata()
    i = 0
    colors = 0
    for y in range(0,2816):
        print(f"Scanning {y}/2816 {int((y/2816)*100)}%   ", end="\r")
        for x in range(0,240):
            color = data_real[i]
            if color not in palette.keys():
                palette[color] = True
                real = Color(color)            
                print(f"Scanning {y}/2816 {int((y/2816)*100)}%   : Adding {str(real)} color to palette...SIZE={len(palette)}            \t",end="\r")
                repl = Color(data_repl[i])            
                with open(f"{filename}.pal", "a") as f:
                    f.write(f'{repl.r} {repl.g} {repl.b}\n')
                    colors +=1
            i+=1
    with open(f"{filename}.pal", "a") as f:
        while colors < 256:
            f.write(f'{real.r} {real.g} {real.b}\n')
            colors+=1        
        f.write("\n")