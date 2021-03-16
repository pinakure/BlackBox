#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "sprite.hpp"
#include <string>
#include <vector>
#include <limits>
#include "entity.hpp"

class Engine {
private:
public:
	static ALLEGRO_TIMER *timer;
	static ALLEGRO_TIMER *clock;
	static ALLEGRO_EVENT_QUEUE *queue;
	static ALLEGRO_EVENT_QUEUE *clock_queue;
	static ALLEGRO_EVENT event;
	static std::vector<Sprite> sprites;
	static Surface* target;
	static int  cycles;
	static long int epoch;
	static int	width;
	static int  height;
	static bool run;
	static bool initialize();
	static void deinitialize();
	static void loop();
	static void tick();
	static void update();
	static void render();
	static void handleEvents();
	static void error(std::string text);
	static void print(std::string text);
	static void printf(const char *fmt, ...);

	static std::vector<Entity*> entities;
	static void destroyEntities();

	static void download(const char *file);


	static void scale(float, float, float);
	static void rotate(float);
	static float _scale[3];
	static float _rotation;
};

#define TRUERAND(a) (((float(rand())/float(RAND_MAX)) * float(a)))

#define ENGINE_FPS					60

#endif