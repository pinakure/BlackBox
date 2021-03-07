cd C:\codigo\blackbox\server\deploy\currentVersion\Xargoon 
"C:\Program Files (x86)\7-Zip\7z.exe" a -tzip xargoon.zip *
move xargoon.zip ../xargoon.zip 
@echo Cleaning cache files...
@erase C:\codigo\blackbox\data\xargoon.zip 1>NUL 2>NUL
@erase C:\codigo\blackbox\data\scripts\* /q 1> NUL 2> NUL
@erase C:\codigo\blackbox\data\scripts\__pycache__\* /q 1> NUL 2>NUL
rmdir C:\codigo\blackbox\data\scripts\__pycache__\ 
rmdir C:\codigo\blackbox\data\scripts\
copy ..\xargoon.zip C:\codigo\blackbox\data\xargoon.zip
cd c:\codigo\blackbox\
blackbox_debug_x64.exe