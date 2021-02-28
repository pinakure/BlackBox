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
	Sprite(const Sprite& o, int left, int top, int right, int bottom);
	Sprite(const char *filename=nullptr);
	void draw(int x, int y);
};

#endif 
