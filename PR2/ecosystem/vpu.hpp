#ifndef __VPU_HPP
#define __VPU_HPP

#include <string>
#include <vector>
#include <map>
#include "surface.hpp"
#include "sprite.hpp"
#include "animation.hpp"

#define VPU_OVERSCAN 0

class Vpu {
private:
	static ALLEGRO_FONT					*font;
	static ALLEGRO_FONT					*legacy_font;
	static ALLEGRO_COLOR				shadow;
	static ALLEGRO_COLOR				transparent;
	static std::vector<ALLEGRO_COLOR>	color_stack;
public:
	static Surface						*target;	
	static ALLEGRO_BITMAP				*buffer;	
	static ALLEGRO_COLOR				color;
	static ALLEGRO_DISPLAY				*display;
	static bool							is_initialized;
	static bool							fullscreen;	
	static long int						animation_handle;
	static long int						surface_handle;
	static long int						sprite_handle;
	static std::map<long int, Surface>	surfaces;
	static std::map<long int, Animation>animations;
	static std::map<long int, Sprite>	sprites;
	static Surface						overlay;
	static Surface						background;
	static Surface						foreground;
	static Surface						console;
	static int							pixel_format;
	static int							scroll[2];
	static int							frames;
	static int							fps;
	static int							width;
	static int							height;
	static bool							redraw;
	static unsigned long int			total_frames;
	
	// Initialization and base routines
	static bool initialize();
	static void initializeFonts();
	static void deinitialize();
	static void render();
	static bool start();
	static bool restart();
	static void loadVars();

	// Text output
	static void printf(int  x, int y, int flags, const char *fmt, ...);//slow!
	static void print			(std::string text, int  x, int y, int flags=0);
	static void printInteger	(std::string text, int d, int  x, int y, int flags=0);
	static void printFloat		(std::string text, float d, int  x, int y, int flags=0);
	static void printDouble		(std::string text, double d, int  x, int y, int flags=0);
	static void printBool		(std::string text, bool d, int  x, int y, int flags=0);

	// Primitives
	static void rectangle(int x, int y, int width, int height);
	static void qfillRectangle(int x, int y, int width, int height);
	static void fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha = 255);
	static void fillSquircle(int x, int y, int width, int height, int r, int g, int b, int alpha = 255);
	static void fillCircle(int x, int y, float radius, int r, int g, int b, int alpha=255);
	static void circle(int x, int y, float radius, int r, int g, int b, int alpha=255);
	static void paint(int r, int g, int b, int alpha = 255);
	static void putpixel(int x, int y);
	static void line(int x, int y, int dx, int dy);

	
	// Color routines
	static void setColor(int r, int g, int b, int alpha = 255);
	static void setColor(ALLEGRO_COLOR color);
	static void pushColor();
	static void popColor();
	static ALLEGRO_COLOR alter(ALLEGRO_COLOR _color, float qr=1.0f, float qg=1.0f, float qb=1.0f, float qa=1.0f);

	// Surface manipulation
	static bool setScale(float sx=0.0f, float sy=0.0f, float sz=0.0f);
	static bool scale	(float sx=0.0f, float sy=0.0f, float sz=0.0f);
	static bool setRotation(float sa=0.0f);
	static bool rotate	(float sa=0.0f);

	// Blitting and Surfaces
	static void clear();
	static void clearAll();	
	static void select(Surface &target);	
	static void lock();
	static void unlock();	
	static void drawSurface(Surface &surface, float sx, float sy, float sw, float sh, float dx=0, float dy=0);
	static Surface &destroySurface(Surface &surface);
	static Surface loadBitmap(std::string filename);
	static Surface createSurface(int width, int height);
	
	/* Sprite and animations*/
	static Sprite createSprite(std::string filename);
	static Animation createAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical);
	static Sprite &destroySprite(Sprite &sprite);
	static Animation &destroyAnimation(Animation &animation);
	static void drawSprite(Sprite &sprite, float dx, float dy);
	static void drawAnimation(Animation &animation, float dx, float dy);

	/* High Level API */
	static long int allocateSurface(int width, int height);
	static void deallocateSurface(long int handle);
	static long int allocateSprite(std::string filename, int priority=0);
	static void deallocateSprite(long int handle);
	static long int allocateAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical);
	static void deallocateAnimation(long int handle);
	
	// Surface effects
	static void randomize();
	static void subdivide(int r, int g, int b, int alpha=255);
	static void fadeout(int r = -1, int g = -1, int b = -1);
	static void fadein( int r = -1, int g = -1, int b = -1);

	static int   fade_color[3];
	static float fade_target_level;
	static float fade_level;
	static float fade_delta;

};

#endif
