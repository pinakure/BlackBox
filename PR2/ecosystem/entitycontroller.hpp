#ifndef __ENTITYCONTROLLER_HPP
#define __ENTITYCONTROLLER_HPP

#include <string>
#include <map>
#include <vector>
#include "input.hpp"
class Entity;

typedef struct s_PathPoint {
	int x;
	int y;
}PathPoint;

class EntityController {
public:
	typedef enum E_Type {
		CONTROLLER_INPUT  = 0x00,
		CONTROLLER_FOLLOW = 0x01,
		CONTROLLER_MOVE   = 0x02,
		CONTROLLER_SHOOT  = 0x03,
		CONTROLLER_AVOID  = 0x04,
		CONTROLLER_MAX    = 0x05,
	}Type;

	float time		= 0.0f;
	Entity* parent	= nullptr;
	EntityController(Entity* parent=nullptr);
	virtual void update(double delta)=0;
	virtual void draw() = 0;
};

class EntityInputController : public EntityController {
public:
	bool a			= false;
	bool b			= false;
	bool x			= false;
	bool y			= false;
	bool l1			= false;
	bool l2			= false;
	bool l3			= false;
	bool r1			= false;
	bool r2			= false;
	bool r3			= false;
	bool start		= false;
	bool select		= false;
	bool menu		= false;
	bool up			= false;
	bool down		= false;
	bool left		= false;
	bool right		= false;
	float lhat[2]	= { 0, 0 };
	float rhat[2]	= { 0, 0 };
	InputDevice* input = nullptr;

	EntityInputController(Entity* parent);
	void update(double delta);
	void draw();
};

class EntityFollowController : public EntityController {
public:
	std::vector<PathPoint> path;
	int target_x			= 0;		// Target position to move to(x coor)
	int target_y			= 0;		// Target position to move to(y coord)
	int target_width		= 16;
	int target_height		= 16;
	Entity *target_entity	= nullptr;	// If set, entity's towards the path will converge to
	float acceleration		= 1.0f;

	EntityFollowController(Entity* parent);
	void generatePath();
	void nextTarget();
	void setTarget(Entity* target);
	void setTarget(int x, int y);
	void update(double delta);
	void draw();
};

class EntityAvoidController : public EntityController {
public:
	Entity* target_entity = nullptr;	// If set, entity's towards the path will converge to
	PathPoint target;
	bool active = false;
	EntityAvoidController(Entity* parent);
	void update(double delta);
	void draw();
	void setTarget(Entity* target);
	void setTarget(int x, int y);
};

class EntityMoveController : public EntityController {
public:
	float delta_x = 0.0f;
	float delta_y = 0.0f;
	float decay_x = 1.0f;
	float decay_y = 1.0f;
	bool  wrap_x  = false;
	bool  wrap_y  = false;
	int   top	  = 0;	// Movement space top boundary
	int   left    = 0;	// Movement space left boundary
	int   right   = 0;	// Movement space right boundary
	int   bottom  = 0;	// Movement space bottom boundary
		
	EntityMoveController(Entity* parent);
	void update(double delta);
	void draw();
};

class EntityShootController : public EntityController {
public:
	int reload_time			= 16;		// Time between shot bursts
	int reload_timer		= 0;		// Time elapsed since last bust
	int burst_size			= 5;		// Rounds shot in each burst
	int burst_counter		= 0;		// Rounds shots since burst start
	int burst_time			= 8;		// Time between shots
	int burst_timer			= 0;		// Time elapsed since last shot
	Entity *target_entity	= nullptr;	// If set, entity's coordinates override target_x and target_y
	int target_x			= 0;		// Target position to shoot at(x coor)
	int target_y			= 0;		// Target position to shoot at(y coord)
	int target_width		= 16;
	int target_height		= 16;
	
	EntityShootController(Entity* parent);
	void setTarget(Entity* target);
	void setTarget(int x, int y);
	void update(double delta);
	void draw();
};


#endif 