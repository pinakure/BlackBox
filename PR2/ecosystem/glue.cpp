Surface *getLayer(int index) {
	if (!Vpu::is_initialized) {
		printf("vpu is not initialized.\nYou must call vpu.restart() before operating.\n");
		return NULL;
	}
	return Vpu::__layers[index];	
}

/* ----------------------------------------------------------------------
	Definicion de funciones pegamento C++ 
---------------------------------------------------------------------- */
static PyObject *vpu_print(PyObject *self, PyObject *args){
	char *text;
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "sii", &text, &x, &y)) return NULL;
	Vpu::print(std::string(text), x, y);
	return PyBool_FromLong(true);
}

static PyObject *vpu_select(PyObject *self, PyObject *args){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer)return PyBool_FromLong(false);
	Vpu::select(*layer);
	return PyBool_FromLong(true);
}

static PyObject *vpu_fullscreen(PyObject *self, PyObject *args){
	bool fullscreen;
	if(!PyArg_ParseTuple(args, "b", &fullscreen)) return NULL;
	Vpu::fullscreen = fullscreen;
	return PyBool_FromLong(Vpu::fullscreen);
}

static PyObject *vpu_update(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;
	Engine::update();
	if (!Engine::run) exit(-1);
	return PyBool_FromLong(1);
}

static PyObject *vpu_enable(PyObject *self, PyObject *args){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(false);
	(*layer).enabled = true;
	return PyBool_FromLong(1);
}

static PyObject *vpu_disable(PyObject *self, PyObject *args){
	int index;
	if(!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(false);
	(*layer).enabled = false;
	return PyBool_FromLong(1);
}

static PyObject *vpu_restart(PyObject *self, PyObject *args) {
	if(!PyArg_ParseTuple(args, "")) return NULL;
	bool r = true;
	r &= Vpu::start();
	r &= Vpu::restart();
	return PyBool_FromLong(r);
}

static PyObject *vpu_rotate(PyObject *self, PyObject *args){
	int index;
	float rotation;
	if(!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] += rotation;	
	return PyBool_FromLong(1);
}

static PyObject *vpu_scale(PyObject *self, PyObject *args){
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

static PyObject *vpu_setrotation(PyObject *self, PyObject *args){
	int index;
	float rotation;
	if(!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface *layer = getLayer(index);
	if(!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] = rotation;	
	return PyBool_FromLong(1);
}

static PyObject *vpu_setscale(PyObject *self, PyObject *args){
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

static PyObject *vpu_fadein(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
	Vpu::fadein();
    return PyLong_FromLong(1);
}
static PyObject *vpu_fadeout(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
	Vpu::fadeout();
	return PyLong_FromLong(1);
}


static PyObject *vpu_frames(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Vpu::total_frames);
}

static PyObject *blackbox_version(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(3);
}

static PyObject *blackbox_epoch(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Engine::epoch);
}

/* ----------------------------------------------------------------------
	Enlazar python{ function() }---> C++{ function(PyObject*, PyObject) }
---------------------------------------------------------------------- */
static PyMethodDef BlackBoxMethods[] = {
    {"version"		, blackbox_version		, METH_VARARGS, "blackbox.version() : Return current BlackBox engine version"},
    {"epoch"		, blackbox_epoch		, METH_VARARGS, "blackbox.epoch() : Return current engine epoch uptime"},
	{NULL, NULL, 0, NULL}
};
static PyMethodDef VpuMethods[] = {
	{"frames"		, vpu_frames			, METH_VARARGS, "vpu.frames() : Return actual frame count"},
	{"fullscreen"	, vpu_fullscreen		, METH_VARARGS, "vpu.fullscreen(enabled) : Toggle fullscreen mode"},
	{"setrotation"	, vpu_setrotation		, METH_VARARGS, "vpu.setrotation(layer, angle) : Sets rotation for specified layer (0-11) at given degrees"},
	{"setscale"		, vpu_setscale			, METH_VARARGS, "vpu.setscale(layer, scale_x, scale_y) : Sets scale for specified layer [0-11] given horizontal and vertical values"},
	{"rotate"		, vpu_rotate			, METH_VARARGS, "vpu.rotate(layer, angle) : Rotate specified layer (0-11) given degrees"},
	{"scale"		, vpu_scale				, METH_VARARGS, "vpu.scale(layer, scale_x, scale_y) : Change specified layer [0-11] given horizontal and vertical scale factor"},
	{"update"		, vpu_update			, METH_VARARGS, "vpu.update() : Allow blackbox engine to perform its rendering based input and output operations"},
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

static PyObject *PyInit_blackbox(void){ return PyModule_Create(&BlackBoxModule); }
static PyObject *PyInit_vpu(void){ return PyModule_Create(&VpuModule); }

#include "console.hpp"

static void Py_LoadCommands() {

}