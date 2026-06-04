#ifndef _FONT_HPP
#define _FONT_HPP

#include <string>
#include "surface.hpp"
#include "sprite.hpp"
#include "animation.hpp"

class Font {
public:	
	ALLEGRO_FONT* data = NULL;
	std::string name;
	int height;
	Font(std::string filename, int size = 12);
	~Font();
};

#endif
