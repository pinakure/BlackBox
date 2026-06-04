#pragma once // Evita inclusiones duplicadas
#include <allegro5/allegro.h>
#define ASCII_NULL					0	
#define ASCII_START_HEADING			1
#define ASCII_START_TEXT			2
#define ASCII_END_TEXT				3
#define ASCII_END_TRANSMISSION		4
#define ASCII_ENQUIRY				5
#define ASCII_ACKNOWLEDGMENT		6
#define ASCII_BELL					7
#define ASCII_BACKSPACE				8
#define ASCII_HORIZONTAL_TAB		9
#define ASCII_LINE_FEED				10
#define ASCII_VERTICAL_TAB			11
#define ASCII_FORM_FEED				12
#define ASCII_CARRIAGE_RETURN		13
#define ASCII_SHIFT_OUT				14
#define ASCII_SHIFT_IN				15
#define ASCII_DATA_LINE_ESCAPE		16
#define ASCII_DEVICE_CONTROL_1		17
#define ASCII_DEVICE_CONTROL_2		18
#define ASCII_DEVICE_CONTROL_3		19
#define ASCII_DEVICE_CONTROL_4		20
#define ASCII_NEGATIVE_ACK			21
#define ASCII_SYNCHRONOUS_IDLE		22
#define ASCII_END_TRANSMIT_BLK		23
#define ASCII_CANCEL				24
#define ASCII_END_OF_MEDIUM			25
#define ASCII_SUBSTITUTE			26
#define ASCII_ESCAPE				27
#define ASCII_FILE_SEPARATOR		28
#define ASCII_GROUP_SEPARATOR		29
#define ASCII_RECORD_SEPARATOR		30
#define ASCII_UNIT_SEPARATOR		31
#define ASCII_SPACE					32 
#define ASCII_EXCLAM				33 
#define ASCII_DOUBLEQUOTE			34 
#define ASCII_HASH					35 
#define ASCII_DOLLAR				36 
#define ASCII_PERCENT				37 
#define ASCII_AMPERSAND				38 
#define ASCII_QUOTE					39 
#define ASCII_LPARENTHESIS			40 
#define ASCII_RPARENTHESIS			41 
#define ASCII_ASTERISK				42 
#define ASCII_PLUS					43 
#define ASCII_COMMA					44 
#define ASCII_SCORE					45 
#define ASCII_DOT					46 
#define ASCII_SLASH					47 
#define ASCII_0						48 
#define ASCII_1						49 
#define ASCII_2						50 
#define ASCII_3						51 
#define ASCII_4						52 
#define ASCII_5						53 
#define ASCII_6						54 
#define ASCII_7						55 
#define ASCII_8						56 
#define ASCII_9						57 
#define ASCII_COLON					58 
#define ASCII_SEMICOLON				59 
#define ASCII_LESSER				60 
#define ASCII_EQUALS				61 
#define ASCII_GREATER				62 
#define ASCII_QUESTION				63 
#define ASCII_AT					64 
#define ASCII_A						65 
#define ASCII_B						66 
#define ASCII_C						67 
#define ASCII_D						68 
#define ASCII_E						69 
#define ASCII_F						70 
#define ASCII_G						71 
#define ASCII_H						72 
#define ASCII_I						73 
#define ASCII_J						74 
#define ASCII_K						75 
#define ASCII_L						76 
#define ASCII_M						77 
#define ASCII_N						78 
#define ASCII_O						79 
#define ASCII_P						80 
#define ASCII_Q						81 
#define ASCII_R						82 
#define ASCII_S						83 
#define ASCII_T						84 
#define ASCII_U						85 
#define ASCII_V						86 
#define ASCII_W						87 
#define ASCII_X						88 
#define ASCII_Y						89 
#define ASCII_Z						90 
#define ASCII_LBRACKET				91 
#define ASCII_BACKSLASH				92 
#define ASCII_RBRACKET				93 
#define ASCII_CIRCUMFLEX			94 
#define ASCII_UNDERSCORE			95 
#define ASCII_APOS					96 
#define ASCII_a						97 
#define ASCII_b						98 
#define ASCII_c						99 
#define ASCII_d						100 
#define ASCII_e						101	
#define ASCII_f						102	
#define ASCII_g						103	
#define ASCII_h						104	
#define ASCII_i						105	
#define ASCII_j						106	
#define ASCII_k						107	
#define ASCII_l						108	
#define ASCII_m						109	
#define ASCII_n						110	
#define ASCII_o						111	
#define ASCII_p						112	
#define ASCII_q						113	
#define ASCII_r						114	
#define ASCII_s						115	
#define ASCII_t						116	
#define ASCII_u						117	
#define ASCII_v						118	
#define ASCII_w						119	
#define ASCII_x						120	
#define ASCII_y						121	
#define ASCII_z						122	
#define ASCII_LCURLY				123	
#define ASCII_PIPE					124	
#define ASCII_RCURLY				125	
#define ASCII_TILDE					126	
#define ASCII_DELETE				127 
#define ASCII_EURO					128	
#define ASCII_129  	 				129	
#define ASCII_SINGLE_LOW9_QUOTE		130	
#define ASCII_FUNCTION				131	
#define ASCII_DOUBLE_LOW9_QUOTE		132	
#define ASCII_H_ELLIPSIS			133	
#define ASCII_DAGGER				134	
#define ASCII_DOUBLE_DAGGER			135	
#define ASCII_SMALL_CIRCUMFLEX		136	
#define ASCII_PER_MILLE				137	
#define ASCII_S_CARON				138	
#define ASCII_LEFT_ANGLE			139	
#define ASCII_OE					140	
#define ASCII_141					141 
#define ASCII_Z_CARON				142	
#define ASCII_143					143 
#define ASCII_144					144 
#define ASCII_QUOTE_LEFT			145	
#define ASCII_QUOTE_RIGHT			146	
#define ASCII_DBLQUOTE_LEFT			147	
#define ASCII_DBLQUOTE_RIGHT		148	
#define ASCII_BULLET				149	
#define ASCII_EN_DASH				150	
#define ASCII_EM_DASH				151	
#define ASCII_SMALL_TILDE			152	
#define ASCII_TRADEMARK				153	
#define ASCII_s_CARON				154	
#define ASCII_ANGLE_RIGHT			155	
#define ASCII_oe					156	
#define ASCII_157					157 
#define ASCII_z_CARON				158	
#define ASCII_Y_DIAERESIS			159	
#define ASCII_NON_BREAKING_SPC		160	
#define ASCII_INVERSE_EXCLAM		161	
#define ASCII_CENT					162	
#define ASCII_POUND					163	
#define ASCII_CURRENCY				164	
#define ASCII_YEN					165	
#define ASCII_SPLIT_PIPE			166	
#define ASCII_SECTION				167	
#define ASCII_UMLAUT				168	
#define ASCII_COPYRIGHT				169	
#define ASCII_FEMININE_ORDINAL		170	
#define ASCII_ANGLE_DOUBLE_LEFT		171	
#define ASCII_OT					172 
#define ASCII_SOFT_HYPHEN			173	
#define ASCII_REGISTERED			174	
#define ASCII_SPACING_MACRON		175	
#define ASCII_DEGREE				176	
#define ASCII_PLUS_OR_MINUS			177	
#define ASCII_SUPER_2				178	
#define ASCII_SUPER_3				179	
#define ASCII_ACUTE_ACCENT			180	
#define ASCII_MICRO					181	
#define ASCII_PILCROW				182	
#define ASCII_MIDDLE_DOT			183	
#define ASCII_SPACING_CEDILLA		184	
#define ASCII_SUPER_1				185	
#define ASCII_MASCULINE_ORDINAL		186	
#define ASCII_ANGLE_DOUBLE_RIGHT	187 
#define ASCII_QUARTER				188	
#define ASCII_HALF					189	
#define ASCII_THREE_QUARTERS		190	
#define ASCII_INVERT_QUESTION		191	
#define ASCII_A_GRAVE				192	
#define ASCII_A_ACUTE				193	
#define ASCII_A_CIRCUMFLEX			194	
#define ASCII_A_TILDE				195	
#define ASCII_A_DIAERESIS			196	
#define ASCII_A_RING				197	
#define ASCII_AE					198	
#define ASCII_CEDILLA				199	
#define ASCII_E_GRAVE				200	
#define ASCII_E_ACUTE				201	
#define ASCII_E_CIRCUMFLEX			202	
#define ASCII_E_DIAERESIS			203	
#define ASCII_I_GRAVE				204	
#define ASCII_I_ACUTE				205	
#define ASCII_I_CIRCUMFLEX			206	
#define ASCII_I_DIAERESIS			207	
#define ASCII_ETH					208	
#define ASCII_N_TILDE				209	
#define ASCII_O_GRAVE				210	
#define ASCII_O_ACUTE				211	
#define ASCII_O_CIRCUMFLEX			212	
#define ASCII_O_TILDE				213	
#define ASCII_O_DIAERESIS			214	
#define ASCII_MULTIPLICATION		215	
#define ASCII_O_SLASH				216	
#define ASCII_U_GRAVE				217	
#define ASCII_U_ACUTE				218	
#define ASCII_U_CIRCUMFLEX			219	
#define ASCII_U_DIAERESIS			220	
#define ASCII_Y_ACUTE				221	
#define ASCII_THORN					222	
#define ASCII_SHARP_S				223	
#define ASCII_a_GRAVE				224	
#define ASCII_a_ACUTE				225	
#define ASCII_a_CIRCUMFLEX			226	
#define ASCII_a_TILDE				227	
#define ASCII_a_DIAERESIS			228	
#define ASCII_a_RING				229	
#define ASCII_ae					230	
#define ASCII_cedilla				231	
#define ASCII_e_GRAVE				232	
#define ASCII_e_ACUTE				233	
#define ASCII_e_CIRCUMFLEX			234	
#define ASCII_e_DIAERESIS			235	
#define ASCII_i_GRAVE				236	
#define ASCII_i_ACUTE				237	
#define ASCII_i_CIRCUMFLEX			238	
#define ASCII_i_DIAERESIS			239	
#define ASCII_SMALL_ETH				240	
#define ASCII_n_TILDE				241	
#define ASCII_o_GRAVE				242	
#define ASCII_o_ACUTE				243	
#define ASCII_o_CIRCUMFLEX			244	
#define ASCII_o_TILDE				245	
#define ASCII_o_DIAERESIS			246	
#define ASCII_DIVISION				247	
#define ASCII_o_SLASH				248	
#define ASCII_u_GRAVE				249	
#define ASCII_u_ACUTE				250	
#define ASCII_u_CIRCUMFLEX			251	
#define ASCII_u_DIAERESIS			252	
#define ASCII_y_ACUTE				253	
#define ASCII_SMALL_THORN			254	
#define ASCII_y_DIAERESIS			255	

#define CODEPAGE \
	KEY(ASCII_NULL					, " "	, "NULL"				)\
	KEY(ASCII_START_HEADING			, " "	, "START_HEADING"		)\
	KEY(ASCII_START_TEXT			, " "	, "START_TEXT"			)\
	KEY(ASCII_END_TEXT				, " "	, "END_TEXT"			)\
	KEY(ASCII_END_TRANSMISSION		, " "	, "END_TRANSMISSION"	)\
	KEY(ASCII_ENQUIRY				, " "	, "ENQUIRY"				)\
	KEY(ASCII_ACKNOWLEDGMENT		, " "	, "ACKNOWLEDGMENT"		)\
	KEY(ASCII_BELL					, " "	, "BELL"				)\
	KEY(ASCII_BACKSPACE				, " "	, "BACKSPACE"			)\
	KEY(ASCII_HORIZONTAL_TAB		, " "	, "HORIZONTAL_TAB"		)\
	KEY(ASCII_LINE_FEED				, " "	, "LINE_FEED"			)\
	KEY(ASCII_VERTICAL_TAB			, " "	, "VERTICAL_TAB"		)\
	KEY(ASCII_FORM_FEED				, " "	, "FORM_FEED"			)\
	KEY(ASCII_CARRIAGE_RETURN		, " "	, "CARRIAGE_RETURN"		)\
	KEY(ASCII_SHIFT_OUT				, " "	, "SHIFT_OUT"			)\
	KEY(ASCII_SHIFT_IN				, " "	, "SHIFT_IN"			)\
	KEY(ASCII_DATA_LINE_ESCAPE		, " "	, "DATA_LINE_ESCAPE"	)\
	KEY(ASCII_DEVICE_CONTROL_1		, " "	, "DEVICE_CONTROL_1"	)\
	KEY(ASCII_DEVICE_CONTROL_2		, " "	, "DEVICE_CONTROL_2"	)\
	KEY(ASCII_DEVICE_CONTROL_3		, " "	, "DEVICE_CONTROL_3"	)\
	KEY(ASCII_DEVICE_CONTROL_4		, " "	, "DEVICE_CONTROL_4"	)\
	KEY(ASCII_NEGATIVE_ACK			, " "	, "NEGATIVE_ACK"		)\
	KEY(ASCII_SYNCHRONOUS_IDLE		, " "	, "SYNCHRONOUS_IDLE"	)\
	KEY(ASCII_END_TRANSMIT_BLK		, " "	, "END_TRANSMIT_BLK"	)\
	KEY(ASCII_CANCEL				, " "	, "CANCEL"				)\
	KEY(ASCII_END_OF_MEDIUM			, " "	, "END_OF_MEDIUM"		)\
	KEY(ASCII_SUBSTITUTE			, " "	, "SUBSTITUTE"			)\
	KEY(ASCII_ESCAPE				, " "	, "ESCAPE"				)\
	KEY(ASCII_FILE_SEPARATOR		, " "	, "FILE_SEPARATOR"		)\
	KEY(ASCII_GROUP_SEPARATOR		, " "	, "GROUP_SEPARATOR"		)\
	KEY(ASCII_RECORD_SEPARATOR		, " "	, "RECORD_SEPARATOR"	)\
	KEY(ASCII_UNIT_SEPARATOR		, " "	, "UNIT_SEPARATOR"		)\
	KEY(ASCII_SPACE					, " "	, "SPACE"				)\
	KEY(ASCII_EXCLAM				, "!"	, "EXCLAM"				)\
	KEY(ASCII_DOUBLEQUOTE			, "\""	, "DOUBLEQUOTE"			)\
	KEY(ASCII_HASH					, "#"	, "HASH"				)\
	KEY(ASCII_DOLLAR				, "$"	, "DOLLAR"				)\
	KEY(ASCII_PERCENT				, "%"	, "PERCENT"				)\
	KEY(ASCII_AMPERSAND				, "&"	, "AMPERSAND"			)\
	KEY(ASCII_QUOTE					, "'"	, "QUOTE"				)\
	KEY(ASCII_LPARENTHESIS			, "("	, "LPARENTHESIS"		)\
	KEY(ASCII_RPARENTHESIS			, ")"	, "RPARENTHESIS"		)\
	KEY(ASCII_ASTERISK				, "*"	, "ASTERISK"			)\
	KEY(ASCII_PLUS					, "+"	, "PLUS"				)\
	KEY(ASCII_COMMA					, ","	, "COMMA"				)\
	KEY(ASCII_SCORE					, "-"	, "SCORE"				)\
	KEY(ASCII_DOT					, "."	, "DOT"					)\
	KEY(ASCII_SLASH					, "/"	, "SLASH"				)\
	KEY(ASCII_0						, "0"	, "0"					)\
	KEY(ASCII_1						, "1"	, "1"					)\
	KEY(ASCII_2						, "2"	, "2"					)\
	KEY(ASCII_3						, "3"	, "3"					)\
	KEY(ASCII_4						, "4"	, "4"					)\
	KEY(ASCII_5						, "5"	, "5"					)\
	KEY(ASCII_6						, "6"	, "6"					)\
	KEY(ASCII_7						, "7"	, "7"					)\
	KEY(ASCII_8						, "8"	, "8"					)\
	KEY(ASCII_9						, "9"	, "9"					)\
	KEY(ASCII_COLON					, ":"	, "COLON"				)\
	KEY(ASCII_SEMICOLON				, ";"	, "SEMICOLON"			)\
	KEY(ASCII_LESSER				, "<"	, "LESSER"				)\
	KEY(ASCII_EQUALS				, "="	, "EQUALS"				)\
	KEY(ASCII_GREATER				, ">"	, "GREATER"				)\
	KEY(ASCII_QUESTION				, "?"	, "QUESTION"			)\
	KEY(ASCII_AT					, "@"	, "AT"					)\
	KEY(ASCII_A						, "A"	, "A"					)\
	KEY(ASCII_B						, "B"	, "B"					)\
	KEY(ASCII_C						, "C"	, "C"					)\
	KEY(ASCII_D						, "D"	, "D"					)\
	KEY(ASCII_E						, "E"	, "E"					)\
	KEY(ASCII_F						, "F"	, "F"					)\
	KEY(ASCII_G						, "G"	, "G"					)\
	KEY(ASCII_H						, "H"	, "H"					)\
	KEY(ASCII_I						, "I"	, "I"					)\
	KEY(ASCII_J						, "J"	, "J"					)\
	KEY(ASCII_K						, "K"	, "K"					)\
	KEY(ASCII_L						, "L"	, "L"					)\
	KEY(ASCII_M						, "M"	, "M"					)\
	KEY(ASCII_N						, "N"	, "N"					)\
	KEY(ASCII_O						, "O"	, "O"					)\
	KEY(ASCII_P						, "P"	, "P"					)\
	KEY(ASCII_Q						, "Q"	, "Q"					)\
	KEY(ASCII_R						, "R"	, "R"					)\
	KEY(ASCII_S						, "S"	, "S"					)\
	KEY(ASCII_T						, "T"	, "T"					)\
	KEY(ASCII_U						, "U"	, "U"					)\
	KEY(ASCII_V						, "V"	, "V"					)\
	KEY(ASCII_W						, "W"	, "W"					)\
	KEY(ASCII_X						, "X"	, "X"					)\
	KEY(ASCII_Y						, "Y"	, "Y"					)\
	KEY(ASCII_Z						, "Z"	, "Z"					)\
	KEY(ASCII_LBRACKET				, "["	, "LBRACKET"			)\
	KEY(ASCII_BACKSLASH				, "\\"	, "BACKSLASH"			)\
	KEY(ASCII_RBRACKET				, "]"	, "RBRACKET"			)\
	KEY(ASCII_CIRCUMFLEX			, "^"	, "CIRCUMFLEX"			)\
	KEY(ASCII_UNDERSCORE			, "_"	, "UNDERSCORE"			)\
	KEY(ASCII_APOS					, "`"	, "APOS"				)\
	KEY(ASCII_a						, "a"	, "a"					)\
	KEY(ASCII_b						, "b"	, "b"					)\
	KEY(ASCII_c						, "c"	, "c"					)\
	KEY(ASCII_d						, "d"	, "d"					)\
	KEY(ASCII_e						, "e"	, "e"					)\
	KEY(ASCII_f						, "f"	, "f"					)\
	KEY(ASCII_g						, "g"	, "g"					)\
	KEY(ASCII_h						, "h"	, "h"					)\
	KEY(ASCII_i						, "i"	, "i"					)\
	KEY(ASCII_j						, "j"	, "j"					)\
	KEY(ASCII_k						, "k"	, "k"					)\
	KEY(ASCII_l						, "l"	, "l"					)\
	KEY(ASCII_m						, "m"	, "m"					)\
	KEY(ASCII_n						, "n"	, "n"					)\
	KEY(ASCII_o						, "o"	, "o"					)\
	KEY(ASCII_p						, "p"	, "p"					)\
	KEY(ASCII_q						, "q"	, "q"					)\
	KEY(ASCII_r						, "r"	, "r"					)\
	KEY(ASCII_s						, "s"	, "s"					)\
	KEY(ASCII_t						, "t"	, "t"					)\
	KEY(ASCII_u						, "u"	, "u"					)\
	KEY(ASCII_v						, "v"	, "v"					)\
	KEY(ASCII_w						, "w"	, "w"					)\
	KEY(ASCII_x						, "x"	, "x"					)\
	KEY(ASCII_y						, "y"	, "y"					)\
	KEY(ASCII_z						, "z"	, "z"					)\
	KEY(ASCII_LCURLY				, "{"	, "LCURLY"				)\
	KEY(ASCII_PIPE					, "|"	, "PIPE"				)\
	KEY(ASCII_RCURLY				, "}"	, "RCURLY"				)\
	KEY(ASCII_TILDE					, "~"	, "TILDE"				)\
	KEY(ASCII_DELETE				, " "	, "DELETE"				)\
	KEY(ASCII_EURO					, " "	, "EURO"				)\
	KEY(ASCII_129  	 				, " "	, "129"  	 			)\
	KEY(ASCII_SINGLE_LOW9_QUOTE		, "‚"	, "SINGLE_LOW9_QUOTE"	)\
	KEY(ASCII_FUNCTION				, "ƒ"	, "FUNCTION"			)\
	KEY(ASCII_DOUBLE_LOW9_QUOTE		, "„"	, "DOUBLE_LOW9_QUOTE"	)\
	KEY(ASCII_H_ELLIPSIS			, "…"	, "H_ELLIPSIS"			)\
	KEY(ASCII_DAGGER				, "†"	, "DAGGER"				)\
	KEY(ASCII_DOUBLE_DAGGER			, "‡"	, "DOUBLE_DAGGER"		)\
	KEY(ASCII_SMALL_CIRCUMFLEX		, "ˆ"	, "SMALL_CIRCUMFLEX"	)\
	KEY(ASCII_PER_MILLE				, "‰"	, "PER_MILLE"			)\
	KEY(ASCII_S_CARON				, "Š"	, "S_CARON"				)\
	KEY(ASCII_LEFT_ANGLE			, "‹"	, "LEFT_ANGLE"			)\
	KEY(ASCII_OE					, "Œ"	, "OE"					)\
	KEY(ASCII_141					, " "	, "141"					)\
	KEY(ASCII_Z_CARON				, "Ž"	, "Z_CARON"				)\
	KEY(ASCII_143					, " "	, "143"					)\
	KEY(ASCII_144					, " "	, "144"					)\
	KEY(ASCII_QUOTE_LEFT			, "‘"	, "QUOTE_LEFT"			)\
	KEY(ASCII_QUOTE_RIGHT			, "’"	, "QUOTE_RIGHT"			)\
	KEY(ASCII_DBLQUOTE_LEFT			, "“"	, "DBLQUOTE_LEFT"		)\
	KEY(ASCII_DBLQUOTE_RIGHT		, "”"	, "DBLQUOTE_RIGHT"		)\
	KEY(ASCII_BULLET				, "•"	, "BULLET"				)\
	KEY(ASCII_EN_DASH				, "–"	, "EN_DASH"				)\
	KEY(ASCII_EM_DASH				, "—"	, "EM_DASH"				)\
	KEY(ASCII_SMALL_TILDE			, "˜"	, "SMALL_TILDE"			)\
	KEY(ASCII_TRADEMARK				, "™"	, "TRADEMARK"			)\
	KEY(ASCII_s_CARON				, "š"	, "s_CARON"				)\
	KEY(ASCII_ANGLE_RIGHT			, "›"	, "ANGLE_RIGHT"			)\
	KEY(ASCII_oe					, "œ"	, "oe"					)\
	KEY(ASCII_157					, " "	, "157"					)\
	KEY(ASCII_z_CARON				, "ž"	, "z_CARON"				)\
	KEY(ASCII_Y_DIAERESIS			, "Ÿ"	, "Y_DIAERESIS"			)\
	KEY(ASCII_NON_BREAKING_SPC		, " "	, "NON_BREAKING_SPC"	)\
	KEY(ASCII_INVERSE_EXCLAM		, "¡"	, "INVERSE_EXAMATION"	)\
	KEY(ASCII_CENT					, "¢"	, "CENT"				)\
	KEY(ASCII_POUND					, "£"	, "POUND"				)\
	KEY(ASCII_CURRENCY				, "¤"	, "CURRENCY"			)\
	KEY(ASCII_YEN					, "¥"	, "YEN"					)\
	KEY(ASCII_SPLIT_PIPE			, "¦"	, "SPLIT_PIPE"			)\
	KEY(ASCII_SECTION				, "§"	, "SECTION"				)\
	KEY(ASCII_UMLAUT				, "¨"	, "UMLAUT"				)\
	KEY(ASCII_COPYRIGHT				, "©"	, "COPYRIGHT"			)\
	KEY(ASCII_FEMININE_ORDINAL		, "ª"	, "FEMININE_ORDINAL"	)\
	KEY(ASCII_ANGLE_DOUBLE_LEFT		, "«"	, "ANGLE_DOUBLE_LEFT"	)\
	KEY(ASCII_OT					, "¬"	, "OSAS"				)\
	KEY(ASCII_SOFT_HYPHEN			, " "	, "SOFT_HYPHEN"			)\
	KEY(ASCII_REGISTERED			, "®"	, "REGISTERED"			)\
	KEY(ASCII_SPACING_MACRON		, "¯"	, "SPACING_MACRON"		)\
	KEY(ASCII_DEGREE				, "°"	, "DEGREE"				)\
	KEY(ASCII_PLUS_OR_MINUS			, "±"	, "PLUS_OR_MINUS"		)\
	KEY(ASCII_SUPER_2				, "²"	, "SUPER_2"				)\
	KEY(ASCII_SUPER_3				, "³"	, "SUPER_3"				)\
	KEY(ASCII_ACUTE_ACCENT			, "´"	, "ACUTE_ACCENT"		)\
	KEY(ASCII_MICRO					, "µ"	, "MICRO"				)\
	KEY(ASCII_PILCROW				, "¶"	, "PILCROW"				)\
	KEY(ASCII_MIDDLE_DOT			, "·"	, "MIDDLE_DOT"			)\
	KEY(ASCII_SPACING_CEDILLA		, "¸"	, "SPACING_CEDILLA"		)\
	KEY(ASCII_SUPER_1				, "¹"	, "SUPER_1"				)\
	KEY(ASCII_MASCULINE_ORDINAL		, "º"	, "MASCULINE_ORDINAL"	)\
	KEY(ASCII_ANGLE_DOUBLE_RIGHT	, " "	, "ANGLE_DOUBLE_RIGHT"	)\
	KEY(ASCII_QUARTER				, "¼"	, "QUARTER"				)\
	KEY(ASCII_HALF					, "½"	, "HALF"				)\
	KEY(ASCII_THREE_QUARTERS		, "¾"	, "THREE_QUARTERS"		)\
	KEY(ASCII_INVERT_QUESTION		, "¿"	, "INVERT_QUESTION"		)\
	KEY(ASCII_A_GRAVE				, "À"	, "A_GRAVE"				)\
	KEY(ASCII_A_ACUTE				, "Á"	, "A_ACUTE"				)\
	KEY(ASCII_A_CIRCUMFLEX			, "Â"	, "A_CIRCUMFLEX"		)\
	KEY(ASCII_A_TILDE				, "Ã"	, "A_TILDE"				)\
	KEY(ASCII_A_DIAERESIS			, "Ä"	, "A_DIAERESIS"			)\
	KEY(ASCII_A_RING				, "Å"	, "A_RING" 				)\
	KEY(ASCII_AE					, "Æ"	, "AE"					)\
	KEY(ASCII_CEDILLA				, "Ç"	, "CEDILLA"				)\
	KEY(ASCII_E_GRAVE				, "È"	, "E_GRAVE"				)\
	KEY(ASCII_E_ACUTE				, "É"	, "E_ACUTE"				)\
	KEY(ASCII_E_CIRCUMFLEX			, "Ê"	, "E_CIRCUMFLEX"		)\
	KEY(ASCII_E_DIAERESIS			, "Ë"	, "E_DIAERESIS"			)\
	KEY(ASCII_I_GRAVE				, "Ì"	, "I_GRAVE"				)\
	KEY(ASCII_I_ACUTE				, "Í"	, "I_ACUTE"				)\
	KEY(ASCII_I_CIRCUMFLEX			, "Î"	, "I_CIRCUMFLEX"		)\
	KEY(ASCII_I_DIAERESIS			, "Ï"	, "I_DIAERESIS"			)\
	KEY(ASCII_ETH					, "Ð"	, "ETH"					)\
	KEY(ASCII_N_TILDE				, "Ñ"	, "N_TILDE"				)\
	KEY(ASCII_O_GRAVE				, "Ò"	, "O_GRAVE"				)\
	KEY(ASCII_O_ACUTE				, "Ó"	, "O_ACUTE"				)\
	KEY(ASCII_O_CIRCUMFLEX			, "Ô"	, "O_CIRCUMFLEX"		)\
	KEY(ASCII_O_TILDE				, "Õ"	, "O_TILDE"				)\
	KEY(ASCII_O_DIAERESIS			, "Ö"	, "O_DIAERESIS"			)\
	KEY(ASCII_MULTIPLICATION		, "×"	, "MULTIPLICATION"		)\
	KEY(ASCII_O_SLASH				, "Ø"	, "O_SLASH"				)\
	KEY(ASCII_U_GRAVE				, "Ù"	, "U_GRAVE"				)\
	KEY(ASCII_U_ACUTE				, "Ú"	, "U_ACUTE"				)\
	KEY(ASCII_U_CIRCUMFLEX			, "Û"	, "U_CIRCUMFLEX"		)\
	KEY(ASCII_U_DIAERESIS			, "Ü"	, "U_DIAERESIS"			)\
	KEY(ASCII_Y_ACUTE				, "Ý"	, "Y_ACUTE"				)\
	KEY(ASCII_THORN					, "Þ"	, "THORN"				)\
	KEY(ASCII_SHARP_S				, "ß"	, "SHARP_S"				)\
	KEY(ASCII_a_GRAVE				, "à"	, "a_GRAVE"				)\
	KEY(ASCII_a_ACUTE				, "á"	, "a_ACUTE"				)\
	KEY(ASCII_a_CIRCUMFLEX			, "â"	, "a_CIRCUMFLEX"		)\
	KEY(ASCII_a_TILDE				, "ã"	, "a_TILDE"				)\
	KEY(ASCII_a_DIAERESIS			, "ä"	, "a_DIAERESIS"			)\
	KEY(ASCII_a_RING				, "å"	, "a_RING"				)\
	KEY(ASCII_ae					, "æ"	, "ae"					)\
	KEY(ASCII_cedilla				, "ç"	, "cedilla"				)\
	KEY(ASCII_e_GRAVE				, "è"	, "e_GRAVE"				)\
	KEY(ASCII_e_ACUTE				, "é"	, "e_ACUTE"				)\
	KEY(ASCII_e_CIRCUMFLEX			, "ê"	, "e_CIRCUMFLEX"		)\
	KEY(ASCII_e_DIAERESIS			, "ë"	, "e_DIAERESIS"			)\
	KEY(ASCII_i_GRAVE				, "ì"	, "i_GRAVE"				)\
	KEY(ASCII_i_ACUTE				, "í"	, "i_ACUTE"				)\
	KEY(ASCII_i_CIRCUMFLEX			, "î"	, "i_CIRCUMFLEX"		)\
	KEY(ASCII_i_DIAERESIS			, "ï"	, "i_DIAERESIS"			)\
	KEY(ASCII_SMALL_ETH				, "ð"	, "SMALL_ETH"			)\
	KEY(ASCII_n_TILDE				, "ñ"	, "n_TILDE"				)\
	KEY(ASCII_o_GRAVE				, "ò"	, "o_GRAVE"				)\
	KEY(ASCII_o_ACUTE				, "ó"	, "o_ACUTE"				)\
	KEY(ASCII_o_CIRCUMFLEX			, "ô"	, "o_CIRCUMFLEX"		)\
	KEY(ASCII_o_TILDE				, "õ"	, "o_TILDE"				)\
	KEY(ASCII_o_DIAERESIS			, "ö"	, "o_DIAERESIS"			)\
	KEY(ASCII_DIVISION				, "÷"	, "DIVISION"			)\
	KEY(ASCII_o_SLASH				, "ø"	, "o_SLASH"				)\
	KEY(ASCII_u_GRAVE				, "ù"	, "u_GRAVE"				)\
	KEY(ASCII_u_ACUTE				, "ú"	, "u_ACUTE"				)\
	KEY(ASCII_u_CIRCUMFLEX			, "û"	, "u_CIRCUMFLEX"		)\
	KEY(ASCII_u_DIAERESIS			, "ü"	, "u_DIAERESIS"			)\
	KEY(ASCII_y_ACUTE				, "ý"	, "y_ACUTE"				)\
	KEY(ASCII_SMALL_THORN			, "þ"	, "SMALL_THORN"			)\
	KEY(ASCII_y_DIAERESIS			, "ÿ"	, "y_DIAERESIS"			)

