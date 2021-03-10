@echo off
set game=%1%
@echo ----------------------------------------------------------------------------
@echo Packing %game%...
@echo ----------------------------------------------------------------------------
cd C:\codigo\blackbox\server\deploy\currentVersion\%game%
"C:\Program Files (x86)\7-Zip\7z.exe" a -tzip %game%.zip *
move %game%.zip ../%game%.zip 
@echo Cleaning cache files...
@erase C:\codigo\blackbox\data\%game%.zip 1>NUL 2>NUL
@erase C:\codigo\blackbox\data\scripts\* /q 1> NUL 2> NUL
@erase C:\codigo\blackbox\data\scripts\__pycache__\* /q 1> NUL 2>NUL
rmdir C:\codigo\blackbox\data\scripts\__pycache__\ 1> NUL 2> NUL
rmdir C:\codigo\blackbox\data\scripts\ 1> NUL 2> NUL
@echo Deploying %game%...
copy ..\%game%.zip C:\codigo\blackbox\data\%game%.zip
@echo ----------------------------------------------------------------------------
@echo Finished Packing %game%
@echo ----------------------------------------------------------------------------
cd ..