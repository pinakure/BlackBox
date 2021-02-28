cd C:\codigo\blackbox\server\deploy\currentVersion\Xargoon 
"C:\Program Files\7-Zip\7z.exe" a -tzip xargoon.zip *
move xargoon.zip ../xargoon.zip 
erase C:\codigo\blackbox\data\xargoon.zip
erase C:\codigo\blackbox\data\scripts\* /q
erase C:\codigo\blackbox\data\scripts\__pycache__\* /q
rmdir C:\codigo\blackbox\data\scripts\__pycache__\
rmdir C:\codigo\blackbox\data\scripts\
copy ..\xargoon.zip C:\codigo\blackbox\data\xargoon.zip
cd c:\codigo\blackbox\
blackbox_debug_x64.exe