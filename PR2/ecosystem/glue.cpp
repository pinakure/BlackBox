Surface *getLayer(int index) {
	if (!Vpu::is_initialized) {
		printf("vpu is not initialized.\nYou must call vpu.restart() before operating.\n");
		return NULL;
	}
	if (index >= 3) {
		if(Vpu::surfaces.find(index)!=Vpu::surfaces.end())
			return &(Vpu::surfaces.at(index-3));
		else index %= 3;
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

pythoncommand(vpu_fading) {
	if(!PyArg_ParseTuple(args, "")) return NULL;
	return PyBool_FromLong(Vpu::fade_level==Vpu::fade_target_level);
}

pythoncommand(vpu_textout) {
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
pythoncommand(vpu_line) {
	int x;
	int y;
	int dx;
	int dy;
	if (!PyArg_ParseTuple(args, "iiii", &x, &y, &dx, &dy)) return NULL;
	Vpu::line(x, y, dx, dy);
	return PyBool_FromLong(true);
}
pythoncommand(vpu_rect) {
	int x;
	int y;
	int dx;
	int dy;
	if (!PyArg_ParseTuple(args, "iiii", &x, &y, &dx, &dy)) return NULL;
	Vpu::rectangle(x, y, dx, dy);
	return PyBool_FromLong(true);
}
pythoncommand(vpu_fillrect) {
	int x;
	int y;
	int dx;
	int dy;
	if (!PyArg_ParseTuple(args, "iiii", &x, &y, &dx, &dy)) return NULL;
	Vpu::qfillRectangle(x, y, dx, dy);
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
	return PyLong_FromLong(handle+3);
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
pythoncommand(vpu_deletesprite){
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Vpu::deallocateSprite(handle);
	return PyLong_FromLong(1);
}
pythoncommand(vpu_createsprite){
	int priority = 0;
	char* filename;
	if (!PyArg_ParseTuple(args, "s|i", &filename, &priority)) return NULL;
	long int handle = Vpu::allocateSprite(filename, priority);
	return PyLong_FromLong(handle);
}
pythoncommand(vpu_drawsprite){
	int handle;
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "iii", &handle, &x, &y)) return NULL;
	Vpu::drawSprite(Vpu::sprites.at(handle), x, y);
	return PyLong_FromLong(1);	
}
pythoncommand(vpu_deleteanim){
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Vpu::deallocateAnimation(handle);
	return PyLong_FromLong(1);
}
pythoncommand(vpu_createanim){
	int width;
	int height;
	int sprite;
	int dx=0;
	int dy=0;
	int sx=1;
	int sy=0;
	bool vertical=false;
	int flags = 0;
	if(!PyArg_ParseTuple(args, "iii|iiiib", &width, &height, &sprite, &sx, &sy, &dx, &dy, &vertical)) return NULL;
	long int handle = Vpu::allocateAnimation(width, height, Vpu::sprites.at(sprite), sx, sy, dx, dy, vertical);
	return PyLong_FromLong(handle);
}
pythoncommand(vpu_drawanim){
	int handle;
	int x;
	int y;
	if(!PyArg_ParseTuple(args, "iii", &handle, &x, &y)) return NULL;
	Vpu::drawAnimation(Vpu::animations.at(handle), x, y);
	return PyLong_FromLong(1);	
}
pythoncommand(vpu_dimensions){
	if(!PyArg_ParseTuple(args, "")) return NULL;
	int dims[2] = { Vpu::target->width, Vpu::target->height };
	
	PyObject* list = PyList_New(2);
	if (!list) throw("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < 2; i++) {
		PyObject *num = PyFloat_FromDouble( (double) dims[i]);
		if (!num) {
			Py_DECREF(list);
			throw("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(list, i, num);
	}
	return list;	
}
pythoncommand(vpu_fill){
	int r = -1;
	int g = -1;
	int b = -1;
	int a = -1;
	if(!PyArg_ParseTuple(args, "|rgba", &r, &g, &b, &a)) return NULL;
	Vpu::paint(r, g, b, a);
	return PyLong_FromLong(1);	
}
pythoncommand(vpu_fadein){
	int r = -1;
	int g = -1;
	int b = -1;
	if(!PyArg_ParseTuple(args, "|iii", &r, &g, &b)) return NULL;	
	Vpu::fadein(r,g,b);
    return PyLong_FromLong(1);
}
pythoncommand(vpu_fadeout){
	int r = -1;
	int g = -1;
	int b = -1;
	if (!PyArg_ParseTuple(args, "|iii", &r, &g, &b)) return NULL;
	Vpu::fadeout(r,g,b);
	return PyLong_FromLong(1);
}
pythoncommand(vpu_frames){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Vpu::total_frames);
}
/* BlackBox ----------------------------------------------------------------------------- */
pythoncommand(blackbox_version){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(3);
}

pythoncommand(blackbox_epoch){
	if(!PyArg_ParseTuple(args, "")) return NULL;	
    return PyLong_FromLong(Engine::epoch);
}

pythoncommand(blackbox_ctrlc) {
	if(!PyArg_ParseTuple(args, "")) return NULL;
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
	char * value=0;
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
			if(it->second) {
				if (it->second->getUUID() == handle) {
					delete cvar;
					CVar::settings.erase(it);
					return PyBool_FromLong(true);
				}
			}
		}
	} else {
		Console::print("Variable not defined");
		return PyBool_FromLong(false);
	}
}

pythoncommand(blackbox_findvar) {
	char *name;
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
	} else {
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

/* TypeWriter --------------------------------------------------------------------------- */
pythoncommand(typewriter_ready) {
	return PyLong_FromLong(TypeWriter::next);
}

pythoncommand(typewriter_setposition) {
	int x;
	int y;
	if (!PyArg_ParseTuple(args, "ii", &x, &y)) return NULL;
	TypeWriter::x = x;
	TypeWriter::y = y;
	return PyLong_FromLong(1);
}
pythoncommand(typewriter_setcolor) {
	int r = -1;
	int g = -1;
	int b = -1;
	int a = -1;
	if (!PyArg_ParseTuple(args, "iii|i", &r, &g, &b, &a)) return NULL;
	TypeWriter::r = r;
	TypeWriter::g = g;
	TypeWriter::b = b;
	TypeWriter::a = a==-1?0:a;
	return PyLong_FromLong(1);
}

pythoncommand(typewriter_prompt) {
	char* placeholder=NULL;
	if (!PyArg_ParseTuple(args, "|s", &placeholder)) return NULL;
	if (GetTextBox::status == GetTextBox::STATUS_DISABLED) {
		TypeWriter::clearTextBox(16, placeholder ? placeholder : "");
		GetTextBox::status = GetTextBox::STATUS_ENABLED;
	}
	return PyLong_FromLong(1);
}

pythoncommand(typewriter_gettext) {
	if (GetTextBox::status == GetTextBox::STATUS_FINISHED) {
		std::string ret = GetTextBox::text;
		GetTextBox::status = GetTextBox::STATUS_DISABLED;
		GetTextBox::text == "";
		return Py_BuildValue("s", ret.c_str());
	}
	return PyLong_FromLong(0); 
}

pythoncommand(typewriter_setfont) {
	int font_index;
	if (!PyArg_ParseTuple(args, "i", &font_index)) return NULL;
	//TypeWriter::font_index = font_index %= TypeWriter::fonts.size();
	return PyLong_FromLong(1);
}

pythoncommand(typewriter_setsize) {
	int width;
	int height;
	if (!PyArg_ParseTuple(args, "ii", &width, &height)) return NULL;
	TypeWriter::final_width = width;
	TypeWriter::final_height = height;
	return PyLong_FromLong(1);
}

pythoncommand(typewriter_addchoice) {
	char* name;
	char* value;
	if (!PyArg_ParseTuple(args, "ss", &name, &value)) return NULL;
	TypeWriter::choices.insert(
		std::pair < std::string, std::string>(name, value)
	);
	return PyLong_FromLong(1);
}

enum Variables {
	VARIABLE_TYPEWRITER_R,
	VARIABLE_TYPEWRITER_G,
	VARIABLE_TYPEWRITER_B,
	VARIABLE_TYPEWRITER_A,
	VARIABLE_TYPEWRITER_X,
	VARIABLE_TYPEWRITER_Y,
	VARIABLE_TYPEWRITER_FONT,
};

pythoncommand(typewriter_clear) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	TypeWriter::options.clear();
	TypeWriter::choices.clear();
	return PyLong_FromLong(1);
}

pythoncommand(typewriter_addoption) {
	char* caption;
	int variable_index;
	if (!PyArg_ParseTuple(args, "si", &caption, &variable_index)) return NULL;
	CVar* var = CVar::findByUUID(variable_index);
	if (var) {
		TypeWriter::options.insert(std::pair < std::string, CVar*>(caption, var));
		return PyBool_FromLong(1);
	}
	return PyBool_FromLong(0);
}

pythoncommand(typewriter_getchoice) {
	char* question = NULL;
	if (!PyArg_ParseTuple(args, "|s",&question)) 
		return NULL;
	if (question != NULL)TypeWriter::question = question;
	return Py_BuildValue("s", TypeWriter::getAnswer().c_str());
}

pythoncommand(typewriter_enqueue) {
	char* buffer;
	int wait = 0;
	if (!PyArg_ParseTuple(args, "s|i", &buffer, &wait)) return NULL;
	TypeWriter::enqueue(buffer);
	TypeWriter::wait_time = wait * ENGINE_FPS;
	return PyLong_FromLong(1);
}
pythoncommand(typewriter_loadpic) {
	char *buffer;
	int x=0;
	int y=0;
	int w=0;
	int h=0;
	if(!PyArg_ParseTuple(args, "s|iiii", &buffer, &x, &y, &w, &h)) return NULL;	
	TypeWriter::loadPicture(buffer, x,y, w,h);
	return PyLong_FromLong(1);
}
pythoncommand(typewriter_clearpic) {	
	if(!PyArg_ParseTuple(args, "")) return NULL;	
	TypeWriter::clearPicture();
	return PyLong_FromLong(1);
}

pythoncommand(tbi) {
	Console::print("To Be Implemented");
	return PyLong_FromLong(1);
}

/* Console methods ---------------------------------------------------------------------- */
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
/* Enlazar python{ function() }---> C++{ function(PyObject*, PyObject) } ---------------- */
static PyMethodDef ConsoleMethods[] = {
    {"echo"			, console_print			, METH_VARARGS, "console.echo(text) : Dump given text over console"},
    {"cls"			, console_cls			, METH_VARARGS, "console.cls() : Clean console buffer"},
	{NULL, NULL, 0, NULL}
};

/* Typewriter methods ---------------------------------------------------------------------- */

static PyMethodDef TypeWriterMethods[] = {
	/* TBI */
	{"addchoice"    , typewriter_addchoice  , METH_VARARGS, "addchoice(name, value) : Add a pair of name: value to the available choice array"},
	{"addoption"    , typewriter_addoption	, METH_VARARGS, "addchoice(name, value) : Add a pair of name: pointer to integer variable to the available option array"},
	{"clear"	    , typewriter_clear		, METH_VARARGS, "clear() : Clear option and choice list"},
	{"clearpic"		, typewriter_clearpic	, METH_VARARGS, "typewriter.clearpic() : Remove picture from typewriter overlay"},
	{"enqueue"		, typewriter_enqueue	, METH_VARARGS, "typewriter.enqueue(text) : Enqueue message into typewriter buffer and open if if closed"},
	{"getchoice"    , typewriter_getchoice  , METH_VARARGS, "typewriter.answer() : Returns last answer, or empty string if no answer was given at last choice list"},
	{"gettext"		, typewriter_gettext	, METH_VARARGS, "typewriter.gettext() : Polls text input dialog upon finished is activated, the it returns text"},
	{"loadpic"		, typewriter_loadpic	, METH_VARARGS, "typewriter.loadpic(picfilename, x=0, y=0, w=0, h=0)  : Load picture from file to typewriter overlay"},
	{"prompt"		, typewriter_prompt		, METH_VARARGS, "typewriter.prompt(placeholder='') : Opens prompt dialog optionally presenting given placeholder text"},
	{"ready"		, typewriter_ready		, METH_VARARGS, "ready() : Returns true once user pressed next or close button"},
	{"setposition"	, typewriter_setposition, METH_VARARGS, "typewriter.setposition(x, y) : Set the origin point for the typewriter box "},
	{"setcolor"		, typewriter_setcolor   , METH_VARARGS, "typewriter.setcolor(r=-1, g=-1, b=-1, a=-1) : Change  typewriter box color scheme"},
	{"setfont"		, typewriter_setfont    , METH_VARARGS, "typewriter.setfont(font_handle) : Switch font to be used in typewriter box"},
	{"setsize"		, typewriter_setsize    , METH_VARARGS, "typewriter.setsize(w, h) : Set new dimensions for the typewriter box"},
	{NULL, NULL, 0, NULL}
};

/* Engine internal methods ---------------------------------------------------------------------- */

static PyMethodDef BlackBoxMethods[] = {
	{"createboolean", blackbox_createboolean, METH_VARARGS, "blackbox.createboolean(name,value,help) : Create a boolean variable and get handle"},
	{"createdecimal", blackbox_createdecimal, METH_VARARGS, "blackbox.createdecimal(name,value,max_value,min_value,help) : Create a decimal variable and get handle"},
	{"createinteger", blackbox_createinteger, METH_VARARGS, "blackbox.createinteger(name,value,max_value,min_value,help) : Create an integer variable and get handle"},
	{"createstring"	, blackbox_createstring	, METH_VARARGS, "blackbox.createstring(name,placeholder,max_length,help) : Create a string variable and get handle"},
	{"ctrlc"		, blackbox_ctrlc		, METH_VARARGS, "blackbox.ctrlc() : Returns TRUE if CTRL+C was pressed"},
	{"deletevar"	, blackbox_deletevar    , METH_VARARGS, "blackbox.deletevar(var_handle) : Deletes variable by given variable handle"},
	{"epoch"		, blackbox_epoch		, METH_VARARGS, "blackbox.epoch() : Return current engine epoch uptime"},
	{"findvar"		, blackbox_findvar		, METH_VARARGS, "blackbox.findvar(var_name) : Find handle for the variable matching var_name"},
	{"getvar"		, blackbox_getvar		, METH_VARARGS, "blackbox.getvar(var_handle) : Return value of the variable identified by var_handle"},
	{"setvar"		, blackbox_setvar		, METH_VARARGS, "blackbox.setvar(var_handle,value) : Set value of the variable identified by var_handle"},
	{"version"		, blackbox_version		, METH_VARARGS, "blackbox.version() : Return current BlackBox engine version"},
	{NULL, NULL, 0, NULL}
};

/* Video Engine internal methods ---------------------------------------------------------------------- */

static PyMethodDef VpuMethods[] = {
	{"createanim"	, vpu_createanim		, METH_VARARGS, "vpu.createanim(width, height, sprite_handle) : Return Handle to new Animation object " },
	{"createsprite"	, vpu_createsprite		, METH_VARARGS, "vpu.createsprite(filename) : Returns Handle to Sprite object create upon given filename" },
	{"createsurf"	, vpu_createsurf		, METH_VARARGS, "vpu.createsurf(width, height) : Returns Handle to Surface object to be drawn arbitrarily to screen" },
	{"deleteanim"	, vpu_deleteanim		, METH_VARARGS, "vpu.deleteanim(handle) : Delete Animation identified by given Handle" },
	{"deletefont"	, tbi					, METH_VARARGS, "vpu.deletefont(font_handle) : Delete Font identified by given Handle"},
	{"deletesprite"	, vpu_deletesprite		, METH_VARARGS, "vpu.deletesprite(handle) : Delete Sprite identified by given Handle" },
	{"deletesurf"	, vpu_deletesurf		, METH_VARARGS, "vpu.deletesurf(handle) : Delete Surface identified by given Handle" },
	{"dimensions"	, vpu_dimensions		, METH_VARARGS, "vpu.dimensions() : Returns selected bitmap [ width, height ] "},
	{"disable"		, vpu_disable			, METH_VARARGS, "vpu.disable(layer) : Toggle off given vpu layer"},
	{"drawanim"		,   vpu_drawanim		, METH_VARARGS, "vpu.drawanim(handle, x, y) : Draw animation identified by given handle onto given coordinates" },
	{"drawsprite"	, vpu_drawsprite		, METH_VARARGS, "vpu.drawsprite(handle, x, y) : Draw Sprite identified by given handle onto given coordinates" },
	{"drawsurf"		, vpu_drawsurf			, METH_VARARGS, "vpu.drawsurf(handle, x, y) : Draw surface identified by given handle onto given coordinates" },
	{"enable"		, vpu_enable			, METH_VARARGS, "vpu.enable(layer) : Toggle on  given vpu layer"},
	{"fadein"		, vpu_fadein			, METH_VARARGS, "vpu.fadein() : Fade screen from black"},
	{"fadeout"		, vpu_fadeout			, METH_VARARGS, "vpu.fadeout() : Fade screen to black"},
	{"fading"		, vpu_fading			, METH_VARARGS, "vpu.fading() : Return True is fade in / fade out is activated" },
	{"fill"			, vpu_fill				, METH_VARARGS, "vpu.fill(r, g, b, a) : Fill selected bitmap with given color or default color"},
	{"fillrect"		, vpu_fillrect			, METH_VARARGS, "vpu.filrect(x, y, dx, dy) : Draw a filled rectangle onto selected surface from x,y to dx,dy" },
	{"frames"		, vpu_frames			, METH_VARARGS, "vpu.frames() : Return actual frame count"},
	{"fullscreen"	, vpu_fullscreen		, METH_VARARGS, "vpu.fullscreen(enabled) : Toggle fullscreen mode"},
	{"rect"			, vpu_rect				, METH_VARARGS, "vpu.rect(x, y, dx, dy) : Draw a rectangle onto selected surface from x,y to dx,dy" },
	{"line"			, vpu_line				, METH_VARARGS, "vpu.line(x, y, dx, dy) : Draw a line onto selected surface from x,y to dx,dy" },
	{"loadfont"		, tbi					, METH_VARARGS, "vpu.loadfont(filename, size) : Loads a font from given filemane and prerenders it at given size "},
	{"pset"			, vpu_pset				, METH_VARARGS, "vpu.pset(x, y) : Draw a pixel onto selected surface onto given coordinates" },
	{"restart"		, vpu_restart			, METH_VARARGS, "vpu.restart() : Restart Video Processing Unit"},
	{"rotate"		, vpu_rotate			, METH_VARARGS, "vpu.rotate(layer, angle) : Rotate specified layer (0-11) given degrees"},
	{"select"		, vpu_select			, METH_VARARGS, "vpu.select(layer) : Select given layer to perform next graphic operations onto it"},
	{"setcolor"		, vpu_setcolor			, METH_VARARGS, "vpu.setcolor(r, g, b, a) : Sets current painting color"},
	{"setfont"		, tbi					, METH_VARARGS, "vpu.setfont(font_handle) : Sets current font to the one identified by given handle"},
	{"setrotation"	, vpu_setrotation		, METH_VARARGS, "vpu.setrotation(layer, angle) : Sets rotation for specified layer (0-11) at given degrees"},
	{"setscale"		, vpu_setscale			, METH_VARARGS, "vpu.setscale(layer, scale_x, scale_y) : Sets scale for specified layer [0-11] given horizontal and vertical values"},
	{"scale"		, vpu_scale				, METH_VARARGS, "vpu.scale(layer, scale_x, scale_y) : Change specified layer [0-11] given horizontal and vertical scale factor"},
	{"textout"		, vpu_textout			, METH_VARARGS, "vpu.textout(text, x, y) : Print given text at given coordinates"},
	{"update"		, vpu_update			, METH_VARARGS, "vpu.update() : Allow blackbox engine to perform its rendering based input and output operations"},
	{NULL, NULL, 0, NULL}
};

/* ----------------------------------------------------------------------
	Definir Modulos
---------------------------------------------------------------------- */
static PyModuleDef BlackBoxModule	= {PyModuleDef_HEAD_INIT, "blackbox"	, NULL, -1, BlackBoxMethods	  , NULL, NULL, NULL, NULL};
static PyModuleDef VpuModule		= {PyModuleDef_HEAD_INIT, "vpu"			, NULL, -1, VpuMethods		  , NULL, NULL, NULL, NULL};
static PyModuleDef ConsoleModule	= {PyModuleDef_HEAD_INIT, "console"		, NULL, -1, ConsoleMethods	  , NULL, NULL, NULL, NULL};
static PyModuleDef TypeWriterModule	= {PyModuleDef_HEAD_INIT, "typewriter"	, NULL, -1, TypeWriterMethods , NULL, NULL, NULL, NULL};

static PyObject *PyInit_blackbox(void){ return PyModule_Create(&BlackBoxModule); }
static PyObject *PyInit_vpu(void){ return PyModule_Create(&VpuModule); }
static PyObject *PyInit_console(void){ return PyModule_Create(&ConsoleModule); }
static PyObject *PyInit_typewriter(void){ return PyModule_Create(&TypeWriterModule); }

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
	for (PyMethodDef &d : TypeWriterMethods) {
		if (!d.ml_name)continue;
		Console::addHelp(d.ml_name, d.ml_doc);
	}
}