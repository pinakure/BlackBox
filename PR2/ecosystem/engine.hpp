#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

class Engine {
private:
public:
	static ALLEGRO_TIMER *timer;
	static ALLEGRO_EVENT_QUEUE *queue;
	static ALLEGRO_DISPLAY *display;
	static ALLEGRO_FONT *font;
	static ALLEGRO_EVENT event;
	static bool run;
	static bool redraw;
	static bool initialize();
	static void deinitialize();
	static void loop();
	static void render();
	static void handleEvents();
};

#endif