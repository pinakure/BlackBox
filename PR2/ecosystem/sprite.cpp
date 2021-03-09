#include "sprite.hpp"
#include "engine.hpp"
#include "vpu.hpp"

Sprite::Sprite(const Sprite& o, int left, int top, int right, int bottom) {
	int w = right - left;
	int h = bottom - top;
	this->picture = Vpu::createSurface(w, h);
	Vpu::select(this->picture);
	Vpu::paint(0,0,0,0);	
	Vpu::drawSurface(((Sprite*)&o)->picture, left, top, w, h, 0, 0);
	this->initialized = true;
}

Sprite::Sprite(const char *filename) {	
	if (filename != nullptr) {
		std::string file("sprites/");
		file.append(filename);
		file.append(".png");
		filename = file.c_str();
		this->picture = Vpu::loadBitmap(filename);
		if (!this->picture.enabled) {
			std::printf("ERROR: Cannot load sprite file '%s'.\n", filename);
			return;
		} 
		this->initialized = true;		
	}
}

void Sprite::deinitialize() {
	if (this->picture.enabled) {
		Vpu::destroySurface(this->picture);
	}
	initialized = false;
}

void Sprite::draw(int x, int y) {
	/*Vpu::drawSurface(this->picture, 
		0, 0, 
		this->picture.width, this->picture.height,
		x, y
	);
	*/
	Vpu::drawSurfaceRotated(this->picture, x, y, this->picture.rotation[0]);
}