#include "engine.hpp"
#include "input.hpp"
#include "hud.hpp"
#include "console.hpp"
#include "world.hpp"
#include "camera.hpp"
#include "dashboard.hpp"
#include "windowmgr.hpp"
#include <iostream>
#include<Windows.h>
#pragma comment(lib, "urlmon.lib")

Surface* Engine::target= &Vpu::background;
#define WORLD_SIZE					65535
#define NPC_COUNT					128
int Engine::width					= 640;
int Engine::height					= 480;
bool Engine::run					= true;
int Engine::cycles					= 0;
long int Engine::epoch				= 0;
MusicPlayer Engine::music			= MusicPlayer();
ALLEGRO_TIMER *Engine::timer		= NULL;
ALLEGRO_TIMER *Engine::clock		= NULL;
ALLEGRO_EVENT_QUEUE *Engine::queue	= NULL;
ALLEGRO_EVENT Engine::event;	
std::vector<Entity*> Engine::entities = std::vector<Entity*>();
/* 
---------------------------------------------------------------------------------------- 
 Point to the showcase we want to test on runtime 
---------------------------------------------------------------------------------------- 
 - dummy_showcase : do nothing
 - typewriter_showcase : test typewriter display box
 - sandbox_showcase : initialize world, update and draw , also calls typewriter_showcase






*/
#include "showcase.cpp"
#include <physfs.h>
#include <allegro5/allegro_physfs.h>

Showcase *showcase = 
	&dummy_showcase
//	&sandbox_showcase		
	;
/* 
---------------------------------------------------------------------------------------- 
#
#
#
#
#
#
#
#
#
#
#
#
---------------------------------------------------------------------------------------- 
*/



bool Engine::initialize() {
	try{
		al_init();
		
		clock   = al_create_timer(1.0);
		timer	= al_create_timer(1.0 / 120.0);
		queue	= al_create_event_queue();
		
		al_set_physfs_file_interface();
		if (!Vpu::initialize()) return false;
		CVar::initialize();
		Console::initialize();
		if (!InputDevice::initialize(true, false, true))return false;
		al_register_event_source(queue, al_get_timer_event_source(timer));
		al_register_event_source(queue, al_get_timer_event_source(clock));
		al_start_timer(timer);
		al_start_timer(clock);
		Hud::initialize();

		WindowManager::initialize();
		
		srand(time(0));

		Engine::music.loadMenuMusic();
		Engine::music.randomMenuMusic();

		Script::execute("from scripts.main import menu");
		return showcase->initialize();
	} catch (int e) {
		e = e;
		return false;
	}
}

void Engine::deinitialize() {
	World::deinitialize();
	//TypeWriter::deInitialize();
	Console::deInitialize();
	InputDevice::deinitialize();
	Vpu::deinitialize();
	if(timer) al_destroy_timer(timer);
	if(queue) al_destroy_event_queue(queue);
}

void Engine::tick() {
	epoch++;
	Vpu::fps = Vpu::frames;
	Vpu::frames = 0;
}

void Engine::handleEvents() {
	al_wait_for_event(queue, &event);
	switch (event.type) {
		case ALLEGRO_EVENT_TIMER:			
			if (event.timer.source == clock) return(tick());
			if(event.timer.source == timer) Vpu::redraw = true;
			break;
		
			//case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			run = false;
			break;
		
		case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
			printf("Got HALT_DRAWING event. Maybe this is the cause for the engine to randomly freeze.");
			while (1) { ; };
			break;

		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			InputDevice::controller[event.joystick.button] = 1;
			break;

		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
			InputDevice::controller[event.joystick.button] = -1;
			break;

		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			al_acknowledge_resize(Vpu::display);
			Engine::width = event.display.width;
			Engine::height = event.display.height;
			if (!Vpu::start()) run = false;
			if (!Vpu::restart()) run = false;
			break;
		
		case ALLEGRO_EVENT_KEY_DOWN:
			if(InputDevice::in_keyboard) InputDevice::handleEvent(event);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if(InputDevice::in_keyboard) InputDevice::handleEvent(event);
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			InputDevice::mouse_button[event.mouse.button-1] = 1;
			break;
		
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			InputDevice::mouse_button[event.mouse.button-1] = -1;
			break;
			
		default:
			break;
	}		
}

void Engine::render() {
	if (Vpu::redraw && al_is_event_queue_empty(queue)) {
		// Clean before drawing HUD
		// -----------------------
		showcase->draw();
		Dashboard::draw();
		Hud::draw();
		InputDevice::draw(2);
		Console::draw(16);
		WindowManager::render();
		Vpu::render();		
		Vpu::select(Vpu::overlay);
		Vpu::paint(0, 0, 0, 16);
		Vpu::frames++;
		Vpu::total_frames++;
		cycles = 0;
	}
}

void Engine::destroyEntities() {
	std::vector<Entity*>::iterator it = Engine::entities.begin();
	for (; it < Engine::entities.end(); it++) {
		delete(*it);
	}
	Engine::entities.clear();
}

#include <chrono>
auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

void updateEntities(float delta) {
	std::vector<Entity*>::iterator it = Engine::entities.begin();
	for (; it != Engine::entities.end(); it++) {
		(*it)->update(delta);
	}
}

void Engine::update() {
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
    float fElapsedTime = elapsedTime.count();
    // From this line on, every value must be multiplied by fElapsedTime
    
	if(int(_rotation*100))
		rotate(_rotation);
	if(int(_scale[0]*100))
		scale(
			_scale[0], 
			_scale[1],
			_scale[2]
		);
	showcase->update(fElapsedTime);
	Camera::update(fElapsedTime);
	Hud::update(fElapsedTime);
	WindowManager::update();
	Dashboard::update(fElapsedTime);
	InputDevice::update(fElapsedTime);
	updateEntities(fElapsedTime);
	Console::update();//!!!
	
	render();
	handleEvents();
	cycles++;
}

void Engine::loop() {
	while(run){
		update();		
	}	
}
void Engine::error(std::string text) {
	if (Console::initialized) {
		Console::print(text);
		return;
	}
	fprintf(stderr, "%s\n", text.c_str());
}

void Engine::printf(const char *fmt, ...){
	char buffer[1024];

	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buffer, 1024, fmt, ap);
	va_end(ap);

	Engine::print(buffer);
}

void Engine::print(std::string text) {	
	if (Console::initialized) {
		Console::print(text);
		return;
	}
	fprintf(stdout, "%s\n", text.c_str());
}

float Engine::_rotation = 0.0f;
void Engine::rotate(float r) {
	Vpu::select(*Engine::target);
	_rotation = r;
	Vpu::target->rotation[0]+=_rotation;
}

float Engine::_scale[3] = {
	0.0f, 0.0f, 0.0f,
};
void Engine::scale(float x, float y, float z) {
	Vpu::select(*Engine::target); 
	_scale[0] = x;
	_scale[1] = y;
	_scale[2] = z;
	if (x > 2.0f)x = 2.0f;
	if (y > 2.0f)y = 2.0f;	
	if((Vpu::target->scale[0] + _scale[0] >= 0.5f)&&(Vpu::target->scale[0] + _scale[0] <= 2.0f)) Vpu::target->scale[0] += _scale[0];
	if((Vpu::target->scale[1] + _scale[1] >= 0.5f)&&(Vpu::target->scale[1] + _scale[1] <= 2.0f)) Vpu::target->scale[1] += _scale[1];
	if((Vpu::target->scale[2] + _scale[2] >= 0.5f)&&(Vpu::target->scale[2] + _scale[2] <= 2.0f)) Vpu::target->scale[2] += _scale[2];
	
	std::printf("Vpu::setScale(%f,%f,%f)\n", Vpu::target->scale[0],Vpu::target->scale[1],Vpu::target->scale[2]);
}
#include "download.hpp"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
void Engine::download(const char* file) {
	Vpu::console.enabled = false;
	Vpu::foreground.enabled = false;
	Vpu::background.enabled = false;
	Vpu::overlay.enabled = true;
	printf("Downloading '%s'...\n", file);
	std::string filename = file;
	std::string src = Download::getServerUrl() + filename;
	std::string dst = "data\\" + filename;
	Download ds;
	
	/*
	INTERNET_CACHE_ENTRY_INFOA info;
	info.dwStructSize = sizeof(INTERNET_CACHE_ENTRY_INFOA);
	info.CacheEntryType = 0xFFFFFFFF;
	info.dwUseCount = 0;
	info.dwHitRate = 0;
	info.dwSizeLow = 0;
	info.dwSizeHigh = 0;
	DWORD size=1024;
	HANDLE cache = FindFirstUrlCacheEntryA(NULL, &info, &size);
	while (cache) {
		std::string status = "Cleaning cache ";
		status += info.lpszSourceUrlName;
		status = status.substr(0, status.size() - 1);
		status += "\n";
		printf(status.c_str());
		ds.draw(status.c_str());
		//DeleteUrlCacheEntryA(info.lpszSourceUrlName);
		if (!FindNextUrlCacheEntryA(cache, &info, &size))
		//	break;
	}
	*/
	ds.draw("Preparing download '"+filename+"'");
	URLDownloadToFileA(NULL, src.c_str(), dst.c_str(), 0, &ds);
	Vpu::console.enabled = true;
	Vpu::foreground.enabled = true;
	Vpu::background.enabled = true;
}



#define pythoncommand(name) static PyObject *name(PyObject *self, PyObject *args)
pythoncommand(blackbox_update) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	Engine::download("toc.py");
	return PyBool_FromLong(true);
}

pythoncommand(blackbox_download) {
	char* file;
	if (!PyArg_ParseTuple(args, "s", &file)) return NULL;
	Engine::download(file);
	return PyBool_FromLong(true);
}

pythoncommand(blackbox_version) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	return PyLong_FromLong(3);
}

pythoncommand(blackbox_epoch) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	return PyLong_FromLong(Engine::epoch);
}

pythoncommand(blackbox_ctrlc) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	if (InputDevice::control_c) {
		InputDevice::control_c = false;
		InputDevice::control = false;
		return PyBool_FromLong(1);
	}
	return PyBool_FromLong(0);
}

pythoncommand(blackbox_createinteger) {
	char* name;
	char* help = 0;
	int min = -65535;
	int max = 65535;
	int value = 0;
	if (!PyArg_ParseTuple(args, "s|iiis", &name, &value, &min, &max, &help)) return NULL;
	CVar::settings[name] = CVar::create<Integer>(
		name,
		help ? help : "",
		value,
		false
		);
	((Integer*)CVar::settings[name])->setMinMax(min, max);
	return PyLong_FromLong(CVar::settings[name]->getUUID());
}
pythoncommand(blackbox_createdecimal) {
	char* name;
	char* help = 0;
	float min = -65535;
	float  max = 65535;
	float value = 0;
	if (!PyArg_ParseTuple(args, "s|fffs", &name, &value, &min, &max, &help)) return NULL;
	CVar::settings[name] = CVar::create<Floating>(
		name,
		help ? help : "",
		value,
		false
		);
	((Floating*)CVar::settings[name])->setMinMax(min, max);
	return PyLong_FromLong(CVar::settings[name]->getUUID());
}

#include "dashboard.hpp"

pythoncommand(blackbox_addtitle) {
	char* name;
	char* description;
	char* developer;
	char* url;
	char* picture;
	char* genre;
	char* font;
	bool  multiplayer = false;
	bool  cooperative = false;
	bool  joystick = false;
	bool  mouse = false;
	bool  keyboard = false;
	int	  rating = 5;
	char* released = 0;
	char* parent = 0;

	if (!
		PyArg_ParseTuple(
			args,
			"sssssssbbbbbiss",
			&name, &description, &developer, &url, &picture, &genre, &font, &multiplayer, &cooperative, &joystick, &mouse, &keyboard, &rating, &released, &parent
		)
		) return NULL;
	Dashboard::addTitle(
		name,
		description,
		developer,
		url,
		picture,
		genre,
		font,
		multiplayer,
		cooperative,
		joystick,
		mouse,
		keyboard,
		rating,
		released ? released : "01-01-2022",
		parent
	);
	return PyBool_FromLong(true);
}

pythoncommand(blackbox_createboolean) {
	char* name;
	char* help = 0;
	int value = 0;
	if (!PyArg_ParseTuple(args, "s|is", &name, &value, &help)) return NULL;
	CVar::settings[name] = CVar::create<Boolean>(
		name,
		help ? help : "",
		value,
		false
		);
	return PyLong_FromLong(CVar::settings[name]->getUUID());
}

pythoncommand(blackbox_createstring) {
	char* name;
	char* help = 0;
	float  max_len = 65535;
	char* value = 0;
	if (!PyArg_ParseTuple(args, "s|sis", &name, &value, &max_len, &help)) return NULL;
	CVar::settings[name] = CVar::create<Text>(
		name,
		help ? help : "",
		value,
		false
		);
	((Text*)CVar::settings[name])->setMaxLength(max_len);
	return PyLong_FromLong(CVar::settings[name]->getUUID());
}


pythoncommand(blackbox_deletevar) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	CVar* cvar = CVar::findByUUID(handle);
	if (cvar) {
		std::map < std::string, CVar*>::iterator it;
		for (it = CVar::settings.begin(); it != CVar::settings.end(); it++) {
			if (it->second) {
				if (it->second->getUUID() == handle) {
					delete cvar;
					CVar::settings.erase(it);
					return PyBool_FromLong(true);
				}
			}
		}
	}
	Console::print("Variable not defined");
	return PyBool_FromLong(false);
}

pythoncommand(blackbox_findvar) {
	char* name;
	if (!PyArg_ParseTuple(args, "s", &name)) return NULL;
	if (CVar::settings[name])return PyLong_FromLong(CVar::settings[name]->getUUID());
	std::string out = "Variable ";
	out += name;
	out += " not defined";
	Console::print(out);
	return PyLong_FromLong(-1);
}

pythoncommand(blackbox_getvar) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	CVar* var = CVar::findByUUID(handle);
	if (var) {
		switch (var->getType()) {
		default:
		case CVAR_TEXT:		return Py_BuildValue("s", var->toString().c_str());
		case CVAR_INTEGER:	return PyLong_FromLong(-1);
		case CVAR_FLOATING: return PyFloat_FromDouble(((Floating*)var)->get());
		case CVAR_BOOLEAN:  return PyBool_FromLong(((Boolean*)var)->get());
		}
	}
	else {
		Console::print("Variable not defined");
	}
	return PyBool_FromLong(0);
}

pythoncommand(blackbox_setvar) {
	char* value;
	int handle;
	if (!PyArg_ParseTuple(args, "is", &handle, &value)) return NULL;
	CVar* var = CVar::findByUUID(handle);
	if (var) {
		var->parseValue(value);
		return PyBool_FromLong(1);
	}
	Console::print("Variable not defined");
	return PyBool_FromLong(0);
}

PyMethodDef Engine::methods[] = {
	{"addtitle"		, blackbox_addtitle		, METH_VARARGS, "addtitle(name,description,developer,url,picture,genre,font,multiplayer,cooperative,joystick,mouse,keyboard,rating,released,parent) : Add given title to the dashboard."},
	{"createboolean", blackbox_createboolean, METH_VARARGS, "createboolean(name,value,help) : Create a boolean variable and get handle"},
	{"createdecimal", blackbox_createdecimal, METH_VARARGS, "createdecimal(name,value,max_value,min_value,help) : Create a decimal variable and get handle"},
	{"createinteger", blackbox_createinteger, METH_VARARGS, "createinteger(name,value,max_value,min_value,help) : Create an integer variable and get handle"},
	{"createstring"	, blackbox_createstring	, METH_VARARGS, "createstring(name,placeholder,max_length,help) : Create a string variable and get handle"},
	{"ctrlc"		, blackbox_ctrlc		, METH_VARARGS, "ctrlc() : Returns TRUE if CTRL+C was pressed"},
	{"epoch"		, blackbox_epoch		, METH_VARARGS, "epoch() : Return current engine epoch uptime"},
	{"deletevar"	, blackbox_deletevar    , METH_VARARGS, "deletevar(var_handle) : Deletes variable by given variable handle"},
	{"download"		, blackbox_download		, METH_VARARGS, "download(filename) : Download file from current version repository."},
	{"findvar"		, blackbox_findvar		, METH_VARARGS, "findvar(var_name) : Find handle for the variable matching var_name"},
	{"getvar"		, blackbox_getvar		, METH_VARARGS, "getvar(var_handle) : Return value of the variable identified by var_handle"},
	{"setvar"		, blackbox_setvar		, METH_VARARGS, "setvar(var_handle,value) : Set value of the variable identified by var_handle"},
	{"update"		, blackbox_update		, METH_VARARGS, "update() : Update Table of Contents up to latest version."},
	{"version"		, blackbox_version		, METH_VARARGS, "version() : Return current BlackBox engine version"},
	{NULL, NULL, 0, NULL}
};
