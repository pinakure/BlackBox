import console 

def _print(msg, end="\n"):
    #console.echo(msg)
    print(msg, end=end)

def printpadded(txt, label, end="\n"):
    msg = []
    inp = txt
    padding = len(label)
    while(len(inp)>80-padding):
        msg.append(inp[0:80-padding])
        inp = inp[80-padding:]
    if len(msg)>0:
        _print(f'{label}{msg[0]}')
        for i in range(1, len(msg)):
            _print(f'{label}{msg[i]}')
    else:
        _print(f'{label}{inp}', end=end)

def debug(classname, message, end="\n", width=80-12):
    classname+=': '
    if len(classname)<12: classname = (" "*(12-len(classname)))+classname
    printpadded(message, classname, end)
    
def panic(message, help=""):
    _print(f"\n{'-'*80}\n")
    printpadded(message, "PANIC: ")    
    printpadded(help   , "       ")
    _print(f"\n{'-'*80}\n")
    quit()

def error(classname, message):
    printpadded(message, f"ERROR @ {classname}: ")

def deprecate(oldfunc, newfunc):
    _print(f"\n{'-'*80}\n")
    _print(f"{oldfunc} IS DECLARED DEPRECATED, please use {newfunc} instead")
    _print(f"\n{'-'*80}\n")
    
def exception(E):
    _print(f"\n{'-'*80}\n")
    import traceback 
    import console
    parts = traceback.format_exc().split('\n')
    _print(f"Exception: {str(E)}")
    for p in parts:
        console.echo(p)
        printpadded("           ", p)
    _print(f"\n{'-'*80}\n")
    