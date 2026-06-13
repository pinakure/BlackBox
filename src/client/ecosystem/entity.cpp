#include "entity.hpp"
#include "entitycontroller.hpp"
#include "engine.hpp"

int		Game::width		= 320;
int		Game::height	= 240;
float	Game::scale		= 1.0f;

PyMethodDef Entity::methods[] = {
	{ "create"			, Entity::pyCreate			, METH_VARARGS, "create(width, height, name) : Create a generic entity width given size and name"},
	{ "enable"			, Entity::pyEnable			, METH_VARARGS, "enable(handle) : Create a generic entity width given size and name" },
	{ "addsprite"		, Entity::pyAddSprite		, METH_VARARGS, "addsprite(entity_handle, sprite_handle) : " },
	{ "addanimation"	, Entity::pyAddAnimation	, METH_VARARGS, "addanimation(entity_handle, anim_handle) : " },
	{ "delete"			, Entity::pyDelete			, METH_VARARGS, "delete(entity_handle) : " },
	{ "draw"			, Entity::pyDraw			, METH_VARARGS, "draw(handle) : " },
	{ "disable"			, Entity::pyDisable			, METH_VARARGS, "disable(handle) : " },
	{ "getposition"		, Entity::pyGetPosition		, METH_VARARGS, "setposition(entity_handle) : " },
	{ "getdelta"		, Entity::pyGetDelta		, METH_VARARGS, "getdelta() : " },
	{ "setanimation"	, Entity::pySetAnimation	, METH_VARARGS, "setanimation(entity_handle, anim_handle) : " },
	{ "setrendertarget"	, Entity::pySetRenderTarget	, METH_VARARGS, "setrendertarget(entity_handle, surface_handle) : " },
	{ "parameter"		, Entity::pySetParameter	, METH_VARARGS, "parameter(entity_handle, parameter_name, value) : " },
	{ "setdelta"		, Entity::pySetDelta		, METH_VARARGS, "setdelta(entity_handle, delta_x, delta_y, controller_type) : " },
	{ "setsprite"		, Entity::pySetSprite		, METH_VARARGS, "setsprite(entity_handle, sprite_handle) : " },
	{ "settarget"		, Entity::pySetTarget		, METH_VARARGS, "settarget(handle, target_handle, controller_type) : " },
	{ "setposition"		, Entity::pySetPosition		, METH_VARARGS, "setposition(entity_handle, x, y) : " },
	{ "update"			, Entity::pyUpdate			, METH_VARARGS, "update(entity, delta) : " },
	{ "addcontroller"	, Entity::pyAddController	, METH_VARARGS, "addcontroller(entity, controller_type) : " },
	{NULL, NULL, 0, NULL}
};

Entity::Entity(int width, int height, std::string name) {
	this->name   = name;
	this->width  = width;
	this->height = height;
	this->controllers = std::map<int, EntityController*>();
	this->x = Vpu::foreground.width >> 1;
	this->y = Vpu::foreground.height >> 1;
	this->rendertarget = &Vpu::foreground;
}

#define _USE_MATH_DEFINES
#include <math.h>

void Entity::update(double delta) {
	if (!this->enabled) return;
	std::map<int, EntityController*>::iterator it;
	for (it = this->controllers.begin(); it != this->controllers.end(); it++) {		
		if(it->second)
			(*it->second).update(delta);
	}
	/*
	int ra = (int((real_angle / M_PI) * 180.0f) + 180)%360;
	int a = (int((angle / M_PI) * 180.0f) + 180)%360;
	float distance_cw  = abs(a - ra);
	float distance_ccw = abs(ra- a);
	printf("RA,A: %04d - %04d | Distances: %f - %f			\r", ra, a, distance_cw, distance_ccw);
	bool clockwise = distance_cw <= distance_ccw;
	if (clockwise && real_angle != angle) real_angle -= 0.05f;
	else if(real_angle != angle) real_angle += 0.05f;	
	*/
	real_angle = angle;
	this->time++;
}


void Entity::addController(EntityController::Type type) {
	type = EntityController::Type(int(type) % int(EntityController::Type::CONTROLLER_MAX));
	switch (type) {
		case EntityController::Type::CONTROLLER_FOLLOW:	this->controllers[(int)type] = new EntityFollowController(this); return;
		case EntityController::Type::CONTROLLER_AVOID:	this->controllers[(int)type] = new EntityAvoidController(this); return;
		case EntityController::Type::CONTROLLER_BOUNCE:	this->controllers[(int)type] = new EntityBounceController(this); return;
		case EntityController::Type::CONTROLLER_MOVE:	this->controllers[(int)type] = new EntityMoveController(this); return;
		case EntityController::Type::CONTROLLER_SHOOT:	this->controllers[(int)type] = new EntityShootController(this); return;
		case EntityController::Type::CONTROLLER_INPUT:	this->controllers[(int)type] = new EntityInputController(this); return;
	}
}

#include "vpu.hpp"
void Entity::draw() {
	if (!this->enabled) return;
	//Vpu::printf(this->x - 4, this->y - 6, 0, "E %s", this->name.c_str());
	//Vpu::print("E", this->x - 4, this->y - 6, 0);
	if (this->sprite		) Vpu::drawSurfaceRotated(this->sprite->picture, this->x, this->y, this->real_angle);
	else if(this->animation	) Vpu::drawAnimationRotated(*(this->animation), this->x, this->y, this->real_angle);
	else Vpu::rectangle(this->x - (this->width >> 1), this->y - (this->height >> 1), this->width, this->height);
	//Vpu::fillRectangle(this->x - (this->width >> 1), this->y - (this->height >> 1), this->width, this->height, 0, 64, 0, 16);

	std::map<int, EntityController*>::iterator it;
	for (it = this->controllers.begin(); it != this->controllers.end(); it++) {
		if (it->second)
			(*it->second).draw();
	}
	
}

void Entity::addProperty(std::string name, ...) {

}

PyObject* Entity::pyCreate(PyObject* self, PyObject* args) {
	int width = 16;
	int height = 16;
	char* name = nullptr;
	if (!PyArg_ParseTuple(args, "|iis", &width, &height, &name)) return NULL;
	Engine::entities.push_back(new Entity(width, height, name ? name : "UnnamedEntity"));
	return PyLong_FromLong((long)Engine::entities.size() - 1);
}

PyObject* Entity::pyDelete(PyObject* self, PyObject* args) {
	int handle = 16;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	//Engine::entities[handle]->enabled = false;
	return PyLong_FromLong((long)Engine::entities.size() - 1);
}

PyObject* Entity::pyEnable(PyObject* self, PyObject* args) {
	int handle = 16;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Engine::entities[handle]->enabled = true;
	return PyLong_FromLong(1);
}

PyObject* Entity::pyDisable(PyObject* self, PyObject* args) {
	int handle = 16;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Engine::entities[handle]->enabled = false;
	return PyLong_FromLong(1);
}

PyObject* Entity::pyUpdate(PyObject* self, PyObject* args) {
	double delta;
	long handle;
	if (!PyArg_ParseTuple(args, "if", &handle, &delta)) return NULL;
	Engine::entities[handle]->update(delta);
	return PyBool_FromLong(1);
}

PyObject* Entity::pySetTarget(PyObject* self, PyObject* args) {
	long handle_entity;
	long handle_target;
	int type;
	if (!PyArg_ParseTuple(args, "iii", &handle_entity, &handle_target, &type)) return NULL;
	Entity* src = Engine::entities[handle_entity];
	Entity* dst = Engine::entities[handle_target];
	if (src && dst) {
		switch (type) {
		#define Controller(a) (src->controllers[EntityController::CONTROLLER_##a])
			case EntityController::CONTROLLER_AVOID: ((EntityAvoidController*)Controller(AVOID))->setTarget(dst); break;
			case EntityController::CONTROLLER_FOLLOW:((EntityFollowController*)Controller(FOLLOW))->setTarget(dst); break;
			case EntityController::CONTROLLER_SHOOT: ((EntityShootController*)Controller(SHOOT))->setTarget(dst); break;
		#undef HasController
		}
	}
	return PyBool_FromLong(1);
}

PyObject* Entity::pySetDelta(PyObject* self, PyObject* args) {
	long  handle_entity;
	float x;
	float y;
	int   type;
	if (!PyArg_ParseTuple(args, "iffi", &handle_entity, &x, &y, &type)) return NULL;
	Entity* src = Engine::entities[handle_entity];
	if (src) {
		switch (type) {
		#define Controller(a) (src->controllers[EntityController::CONTROLLER_##a])
			case EntityController::CONTROLLER_BOUNCE: ((EntityBounceController*)Controller(BOUNCE))->setDelta(x, y); break;
			case EntityController::CONTROLLER_MOVE:((EntityMoveController*)Controller(MOVE))->setDelta(x, y); break;
		#undef HasController
		}
	}
	return PyBool_FromLong(1);
}

PyObject* Entity::pyAddSprite(PyObject* self, PyObject* args) {
	long entity_handle;
	long sprite_handle;
	int type;
	if (!PyArg_ParseTuple(args, "ii", &entity_handle, &sprite_handle, &type)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	Sprite* spr = &Vpu::sprites[sprite_handle];
	if (ent && spr) {
		//ent->sprites.push_back(spr);
		//return PyLong_FromLong((long)ent->sprites.size() - 1);
	}
	return PyLong_FromLong(-1);
}

PyObject* Entity::pyAddAnimation(PyObject* self, PyObject* args) {
	long entity_handle;
	long anim_handle;
	if (!PyArg_ParseTuple(args, "ii", &entity_handle, &anim_handle)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	Animation* ani = &Vpu::animations[anim_handle];
	if (ent && ani) {
		//ent->animations.push_back(ani);
		//return PyLong_FromLong((long)ent->animations.size() - 1);
	}
	return PyLong_FromLong(-1);
}

PyObject* Entity::pySetSprite(PyObject* self, PyObject* args) {
	long entity_handle;
	long sprite_handle;
	int type;
	if (!PyArg_ParseTuple(args, "ii", &entity_handle, &sprite_handle, &type)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	Sprite* spr = &Vpu::sprites[sprite_handle];
	if (ent && spr) {
		ent->sprite = spr;
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}

PyObject* Entity::pySetAnimation(PyObject* self, PyObject* args) {
	long entity_handle;
	long anim_handle;
	if (!PyArg_ParseTuple(args, "ii", &entity_handle, &anim_handle)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	Animation* ani = &Vpu::animations[anim_handle];
	if (ent && ani) {
		ent->animation = ani;
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}

bool Entity::parseValue(int controller_type, std::string name, std::string value) {
	EntityController* ctl = this->controllers[controller_type];
	switch (controller_type) {
		case EntityController::CONTROLLER_AVOID:  return ((EntityAvoidController* )ctl)->parseValue(name, value);
		case EntityController::CONTROLLER_BOUNCE: return ((EntityBounceController*)ctl)->parseValue(name, value);
		case EntityController::CONTROLLER_FOLLOW: return ((EntityFollowController*)ctl)->parseValue(name, value);
		case EntityController::CONTROLLER_INPUT:  return ((EntityInputController* )ctl)->parseValue(name, value);
		case EntityController::CONTROLLER_MOVE:   return ((EntityMoveController*  )ctl)->parseValue(name, value);
		case EntityController::CONTROLLER_SHOOT:  return ((EntityShootController* )ctl)->parseValue(name, value);		
		default: 
			printf("ParseValue: NOT IMPLEMENTED");
			return false;
	}
}

extern Surface* getLayer(int index);


PyObject* Entity::pySetRenderTarget(PyObject* self, PyObject* args) {
	long entity_handle;
	long surface_handle;
	if (!PyArg_ParseTuple(args, "ii", &entity_handle, &surface_handle)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	if (ent) ent->rendertarget = getLayer(surface_handle);
	return False;
}

PyObject* Entity::pySetParameter(PyObject* self, PyObject* args) {
	long entity_handle;
	char* parameter = 0;
	int controller_type = -1;
	char *value = 0;
	if (!PyArg_ParseTuple(args, "iiss", &entity_handle, &controller_type , &parameter, &value)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	if ((ent) && (ent->parseValue(controller_type, parameter, value))) return True;
	return False;
}

PyObject* Entity::pySetPosition(PyObject* self, PyObject* args) {
	long entity_handle;
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "iff", &entity_handle, &x, &y)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	if (ent) {
		ent->x = x;
		ent->y = y;
		return PyBool_FromLong(1);
	}
	return PyBool_FromLong(0);
}

PyObject* Entity::pyGetPosition(PyObject* self, PyObject* args) {
	long entity_handle;
	if (!PyArg_ParseTuple(args, "i", &entity_handle)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	if (ent) {
		PyObject* list = PyList_New(2);
		Py_INCREF(list);
		if (!list) throw("Unable to allocate memory for Python list");
		PyList_SET_ITEM(list, 0, PyFloat_FromDouble((double)ent->x));
		PyList_SET_ITEM(list, 1, PyFloat_FromDouble((double)ent->y));
		Py_DECREF(list);
		return list;
	}
	return PyBool_FromLong(0);
}

PyObject* Entity::pyGetDelta(PyObject* self, PyObject* args) {
	long entity_handle;
	if (!PyArg_ParseTuple(args, "i", &entity_handle)) return NULL;
	Entity* ent = Engine::entities[entity_handle];
	if (ent) {
		PyObject* list = PyList_New(2);
		if (!list) throw("Unable to allocate memory for Python list");
		Py_INCREF(list);
		PyList_SET_ITEM(list, 0, PyFloat_FromDouble((double)((EntityMoveController*)(ent->controllers[EntityController::CONTROLLER_MOVE]))->delta_x));
		PyList_SET_ITEM(list, 1, PyFloat_FromDouble((double)((EntityMoveController*)(ent->controllers[EntityController::CONTROLLER_MOVE]))->delta_y));
		Py_DECREF(list);
		return list;
	}
	return PyBool_FromLong(0);
}

PyObject* Entity::pyDraw(PyObject* self, PyObject* args) {
	long handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	Engine::entities[handle]->draw();
	return PyBool_FromLong(1);
}

PyObject* Entity::pyAddController(PyObject* self, PyObject* args) {
	long handle;
	int type;
	if (!PyArg_ParseTuple(args, "ii", &handle, &type)) return NULL;
	Entity* e = Engine::entities[handle];
	e->addController((EntityController::Type)type);
	return PyLong_FromLong((long)e->controllers.size() - 1);
}


















