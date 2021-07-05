import gglib 

class GenericGame:
    running = True
    width   = 0
    height  = 0
    
    def setup(init_callback, loop_callback, deinit_callback):
        GenericGame.init_callback = init_callback
        GenericGame.loop_callback = loop_callback
        GenericGame.deinit_callback = deinit_callback
        
    def init(width, height, map_type):
        GenericGame.width = width
        GenericGame.height = height
        gglib.create(width, height, map_type)
        GenericGame.init_callback()

    def loop():
        gglib.update(1.0)
        GenericGame.loop_callback()
        gglib.draw()

    def deinit():
        GenericGame.deinit_callback()
        gglib.destroy()
