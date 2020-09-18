#ifndef __HUD_HPP
#define __HUD_HPP

#include "typewriter.hpp"

class Hud {
private:
public:
	static bool initialize();
	static void draw();
	static void drawDebugInfo();
	static void update();
};

#endif
