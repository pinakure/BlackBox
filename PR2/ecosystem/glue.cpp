static PyModuleDef BlackBoxModule	= {PyModuleDef_HEAD_INIT, "blackbox"	, NULL, -1, Engine::methods		, NULL, NULL, NULL, NULL};
static PyModuleDef ConsoleModule	= {PyModuleDef_HEAD_INIT, "console"		, NULL, -1, Console::methods	, NULL, NULL, NULL, NULL};
static PyModuleDef EntityLibModule  = {PyModuleDef_HEAD_INIT, "entitylib"	, NULL, -1, Entity::methods		, NULL, NULL, NULL, NULL};
static PyModuleDef JoypadModule		= {PyModuleDef_HEAD_INIT, "joypad"		, NULL, -1, InputDevice::methods, NULL, NULL, NULL, NULL};
static PyModuleDef TiledMapModule	= {PyModuleDef_HEAD_INIT, "tm"			, NULL, -1, TiledMap::methods	, NULL, NULL, NULL, NULL};
static PyModuleDef TypeWriterModule	= {PyModuleDef_HEAD_INIT, "typewriter"	, NULL, -1, TypeWriter::methods	, NULL, NULL, NULL, NULL};
static PyModuleDef VpuModule		= {PyModuleDef_HEAD_INIT, "vpu"			, NULL, -1, Vpu::methods		, NULL, NULL, NULL, NULL};

static PyObject *PyInit_blackbox	(void){ return PyModule_Create(&BlackBoxModule		);}
static PyObject *PyInit_entitylib	(void){ return PyModule_Create(&EntityLibModule		);}
static PyObject *PyInit_console		(void){ return PyModule_Create(&ConsoleModule		);}
static PyObject *PyInit_joypad		(void){ return PyModule_Create(&JoypadModule		);}
static PyObject* PyInit_tiledmap	(void){ return PyModule_Create(&TiledMapModule		);}
static PyObject *PyInit_typewriter	(void){ return PyModule_Create(&TypeWriterModule	);}
static PyObject *PyInit_vpu			(void){ return PyModule_Create(&VpuModule			);}

static void Py_LoadCommands() {
	//	for (PyMethodDef &d : Engine::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : InputDevice::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : Console::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : TypeWriter::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : Vpu::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef& d : Entity::methods) { if (!d.ml_name)continue; Console::addHelp(d.ml_name, d.ml_doc); }
	//	for (PyMethodDef &d : TiledMap::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
}