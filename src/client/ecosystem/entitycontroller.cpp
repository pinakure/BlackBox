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

bool EntityInputController::parseValue(std::string name, std::string value) {
	return false;
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

bool EntityFollowController::parseValue(std::string name, std::string value) {
	return false;
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
	float dx = int(target_x - parent->x);
	float dy = int(target_y - parent->y);
	this->parent->angle = atan2(dx, dy) * -1;
	
	dx /= 16.0f;
	dy /= 16.0f;
	
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

void EntityMoveController::setDelta(float x, float y) {
	this->delta_x = x;
	this->delta_y = y;
}

void EntityMoveController::draw() {

}

bool EntityMoveController::parseValue(std::string name, std::string value) {
	return false;
}


/*---------------------------------------------------------------------------------*/

EntityShootController::EntityShootController(Entity* parent) : EntityController(parent) {
	this->reload_time	= 16.0f;
	this->reload_timer	= 0.0f;
	this->burst_time	= 8.0f;
	this->burst_timer	= 0.0f; 
	this->burst_size	= 5;
	this->burst_counter = 0;
	
	this->shoot			= false;
	this->projectile_speed = 0.125f;
	this->projectile_type  = 0;
	this->target_x = 0;
	this->target_y = 0;
	this->target_width  = 0;
	this->target_height = 0;
	this->range			= 64;
	this->target_entity = nullptr;
}

void EntityShootController::draw() {
	if (this->target_entity) {
		Vpu::color = al_map_rgba(
			(this->target_in_range ? 255 : 0),
			(this->shoot ? 255 : this->reloading ? 128 : 0),
			0,
			127
		);
		Vpu::circle(
			this->parent->x,
			this->parent->y,
			this->range>>1,
			Vpu::color.r * 255,
			Vpu::color.g * 255,
			Vpu::color.b * 255,
			Vpu::color.a * 255
		);
		Vpu::line(this->parent->x, this->parent->y, this->target_x, this->target_y);
	}
}


bool EntityShootController::parseValue(std::string name, std::string value) {
	return false;
}

void EntityShootController::update(double delta){
	#define min(a,b) (a<b?a:b)
	#define max(a,b) (a>b?a:b)
	if (target_entity) setTarget(target_entity);
	float x1 = min(target_x, parent->x);
	float x2 = max(target_x, parent->x);
	float y1 = min(target_y, parent->y);
	float y2 = max(target_y, parent->y);
	int distance = int((x2 - x1) + (y2 - y1));
	target_in_range = distance <= range;
	this->shoot = false;
	if(target_in_range) {
		int dx = int(target_x - parent->x);
		int dy = int(target_y - parent->y);
		if (reloading) {
			reload_time += 1.0f * delta;
			if (reload_time >= reload_timer) {
				reload_time = 0.0f;
				reloading = false;
			}
		} else {
			burst_time += 1.0f * delta;
			if (burst_time >= burst_timer) {
				this->shoot = true;
				burst_time = 0.0f;
				burst_counter++;
				if (burst_counter >= burst_size) {
					burst_counter = 0;
					reloading = true;
				}
			}
		}
		
		this->parent->angle = atan2(dx, dy) * -1;

		if (this->shoot) {
			distance *= this->projectile_speed;
			if (distance == 0)distance = 1;// avoid zero division!
			
			int w = this->parent->width >> 1;
			//Projectile.spawn(self.x, self.y, self.weapon_type, self.weapon_level, dx / distance, dy / distance, Foe)
		}
	}
	#undef min
	#undef max
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

bool EntityAvoidController::parseValue(std::string name, std::string value) {
	return false;
}

void EntityAvoidController::update(double delta) {
	// update avoid point if it is an entity
	if (this->target_entity)this->setTarget(this->target_entity);
	// calculate deltas
	const float min_distance = parent->width;
	float dx = int(target.x - parent->x);
	float dy = int(target.y - parent->y);
	float angle = atan2(-dx, -dy) * -1;
	dx = min_distance - dx;
	dy = min_distance - dy;
	
	if (abs(dx) > min_distance) dx = 0;
	if (abs(dy) > min_distance) dy = 0;
	// apply deltlas
	this->active = false;
	if ((abs(dx) > 0.0f) || (abs(dy) > 0.0f)) {
		this->active = true;
		this->parent->angle = angle;
		if (this->parent->controllers[CONTROLLER_FOLLOW]) {
			EntityFollowController* followctl = (EntityFollowController*)this->parent->controllers[CONTROLLER_FOLLOW];
			followctl->path.clear();
			followctl->target_x = followctl->target_x - (dx);
			followctl->target_y = followctl->target_y - (dy);
			followctl->path.push_back({ int(followctl->target_x) ,int(followctl->target_y) });
		}
		if (this->parent->controllers[CONTROLLER_MOVE]) {
			EntityMoveController* movectl = (EntityMoveController*)this->parent->controllers[CONTROLLER_MOVE];
			//movectl->delta_x -= dx / 256.0f;
			//movectl->delta_y -= dy / 256.0f;
			//movectl->delta_x *= 0.97f;
			//movectl->delta_y *= 0.97f;		
			movectl->delta_x = movectl->delta_x < -2.0f ? -2.0f : movectl->delta_x > 2.0f ? 2.0f : movectl->delta_x;
			movectl->delta_y = movectl->delta_y < -2.0f ? -2.0f : movectl->delta_y > 2.0f ? 2.0f : movectl->delta_y;
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

/*---------------------------------------------------------------------------------*/
EntityBounceController::EntityBounceController(Entity *parent) : EntityController(parent) {
	this->setBounds();
	this->acceleration_x = 0.0125f;
	this->acceleration_y = 0.0097f;
}

void EntityBounceController::setBounds() {
	Surface* srf = &Vpu::foreground;
	float surf_width  = srf->width;
	float surf_height = srf->height;
	real_width   = (surf_width / 2) * (srf->scale[0]/2);
	real_height  = (surf_height/ 2) * (srf->scale[1]/2);
	this->top    = (srf->height / 2) - (real_height / 2);
	this->left   = (srf->width  / 2) - (real_width  / 2);
	this->right  = this->left   + real_width;
	this->bottom = this->top    + real_height;
}

void EntityBounceController::bounce_left(double delta) {
	acceleration_x = abs(acceleration_x);
	delta_x = abs(delta_x);
	this->parent->x = this->left + (parent->width >> 1);
}
void EntityBounceController::bounce_right(double delta) {
	acceleration_x = -abs(acceleration_x);
	delta_x = -abs(delta_x);
	this->parent->x = this->right - (parent->width >> 1);
}
void EntityBounceController::bounce_top(double delta) {
	delta_y = abs(delta_y);
	this->parent->y = this->top + (parent->height >> 1);
}
void EntityBounceController::bounce_bottom(double delta) {
	//delta_y = -float(this->parent->width/4) / 2.5f;
	delta_y = -abs(delta_y);
	this->parent->y = this->bottom - (parent->height >> 1);
}

void EntityBounceController::checkObstacle(int left, int top, int right, int bottom) {
	int x = right - left;
	int y = bottom - top;
	int l, r;
	int t, b;
	int px, py;
	int dx, dy;
	dx = this->delta_x;
	dy = this->delta_y;
	t = this->parent->y - (this->parent->height >> 1) + this->delta_y;
	b = this->parent->y + (this->parent->height >> 1) + this->delta_y;
	// Check left
	r = this->parent->x + (this->parent->width >> 1) + this->delta_x;
	px = this->parent->x + dx;
	py = this->parent->y + dy;
	if ((px <= right) && (r >= left) && (py >= top) && (py <= bottom)) {
		this->acceleration_x	 = -abs(this->acceleration_x);
		this->delta_x			 = -abs(this->delta_x);
		this->parent->x = left - (this->parent->width >> 1);
	}
	// Check right
	l = this->parent->x - (this->parent->width >> 1) + this->delta_x;
	px = this->parent->x + dx;
	if ((px >= left) && (l <= right) && (py >= top) && (py <= bottom)) {
		this->acceleration_x	 = +abs(this->acceleration_x);
		this->delta_x			 = +abs(this->delta_x);
		this->parent->x = right + (this->parent->width >> 1);
	}
	// Check bottom
	t = this->parent->y - (this->parent->height >> 1) + this->delta_y;
	px = this->parent->x + dx;
	if ((py >= top) && (t <= bottom) && (px >= left) && (px <= right)) {
		this->delta_y		 	 = +abs(this->delta_y);
		this->parent->y = bottom + (this->parent->height >> 1)+1;
	}
	// Check top
	b = this->parent->y + (this->parent->height >> 1) + this->delta_y;
	py = this->parent->y + dy;
	if ((py <= bottom) && (b >= top) && (px >= left) && (px <= right)) {
		this->delta_y			 = -abs(this->delta_y);
		this->parent->y = top - (this->parent->height >> 1);
	}
	
}

void EntityBounceController::checkBoundaries(double delta, int left, int top, int right, int bottom) {
	if (this->parent->x > right	) this->bounce_right(delta);
	if (this->parent->x < left	) this->bounce_left(delta); 
	if (this->parent->y < top	) this->bounce_top(delta);
	if (this->parent->y > bottom) this->bounce_bottom(delta); 
}

void EntityBounceController::update(double delta) {
	// Check boundary bounce 
	checkBoundaries(
		delta,
		this->left + (parent->width >> 1),
		this->top + (parent->height >> 1),
		this->right - (parent->width >> 1),
		this->bottom - (parent->height >> 1)
	);
	
	// Check colission map bounce
	if (this->map) {
		int dx, dy;
		dy = 0;
		for (int y = 0; y < this->map->h; y++) {
			dx = 0;
			for (int x = 0; x < this->map->w; x++) {
				if (this->map->layers[0].get(x, y) != 0x0) {
					checkObstacle(
						this->left + dx,
						this->top  + dy,
						this->left + dx + this->map->tile_width,
						this->top  + dy + this->map->tile_height
					);					
				}
				dx += this->map->tile_width;
			}
			dy += this->map->tile_height;
		}
	}

	/* Apply delta acceleration */
	delta_y += acceleration_y;
	delta_x += acceleration_x;
	
	/* Limit delta acceleration */
	float max_speed = 0.5f;
	if (delta_x >  max_speed)delta_x =  max_speed;
	if (delta_x < -max_speed)delta_x = -max_speed;
	
	if (parent->controllers[CONTROLLER_MOVE]) {
		EntityMoveController* movectl = (EntityMoveController*)parent->controllers[CONTROLLER_MOVE];
		movectl->delta_x = delta_x;
		movectl->delta_y = delta_y;
	}
}

void EntityBounceController::setDelta(float x, float y) {
	if (x < 0) acceleration_x = -abs(acceleration_x);
	if (y < 0) acceleration_y = -abs(acceleration_y);
	if (x > 0) acceleration_x = abs(acceleration_x);
	if (y > 0) acceleration_y = abs(acceleration_y);
	this->delta_x = x;
	this->delta_y = y;
}

void EntityBounceController::draw() {
	Vpu::color = al_map_rgba(255, 64, 0, 128);
	Vpu::rectangle(left + 1, top, right - left - 1, bottom - top - 1);
	Vpu::rectangle(left + 2, top + 1, right - left - 3, bottom - top - 3);
	Vpu::select(Vpu::foreground);
	/*
	if (this->map) {
		int dx, dy;
		dy = 0;
		for (int y = 0; y < this->map->h; y++) {
			dx = 0;
			for (int x = 0; x < this->map->w; x++) {
				if (this->map->layers[0].get(x, y) != 0x0) {
					Vpu::fillRectangle(
						this->left + dx,
						this->top + dy,
						this->map->tile_width,
						this->map->tile_height,
						255, 0, 0, 8
					);
				}
				dx += this->map->tile_width;
			}
			dy += this->map->tile_height;
		}
	}
	*/
}

bool EntityBounceController::parseValue(std::string name, std::string value) {
	if	    (name == "left"	 ) this->left   = atof(value.c_str());
	else if (name == "right" ) this->right  = atof(value.c_str());
	else if (name == "top"	 ) this->top    = atof(value.c_str());
	else if (name == "bottom") this->bottom = atof(value.c_str());
	else if (name == "accelx") this->acceleration_x = atof(value.c_str());
	else if (name == "accely") this->acceleration_y = atof(value.c_str());
	else if (name == "deltay") this->delta_y = atof(value.c_str());
	else if (name == "deltax") this->delta_y = atof(value.c_str());
	else if (name == "map"	 ) this->map = TiledMap::find(atoi(value.c_str()));
	//else if (name == "maxdeltax") this->max_delta_x = atof(value.c_str());
	//else if (name == "maxdeltay") this->max_delta_y = atof(value.c_str());
	else return false;
	return true;
}
