BlackBox

BlackBox Engine Core
Use ESC to open the console. 

This black box has Py powers, being more accurate, version 3.8.5.

alias                    Set given alias for a command line
aliases                  Show list of current aliases
addtoggle                Adds a toggle with given id and name
concat                   Concatenate given arguments and store the value in $result
cd                       Changes active working directory
cls                      Clear console buffer   
cwd                      Shows active working directory
divide                   Divide given operators and store the value in $result
dir                      Shows files and folders in current working directory
dump                     Dump console buffer into given filename
echo                     Dump given text on the console buffer
help                     Show help about requested command
multiply                 Multiply given operators and store the value in $result
memdump                  Dump heap blocks to the filesystem
subtract                 Subtract given operators and store the value in $result
sum                      Sum given operators and store the value in $result
set                      Set or view value of given variable
toggle                   Set or toggle given in-game toggle
toggleconsole            Open or close the console interface
togglelist               Show list of current in-game toggles
unalias                  Unset given alias      
unset                    Unset given variable   
unsetall                 Unsets every variable set
vars                     Show list of defined variables
view                     Dump given filename over the console
writeconfig              Stores variable settings under given filename
xor                      Apply XOR over given value and store value in $result
-----------------------------------------------
 SCRIPTING API. Use help <term> for details
-----------------------------------------------
cls                      Clean console buffer   
createsurf               Returns Surface object to be drawn arbitrarily to screen
deletesurf               Delete given Surface   
disable                  Toggle off given vpu layer
enable                   Toggle on  given vpu layer
epoch                    Return current engine epoch uptime
fadein                   Fade screen from black 
fadeout                  Fade screen to black   
frames                   Return actual frame count
fullscreen               Toggle fullscreen mode 
print                    Print given text at given coordinates
restart                  Restart Video Processing Unit
rotate                   Rotate specified layer (0-11) given degrees
scale                    Change specified layer [0-11] given horizontal and vertical scale factor
select                   Select given layer to perform next graphic operations onto it
setrotation              Sets rotation for specified layer (0-11) at given degrees
setscale                 Sets scale for specified layer [0-11] given horizontal and vertical values
update                   Allow blackbox engine to perform its rendering based input and output operations
version                  Return current BlackBox engine version
</pre>
