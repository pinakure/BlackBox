from console    import echo as cout
import console
import typewriter
import vpu

class Menu:
    @staticmethod
    def execute(choices, caption, show_back=True):
        choice = ''
        for k in choices:
            typewriter.addchoice(str(k), f' {choices[k]} ')
        if show_back:
            typewriter.addchoice('None', 'Back' )
        while choice=='':
            vpu.update()
            choice = typewriter.getchoice( caption )
        return choice if choice != 'None' else None

    def __init__(self, caption):
        self.parent     = None
        self.caption    = caption
        self.children   = {}
        self.callback   = {}

    def addchild(self, caption, callback=None):
        key = str(len(self.children))
        key = (("0"*(3-len(key)))+key) if len(key)<2 else key 
        key = (("0"*(2-len(key)))+key) if len(key)<1 else key
        print(key, caption)
        key = f'a{key}'
        self.children[key] = caption
        self.callback[key] = callback
        
    def update(self):
        try:
            while(1):
                choice = Menu.execute(self.children, self.caption, self.parent is not None)
                if self.callback[choice] is None: return
                self.callback[choice]()    
                if not typewriter.ready():
                    vpu.update()
        except Exception as E:
            import traceback
            tb = traceback.format_exc()
            tb = tb.split('last):')[1]
            cout('-'*70)
            cout((" "*30)+"Exception!")
            cout('-'*70)
            for s in tb.split('\n'):
                if len(s)==0: continue
                try:
                    line = s.split(' line ')[1].split(',')[0]
                    function = s.split(' in ')[1]
                    file = s.split('File "')[1].split('"')[0]
                    cout(f'{file}:{function}:{line}')
                except:
                    cout(s)
            console.toggle()
