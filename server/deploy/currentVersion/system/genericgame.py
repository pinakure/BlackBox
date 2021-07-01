class GenericGame:
    running = True
    width   = 0
    height  = 0
    
    def setup(self, init_callback, loop_callback, deinit_callback):
        GenericGame.init_callback = init_callback
        GenericGame.loop_callback = loop_callback
        GenericGame.deinit_callback = deinit_callback
        GenericGame.init_callback()

    def loop():
        GenericGame.loop_callback()

    def deinit():
        GenericGame.deinit_callback()

