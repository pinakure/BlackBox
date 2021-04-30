#include "engine.hpp"
#include "script.hpp"
#include "input.hpp"
#include "console.hpp"
#include "vpu.hpp"
#include "typewriter.hpp"
#include "windowmgr.hpp"
#include "dashboard.hpp"
#include "glue.cpp"

bool Script::initialize() {
	try {
		PyObject* pInt = NULL;

		PyImport_AppendInittab("blackbox"	, &PyInit_blackbox);
		PyImport_AppendInittab("console"	, &PyInit_console);
		PyImport_AppendInittab("entitylib"	, &PyInit_entitylib);
		PyImport_AppendInittab("joypad"		, &PyInit_joypad);
		PyImport_AppendInittab("tm"			, &PyInit_tiledmap);
		PyImport_AppendInittab("wmgr"		, &PyInit_windowmgr);
		PyImport_AppendInittab("typewriter"	, &PyInit_typewriter);
		PyImport_AppendInittab("vpu"		, &PyInit_vpu);
		Py_SetPath(L".\\;.\\data\\system.zip");
		Py_Initialize();
		Py_LoadCommands();
	}catch (int e) {
		std::printf("ERROR: Scripting Machine cannot run.\n\tCritical Modules could not be loaded.\n\tIt may be some files missing.\nException code: 0x%x\n", e);
		return false;
	}
	return true;
}

void Script::deinitialize() {
	Py_Finalize();
}

Script::Script(std::string path, std::string basename) {
	this->basename = basename;
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
	std::string pythonfile = this->basename+"." + this->path;
	PyObject *name = PyUnicode_DecodeFSDefault(pythonfile.c_str());
	__module__ = PyImport_Import(name);
	if(__module__)__module__ = PyImport_ReloadModule(__module__);

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

bool Script::execute(const char *python_code) {
	std::string temp(python_code);
	return execute(temp);
}
bool Script::execute(std::string &python_code) {
	try {
		int i = PyRun_SimpleStringFlags(python_code.c_str(), NULL);
		python_code = "";
		if (i != 0) {
			return false;			
		}
		return true;
	}catch (int e) {
		e = e;
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);		
		return false;
	}
}

PyObject* IntegerList(int size, ...) {
	PyObject* list = PyList_New(size);
	if (!list) throw("Unable to allocate memory for Python list");
	va_list ap;
	va_start(ap, size);
	for (unsigned int i = 0; i < 2; i++) {
		int val = va_arg(ap, int);
		PyObject* num = PyLong_FromLong((long)val);
		if (!num) {
			Py_DECREF(list);
			throw("Unable to allocate memory for Python list");
		}
		Py_INCREF(list);
		PyList_SET_ITEM(list, i, num);
		Py_DECREF(list);
	}
	va_end(ap);
	return list;
}

PyObject* FloatList(int size, ...) {
	PyObject* list = PyList_New(size);
	if (!list) throw("Unable to allocate memory for Python list");
	va_list ap;
	va_start(ap, size);
	for (unsigned int i = 0; i < 2; i++) {
		float val = va_arg(ap, float);
		PyObject* num = PyFloat_FromDouble((double)val);
		if (!num) {
			Py_DECREF(list);
			throw("Unable to allocate memory for Python list");
		}
		Py_INCREF(list);
		PyList_SET_ITEM(list, i, num);
		Py_DECREF(list);
	}
	va_end(ap);
	return list;
}
