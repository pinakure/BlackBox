import tm
class TiledMap:
    game = None

    def __init__(self, game, width, height, layer_count=1, tile_width=8, tile_height=8):
        TiledMap.game = game
        self.x           = ( game.width   >> 1 ) - (( width  * tile_width  ) >> 1 )
        self.y           = ( game.height  >> 1 ) - (( height * tile_height ) >> 1 )
        self.width       = width        
        self.height      = height
        self.layer_count = layer_count
        self.tile_width  = tile_width
        self.tile_height = tile_height
        self.target      = 0 # background
        self.handle      = tm.create(width, height, layer_count, tile_width, tile_height)

    def redraw(self, layer_index=-1):
        tm.redraw(self.handle, layer_index)
    
    def fill(self, value, layer_index=-1):
        tm.fill(self.handle, value, layer_index)
    
    def clear(self, layer_index=-1):
        tm.clear(self.handle, layer_index)

    def set(self, x, y, tile_index, layer_index=0):
        tm.set(self.handle, x, y, tile_index, layer_index)

    def get(self, x, y, layer_index=0):
        return tm.get(self.handle, x, y, layer_index)

    def gettileset(self, x, y, layer_index=0):
        return (tm.get(self.handle, x, y, layer_index) & 0xFF000000) >> 6

    def gettile(self, x, y, layer_index=0):
        return (tm.get(self.handle, x, y, layer_index) & 0x00FFFFFF)

    def getdata(self, layer_index=0):
        return tm.getdata(self.handle, layer_index)

    def setdata(self, data, layer_index=0):
        tm.setdata(self.handle, data, layer_index)

    def settarget(self, surface_handle):
        tm.settarget(self.handle, surface_handle)

    def draw(self):
        tm.draw(self.handle, self.x, self.y)

    def update(self, delta):
        tm.update(self.handle, delta)
