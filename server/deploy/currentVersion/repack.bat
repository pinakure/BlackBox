@echo off
call c:\codigo\blackbox\server\deploy\currentVersion\pack axzle
call c:\codigo\blackbox\server\deploy\currentVersion\pack bubblesplitters
call c:\codigo\blackbox\server\deploy\currentVersion\pack minigolf
call c:\codigo\blackbox\server\deploy\currentVersion\pack pookman
call c:\codigo\blackbox\server\deploy\currentVersion\pack repege
call c:\codigo\blackbox\server\deploy\currentVersion\pack rocksncrystals
call c:\codigo\blackbox\server\deploy\currentVersion\pack snake
call c:\codigo\blackbox\server\deploy\currentVersion\pack tetrax
call c:\codigo\blackbox\server\deploy\currentVersion\pack trickynoid
call c:\codigo\blackbox\server\deploy\currentVersion\pack tritriad
call c:\codigo\blackbox\server\deploy\currentVersion\pack xargoon
@echo Running engine...
cd c:\codigo\blackbox\
blackbox_debug_x64.exe
@echo Returning to previous path
cd c:\codigo\blackbox\server\deploy\currentVersion\
