from debug import debug, deprecate, error, exception, panic
from random import random
from data.scripts.song import Song

class Music:
    def fade(self, start, end, unknown):
        pass


class MusicPlayer:

    def __init__(self, hud):
        #rand.setSeed(System.nanoTime())        
        self.hud             = hud        
        self.songList        = self.getSongList()        
        self.music           = Music()
        self.playbackTime    = 0
        self.playbackLimit   = 100000
        self.author          = ""
        self.title           = ""
        self.songTitleOffset = 0.0
        self.songTitleDelta  = 0.015
        self.maxSongs        = 0
        self.nextSong        = None

    def getSongAuthor(self): 
        return self.author
    
    def getSongList(self):
        songlist = {}
        self.maxSongs = 0
        
        try:                 
            """
            f = new File("data/music");

            # array of files and directory
            files = f.list();
            
            # for each name in the path array
            for file in files:
            
                # prints filename and directory name
                if file.toLowerCase().endsWith(".mod"):
                    songName = getModTitle(0, file);                  
                    songTempo = 256;
                elif file.toLowerCase().endsWith(".xm"):
                    songName = getModTitle(0x11, file);
                    songTempo = getModTempo(file);
                    //songTempo *= 2;
                elif file.toLowerCase().endsWith(".ogg"):
                    songName = getModTitle(0x11, file);
                    songTempo = 400;#getModTempo(file);
                else:
                    # Not a valid module, dont load it.
                    continue;
                songlist.add(new Song(file, songName, "Unauthed", songTempo));
                self.maxSongs++;
            """      
        except Exception as E:
            exception(E)
        return songlist
    
    def deleteSong(self):
        self.music.stop()        
        # Take song reference 
        s = self.songList.get(self.nextSong)        
        # Unlink from songList
        self.songList.remove(self.nextSong)
        self.maxSongs -= 1
        """
        File file =new File("data/music/" + s.getFilename());
        # Move song to trash
        file.renameTo(new File("data/music/deleted.trash/" + s.getFilename()));    		
   	    """
        self.nextSong()

    def getModTempo(self, filename):
        songTempo = 140
        """
        f = new File("data/music/" + filename);
        
        byte padding[];
        byte bytes[];
        byte mult[];
        padding = new byte[76];
        bytes = new byte[2];
        mult = new byte[2];
        float factor;
        
        try {
            
            FileInputStream fr = new FileInputStream(f);
            fr.read(padding, 0, 76);            
            fr.read(mult, 0, 2);
            fr.read(bytes, 0, 2);
            
        } catch (FileNotFoundException ex) {
            Logger.getLogger(JTrickyNoid.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            
            factor = mult[0]*2;
            songTempo = ((int)bytes[0]);
            if(songTempo < 0) songTempo *= -1.0f;
            songTempo *= factor;
        }
        """
        return songTempo

    def getPlaybackTime(self):
        return self.playbackTime
    
    def isPlaying(self):
        return self.music.playing()
    
    def getModTitle(self, offset, filename):
        """
        f = new File("data/music/" + filename);
        byte bytes[];
        String songName;
        
        bytes = new byte[20];
        
        try {
            
            FileInputStream fr = new FileInputStream(f);
            fr.read(bytes, 0, offset);
            fr.read(bytes, 0, 20);
            songName = new String(bytes);
            
            fr.read(bytes, 0, 1);//skip 1 byte
            fr.read(bytes, 0, 20);
            author = new String(bytes);
            
        } catch (FileNotFoundException ex) {
            Logger.getLogger(JTrickyNoid.class.getName()).log(Level.SEVERE, null, ex);
            songName = "ERROR";
            author = "ERROR";
        }
        return songName;
        """
        return "NONAME"
    
    def songTitleUpdate(self):
        name = self.songList.get(self.nextSong).getTitle()
        for i in range(0, 14):            
            """
            # 1ST Line : Song Title
            code = name.codePointAt(i + (int)songTitleOffset);
            if((code > 31)&&(code < 0xff)){
                hud.setTileId(48+i, 1, 1, code - 31);
            } else {
                hud.setTileId(48+i, 1, 1, 0x01);
            }
            
            # 2ND Line : Song Author
            code = author.codePointAt(i + (int)songTitleOffset);
            if((code > 31)&&(code < 0xff)){
                hud.setTileId(48+i, 2, 1, code - 31);
            } else {
                hud.setTileId(48+i, 2, 1, 0x01);
            }
            
            # 3RD Line : Song timer
            
            int time = playbackTime;
            
            int seconds = playbackTime / 1000;
            int minutes = seconds / 60;
            int hours  = minutes / 60;
            
            int value[] = { (time % 100) / 10, 
                            (time % 1000) / 100,
                            0xa,
                            seconds % 10,
                            (seconds % 60) /10,
                            0xa,
                            minutes % 60,
                            (minutes % 60) / 10,
                            0xa, // :
                            hours % 10};
            """
            if i<10:
                hud.setTileId(61-i, 3, 1, 0x11 + value[i] )
            else:
                hud.setTileId(61-i, 3, 1, 0x1)
    
    def quiet(self): 
        self.music.fade(300, 0.50, False)

    def loud(self):
        self.music.fade(300, 1.0, False)
    
    def musicEnded(self, music):
        nextSong()
    
    def musicSwapped(self, music,  newMusic):
        pass
    
    def getTempo(self):
        speed = int(self.songList.get(self.nextSong).getSpeed())
        print(f"TITLE {self.songList.get(self.nextSong).getTitle() } : - : Song speed: {speed}")
        return speed
    
    def update(self, delta):        
        self.songTitleOffset += self.songTitleDelta * float(delta)
        self.songTitleUpdate()
        
        if (self.songTitleOffset >= 6.5) or (self.songTitleOffset <= 0.0):
            self.songTitleDelta *= -1.0
        
        self.playbackTime += delta
        
        if self.playbackTime > self.playbackLimit:
            self.music.fade(1500, 0.0, True)
            if self.playbackTime > self.playbackLimit + 3000:
                self.nextSong()
    
    def nextSong(self):
        if self.music is not None:
            #music.removeListener(self)
            pass
        
        self.nextSong = int(random() * maxSongs-1)
        try:
            self.music = Music("data/music/" + songList.get(nextSong).getFilename())
        except Exception as E:
            exception(E)
        
        self.songTitleOffset    = 0.0
        self.songTitleDelta     = 0.0015
        self.playbackTime       = 0
        
        self.music.addListener(self)
        self.music.loop(1.0, 0.0)
        self.music.fade(1000, 1.0, False)
        self.songTitleUpdate()