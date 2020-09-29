#ifndef __SPRITE_HPP
#define __SPRITE_HPP

#include "vpu.hpp"

class Sprite {
private:
	bool initialized = false;

public:
	int x = 0;
	int y = 0;
	int width	= 0; 
	int height	= 0;
	Surface picture;
	bool isInitialized() { return this->initialized; }
	void deinitialize();
	Sprite(const char *filename=nullptr);
};

#endif 
