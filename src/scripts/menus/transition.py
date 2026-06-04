from    menu                     import Menu
import  vpu

def do_transition(transition_type):            
    progress = 0
    print("Close")
    vpu.transition(transition_type)
    while not vpu.update():
        print("Closing", end=('.'*(int(progress/10)%4))+"   \r")
        progress += 1
    print("Closed        ")                    
    # change screen content here
    vpu.enable(1)
    print("Change Screen contents here")
    if TransitionMenu.background_status: 
        TransitionMenu.background_status = False
        vpu.select(1)
        vpu.fill(50,100,25,32)
    else: 
        TransitionMenu.background_status = True
        vpu.select(1)
        vpu.fill(100,50,25,32)
    print("Opening")
    vpu.transition(transition_type)
    while not vpu.update():
        print("Opening", end=('.'*(int(progress/10)%4))+"   \r")
        progress += 1
    print("Opened        ")

class TransitionMenu:
    background_status = False    

    @staticmethod
    def open():
        menu = Menu("Transitions")
        menu.addchild("Diagonal"    , TransitionMenu.diagonal   )
        menu.addchild("Horizontal"  , TransitionMenu.horizontal )
        menu.addchild("Vertical"    , TransitionMenu.vertical   )
        menu.addchild("Shaped"      , TransitionMenu.shaped     )
        menu.addchild("Back")
        menu.update()
                
    @staticmethod
    def diagonal():
        choices = {
            "1" : "SE",
            "2" : "SW",
            "3" : "NW",
            "4" : "NE",
        }
        choice = Menu.execute(choices, "Diagonal Sweep Transition", True)
        if choice is None: return
        do_transition(int(choice))
            
    @staticmethod
    def horizontal():
        choices = {
            '5'  : 'S to N'      ,
            '6'  : 'N to S'      ,
            '7'  : 'N&S to MID'  ,
            '8'  : 'MID to N&S'  ,
            '14' : 'STRIPS'      ,
            '21' : 'DIAGONAL IN' ,
            '23' : 'DIAGONAL OUT',
        }
        choice = Menu.execute(choices, "Horizontal Sweep Transition", True)
        if choice is None: return                    
        do_transition(int(choice))

    @staticmethod
    def vertical():
        choices = {
            9  : 'E to W'      ,
            10 : 'W to E'      ,
            11 : 'W&E to MID'  ,
            12 : 'MID to W&E'  , 
            13 : 'STRIPS'      ,
            20 : 'DIAGONAL IN' ,
            22 : 'DIAGONAL OUT',
        }
        choice = Menu.execute(choices, "Vertical Sweep Transition")
        if choice is None: return
        do_transition(int(choice))                    
        
    @staticmethod
    def shaped():
        choices = {
            0  : 'PLAIN'     ,
            15 : 'SQUARE IN' ,
            16 : 'SQUARE OUT',
            17 : 'CIRCLE IN' ,
            18 : 'CIRCLE_OUT', 
            19 : 'CHECKERS'  ,
            24 : 'SPIRAL'    ,
        }
        choice = Menu.execute(choices, "Shaped Transitions", True)
        if choice is None: return
        do_transition(int(choice))                    
