titles = [
    {
        'name' : 'xargoon',
        'url' : 'A452BA21',
        'picture' : 'xargoon' ,
        'genre' : 'space shooter',
        'multiplayer' : False,
        'cooperative' : False,
        'joystick' : True,
        'mouse' : False,
        'keyboard' : True,
        'rating' : 5,
        'released' : '1-6-2021',
        'status' : 'beta',
        'version' : 14.32,
        'parent' : None,
        'downloaded' : False,
        'developer' : 'Black Hat Cat Studio',
        'title_key' : 0xDEADBEEFDEADBABACAFECAFEC0C0CACA,        
    },
    {
        'name' : 'axzle',
        'url' : 'A75AE321A',
        'picture' : 'axzle' ,
        'genre' : 'puzzle',
        'multiplayer' : False,
        'cooperative' : False,
        'joystick' : True,
        'mouse' : True,
        'keyboard' : True,
        'rating' : 5,
        'released' : '1-6-2021',
        'status' : 'beta',
        'version' : 14.32,
        'parent' : None,
        'downloaded' : False,
        'developer' : 'Black Hat Cat Studio',
        'title_key' : 0xBABEBABEBABEBABECAFECAFEC0C0CACA,        
    }
]

def load():
    import blackbox
    for title in titles:
        blackbox.addtitle(
            title['name'], 
            title['url'], 
            title['picture'], 
            title['genre'], 
            title['multiplayer'], 
            title['cooperative'], 
            title['joystick'], 
            title['mouse'], 
            title['keyboard'], 
            title['rating'], 
            title['released'], 
            title['parent'] if title['parent'] else ""
        )
