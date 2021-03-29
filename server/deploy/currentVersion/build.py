from datetime import datetime
import sqlite3
from toc import titles
import os

class Build:
    drive       = 'C'
    code_path   = f'{drive}:/codigo/BlackBox'
    server_path = f'{code_path}/server/deploy/currentVersion'
    system_path = f'{server_path}/system'
    sevenzip    = '"C:\\Program Files (x86)\\7-Zip\\7z.exe"'
    move        = '@move'
    erase       = '@erase'
    erasedir    = '@rmdir'
    copy        = '@copy'
    clear       = 'cls'

    def report(self, msg):
        msg+='...'
        msg = msg + (" "*(48-len(msg)))
        print(f"{msg}", end="")
        
    def __init__(self):
        self.titles = {}
        print("---------------------------------------------------\n")
        self.con = sqlite3.connect('build.db')
        self.cur = self.con.cursor()
        try:
            self.cur.execute("CREATE TABLE system (filename text primary key, date datetime)")
            self.cur.execute("CREATE TABLE titles (title text, filename text, date datetime, PRIMARY KEY(title, filename)) ")
            self.con.commit()
            print("Database initialized")
        except:
            pass
        print("Database loaded\n---------------------------------------------------\n")

    def load_toc(self):
        self.titles = titles

    def update_system_timestamp(self, filename):
        date = datetime.now()
        try:
            self.cur.execute(f"INSERT INTO system VALUES ('{filename}', '{date.strftime('%Y-%m-%d %H:%M:%S')}')")
            self.con.commit()
            print(f"\nCreated entry for {filename}")
        except:
            self.cur.execute(f"UPDATE system SET date='{date.strftime('%Y-%m-%d %H:%M:%S')}' WHERE filename='{filename}'")
            self.con.commit()
            print(f"\nUpdated entry for {filename}")

    def get_timestamp(self, path, filename):
        try:
            import time
            modTimesinceEpoc = os.path.getmtime(f'{path}/{filename}')
            # Convert seconds since epoch to readable timestamp
            
            localtime = time.localtime(modTimesinceEpoc)
            localtime = time.strftime('%Y-%m-%d %H:%M:%S', localtime)          
            return localtime
        except Exception as E:
            print(f"ERROR: {str(E)}")
            return datetime.strptime(datetime.now().strftime('%Y-%m-%d %H:%M:%S'), '%Y-%m-%d %H:%M:%S')

    def check_system_timestamp(self, filename):
        try:
            self.cur.execute(f"SELECT date FROM system WHERE filename='{filename}'")        
            current_timestamp = self.get_timestamp(self.system_path, filename)
            last_timestamp = self.cur.fetchone()                
            if last_timestamp is None:
                last_timestamp = current_timestamp
                #print(f"\n\t{filename} not found in database")
                #print(f"\tCreating entry for {filename}")
                #print(f"\tTimestamp: {last_timestamp}")
                self.cur.execute(f"INSERT INTO system VALUES ('{filename}', '{last_timestamp}')")
                self.con.commit()
            else:
                last_timestamp = last_timestamp[0]
            last_timestamp = datetime.strptime(last_timestamp,"%Y-%m-%d %H:%M:%S")            
            current_timestamp = datetime.strptime(current_timestamp, "%Y-%m-%d %H:%M:%S")
            return current_timestamp > last_timestamp            
        except Exception as E:
            print(f"\nERROR: {str(E)}")
            return True

    def check_title_timestamp(self, title, filename):
        try:
            self.cur.execute(f"SELECT date FROM titles WHERE filename='{filename}' and title='{title}'")        
            current_timestamp = self.get_timestamp(f'{self.server_path}/{title}', filename)
            last_timestamp = self.cur.fetchone()                
            if last_timestamp is None:
                last_timestamp = current_timestamp
                self.cur.execute(f"INSERT INTO titles VALUES ('{title}', '{filename}', '{last_timestamp}')")
                self.con.commit()
            else:
                last_timestamp = last_timestamp[0]
                last_timestamp = last_timestamp.split('.')[0]                
            last_timestamp = datetime.strptime(last_timestamp,"%Y-%m-%d %H:%M:%S")            
            current_timestamp = datetime.strptime(current_timestamp, "%Y-%m-%d %H:%M:%S")
            return current_timestamp > last_timestamp            
        except Exception as E:
            print(f"\nERROR: {str(E)}")
            return True

    def update_title_timestamp(self, title, filename):
        date = datetime.now()
        try:
            self.cur.execute(f"INSERT INTO titles VALUES ('{title}', '{filename}', '{date}')")        
            self.con.commit()
        except:
            self.cur.execute(f"UPDATE titles SET date='{date}' WHERE title='{title}' AND filename='{filename}'")
            self.con.commit()

    def get_title_filelist(self, title):
        from os import listdir
        path = f'{self.server_path}/{title}'
        from os.path import isfile, join
        files  =  [f for f in listdir(f'{path}'         ) if isfile(join(f'{path}'          , f))]
        try: files.extend([f'scripts/{f}' for f in listdir(f'{path}/scripts' ) if isfile(join(f'{path}/scripts'  , f))])
        except: pass
        try: files.extend([f'tilesets/{f}' for f in listdir(f'{path}/tilesets') if isfile(join(f'{path}/tilesets' , f))])
        except: pass
        try: files.extend([f'sprites/{f}' for f in listdir(f'{path}/sprites' ) if isfile(join(f'{path}/sprites'  , f))])
        except: pass
        try: files.extend([f'sfx/{f}' for f in listdir(f'{path}/sfx'     ) if isfile(join(f'{path}/sfx'      , f))])
        except: pass
        try: files.extend([f'maps/{f}' for f in listdir(f'{path}/maps'    ) if isfile(join(f'{path}/maps'     , f))])
        except: pass
        try: files.extend([f'sounds/{f}' for f in listdir(f'{path}/sounds'  ) if isfile(join(f'{path}/sounds'   , f))])
        except: pass
        return files

    def get_system_filelist(self):
        from os import listdir
        from os.path import isfile, join
        files =  [f for f in listdir(self.system_path) if isfile(join(self.system_path, f))]
        return files

    def check_system(self):
        print("-------------------------------------------------------")
        print("Checking system libraries")
        modified = False
        filelist = self.get_system_filelist()
        for filename in filelist:
            self.report(f"Checking {filename}")
            _modified = self.check_system_timestamp(filename)
            if _modified:
                print("[ MODIFY ]", end="\r")    
                modified = True
                self.update_system_timestamp(filename)
            else: 
                print("[ INTACT ]", end="\r")
        print("Finished Checking System Files.                                                            ")
        if modified:
            b.pack_system()
            b.deploy_system()
            return True
        else:
            return False

    def check_title(self, title):
        print(f"Checking title {title['name']} ({title['url'].split('.')[0]})", end="\r")
        modified = False
        filelist = self.get_title_filelist(title['url'].split('.')[0])
        for filename in filelist:        
            self.report(f"Checking {filename}")
            _modified = self.check_title_timestamp(title['url'].split('.')[0], filename)
            if _modified:
                modified = True
                print("[ MODIFY ]", end="\r")
                self.update_title_timestamp(title['url'].split('.')[0], filename)
            else:
                print("[ INTACT ]", end="\r")
        print(f"Finished Checking {title['name']}.                                       ")
        if modified:
            b.pack_title(title)
            b.deploy_title(title)
            return True
        else:
            return False

    def pack_system(self):
        self.pack_title({'name': 'System Libraries', 'url' : 'system.zip'})
        
    def deploy_system(self):
        self.deploy_title({'name': 'System Libraries', 'url' : 'system.zip'})
        
    def pack_title(self, title):
        self.report(f"Packing title {title['name']}")
        target = title['url'].split('.')[0]
        os.system(f"cd {self.server_path}\\{target}")
        os.system(f'{self.sevenzip} a -tzip {target}.zip {self.server_path}\\{target}\\*  1> NUL 2> NUL')
        os.system(f'{self.move} {target}.zip {self.server_path}\\{target}.zip  1> NUL 2> NUL')
        print("[  DONE  ]")
        
    def deploy_title(self, title):
        self.report(f"Deploying title {title['name']}")
        target = title['url'].split('.')[0]
        os.chdir(self.code_path)
        os.system(f'{self.erase}    data\\{target}.zip 1>NUL 2>NUL')
        os.system(f'{self.erase}    data\\scripts\\* /q 1> NUL 2> NUL')
        os.system(f'{self.erase}    data\\scripts\\__pycache__\\* /q 1> NUL 2>NUL')
        os.system(f'{self.erasedir} data\\scripts\\__pycache__\\ 1> NUL 2> NUL')
        os.system(f'{self.erasedir} data\\scripts\\ 1> NUL 2> NUL')
        os.chdir(self.server_path)
        ttarget = f"{self.code_path}/data/{target}.zip".replace('/', '\\')
        os.system(f'{self.copy} {target}.zip {ttarget} 1> NUL 2> NUL')
        print("[  DONE  ]")
        print("---------------------------------------")        
        
    def check_titles(self):
        print("-------------------------------------------------------")
        print("                 Checking titles")
        print("-------------------------------------------------------")
        
        for title in self.titles:
            self.check_title(title)
            
    def run(self):
        os.system(f'{self.clear}')
        print("\bBuild.py Maintenance Script\n-------------------------------------------------------")
        print("Updating Data Files")
        self.load_toc()
        self.check_system()
        self.check_titles()

b = Build()
b.run()
os.chdir(b.code_path)
print("-------------------------------------------------------\nRunning BlackBox...\n-------------------------------------------------------\n")
os.system('blackbox_debug_x64.exe')
b.report("Cleaning up...")
os.chdir(b.code_path)
os.system(f'{b.erase}    data\\scripts\\* /q 1> NUL 2> NUL')
os.system(f'{b.erase}    data\\scripts\\__pycache__\\* /q 1> NUL 2>NUL')
os.system(f'{b.erasedir} data\\scripts\\__pycache__\\ 1> NUL 2> NUL')
os.system(f'{b.erasedir} data\\scripts\\ 1> NUL 2> NUL')
print("[ FINISHED ]")