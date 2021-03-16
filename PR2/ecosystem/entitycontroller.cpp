#include "entitycontroller.hpp"

EntityController::EntityController(Entity* parent) {
	this->parent = parent;
	this->time = 0;
}

/*---------------------------------------------------------------------------------*/

EntityInputController::EntityInputController(Entity* parent) : EntityController(parent) {
	InputDevice d;
	this->input = &d;
}

void EntityInputController::update(double delta) {
	this->a = this->input->controller[INPUT_CIRCLE];
	this->b = this->input->controller[INPUT_CROSS];
	this->x = this->input->controller[INPUT_TRIANGLE];
	this->y = this->input->controller[INPUT_SQUARE];
	this->l1 = this->input->controller[INPUT_L1];
	this->l2 = this->input->controller[INPUT_L2];
	this->l3 = this->input->controller[INPUT_L3];
	this->r1 = this->input->controller[INPUT_R1];
	this->r2 = this->input->controller[INPUT_R2];
	this->r3 = this->input->controller[INPUT_R3];
	this->start = this->input->controller[INPUT_START];
	this->select = this->input->controller[INPUT_SELECT];
	this->menu = this->input->controller[INPUT_START] && this->input->controller[INPUT_SELECT];
	this->up = this->input->controller[INPUT_UP];
	this->down = this->input->controller[INPUT_DOWN];
	this->left = this->input->controller[INPUT_LEFT];
	this->right = this->input->controller[INPUT_RIGHT];
	this->lhat[0] = this->input->axis_x[0];
	this->lhat[1] = this->input->axis_y[0];
	this->rhat[0] = this->input->axis_x[1];
	this->rhat[1] = this->input->axis_y[1];
}

void EntityInputController::draw() {

}

/*---------------------------------------------------------------------------------*/

EntityFollowController::EntityFollowController(Entity* parent) : EntityController(parent) {
	this->path			= std::vector<PathPoint>();
	this->target_x = Vpu::foreground.width  >> 1;	// Target position to move to(x coor)
	this->target_y = Vpu::foreground.height >> 1;	// Target position to move to(y coord)
	this->target_width =  16;// Target position to move to(x coor)
	this->target_height = 16;// Target position to move to(y coord)
	this->target_entity = nullptr;	// If set, entity's towards the path will converge to
	this->acceleration  = 1.0f;
}

void EntityFollowController::setTarget(Entity* target) {
	this->target_entity = target;
	this->target_width = target->width;
	this->target_height = target->height;
	this->target_x = target->x;
	this->target_y = target->y;
}

void EntityFollowController::setTarget(int x, int y) {
	this->target_entity = nullptr;
	this->target_width  = 16;
	this->target_height = 16;
	this->target_x = x;
	this->target_y = y;
}

void EntityFollowController::draw() {
	std::vector<PathPoint>::iterator it = this->path.begin();
	int ox = this->parent->x;
	int oy = this->parent->y;
	while (it != this->path.end()) {
		Vpu::color = al_map_rgba(64, 64, 64, 8);
		Vpu::line(ox, oy, it->x, it->y);
		ox = it->x;
		oy = it->y;
		Vpu::color = al_map_rgba(200, 200, 0, 8);
		Vpu::putpixel(ox, oy);
		it++;
	}
	Vpu::color = al_map_rgba(128, 0, 0, 64);
	Vpu::line(this->parent->x, this->parent->y, this->target_x, this->target_y);
	if (this->target_entity) {
		Vpu::color = al_map_rgba(255, 0, 0, 64);
		int ww = this->target_width >> 1;
		int hh = this->target_height >> 1;
		Vpu::line(this->target_entity->x - ww, this->target_entity->y - hh, this->target_entity->x + ww, this->target_entity->y + hh);
		Vpu::line(this->target_entity->x - ww, this->target_entity->y + hh, this->target_entity->x + ww, this->target_entity->y - hh);
		Vpu::line(this->parent->x, this->parent->y, this->target_entity->x, this->target_entity->y);
	}
}

void EntityFollowController::generatePath() {
	const int	min_steps	= 8;
	const int	max_steps	= 16;
	const int   mult		= 64;
	bool alt_x = false;
	bool alt_y = false;
	float x = this->parent->x;
	float y = this->parent->y;
	if (!this->target_entity) {
		this->target_x = (Vpu::foreground.width  >> 1) + (rand() % Game::width ) - (Game::width  >> 1);
		this->target_y = (Vpu::foreground.height >> 1) + (rand() % Game::height) - (Game::height >> 1);
		this->target_width = 32;
		this->target_height = 32;
	} else {
		this->target_x = this->target_entity->x;
		this->target_y = this->target_entity->y;
		this->target_width = this->target_entity->width;
		this->target_height = this->target_entity->height;
	}
	float delta_x = 0; // - (x - target.x) / 10
	float delta_y = 0; // - (y - target.y) / 10
	int _top, _left, _right, _bottom;
	/*if (this->parent->controllers[EntityController::CONTROLLER_MOVE]) {
		EntityMoveController* movectl = (EntityMoveController*)(&(this->parent->controllers[EntityController::CONTROLLER_MOVE]));
		_top    = movectl->top;
		_left   = movectl->left;
		_right  = movectl->right;
		_bottom = movectl->bottom;
	} else {*/
		_top	= 0;
		_left	= 0;
		_right	= Game::width;
		_bottom = Game::height;
	//}
	int limit = (rand() % (max_steps - min_steps)) + min_steps;
	limit = max_steps;// <<----------------------------------------------  debug!!!!
	for (int i = 0; i < limit; i++) {
		delta_x += (alt_x ? (mult + (rand() % mult)) * int((rand() % 3) - 1) : -(x - target_x)) / 50.0f;
		delta_y += (alt_y ? (mult + (rand() % mult)) * int((rand() % 3) - 1) : -(y - target_y)) / 50.0f;
		alt_x ^= 1;
		alt_y ^= 1;
		x = x + (delta_x);
		y = y + (delta_y);
		delta_x *= 0.9f;
		delta_y *= 0.9f;
		
		int left	= target_x - (target_width >> 1);
		int right	= target_x + (target_width >> 1);
		int top		= target_y - (target_height >> 1);
		int bottom	= target_y + (target_height >> 1);
		if(		 x >= left   && x <= target_x) x += 32;
		else if( x <= right  && x >= target_x) x -= 32;
		if(		 y >= top	 && y <= target_y) y += 32;
		else if( y <= bottom && y >= target_y) y -= 32;
		/*
		if(		 x < _left   ) x += 64;
		else if( x > _right  ) x -= 64;
		if(		 y < _top    ) y += 64;
		else if( y > _bottom ) y -= 64;
		*/
		this->path.push_back({ (int)x,(int)y });
	}
	PathPoint pp = this->path[0];
	this->target_x = pp.x;
	this->target_y = pp.y;
}

void EntityFollowController::nextTarget() {
	if (this->path.size() > 0) {// <-- Murphy's law failsafe cover check...
		PathPoint pp = this->path[0];
		this->target_x = pp.x;
		this->target_y = pp.y;
		this->path.erase(this->path.begin());
	}
}

void EntityFollowController::update(double delta){
	float dx = ((float(this->target_x) - this->parent->x)/16.0f);
	float dy = ((float(this->target_y) - this->parent->y)/16.0f);
	printf("dx: %f - dy: %f\r", dx, dy);
	if (this->path.size() == 0) {
		this->generatePath();
	}
	
	if (abs(dx)+abs(dy) < 2.0f) {
		this->nextTarget();
	}
	
	if (this->parent->controllers[CONTROLLER_MOVE]) {
		EntityMoveController* movectl = (EntityMoveController*)this->parent->controllers[CONTROLLER_MOVE];
		movectl->delta_x += dx / 64.0f;
		movectl->delta_y += dy / 64.0f;		
	}	
}

/*---------------------------------------------------------------------------------*/

EntityMoveController::EntityMoveController(Entity* parent) : EntityController(parent) {
	this->delta_x	= 0.0f;
	this->delta_y	= 0.0f;
	this->decay_x	= 0.97f;
	this->decay_y	= 0.97f;
	this->wrap_x	= false;
	this->wrap_y	= false;
	this->top		= 0;
	this->left		= 0;
	this->right		= Game::width;
	this->bottom	= Game::height;
}

void EntityMoveController::update(double delta){
	// scale delta with engine's delta value
	float dx = this->delta_x;
	float dy = this->delta_y;
	// calculate next position
	float fx = this->parent->x + dx;
	float fy = this->parent->y + dy;
	// delta decay
	this->delta_x *= this->decay_x;
	this->delta_y *= this->decay_y;
	// boundary check
	float w = this->parent->width  >> 1;
	float h = this->parent->height >> 1;
	float vw = this->right - this->left;
	float vh = this->bottom - this->top;
	// Stop on boundaries or wrap around axis
	/*
	if		(fx - w < this->left	) fx = this->wrap_x ? fx + vw : w;
	else if (fx + w > this->right	) fx = this->wrap_x ? fx - vw : (this->right - w);
	if		(fy - h < this->top		) fy = this->wrap_y ? fy + vh : h;
	else if (fy + h > this->bottom	) fy = this->wrap_y ? fy - vh : (this->bottom - h);
	*/
	this->parent->x = fx;
	this->parent->y = fy;
}

void EntityMoveController::draw() {

}

/*---------------------------------------------------------------------------------*/

EntityShootController::EntityShootController(Entity* parent) : EntityController(parent) {
	this->reload_time	= 16;
	this->reload_timer	= 0;
	this->burst_size	= 5;
	this->burst_counter = 0;
	this->burst_time	= 8;
	this->burst_timer	= 0;
	this->target_x = 0;
	this->target_y = 0;
	this->target_width  = 0;
	this->target_height = 0;
	this->target_entity = nullptr;
}

void EntityShootController::draw() {
}

void EntityShootController::update(double delta){
	// aim and shoot...
}

void EntityShootController::setTarget(Entity* target) {
	this->target_entity = target;
	this->target_width = target->width;
	this->target_height = target->height;
	this->target_x = target->x;
	this->target_y = target->y;
}

void EntityShootController::setTarget(int x, int y) {
	this->target_entity = nullptr;
	this->target_width = 16;
	this->target_height = 16;
	this->target_x = x;
	this->target_y = y;
}

/*---------------------------------------------------------------------------------*/

EntityAvoidController::EntityAvoidController(Entity* parent) : EntityController(parent) {
	this->target.x = 0;
	this->target.y = 0;
	this->target_entity = nullptr;
}

void EntityAvoidController::draw() {
	if (!this->target_entity || !this->active)return;
	Vpu::color = al_map_rgba(0, 128, 255, 32);
	Vpu::line(this->parent->x, this->parent->y, this->target.x, this->target.y);
}

void EntityAvoidController::update(double delta) {
	// update avoid point if it is an entity
	if (this->target_entity)this->setTarget(this->target_entity);
	// calculate deltas
	const float min_distance = 32.0f;
	float dx = min_distance - ((float(this->target.x) - this->parent->x));
	float dy = min_distance - ((float(this->target.y) - this->parent->y));
	if (abs(dx) > min_distance) dx = 0;
	if (abs(dy) > min_distance) dy = 0;
	// apply deltlas
	this->active = false;
	if ((abs(dx) > 0.0f) || (abs(dy) > 0.0f)) {
		this->active = true;
		if (this->parent->controllers[CONTROLLER_FOLLOW]) {
			EntityFollowController* followctl = (EntityFollowController*)this->parent->controllers[CONTROLLER_FOLLOW];
			followctl->path.clear();
		}
		if (this->parent->controllers[CONTROLLER_MOVE]) {
			EntityMoveController* movectl = (EntityMoveController*)this->parent->controllers[CONTROLLER_MOVE];
			movectl->delta_x -= dx / 128.0f;
			movectl->delta_y -= dy / 128.0f;			
		}
	}
}

void EntityAvoidController::setTarget(Entity* target) {
	this->target_entity = target;
	this->target.x = target->x;
	this->target.y = target->y;
}

void EntityAvoidController::setTarget(int x, int y) {
	this->target_entity = nullptr;
	this->target.x = x;
	this->target.y = y;
}
