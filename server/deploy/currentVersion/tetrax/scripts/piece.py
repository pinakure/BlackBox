from random                 import random
from data.scripts.shapes    import ShapeTypes
from data.scripts.colors    import Colors
import console
            
class Piece:
    def __init__(self, triste, shape, x, y, rotation=0):
        color_index       = int( random()*  len(Colors) )
        self.triste       = triste
        self.color        = Colors[color_index]
        self.shape        = shape
        self.data         = shape.data
        self.width        = shape.width
        self.height       = shape.height
        self.rotation     = rotation
        self.angle        = rotation
        self.rows         = self.get_rows()
        self.columns      = self.get_columns()
        self.x            = x
        self.y            = y
        self.active       = True
        self.target       = None
        self.target_angle = rotation
        print("Piece initialized")

    def get_rows(self):
        """ Returns a set of rows from the set of rows conforming the shape """
        rows = {}
        for row_index in (0, self.shape.height-1):
            row = {}
            for column_index in range(0, self.shape.width-1):
                row[column_index] = self.shape.data[self.rotation][(row_index * self.shape.width)+column_index]
            rows[row_index] = row
        return rows

    def get_columns(self):
        """ Returns a set of columns from the set of rows conforming the shape"""
        cols = {}
        for column_index in range(0, self.shape.width-1):
            col = {}
            for row_index in range(0, self.shape.height-1):
                col[row_index] = self.shape.data[self.rotation][(row_index*self.shape.width)+column_index]
            cols[column_index] = col
        return cols

    def logic(self):
        self.update()
        self.y+=1
        if not self.is_placeable(self.triste.map):
            self.y-=1
            self.update()
            self.active = False
            if self.write(self.triste.map):
                return self.triste.NEWSHAPE;            
            else:
                return self.triste.GAMEOVER
        if self.target:
            self.follow_target()
        else:
            self.find_target()
        self.update()
        penalty = self.triste.map.get_penalty(self, self.x)
        return self.triste.CONTINUE

    def update(self):
        pass
        """
        var node = $(`table.self.triste#${self.id}`)
        node.attr('x', self.x)
        node.attr('y', self.y)
        node.css('left' , `${int(node.attr('x')) * 16}px`)
        node.css('top'  , `${int(node.attr('y')) * 16}px`)
        """

    def is_placeable(self, _map):
        ip_index = 0
        for ip_row in range(0, self.shape.height-1):
            for ip_col in range(0, self.shape.width-1):
                if self.shape.data[self.rotation][ip_index]==1:
                    x = self.x + ip_col
                    y = self.y + ip_row
                    if not _map.check(x,y):
                        return False
                ip_index+=1
        return True

    def draw(self):
        html = ''
        """
        for(py=0; py < self.shape.height; py++){
            html += '<tr>'
            for(px=0; px < self.shape.width; px++){
                var value =  self.data[self.rotation][(py*self.width)+px]
                var piece_class = `${self.color} ${ value ? 'piece':''}`
                html += `<td><span class="${piece_class}"><span></td>`
            }
            html += '</tr>'
        }
        $('#scene').append(`<table 
            id="${self.id}" 
            class="self.triste ${self.type}" 
            x="${self.x}" 
            y="${self.y}" 
            width="${self.shape.width}" 
            height="${self.shape.height}" 
            type="${self.shape.name}" 
            rotation="${self.rotation}"
        >
            ${html}
        </table>`)
        self.update()
        """

    #-------------------------------------------------------------------------------------------------------------


    def get_weight(self, column):
        weight  = 0
        cols    = self.get_columns()
        column  = cols[column]
        for col_index in range(0, column.length):
            weight += column[col_index]
        return weight

    def get_angle_scores(self):
        return [1,1,1,1]

    def find_optimal_column(self):
        optimal_value   = 0
        optimal_index   = -1
        columns         = {}
        for foci in range(0, self.shape.width):
            columns[foci] = self.getWeight(foci)
        for foci in range(0, self.shape.width):
            if columns[foci] > optimal_value:
                optimal_index = foci
                optimal_value = columns[foci]
        return optimal_index

    def follow_angle(self):
        fallback = self.rotation
        if self.rotation > self.target_angle: 
            self.rotation -= 1
        if self.rotation < self.target_angle:
            self.rotation += 1
        if not self.is_placeable( self.triste.map ):
            self.rotation = fallback
        else:
            self.draw()
    
    def follow_target(self):
        self.follow_angle()
        if self.target is None: 
            return False
        old_x = self.x
        if self.target  > self.x: 
            self.x+=1
        elif self.target  < self.x:
            self.x-=1
        if self.target == self.x:
            self.target = None
        if not self.is_placeable(self.triste.map): 
            self.x = old_x

    def find_target(self):
        """ List all possible landing spots, 
            Considerations:
            - Size of each spot is defined by horizontal surface in self angle
            - Every landing spot sharing same vertical dimension can be overlapped in the list
            - The lower the landing spot, the higher the score for self algoritm
            - More than a row can be taken in count in the piece and the map
        """
        if self.y<0:
            return
        while not self.target:
            old_x = self.x
            pseudo_x = int( random() * self.triste.map.width )
            self.target_angle = int( random() * len(self.shape.data)-1 )
            self.x = pseudo_x
            if self.is_placeable(self.triste.map):
                self.target = pseudo_x
            self.x = old_x
            self.update()

    def get_real_width(self):
        """ Return real piece width, skipping unpopulated columns """    
        gwr     = 0
        gwrlist = self.get_columns()
        for gwri in gwrlist:
            gw      = gwrlist[gwri]
            gwrskip = True
            for oz in gw:
                if gw[oz]:
                    gwrskip=False
            if not gwrskip:
                gwr+=1
        return gwr

    def get_landing_row(self):
        """ Returns first column whose block is landing, starting from bottom left """
        glclist = self.get_rows()
        """ Iterate all rows from bottom to top """
        for glci in range(glclist.length-1, 0):
            glcrow = glclist[glci]
            # Find first populated column in self row and return row index when found 
            for glcri in glcrow:
                glb = glcrow[glcri]
                if glb: 
                    return glci

    def get_landing_column(self):
        """ Returns first column whose block is landing, starting from bottom left """
        glclist = self.get_rows()
        # Iterate all rows from bottom to top 
        landing_row = self.get_landing_row()
        for glci in glclist[landing_row]:
            glb = glclist[landing_row][glci]
            if glb:
                return glci
     
    
    def write(self, _map):
        try:
            for write_y in range(0, self.height):
                for write_x in range(0, self.width):
                    if self.shape.data[self.rotation][(write_y*self.width) + write_x]:
                        _map.data[self.y + write_y][self.x + write_x] = 1
            return True
        except Exception as E:
            console.echo(f"ERROR: {str(E)}")
            return False
    

    

    def to_table(self):
        shape = self.data[self.rotation]
        hrows = ''
        rows = ''
        cells = ''
        """
        for x in range(0, self.width+1):
            cells += f'{x-1 if x>0 else ''}'
        hrows = `<thead><tr>${cells}</tr></thead>`
        for(y=0; y<self.height; y++){
            cells = `<th><strong>${y}</strong></th>`
            for(x=0; x<self.width; x++){
                cells += `<td class="${shape[(y*self.width)+x]?'dark':''}">${shape[(y*self.width)+x]}</td>`
            }
            var row =`<tr>${cells}</tr>`
            rows += row;    
        }
        return `<table>${hrows}<tbody>${rows}</tbody></table>`
        """

    