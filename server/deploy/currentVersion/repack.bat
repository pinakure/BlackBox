cd Xargoon 
"C:\Program Files\7-Zip\7z.exe" a -tzip xargoon.zip *
move xargoon.zip ../xargoon.zip 
erase C:\codigo\blackbox\data\xargoon.zip
cd c:\codigo\blackbox\
blackbox_debug_x64.exe