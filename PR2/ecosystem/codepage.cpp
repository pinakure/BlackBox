const char *codepageName[] = { 	
	//SCANCODE		ASCII CODES		SHIFT ASCII			ALTGR ASCII
	"KB_TAB"		,"32"				,"32"				,"32"			,
	"KB_A"			,"ASCII_a"			,"ASCII_A"			,"ASCII_a"		,
	"KB_B"			,"ASCII_b"			,"ASCII_B"			,"ASCII_b"		,
	"KB_C"			,"ASCII_c"			,"ASCII_C"			,"ASCII_c"		,
	"KB_D"			,"ASCII_d"			,"ASCII_D"			,"ASCII_d"		,
	"KB_E"			,"ASCII_e"			,"ASCII_E"			,"ASCII_e"		,
	"KB_F"			,"ASCII_f"			,"ASCII_F"			,"ASCII_f"		,
	"KB_G"			,"ASCII_g"			,"ASCII_G"			,"ASCII_g"		,
	"KB_H"			,"ASCII_h"			,"ASCII_H"			,"ASCII_h"		,
	"KB_I"			,"ASCII_i"			,"ASCII_I"			,"ASCII_i"		,
	"KB_J"			,"ASCII_j"			,"ASCII_J"			,"ASCII_j"		,
	"KB_K"			,"ASCII_k"			,"ASCII_K"			,"ASCII_k"		,
	"KB_L"			,"ASCII_l"			,"ASCII_L"			,"ASCII_l"		,
	"KB_M"			,"ASCII_m"			,"ASCII_M"			,"ASCII_m"		,
	"KB_N"			,"ASCII_n"			,"ASCII_N"			,"ASCII_n"		,
	"KB_O"			,"ASCII_o"			,"ASCII_O"			,"ASCII_o"		,
	"KB_P"			,"ASCII_p"			,"ASCII_P"			,"ASCII_p"		,
	"KB_Q"			,"ASCII_q"			,"ASCII_Q"			,"ASCII_q"		,
	"KB_R"			,"ASCII_r"			,"ASCII_R"			,"ASCII_r"		,
	"KB_S"			,"ASCII_s"			,"ASCII_S"			,"ASCII_s"		,
	"KB_T"			,"ASCII_t"			,"ASCII_T"			,"ASCII_t"		,
	"KB_U"			,"ASCII_u"			,"ASCII_U"			,"ASCII_u"		,
	"KB_V"			,"ASCII_v"			,"ASCII_V"			,"ASCII_v"		,
	"KB_W"			,"ASCII_w"			,"ASCII_W"			,"ASCII_w"		,
	"KB_X"			,"ASCII_x"			,"ASCII_X"			,"ASCII_x"		,
	"KB_Y"			,"ASCII_y"			,"ASCII_Y"			,"ASCII_y"		,
	"KB_Z"			,"ASCII_z"			,"ASCII_Z"			,"ASCII_z"		,
	"KB_0"			,"ASCII_0"			,"ASCII_EQUALS"		,"ASCII_0"		,
	"KB_1"			,"ASCII_1"			,"ASCII_EXCLAMATION","ASCII_PIPE"	,
	"KB_2"			,"ASCII_2"			,"ASCII_QUOTE"		,"ASCII_AT"		,
	"KB_3"			,"ASCII_3"			,"ASCII_DOT"		,"ASCII_HASH"	,
	"KB_4"			,"ASCII_4"			,"ASCII_DOLLAR"		,"ASCII_CAGADILLA",
	"KB_5"			,"ASCII_5"			,"ASCII_PERCENT"	,"ASCII_5"		,
	"KB_6"			,"ASCII_6"			,"ASCII_AMPERSAND"	,"ASCII_BENDPIPE",
	"KB_7"			,"ASCII_7"			,"ASCII_SLASH"		,"ASCII_7"		,
	"KB_8"			,"ASCII_8"			,"ASCII_LPARENTHESIS","ASCII_8"		,
	"KB_9"			,"ASCII_9"			,"ASCII_RPARENTHESIS","ASCII_9"		,
	"KB_APOS"		,"ASCII_SINGLEQUOTE","ASCII_QUESTION"	,"ASCII_SINGLEQUOTE",
	"KB_EXCLAM"		,"ASCII_IEXCLAMATION","ASCII_IQUESTION"	,"ASCII_IEXCLAMATION",
	"KB_CONSOLE"	,"ASCII_NUM"		,"ASCII_ORD"		,"ASCII_BACKSLASH",
	"KB_COMMA"		,"ASCII_COMMA"		,"ASCII_SEMICOLON"	,"ASCII_COMMA"	,
	"KB_PERIOD"		,"ASCII_PERIOD"		,"ASCII_COLON"		,"ASCII_PERIOD"	,
	"KB_MINUS"		,"ASCII_MINUS"		,"ASCII_UNDERSCORE"	,"ASCII_MINUS"	,
	"KB_KPMINUS"	,"ASCII_MINUS"		,"ASCII_MINUS"		,"ASCII_MINUS"	,
	"KB_KPPLUS"		,"ASCII_PLUS"		,"ASCII_PLUS"		,"ASCII_PLUS"	,
	"KB_LBRACKET"	,"ASCII_ACCENT"		,"ASCII_CIRCUNFLEX"	,"ASCII_LBRACKET",
	"KB_RBRACKET"	,"ASCII_PLUS"		,"ASCII_ASTERISK"	,"ASCII_RBRACKET",
	"KB_LKEY"		,"ASCII_LKEY"		,"ASCII_LKEY"		,"ASCII_LKEY"	,
	"KB_RKEY"		,"ASCII_cedilla"	,"ASCII_CEDILLA"	,"ASCII_RKEY"	,
	"KB_ENYE"		,"ASCII_enye"		,"ASCII_ENYE"		,"ASCII_enye"	,
	"KB_BACKSPACE"	,"8"				,"8"				,"8"			,
	"KB_ENTER"		,"13"				,"13"				,"13"			,
	"KB_ESC"		,"27"				,"27"				,"27"			,
	"KB_SPACEBAR"	,"32"				,"32"				,"32"			,	
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	NULL};

const char *keyNames[] = { 	
	"KB_TAB"		,"A"				,"B"				,"C"			,
	"D"				,"E"				,"F"				,"G"			,
	"H"				,"I"				,"J"				,"K"			,
	"L"				,"M"				,"N"				,"O"			,
	"P"				,"Q"				,"R"				,"S"			,
	"T"				,"U"				,"V"				,"W"			,
	"X"				,"Y"				,"Z"				,"0"			,
	"1"				,"2"				,"3"				,"4"			,
	"5"				,"6"				,"7"				,"8"			,
	"9"				,"KP_0"				,"KP_1"				,"KP_2"			,
	"KP_3"			,"KP_4"				,"KP_5"				,"KP_6"			,
	"KP_7"			,"KP_8"				,"KP_9"				,"F1"			,
	"F2"			,"F3"				,"F4"				,"F5"			,
	"F6"			,"F7"				,"F8"				,"F9"			,
	"F10"			,"F11"				,"F12"				,"ESC"			,
	"BACKSLASH"		,"MINUS"			,"ASCII_O"			,"BACKSPACE"	,
	"TAB"			,"ENTER"			,"ASCII_P"			,"ASCII_p"		,
	"COLON"			,"ASCII_q"			,"ASCII_Q"			,"LT"			,
	"COMMA"			,"ASCII_r"			,"SLASH"			,"SPACE"		,
	"INSERT"		,"DELETE"			,"HOME"				,"END"			,
	"PAGEUP"		,"PAGEDOWN"			,"LEFT"				,"RIGHT"		,
	"UP"			,"DOWN"				,"KP_SLASH"			,"ASTERISK"		,
	"KP_MINUS"		,"KP_PLUS"			,"KP_DELETE"		,"KP_ENTER"		,
	"PRINT"			,"PAUSE"			,"ASCII_W"			,"ASCII_w"		,
	"KB_X"			,"ASCII_x"			,"ASCII_X"			,"ASCII_x"		,
	"KB_Y"			,"ASCII_y"			,"ASCII_Y"			,"ASCII_y"		,
	"KB_Z"			,"SEMICOLON"		,"ASCII_Z"			,"ASCII_z"		,
	"KB_0"			,"ASCII_0"			,"ASCII_EQUALS"		,"ASCII_0"		,
	"KB_1"			,"ASCII_1"			,"ASCII_EXCLAMATION","SHIFT"		,
	"RSHIFT"		,"CONTROL"			,"RCONTROL"			,"ASCII_AT"		,
	"KB_3"			,"ASCII_3"			,"ASCII_DOT"		,"MENU"			,
	"SCROLL"		,"NUM"				,"CAPS"				,"ASCII_CAGADILLA",
	"KB_5"			,"ASCII_5"			,"ASCII_PERCENT"	,"ASCII_5"		,
	"KB_6"			,"ASCII_6"			,"ASCII_AMPERSAND"	,"ASCII_BENDPIPE",
	"KB_7"			,"ASCII_7"			,"ASCII_SLASH"		,"ASCII_7"		,
	"KB_8"			,"ASCII_8"			,"ASCII_LPARENTHESIS","ASCII_8"		,
	"KB_9"			,"ASCII_9"			,"ASCII_RPARENTHESIS","ASCII_9"		,
	"KB_APOS"		,"ASCII_SINGLEQUOTE","ASCII_QUESTION"	,"ASCII_SINGLEQUOTE",
	"KB_EXCLAM"		,"ASCII_IEXCLAMATION","ASCII_IQUESTION"	,"ASCII_IEXCLAMATION",
	"KB_CONSOLE"	,"ASCII_NUM"		,"ASCII_ORD"		,"ASCII_BACKSLASH",
	"KB_COMMA"		,"ASCII_COMMA"		,"ASCII_SEMICOLON"	,"ASCII_COMMA"	,
	"KB_PERIOD"		,"ASCII_PERIOD"		,"ASCII_COLON"		,"ASCII_PERIOD"	,
	"KB_MINUS"		,"ASCII_MINUS"		,"ASCII_UNDERSCORE"	,"ASCII_MINUS"	,
	"KB_KPMINUS"	,"ASCII_MINUS"		,"ASCII_MINUS"		,"ASCII_MINUS"	,
	"KB_KPPLUS"		,"ASCII_PLUS"		,"ASCII_PLUS"		,"ASCII_PLUS"	,
	"KB_LBRACKET"	,"ASCII_ACCENT"		,"ASCII_CIRCUNFLEX"	,"ASCII_LBRACKET",
	"KB_RBRACKET"	,"ASCII_PLUS"		,"ASCII_ASTERISK"	,"ASCII_RBRACKET",
	"KB_LKEY"		,"ASCII_LKEY"		,"ASCII_LKEY"		,"ASCII_LKEY"	,
	"KB_RKEY"		,"ASCII_cedilla"	,"ASCII_CEDILLA"	,"ASCII_RKEY"	,
	"KB_ENYE"		,"ASCII_enye"		,"ASCII_ENYE"		,"ASCII_enye"	,
	"KB_BACKSPACE"	,"8"				,"8"				,"8"			,
	"KB_ENTER"		,"13"				,"13"				,"13"			,
	"KB_ESC"		,"27"				,"27"				,"27"			,
	"KB_SPACEBAR"	,"32"				,"32"				,"32"			,	
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	NULL};
#define ASCII_SPACE			32
#define ASCII_EXCLAM		33
#define ASCII_DOUBLEQUOTE	34
#define ASCII_HASH			35
#define ASCII_DOLLAR		36
#define ASCII_PERCENT		37
#define ASCII_AMPERSAND		38
#define ASCII_QUOTE			39
#define ASCII_LPARENTHESIS	40
#define ASCII_RPARENTHESIS	41
#define ASCII_ASTERISK		42
#define ASCII_PLUS			43
#define ASCII_COMMA			44
#define ASCII_SCORE			45
#define ASCII_DOT			46
#define ASCII_SLASH			47
#define ASCII_0				48
#define ASCII_1				49
#define ASCII_2				50
#define ASCII_3				51
#define ASCII_4				52
#define ASCII_5				53
#define ASCII_6				54
#define ASCII_7				55
#define ASCII_8				56
#define ASCII_9				57
#define ASCII_COLON			58
#define ASCII_SEMICOLON		59
#define ASCII_LESSER		60
#define ASCII_EQUALS		61
#define ASCII_GREATER		62
#define ASCII_QUESTION		63
#define ASCII_AT			64
#define ASCII_A				65
#define ASCII_B				66
#define ASCII_C				67
#define ASCII_D				68
#define ASCII_E				69
#define ASCII_F				70
#define ASCII_G				71
#define ASCII_H				72
#define ASCII_I				73
#define ASCII_J				74
#define ASCII_K				75
#define ASCII_L				76
#define ASCII_M				77
#define ASCII_N				78
#define ASCII_O				79
#define ASCII_P				80
#define ASCII_Q				81
#define ASCII_R				82
#define ASCII_S				83
#define ASCII_T				84
#define ASCII_U				85
#define ASCII_V				86
#define ASCII_W				87
#define ASCII_X				88
#define ASCII_Y				89
#define ASCII_Z				90
#define ASCII_LBRACKET		91
#define ASCII_BACKSLACK		92
#define ASCII_RBRACKET		93
#define ASCII_CIRCUMFLEX	94
#define ASCII_UNDERSCORE	95
#define ASCII_APOS			96
#define ASCII_a				97
#define ASCII_b				98
#define ASCII_c				99
#define ASCII_d				100
#define ASCII_e				101
#define ASCII_f				102
#define ASCII_g				103
#define ASCII_h				104
#define ASCII_i				105
#define ASCII_j				106
#define ASCII_k				107
#define ASCII_l				108
#define ASCII_m				109
#define ASCII_n				110
#define ASCII_o				111
#define ASCII_p				112
#define ASCII_q				113
#define ASCII_r				114
#define ASCII_s				115
#define ASCII_t				116
#define ASCII_u				117
#define ASCII_v				118
#define ASCII_w				119
#define ASCII_x				120
#define ASCII_y				121
#define ASCII_z				122
#define ASCII_LCURLY		123
#define ASCII_PIPE			124
#define ASCII_RCURLY		125
#define ASCII_TILDE			126
#define ASCII_DELETE		127

int codepage[] = { 	
	//   SCANCODE		ASCII CODES			SHIFT ASCII			ALTGR ASCII
	ALLEGRO_KEY_TAB,		32,					32,					32,
	ALLEGRO_KEY_A,    	ASCII_a,			ASCII_A,			ASCII_a,
	ALLEGRO_KEY_B, 		ASCII_b,			ASCII_B,			ASCII_b,
	ALLEGRO_KEY_C, 		ASCII_c,			ASCII_C,			ASCII_c,
	ALLEGRO_KEY_D, 		ASCII_d,			ASCII_D,			ASCII_d,
	ALLEGRO_KEY_E, 		ASCII_e,			ASCII_E,			ASCII_e,
	ALLEGRO_KEY_F, 		ASCII_f,			ASCII_F,			ASCII_f,
	ALLEGRO_KEY_G, 		ASCII_g,			ASCII_G,			ASCII_g,
	ALLEGRO_KEY_H, 		ASCII_h,			ASCII_H,			ASCII_h,
	ALLEGRO_KEY_I, 		ASCII_i,			ASCII_I,			ASCII_i,
	ALLEGRO_KEY_J, 		ASCII_j,			ASCII_J,			ASCII_j,
	ALLEGRO_KEY_K, 		ASCII_k,			ASCII_K,			ASCII_k,
	ALLEGRO_KEY_L, 		ASCII_l,			ASCII_L,			ASCII_l,
	ALLEGRO_KEY_M, 		ASCII_m,			ASCII_M,			ASCII_m,
	ALLEGRO_KEY_N, 		ASCII_n,			ASCII_N,			ASCII_n,
	ALLEGRO_KEY_O, 		ASCII_o,			ASCII_O,			ASCII_o,
	ALLEGRO_KEY_P, 		ASCII_p,			ASCII_P,			ASCII_p,
	ALLEGRO_KEY_Q, 		ASCII_q,			ASCII_Q,			ASCII_q,
	ALLEGRO_KEY_R, 		ASCII_r,			ASCII_R,			ASCII_r,
	ALLEGRO_KEY_S, 		ASCII_s,			ASCII_S,			ASCII_s,
	ALLEGRO_KEY_T, 		ASCII_t,			ASCII_T,			ASCII_t,
	ALLEGRO_KEY_U, 		ASCII_u,			ASCII_U,			ASCII_u,
	ALLEGRO_KEY_V, 		ASCII_v,			ASCII_V,			ASCII_v,
	ALLEGRO_KEY_W, 		ASCII_w,			ASCII_W,			ASCII_w,
	ALLEGRO_KEY_X, 		ASCII_x,			ASCII_X,			ASCII_x,
	ALLEGRO_KEY_Y, 		ASCII_y,			ASCII_Y,			ASCII_y,
	ALLEGRO_KEY_Z, 		ASCII_z,			ASCII_Z,			ASCII_z,
	ALLEGRO_KEY_0,		ASCII_0,			ASCII_EQUALS,		ASCII_0,
	ALLEGRO_KEY_1,		ASCII_1,			ASCII_EXCLAM,		ASCII_PIPE,
	ALLEGRO_KEY_2,		ASCII_2,			ASCII_QUOTE,		ASCII_AT,
	ALLEGRO_KEY_3,		ASCII_3,			ASCII_DOT,			ASCII_HASH,
	ALLEGRO_KEY_4,		ASCII_4,			ASCII_DOLLAR,		ASCII_TILDE,
	ALLEGRO_KEY_5,		ASCII_5,			ASCII_PERCENT,		ASCII_5,
	ALLEGRO_KEY_6,		ASCII_6,			ASCII_AMPERSAND,	ASCII_6,//ascii_bendpipe
	ALLEGRO_KEY_7,		ASCII_7,			ASCII_SLASH,		ASCII_7,
	ALLEGRO_KEY_8,		ASCII_8,			ASCII_LPARENTHESIS,	ASCII_8,
	ALLEGRO_KEY_9,		ASCII_9,			ASCII_RPARENTHESIS,	ASCII_9,
	ALLEGRO_KEY_MINUS,	ASCII_SCORE,		ASCII_SCORE,		ASCII_SCORE,
	ALLEGRO_KEY_MINUS,	ASCII_SCORE,		ASCII_UNDERSCORE,	ASCII_SCORE,
	ALLEGRO_KEY_1,		ASCII_EXCLAM,		ASCII_QUESTION,		ASCII_EXCLAM,
	ALLEGRO_KEY_COMMA,	ASCII_COMMA,		ASCII_SEMICOLON,	ASCII_COMMA,
	ALLEGRO_KEY_BACKSPACE,	8,					8, 					8,
	ALLEGRO_KEY_ENTER, 		13,					13, 				13,
	ALLEGRO_KEY_ESCAPE,		27,					27, 				27,
	ALLEGRO_KEY_SPACE,		32,					32,					32,	
	ALLEGRO_KEY_OPENBRACE,ASCII_QUOTE,		ASCII_CIRCUMFLEX,	ASCII_LBRACKET,
	ALLEGRO_KEY_CLOSEBRACE,ASCII_PLUS,		ASCII_ASTERISK,		ASCII_RBRACKET,
	ALLEGRO_KEY_FULLSTOP,	ASCII_DOT,			ASCII_COLON,		ASCII_DOT,
	/*
	ALLEGRO_KEY_PLUS,	ASCII_PLUS,			ASCII_PLUS,			ASCII_PLUS,
	ALLEGRO_KEY_APOS,	ASCII_QUOTE,		ASCII_QUESTION,		ASCII_QUOTE,
	ALLEGRO_KEY_CONSOLE,ASCII_NUM,			ASCII_ORD,			ASCII_BACKSLASH,
	ALLEGRO_KEY_LKEY,	ASCII_LKEY,			ASCII_LKEY,			ASCII_LKEY,
	ALLEGRO_KEY_RKEY,	ASCII_cedilla,		ASCII_CEDILLA,		ASCII_RKEY,
	ALLEGRO_KEY_ENYE,	ASCII_enye,			ASCII_ENYE,			ASCII_enye,
	*/
	/* new entries not below this line, terminator 0,0,0,0 MUST BE LAST DEFINED ENTRY*/
	0,			0,					0,					0
};

