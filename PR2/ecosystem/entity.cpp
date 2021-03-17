#include "entity.hpp"
#include "entitycontroller.cpp"
#include "engine.hpp"

int		Game::width		= 320;
int		Game::height	= 240;
float	Game::scale		= 1.0f;

Entity::Entity(int width, int height, std::string name) {
	this->name   = name;
	this->width  = width;
	this->height = height;
	this->controllers = std::map<int, EntityController*>();
	this->x = Vpu::foreground.width >> 1;
	this->y = Vpu::foreground.height >> 1;
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
	else Vpu::rectangle(this->x - (this->width >> 1), this->y - (this->height >>1), this->width, this->height);

	std::map<int, EntityController*>::iterator it;
	for (it = this->controllers.begin(); it != this->controllers.end(); it++) {
		if (it->second)
			(*it->second).draw();
	}
	
}

void Entity::addProperty(std::string name, ...) {

}


ExampleEntity::ExampleEntity(std::string name) : Entity(32,32,name) {
	this->addController(EntityController::Type::CONTROLLER_INPUT);
	this->addController(EntityController::Type::CONTROLLER_MOVE);
	this->addController(EntityController::Type::CONTROLLER_FOLLOW);
	this->addController(EntityController::Type::CONTROLLER_SHOOT);
}

void ExampleEntity::update(double delta) {

}