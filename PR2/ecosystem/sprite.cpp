#include "sprite.hpp"
#include "engine.hpp"

Sprite::Sprite(const char *filename) {	
	if (filename != nullptr) {
		std::string file("data/sprites/");
		file.append(filename);
		file.append(".png");
		filename = file.c_str();
		this->picture = Vpu::loadBitmap(filename);
		if (!this->picture.enabled) {
			std::printf("ERROR: Cannot load sprite file '%s'.\n", filename);
			return;
		} 
		this->width = this->picture.width;
		this->height= this->picture.height;
		this->initialized = true;		
	}

}

void Sprite::deinitialize() {
	if (this->picture.enabled) {
		Vpu::destroySurface(this->picture);
	}
	initialized = false;
}
