#ifndef __VPU_HPP
#define __VPU_HPP

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
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
	static ALLEGRO_DISPLAY				*display;
	static ALLEGRO_BITMAP				*buffer;	
	static ALLEGRO_FONT					*font;
	static ALLEGRO_COLOR				color;
	static ALLEGRO_COLOR				shadow;
	static ALLEGRO_COLOR				transparent;
	static ALLEGRO_BITMAP				*target;	
	static std::vector<ALLEGRO_COLOR>	color_stack;
	static bool							fullscreen;	
public:
	static Surface overlay[4];
	static Surface background[4];
	static Surface foreground[4];
	static int scroll[2];
	static int frames;
	static int fps;
	static int width;
	static int height;
	static bool redraw;
	static bool initialize();
	static void deinitialize();
	static void render();
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
	static void setColor(int r, int g, int b, int alpha = 255);
	static void pushColor();
	static void popColor();
	static Surface loadBitmap(std::string filename);
	static Surface createBitmap(int width, int height);
};

#endif
