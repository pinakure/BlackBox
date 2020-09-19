#include "engine.hpp"
#include "script.hpp"
#include "vpu.hpp"

/* ----------------------------------------------------------------------
	Definicion de funciones pegamento C++ 
---------------------------------------------------------------------- */
static PyObject *vpu_fullscreen(PyObject *self, PyObject *args){
	bool fullscreen;
	if(!PyArg_ParseTuple(args, "b", &fullscreen))
        return NULL;
	Vpu::fullscreen = fullscreen;
	return PyBool_FromLong(Vpu::fullscreen);
}

static PyObject *vpu_update(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, ""))
        return NULL;
	Engine::update();
	if (!Engine::run)exit(-1);
	return PyBool_FromLong(1);
}
static PyObject *vpu_rotate(PyObject *self, PyObject *args){
	int index;
	float rotation;
	if(!PyArg_ParseTuple(args, "if", &index, &rotation))
        return NULL;
	switch (index) {
		case 0:case 1:case 2:case 3: 
			Vpu::background[index].rotation[0] = rotation;
			break;
		case 4:case 5:case 6:case 7: 
			Vpu::foreground[index-4].rotation[0] = rotation;
			break;
		case 8:case 9:case 10:case 11: 
			Vpu::overlay[index-8].rotation[0] = rotation;
			break;
	
	}
	return PyBool_FromLong(1);
}
static PyObject *vpu_scale(PyObject *self, PyObject *args){
	int index;
	float scale_x;
	float scale_y;
	if(!PyArg_ParseTuple(args, "iff", &index, &scale_x, &scale_y))
        return NULL;
	switch (index) {
		case 0:case 1:case 2:case 3: 
			Vpu::background[index].scale[0] = scale_x;
			Vpu::background[index].scale[1] = scale_y;
			break;
		case 4:case 5:case 6:case 7: 
			Vpu::foreground[index-4].scale[0] = scale_x;
			Vpu::foreground[index-4].scale[1] = scale_y;
			break;
		case 8:case 9:case 10:case 11: 
			Vpu::overlay[index-8].scale[0] = scale_x;
			Vpu::overlay[index-8].scale[1] = scale_y;
			break;
	
	}
	return PyBool_FromLong(1);
}
static PyObject *blackbox_version(PyObject *self, PyObject *args){
	if(!PyArg_ParseTuple(args, ""))
        return NULL;	
    return PyLong_FromLong(3);
}

/* ----------------------------------------------------------------------
	Enlazar python{ function() }---> C++{ function(PyObject*, PyObject) }
---------------------------------------------------------------------- */
static PyMethodDef BlackBoxMethods[] = {
    {"version", blackbox_version, METH_VARARGS, "Return current BlackBox engine version"},
	{NULL, NULL, 0, NULL}
};
static PyMethodDef VpuMethods[] = {
	{"fullscreen", vpu_fullscreen	, METH_VARARGS, "Toggle fullscreen mode ( True ~ False )"},
	{"rotate"	 , vpu_rotate		, METH_VARARGS, "Rotate specified layer (0-11) given degrees ( -6.30 ~ 6.30 )"},
	{"scale"	 , vpu_scale		, METH_VARARGS, "Change specified layer (0-11) given horizontal and vertical scale factor ( 20000.0 ~ 0.0 )"},
	{"update"	 , vpu_update		, METH_VARARGS, "Must be called inside any loop"},
	{NULL, NULL, 0, NULL}
};

/* ----------------------------------------------------------------------
	Definir Modulos
---------------------------------------------------------------------- */
static PyModuleDef BlackBoxModule	= {PyModuleDef_HEAD_INIT, "blackbox", NULL, -1, BlackBoxMethods	,NULL, NULL, NULL, NULL};
static PyModuleDef VpuModule		= {PyModuleDef_HEAD_INIT, "vpu"		, NULL, -1, VpuMethods		,NULL, NULL, NULL, NULL};

static PyObject *PyInit_blackbox(void){
    return PyModule_Create(&BlackBoxModule);
}
static PyObject *PyInit_vpu(void){
    return PyModule_Create(&VpuModule);
}

bool Script::initialize() {
	try {
		//std::string str = "hello";
		PyObject* pInt = NULL;

		PyImport_AppendInittab("blackbox", &PyInit_blackbox);
		PyImport_AppendInittab("vpu"	 , &PyInit_vpu);
		Py_Initialize();
	}catch (int e) {
		e = e;
		return false;
	}
	return true;
}

void Script::deinitialize() {
	Py_Finalize();
}

Script::Script(std::string path) {
	this->path = path;
	this->loaded = this->load();
}

Script::~Script() {
	// Destroy any dynamic stuff
	// decref python object references
	if(__module__)
		Py_DECREF(__module__);	
}

bool Script::run_sentence() {
	if (this->loaded) return false;
	if (this->finalized) return false;
	this->execution_counter++;
	if (this->execution_counter == this->program_size) this->finalized = true;
	// run sentence
	// return true if sentence was executed
	// get errors if produced and dump'em thru this->error
	// return false if errors were produced
	return true;
}

bool Script::load() {
	std::string pythonfile = "scripts." + this->path;
	PyObject *name = PyUnicode_DecodeFSDefault(pythonfile.c_str());
	__module__ = PyImport_Import(name);
	Py_DECREF(name);// ya no necesitamos name, dereferenciarlo
	if (!__module__) {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", pythonfile.c_str());
		return false;
	}
	this->loaded = true;
	return true;
}

bool Script::call(std::string function_name){
	PyObject *__function__ = PyObject_GetAttrString(__module__, function_name.c_str());
	if (!__function__ || !PyCallable_Check( __function__ )) {
		if (PyErr_Occurred()) PyErr_Print();
		fprintf(stderr, "Cannot find function \"%s\"\n", function_name.c_str());
		Py_XDECREF(__function__);
		return false;
	}
	PyObject *args  = PyTuple_New(0);
    PyObject *value = PyObject_CallObject(__function__, args);
    Py_DECREF(args); // una vez realizada la llamada, args no sirve para nada, dereferenciarlo
	if (!value) {
	    Py_DECREF(__function__);
        PyErr_Print();
        fprintf(stderr,"Call failed\n");
        return false;
    }
    //printf("Result of call: %ld\n", PyLong_AsLong(value));
    Py_DECREF(value); // Una vez utilizado el valor, descartamos la referencia a value
    Py_XDECREF(__function__);    
    return true;	
}

void Script::print(std::string txt) {
	this->out.push_back(txt);
}

void Script::error(std::string txt) {
	this->err.push_back(txt);
}

bool Script::isLoaded() { 
	return this->loaded; 
}

bool Script::execute(std::string python_code) {
	try {
		PyRun_SimpleString(python_code.c_str());
		return true;
	} catch (int e) {
		e = e;
		PyErr_Print();        
		return false;
	}
}
