#include "script.hpp"

static PyObject *emb_numargs(PyObject *self, PyObject *args){
    // Definicion de funcion C++ { emb_numargs }
	if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
	// Por ejemplo, devolvemos un 3 (podríamos devolver cualquier dato, siempre y cuando lo casteemos al tipo esperado en el script)
    return PyLong_FromLong(3);
}

static PyMethodDef EmbMethods[] = {
	// Enlazar python{ numargs() }---> C++{ emb_numargs(PyObject*, PyObject) }
    {"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},{NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,NULL, NULL, NULL, NULL};

static PyObject *PyInit_emb(void){
    return PyModule_Create(&EmbModule);
}

bool Script::initialize() {
	try {
		//std::string str = "hello";
		PyObject* pInt = NULL;

		PyImport_AppendInittab("emb", &PyInit_emb);
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
    printf("Result of call: %ld\n", PyLong_AsLong(value));
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
