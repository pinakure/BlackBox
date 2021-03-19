#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include "entitycontroller.hpp"
#include "script.hpp"

class Game {
public:
	static int   width;
	static int   height;
	static float scale;
};

class Entity {
public:
	static PyMethodDef methods[];
	std::string name	= "";
	float		x		= 0;
	float		y		= 0;
	int			width	= 0;
	int			height	= 0;
	int			time	= 0;
	float		angle	= 0.0f;
	float		real_angle= 0.0f;
	bool		enabled = true;
	std::map<int, EntityController*> controllers;
	//std::map<std::string, EntityProperty> properties;
	
	//std::vector<Sprite*> sprites;
	Sprite* sprite = nullptr;
	//std::vector<Animation*> animations;
	Animation* animation = nullptr;

	Entity(int width = 16, int height = 16, std::string name = "UnnamedEntity");
	void update(double delta);
	void addController(EntityController::Type type);
	void addProperty(std::string name, ...);
	
	void draw();
	/*
	void addIntegerProperty(std::string name, ...);
	void addFloatProperty(std::string name, ...);
	void addStringProperty(std::string name, ...);
	void addColorProperty(std::string name, ...);
	*/
	static PyObject* pyCreate(PyObject* self, PyObject* args);
	static PyObject* pyEnable(PyObject* self, PyObject* args);
	static PyObject* pyAddSprite(PyObject* self, PyObject* args);
	static PyObject* pyAddAnimation(PyObject* self, PyObject* args);
	static PyObject* pyDelete(PyObject* self, PyObject* args);
	static PyObject* pyDraw(PyObject* self, PyObject* args);
	static PyObject* pyDisable(PyObject* self, PyObject* args);
	static PyObject* pyGetPosition(PyObject* self, PyObject* args);
	static PyObject* pyGetDelta(PyObject* self, PyObject* args);
	static PyObject* pySetAnimation(PyObject* self, PyObject* args);
	static PyObject* pySetParameter(PyObject* self, PyObject* args);
	static PyObject* pySetDelta(PyObject* self, PyObject* args);
	static PyObject* pySetSprite(PyObject* self, PyObject* args);
	static PyObject* pySetTarget(PyObject* self, PyObject* args);
	static PyObject* pySetPosition(PyObject* self, PyObject* args);
	static PyObject* pyUpdate(PyObject* self, PyObject* args);
	static PyObject* pyAddController(PyObject* self, PyObject* args);
};

#endif