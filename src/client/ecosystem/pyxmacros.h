
#define BLACKBOX_COMMANDS \
	MAKECOMMAND("epoch"			, blackbox_epoch	, "blackbox.epoch"				,""							, "Return current engine epoch uptime")\
	MAKECOMMAND("version"		, blackbox_version	, "blackbox.version"			,""							, "Return current BlackBox engine version")\
	
#define VPU_COMMANDS \
	MAKECOMMAND("frames"		, vpu_frames		, "vpu.frames"					,""							, "Return actual frame count")\
	MAKECOMMAND("fullscreen"	, vpu_fullscreen	, "vpu.fullscreen"				,"enabled"					, "Handle fullscreen video mode")\
	MAKECOMMAND("setrotation"	, vpu_setrotation	, "vpu.setrotation"				,"layer, angle"				, "Sets rotation for specified layer (0-11) at given degrees")\
	MAKECOMMAND("setscale"		, vpu_setscale		, "vpu.setscale"				,"layer, scale_x, scale_y"	, "Sets scale for specified layer [0-11] given horizontal and vertical values")\
	MAKECOMMAND("rotate"		, vpu_rotate		, "vpu.rotate"					,"layer, angle"				, "Rotate specified layer (0-11) given degrees")\
	MAKECOMMAND("scale"			, vpu_scale			, "vpu.scale"					,"layer, scale_x, scale_y"	, "Change specified layer [0-11] given horizontal and vertical scale factor")\
	MAKECOMMAND("update"		, vpu_update		, "vpu.update"					,""							, "Allow blackbox engine to perform its rendering based input and output operations")\
	MAKECOMMAND("fadein"		, vpu_fadein		, "vpu.fadein"					,"rate"						, "Fade screen from black at given rate")\
	MAKECOMMAND("fadeout"		, vpu_fadeout		, "vpu.fadeout"					,"rate"						, "Fade screen to black at given rate")\
	MAKECOMMAND("enable"		, vpu_enable		, "vpu.enable"					,"layer"					, "Enable rendering for given Vpu layer (0-11)")\
	MAKECOMMAND("disable"		, vpu_disable		, "vpu.disable"					,"layer"					, "Disable rendering for given Vpu layer (0-11)")\
	MAKECOMMAND("select"		, vpu_select		, "vpu.select"					,"layer"					, "Select given layer as target to perform next graphic operations")\
	MAKECOMMAND("print"			, vpu_print			, "vpu.print"					,"text, x, y"				, "Print given text at given coordinates")\
	
#define LOADPYMODULE(name, mod) \
	static PyModuleDef mod#Module = {PyModuleDef_HEAD_INIT, #name#, NULL, -1, mod#Methods,NULL, NULL, NULL, NULL};\
	static PyObject *PyInit_ ## name(void){ return PyModule_Create(&mod#Module); }



