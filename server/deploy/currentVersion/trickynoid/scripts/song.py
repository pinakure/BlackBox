class Song:
    def __init__(self, filename, title, author, speed):
        self.filename   = filename
        self.title      = title
        self.author     = author
        self.speed      = speed

    def getFilename(self):      return self.filename
    def setFilename(filename):  self.filename = filename
    def getTitle(self):         return self.title
    def setTitle(title):        self.title = title
    def getAuthor(self):        return self.author
    def setAuthor(author):      self.author = author
    def getSpeed(self):         return self.speed
    def setSpeed(speed):        self.speed = speed
