#ifndef __VPU_HPP
#define __VPU_HPP

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

class Vpu {
private:
	static ALLEGRO_DISPLAY *display;
	static ALLEGRO_FONT *font;
public:
	static int frames;
	static int fps;
	static int width;
	static int height;
	static bool redraw;
	static bool initialize();
	static void deinitialize();
	static void render();
};

#endif
