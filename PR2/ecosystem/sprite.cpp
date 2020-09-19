#include "sprite.hpp"

Sprite::Sprite(const char *filename) {
	if (filename != nullptr) {
		this->picture = Vpu::loadBitmap(filename);
		if (this->picture.enabled) {
			this->width = this->picture.width;
			this->height= this->picture.height;
			this->initialized = true;
		}
	}
}

