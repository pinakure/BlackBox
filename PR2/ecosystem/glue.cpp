Surface *getLayer(int index) {
	if (!Vpu::is_initialized) {
		printf("vpu is not initialized.\nYou must call vpu.restart() before operating.\n");
		return NULL;
	}
	if (index >= 3) {
		return &Vpu::surfaces.at(index-3);
	}	
	return 
		index == 0 ? &Vpu::background :
		index == 1 ? &Vpu::foreground :
		&Vpu::overlay;
	
}

/* ----------------------------------------------------------------------
	Definicion de funciones pegamento C++ 
---------------------------------------------------------------------- */
#define pythoncommand(name) static PyObject *name(PyObject *self, PyObject *args)

pythoncommand(vpu_print) {
	char *text;
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "sii", &text, &x, &y)) return NULL;
	Vpu::print(std::string(text), x, y);
	return PyBool_FromLong(true);
}

pythoncommand(vpu_pset) {
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "ii", &x, &y)) return NULL;
	Vpu::putpixel(x, y);
	return PyBool_FromLong(true);
}

pythoncommand(vpu_reload){
	if(!PyArg_ParseTuple(args, "")) return NULL;
	if (!Vpu::start())exit(1);
	if (!Vpu::restart())exit(1);
	return PyBool_FromLong(true);
}

pythoncommand(vpu_select){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer)return PyBool_FromLong(false);
	Vpu::select(*layer);
	return PyBool_FromLong(true);
}

pythoncommand(vpu_fullscreen){
	bool fullscreen;
	if(!PyArg_ParseTuple(args, "b", &fullscreen)) return NULL;
	Vpu::fullscreen = fullscreen;
	return PyBool_FromLong(Vpu::fullscreen);
}

pythoncommand(vpu_update){
	if(!PyArg_ParseTuple(args, "")) return NULL;
	Engine::update();
	if (!Engine::run) exit(-1);
	return PyBool_FromLong(1);
}

pythoncommand(vpu_enable){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(false);
	(*layer).enabled = true;
	return PyBool_FromLong(1);
}

pythoncommand(vpu_disable){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(false);
	(*layer).enabled = false;
	return PyBool_FromLong(1);
}

pythoncommand(vpu_restart) {
	if(!PyArg_ParseTuple(args, "")) return NULL;
	bool r = true;
	r &= Vpu::start();
	r &= Vpu::restart();
	return PyBool_FromLong(r);
}

pythoncommand(vpu_rotate){
	int index;
	float rotation;
	if(!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] += rotation;	
	return PyBool_FromLong(1);
}

pythoncommand(vpu_scale){
	int index;
	float scale_x;
	float scale_y;
	if(!PyArg_ParseTuple(args, "iff", &index, &scale_x, &scale_y)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).scale[0] += scale_x;
	(*layer).scale[1] += scale_y;	
	return PyBool_FromLong(1);
}

pythoncommand(vpu_setrotation){
	int index;
	float rotation;
	if(!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] = rotation;	
	return PyBool_FromLong(1);
}

pythoncommand(vpu_setscale){
	int index;
	float scale_x;
	float scale_y;
	if(!PyArg_ParseTuple(args, "iff", &index, &scale_x, &scale_y)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).scale[0] = scale_x;
	(*layer).scale[1] = scale_y;	
	return PyBool_FromLong(1);
}
pythoncommand(vpu_setcolor){
	int r;
	int g;
	int b;
	int a=255;
	if(!PyArg_ParseTuple(args, "iii|i", &r, &g, &b, &a)) return NULL;
	Vpu::setColor(r, g, b, a);
	return PyBool_FromLong(1);
}

pythoncommand(vpu_deletesurf){
	// Must subtract 3 units from handle (3 system layers to be selected which are not in <Vpu::surfaces>)
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Vpu::deallocateSurface(handle-3);
	return PyLong_FromLong(1);
}

pythoncommand(vpu_createsurf){
	// Must add 3 units to handle (3 system layers to be selected which are not in <Vpu::surfaces> )
	int width;
	int height;
	if(!PyArg_ParseTuple(args, "ii", &width, &height)) return NULL;
	long int handle = Vpu::allocateSurface(width, height);
	return PyLong_FromLong(handle+2);	
}

pythoncommand(vpu_drawsurf){
	// Must subtract 3 units from handle (3 system layers to be selected which are not in <surfaces>)
	int handle;
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "iii", &handle, &x, &y)) return NULL;
	Surface *s = getLayer(handle);
	Vpu::drawSurface(*s, 0, 0, s->width, s->height, x, y);
	return PyLong_FromLong(1);	
}

pythoncommand(vpu_fill){
	int r = Vpu::color.r*255;
	int g = Vpu::color.g*255;
	int b = Vpu::color.b*255;
	int a = Vpu::color.a*255;
	if(!PyArg_ParseTuple(args, "")) return NULL;
	Vpu::paint(r, g, b, a);
	return PyLong_FromLong(1);	
}

pythoncommand(vpu_fadein){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
	Vpu::fadein();
    return PyLong_FromLong(1);
}

pythoncommand(vpu_fadeout){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
	Vpu::fadeout();
	return PyLong_FromLong(1);
}

pythoncommand(vpu_frames){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Vpu::total_frames);
}

pythoncommand(blackbox_version){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(3);
}

pythoncommand(blackbox_epoch){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Engine::epoch);
}
/* ----------------------------------------------------------------------
	Console methods
---------------------------------------------------------------------- */
pythoncommand(console_print) {
	char *buffer;
	if(!PyArg_ParseTuple(args, "s", &buffer)) return NULL;	
	Engine::print(buffer);
	return PyLong_FromLong(1);
}
pythoncommand(console_cls) {
	Console::clear();
	return PyLong_FromLong(1);
}
/* ----------------------------------------------------------------------
	Enlazar python{ function() }---> C++{ function(PyObject*, PyObject) }
---------------------------------------------------------------------- */
static PyMethodDef ConsoleMethods[] = {
    {"print"		, console_print			, METH_VARARGS, "console.print(text) : Dump given text over console"},
    {"cls"			, console_cls			, METH_VARARGS, "console.cls() : Clean console buffer"},
	{NULL, NULL, 0, NULL}
};
static PyMethodDef BlackBoxMethods[] = {
    {"version"		, blackbox_version		, METH_VARARGS, "blackbox.version() : Return current BlackBox engine version"},
    {"epoch"		, blackbox_epoch		, METH_VARARGS, "blackbox.epoch() : Return current engine epoch uptime"},
	{NULL, NULL, 0, NULL}
};
static PyMethodDef VpuMethods[] = {
	{"pset"			, vpu_pset				, METH_VARARGS, "vpu.pset(x, y) : Draw a pixel onto selected surface onto given coordinates" },
	{"drawsurf"		, vpu_drawsurf			, METH_VARARGS, "vpu.drawsurf(handle, x, y) : Draw surface identified by given handle onto given coordinates" },
	{"deletesurf"	, vpu_deletesurf		, METH_VARARGS, "vpu.deletesurf(handle) : Delete Surface identified by given Handle" },
	{"createsurf"	, vpu_createsurf		, METH_VARARGS, "vpu.createsurf(width, height) : Returns Handle to Surface object to be drawn arbitrarily to screen" },
	{"frames"		, vpu_frames			, METH_VARARGS, "vpu.frames() : Return actual frame count"},
	{"fullscreen"	, vpu_fullscreen		, METH_VARARGS, "vpu.fullscreen(enabled) : Toggle fullscreen mode"},
	{"restart"		, vpu_restart			, METH_VARARGS, "vpu.restart() : Restart Video Processing Unit"},
	{"setrotation"	, vpu_setrotation		, METH_VARARGS, "vpu.setrotation(layer, angle) : Sets rotation for specified layer (0-11) at given degrees"},
	{"setscale"		, vpu_setscale			, METH_VARARGS, "vpu.setscale(layer, scale_x, scale_y) : Sets scale for specified layer [0-11] given horizontal and vertical values"},
	{"setcolor"		, vpu_setcolor			, METH_VARARGS, "vpu.setcolor(r, g, b, a) : Sets current painting color"},
	{"rotate"		, vpu_rotate			, METH_VARARGS, "vpu.rotate(layer, angle) : Rotate specified layer (0-11) given degrees"},
	{"scale"		, vpu_scale				, METH_VARARGS, "vpu.scale(layer, scale_x, scale_y) : Change specified layer [0-11] given horizontal and vertical scale factor"},
	{"update"		, vpu_update			, METH_VARARGS, "vpu.update() : Allow blackbox engine to perform its rendering based input and output operations"},
	{"fill"			, vpu_fill				, METH_VARARGS, "vpu.fill(r, g, b, a) : Fill selected bitmap with given color or default color"},
	{"fadein"		, vpu_fadein			, METH_VARARGS, "vpu.fadein() : Fade screen from black"},
	{"fadeout"		, vpu_fadeout			, METH_VARARGS, "vpu.fadeout() : Fade screen to black"},
	{"enable"		, vpu_enable			, METH_VARARGS, "vpu.enable(layer) : Toggle on  given vpu layer"},
	{"disable"		, vpu_disable			, METH_VARARGS, "vpu.disable(layer) : Toggle off given vpu layer"},
	{"select"		, vpu_select			, METH_VARARGS, "vpu.select(layer) : Select given layer to perform next graphic operations onto it"},
	{"print"		, vpu_print				, METH_VARARGS, "vpu.print(text, x, y) : Print given text at given coordinates"},
	{NULL, NULL, 0, NULL}
};

/* ----------------------------------------------------------------------
	Definir Modulos
---------------------------------------------------------------------- */
static PyModuleDef BlackBoxModule	= {PyModuleDef_HEAD_INIT, "blackbox", NULL, -1, BlackBoxMethods	,NULL, NULL, NULL, NULL};
static PyModuleDef VpuModule		= {PyModuleDef_HEAD_INIT, "vpu"		, NULL, -1, VpuMethods		,NULL, NULL, NULL, NULL};
static PyModuleDef ConsoleModule	= {PyModuleDef_HEAD_INIT, "console"	, NULL, -1, ConsoleMethods	,NULL, NULL, NULL, NULL};

static PyObject *PyInit_blackbox(void){ return PyModule_Create(&BlackBoxModule); }
static PyObject *PyInit_vpu(void){ return PyModule_Create(&VpuModule); }
static PyObject *PyInit_console(void){ return PyModule_Create(&ConsoleModule); }

static void Py_LoadCommands() {
	for (PyMethodDef &d : VpuMethods) {
		if (!d.ml_name)continue;
		Console::addHelp(d.ml_name, d.ml_doc);
	}
	for (PyMethodDef &d : BlackBoxMethods) {
		if (!d.ml_name)continue;
		Console::addHelp(d.ml_name, d.ml_doc);
	}
	for (PyMethodDef &d : ConsoleMethods) {
		if (!d.ml_name)continue;
		Console::addHelp(d.ml_name, d.ml_doc);
	}
}