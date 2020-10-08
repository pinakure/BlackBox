#ifndef __SPRITE_HPP
#define __SPRITE_HPP

#include "surface.hpp"

class Sprite {
private:
	bool initialized = false;

public:
	Surface picture;
	bool isInitialized() { return this->initialized; }
	void deinitialize();
	Sprite(const char *filename=nullptr);
	void draw(int x, int y);
};

#endif 
