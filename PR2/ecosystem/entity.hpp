#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include "entitycontroller.hpp"

class Game {
public:
	static int   width;
	static int   height;
	static float scale;
};

class Entity {
public:
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
};

class ExampleEntity : public Entity {
public:
	ExampleEntity(std::string name = "UnnamedExampleEntity");
	void update(double delta);
};


#endif