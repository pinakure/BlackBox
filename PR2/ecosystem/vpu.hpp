#ifndef __VPU_HPP
#define __VPU_HPP

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <vector>

#define VPU_OVERSCAN 0

typedef struct s_Surface {
	ALLEGRO_BITMAP *bitmap = NULL;
	bool  enabled = false;
	float rotation[3] = { 0.0f, 0.0f, 0.0f };	
	float scale[3]	  = { 1.0f, 1.0f, 1.0f };	
	int width  = 0;
	int height = 0;
}Surface;

class Vpu {
private:
	static ALLEGRO_BITMAP				*buffer;	
	static ALLEGRO_FONT					*font;
	static ALLEGRO_FONT					*legacy_font;
	static ALLEGRO_COLOR				color;
	static ALLEGRO_COLOR				shadow;
	static ALLEGRO_COLOR				transparent;
	static ALLEGRO_BITMAP				*target;	
	static std::vector<ALLEGRO_COLOR>	color_stack;
public:
	static ALLEGRO_DISPLAY				*display;
	static bool							is_initialized;
	static bool							fullscreen;	
	static Surface						overlay[4];
	static Surface						background[4];
	static Surface						foreground[4];
	static Surface						console;
	static int							scroll[2];
	static int							frames;
	static int							fps;
	static int							width;
	static int							height;
	static bool							redraw;
	static unsigned long int			total_frames;
	
	static bool initialize();
	static void initializeFonts();
	static void deinitialize();
	static void render();
	static bool start();
	static bool restart();
	static void select(Surface &target);	
	static void print(std::string text, int  x, int y);
	static void printInteger(std::string text, int d, int  x, int y);
	static void printFloat(std::string text, float d, int  x, int y);
	static void printDouble(std::string text, double d, int  x, int y);
	static void printBool(std::string text, bool d, int  x, int y);
	static void fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha = 255);
	static void fillSquircle(int x, int y, int width, int height, int r, int g, int b, int alpha = 255);
	static void fillCircle(int x, int y, float radius, int r, int g, int b, int alpha=255);
	static void circle(int x, int y, float radius, int r, int g, int b, int alpha=255);
	static void paint(int r, int g, int b, int alpha = 255);
	static void lock();
	static void unlock();
	static void clear();
	static void clearAll();
	static void setColor(int r, int g, int b, int alpha = 255);
	static void setColor(ALLEGRO_COLOR color);
	static ALLEGRO_COLOR alter(ALLEGRO_COLOR _color, float qr=1.0f, float qg=1.0f, float qb=1.0f, float qa=1.0f);
	static void randomize();
	static void subdivide(int r, int g, int b, int alpha=255);
	static void pushColor();
	static void popColor();
	static void fadeout();
	static void fadein();
	static void putpixel(int x, int y);
	static void drawSurface(Surface &surface, float sx, float sy, float sw, float sh, float dx=0, float dy=0);
	static Surface &destroySurface(Surface &surface);
	static Surface loadBitmap(std::string filename);
	static Surface createBitmap(int width, int height);
	static Surface *__layers[12];
};

#endif
