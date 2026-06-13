#include <Python.h>
#include "typewriter.hpp"
#include "vpu.hpp"
#include "windowmgr.hpp"
#include "tiledmap.hpp"
#include "input.hpp"
#include "engine.hpp"
#include "genericgame.hpp"
#include "entity.hpp"
#include "console.hpp"


class Menu {
private:
public:
	static PyMethodDef methods[];

};

PyMethodDef Menu::methods[] = {
	// {"version"		, blackbox_version		, METH_VARARGS, "version() : Return current BlackBox engine version"},
	{NULL, NULL, 0, NULL}
};


PyModuleDef BlackBoxModule	= {PyModuleDef_HEAD_INIT, "blackbox"	, NULL, -1, Engine::methods			, NULL, NULL, NULL, NULL};
PyModuleDef ConsoleModule	= {PyModuleDef_HEAD_INIT, "console"		, NULL, -1, Console::methods		, NULL, NULL, NULL, NULL};
PyModuleDef EntityLibModule  = {PyModuleDef_HEAD_INIT, "entitylib"	, NULL, -1, Entity::methods			, NULL, NULL, NULL, NULL};
PyModuleDef JoypadModule		= {PyModuleDef_HEAD_INIT, "joypad"		, NULL, -1, InputDevice::methods	, NULL, NULL, NULL, NULL};
PyModuleDef TiledMapModule	= {PyModuleDef_HEAD_INIT, "tm"			, NULL, -1, TiledMap::methods		, NULL, NULL, NULL, NULL};
PyModuleDef GenericGameModule= {PyModuleDef_HEAD_INIT, "gglib"		, NULL, -1, GenericGame::methods	, NULL, NULL, NULL, NULL};
PyModuleDef WindowMgrModule  = {PyModuleDef_HEAD_INIT, "wm"			, NULL, -1, WindowManager::methods	, NULL, NULL, NULL, NULL};
PyModuleDef TypeWriterModule	= {PyModuleDef_HEAD_INIT, "typewriter"	, NULL, -1, TypeWriter::methods		, NULL, NULL, NULL, NULL};
PyModuleDef VpuModule		= {PyModuleDef_HEAD_INIT, "vpu"			, NULL, -1, Vpu::methods			, NULL, NULL, NULL, NULL};
PyModuleDef MenuModule		= {PyModuleDef_HEAD_INIT, "menu"		, NULL, -1, Menu::methods			, NULL, NULL, NULL, NULL};

PyObject *PyInit_blackbox	(void){ return PyModule_Create(&BlackBoxModule		);}
PyObject *PyInit_entitylib	(void){ return PyModule_Create(&EntityLibModule		);}
PyObject *PyInit_console	(void){ return PyModule_Create(&ConsoleModule		);}
PyObject *PyInit_joypad		(void){ return PyModule_Create(&JoypadModule		);}
PyObject *PyInit_tiledmap	(void){ return PyModule_Create(&TiledMapModule		);}
PyObject *PyInit_genericgame(void){ return PyModule_Create(&GenericGameModule	);}
PyObject *PyInit_windowmgr	(void){ return PyModule_Create(&WindowMgrModule		);}
PyObject *PyInit_typewriter	(void){ return PyModule_Create(&TypeWriterModule	);}
PyObject *PyInit_vpu		(void){ return PyModule_Create(&VpuModule			);}
PyObject *PyInit_menu		(void){ return PyModule_Create(&MenuModule			);}

void Py_LoadCommands() {
	//	for (PyMethodDef &d : Engine::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : InputDevice::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : Console::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : TypeWriter::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef &d : Vpu::methods		){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
	//	for (PyMethodDef& d : Entity::methods) { if (!d.ml_name)continue; Console::addHelp(d.ml_name, d.ml_doc); }
	//	for (PyMethodDef &d : TiledMap::methods	){if(!d.ml_name)continue;Console::addHelp(d.ml_name, d.ml_doc);}
}