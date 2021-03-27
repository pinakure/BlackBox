import console 
from data.scripts.piece import Piece

class Map:
    def __init__(self, triste, width, height):
        self.triste = triste
        self.width  = width
        self.height = height
        self.data   = {}

    def copy(self, data):
        for di in data:
            arr = {}
            c = data[di]
            for ci in c:
                dc = c[ci]
                arr[ci] = dc
            self.data[di] = arr

    def create(self):
        i=0
        self.data = {}
        for y in range(0, self.height):
            self.data[y] = {}
            for x in range (0, self.width):
                self.data[y][x] = 0x00
                i+=1
    
    def add_shape(self, shape, x=0, y=0, angle=0):
        angle = angle % len(shape.data)
        piece = Piece(self.triste, shape, x, y, angle)
        #if shape.center[0] > -1: piece.x -= shape.center[0]
        if piece.is_placeable(self.triste.map): 
            self.triste.piececount+=1
            # draw piece
            # html = piece.render() 
            piece.logic()
            return piece
        offset = piece.find_optimal_column()
        return None

    def check(self, x, y):
        """ 
            Checks for obstacles in the put bloc procedure:
            - Returns False if...
                ...a block is found on given position or
                ...given position is outside the map or
                ...given position is already occupied by a block
            - Returns True if...
                ...no block is occuping given position or
                ...vertical position is negative  
        """
        if y <  0           : return True
        if x >= self.width  : return False
        if x <  0           : return False
        if y >= self.height : return False
        return self.data[y][x] == 0

    #----------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------

    def get_heights(self):
        """ 
            Returns array of column heights 
        """
        height_score = []
        columns = self.get_columns()
        for col_index in columns:
            column = columns[col_index]
            h_score = self.height
            for row_index in range(0,self.height):
                if column[row_index]:
                    row_index = self.height
                    continue
                h_score-=1
            height_score[height_score.length] = h_score
        return height_score

    def max_height(self):
        """ 
            Returns the highest point reached by a column 
        """
        heights = self.get_heights()
        hi_height = 0
        for h_index in heights:
            val = heights[h_index]
            if val > hi_height:
                hi_height = val
        return val

    def get_columns(self):
        """ 
            Returns a set of columns from the set of rows conforming the map 
        """
        columns = []
        for column_index in range(0, self.width):
            column = []
            for row_index in range(0, self.height):
                column[row_index] = self.data[row_index][column_index]
            columns[column_index] = column
        return columns

    def get_weights(self):
        """ 
            Returns array of scores based on how much space next to self column exists. 
            Columns with less populated neighbor columns will score less 
        """
        columns = self.get_columns()
        ret = []
        for col_index in columns:
            column = columns[col_index]
        return ret

    def get_hole_count(self):
        """ 
            Returns array of scores based on how much lost holes are there in each column.
            A lost hole is a block which is not set, but has a block over it 
        """
        hole_score = []
        columns = self.get_columns()
        for col_index in columns:
            column = columns[col_index]
            holes = 0
            block_found = False
            for index in column:
                if column[index]:
                    block_found = True
                elif not block_found:
                    holes+=1
            hole_score[hole_score.length] = holes
        return hole_score

    def get_scores(self):
        heights = self.get_heights()
        ret = []
        for hi in range(0, heights.length):
            ret[hi] = self.height-heights[hi]
        return ret

    def to_table(self, extra=False):
        pass
        """
            shape = self.data;
            hrows = '';
            rows = '';
            cells = '';
            for(x=0; x<self.width+1; x++){
                cells += `<th><strong>${x<self.width?x:''}</strong></th>`;
            }
            hrows = `<thead><tr>${cells}</tr></thead>`;
            for(y=0; y<self.height; y++){
                cells = '';
                for(x=0; x<self.width; x++){
                    cells += `<td class="${shape[y][x]?'dark':''}">${shape[y][x]}</td>`;
                }
                cells += `<th><strong>${y}</strong></th>`;
                row =`<tr>${cells}</tr>`;
                rows += row;    
            }
            if(extra){
                score_array = self.getScores();
                scores = '';
                for(si in score_array){
                    scores += `<th>${score_array[si]}</th>`;
                }
                rows += `<tr></tr><tr>${scores}<td colspan=>S</td></tr>`;
                score_array = self.getLandingZones(self.triste.currentpiece);
                scores = '';
                d = 0;
                for(i=0;i<self.width; i++){
                    scores += `<th>${score_array.includes(i)?score_array[d]:'--'}</th>`;
                    d+= score_array.includes(i)*1;
                }
                rows += `<tr></tr><tr>${scores}<td colspan=>LZ</td></tr>`;
            }
            return `<table>${hrows}<tbody>${rows}</tbody></table>`;
        }
    """

    def simulate(self, shape, column):
        """ Simulate piece in map and return map data """
        map = Map(self.triste, self.width, self.height)
        map.copy(self.data)
        for shape.y in range(0, map.height-1):
            if not shape.is_placeable(map):
                shape.y-=1
                shape.write(map)
                return map
        return map    

    def get_landing_zones(self, shape):
        lc = shape.get_landing_column()
        zones = []
        s = Piece(shape.shape, shape.x, shape.y)
        for i in range(0, self.width):
            tx = i-lc
            s.x = tx
            if s.is_placeable(self): zones[zones.length] = i
        return zones

    def get_penalty(self,shape,column):
        if shape.y < 0: 
            return 0
        shape = Piece(self.triste, shape.shape, column, shape.y, shape.rotation)
        s = shape.data[shape.rotation]
        map = self.simulate(shape, column)
        penalty = 0
        w = shape.get_real_width()
        for ri in range(map.height-1, shape.y):
            for ci in range(0, w):
                ii = shape.x + ci
                col = map.data[ri][ii]
                if not col: 
                    penalty+=1
        #print(self.triste.map.toTable(True), 'map');
        #print(map.toTable(), 'map', False);
        #print(`<pre style="float: right">Penalty: ${penalty}</pre>`, 'map', False)
        #print(shape.to_table(), 'pieces')
