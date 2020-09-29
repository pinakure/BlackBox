#include "animation.hpp"
#include "direction.hpp"

// TODO: Port every call to use only Vpu methods, remove allegro include from animation

Animation::Animation() {
	this->width = 0;
	this->height = 0;
}

Animation::Animation(Sprite &sprite, int width, int height, int sx, int sy, int dx, int dy, bool vertical) {
	this->width  = width;
	this->height = height;
	this->extractFrames(sprite, sx, sy, dx, dy, vertical);
}

int Animation::extractFrames(Sprite &sprite, int source_x, int source_y, int dest_x, int dest_y, bool read_vertically) {
	#define DOTHEWORK	 	ALLEGRO_BITMAP *bitmap = al_create_bitmap(this->width, this->height);											\
							al_set_target_bitmap(bitmap);																					\
							al_draw_bitmap_region(sprite.picture.bitmap, x*this->width, y*this->height, this->width, this->height, 0, 0, 0);\
							this->frame.push_back(bitmap);	
	if (read_vertically) {
		for (int y = source_y; y <= dest_y; y++) {
			for (int x = source_x; x <= dest_x;x++) {
				ALLEGRO_BITMAP *bitmap = al_create_bitmap(this->width, this->height);											
				al_set_target_bitmap(bitmap);																					
				al_draw_bitmap_region(sprite.picture.bitmap, x*this->width, y*this->height, this->width, this->height, 0, 0, 0);
				this->frame.push_back(bitmap);	
			}
		}
	} else {
		for (int x = source_x; x <= dest_x;x++) {
			for (int y = source_y; y <= dest_y; y++) {
				ALLEGRO_BITMAP *bitmap = al_create_bitmap(this->width, this->height);											
				al_set_target_bitmap(bitmap);																					
				al_draw_bitmap_region(sprite.picture.bitmap, x*this->width, y*this->height, this->width, this->height, 0, 0, 0);
				this->frame.push_back(bitmap);	
	
			}
		}
	}
	#undef DOTHEWORK
	return 0;
}

bool Animation::run(double delta){
	switch (this->direction) {
		default:
		case Direction::UP:
			this->current_frame += abs(delta);
			if (this->current_frame > this->frame.size() - 1)
				if (this->bidirectional) {
					this->current_frame = this->frame.size() - 1;
					this->direction = Direction::DOWN;
				} else {
					this->current_frame = 0;
				}
			break;
		case Direction::DOWN:
			this->current_frame -= abs(delta);
			if (this->current_frame < 0)				
				if (this->bidirectional) {
					this->current_frame = 0;
					this->direction = Direction::UP;
				} else {
					this->current_frame = this->frame.size()-1;					
				}
			break;
	}	
	if (last_frame != this->current_frame) {
		last_frame = this->current_frame;
		return true;
	}
	return false;
}

void Animation::draw(Surface &surface, int x, int y, ALLEGRO_COLOR &color) {
	al_set_target_bitmap(surface.bitmap);
	if(this->frame.size() > 0)
		al_draw_tinted_bitmap(this->frame[this->current_frame],color, x, y,  0);
}

