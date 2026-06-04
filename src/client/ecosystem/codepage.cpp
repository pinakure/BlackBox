#include "keymap.h"

#define KEYMAP(allegro_key, ascii, shift, ctrl, altgr, alt) #allegro_key,#ascii,#shift,#altgr,
const char *codepageName[] = { 	
	KEYMAPPING
	"KB_BACKSPACE"	,"8"				,"8"				,"8"			,
	"KB_ENTER"		,"13"				,"13"				,"13"			,
	"KB_ESC"		,"27"				,"27"				,"27"			,
	"KB_SPACEBAR"	,"32"				,"32"				,"32"			,	
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	NULL};
#undef KEYMAP

#define KEYMAP(allegro_key, ascii, shift, ctrl, altgr, alt) #allegro_key,
const char *keyNames[] = { 	
	KEYMAPPING
	"KB_BACKSPACE"	,"8"				,"8"				,"8"			,
	"KB_ENTER"		,"13"				,"13"				,"13"			,
	"KB_ESC"		,"27"				,"27"				,"27"			,
	"KB_SPACEBAR"	,"32"				,"32"				,"32"			,	
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	NULL};
#undef KEYMAP

#define KEYMAP(allegro_key, ascii, shift, ctrl, altgr, alt) allegro_key, ascii, shift, altgr,
int codepage[] = { 	
	KEYMAPPING
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	0,			0,					0,					0
};
#undef KEYMAP

