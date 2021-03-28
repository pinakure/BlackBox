#include "utils.hpp"
#include "vpu.hpp"
#include "curtain.hpp"
#include "engine.hpp"
#include "console.hpp"
#include "vfx.hpp"

std::map<long int, Surface>			Vpu::surfaces;
std::map<long int, Animation>		Vpu::animations;
std::map<long int, Sprite>			Vpu::sprites;
std::map<long int, TiledMap>		Vpu::tiledmaps;

TiledMap							*Vpu::active_map = nullptr;
long int							Vpu::animation_handle=0;
long int							Vpu::surface_handle = 0;
long int							Vpu::sprite_handle=0;
long int							Vpu::tiledmap_handle = 0;
float								Vpu::fade_target_level=0.0f;
float								Vpu::fade_level=0.0f;
float								Vpu::fade_delta=1.5f;
int									Vpu::fade_color[3] = {0, 0, 0};
Pixel								Vpu::__getpixel;
int Vpu::pixel_format = 0;

Surface Vpu::console;
Surface Vpu::overlay;
Surface Vpu::background;
Surface Vpu::foreground;

std::vector<ALLEGRO_COLOR> Vpu::color_stack;
std::vector<Font*> Vpu::font_stack;
Surface* Vpu::target = nullptr;
ALLEGRO_BITMAP *Vpu::buffer= nullptr;
int Vpu::frames = 0;
unsigned long int Vpu::total_frames = 0;
int Vpu::fps = 0;
int Vpu::width = 0;
int Vpu::height = 0;
int Vpu::scroll[2] = { 0, 0 };
bool Vpu::redraw = true;
bool Vpu::is_initialized = false;
ALLEGRO_DISPLAY *Vpu::display = NULL;
ALLEGRO_COLOR Vpu::color;
ALLEGRO_COLOR Vpu::shadow;
bool Vpu::fullscreen = false;
bool Vpu::ready= false;
ALLEGRO_COLOR Vpu::transparent;
std::vector<Font*> Vpu::fonts;

Font* Vpu::font = NULL;
Font* Vpu::biggest_font = NULL;
Font* Vpu::smallest_font = NULL;
Font* Vpu::legacy_font = NULL;



PyMethodDef Vpu::methods[] = {
	{"transition"	, Vpu::pyTransition		, METH_VARARGS, "transition(type) : Performs half of an interactive transition and wait for user to change screen to continue" },
	{"createanim"	, Vpu::pyCreateAnimation, METH_VARARGS, "createanim(width, height, sprite_handle) : Return Handle to new Animation object " },
	{"createsprite"	, Vpu::pyCreateSprite	, METH_VARARGS, "createsprite(filename) : Returns Handle to Sprite object create upon given filename" },
	{"createsurf"	, Vpu::pyCreateSurface	, METH_VARARGS, "createsurf(width, height) : Returns Handle to Surface object to be drawn arbitrarily to screen" },
	{"deleteanim"	, Vpu::pyDeleteAnimation, METH_VARARGS, "deleteanim(handle) : Delete Animation identified by given Handle" },
	{"deletesprite"	, Vpu::pyDeleteSprite	, METH_VARARGS, "deletesprite(handle) : Delete Sprite identified by given Handle" },
	{"deletesurf"	, Vpu::pyDeleteSurface	, METH_VARARGS, "deletesurf(handle) : Delete Surface identified by given Handle" },
	{"dimensions"	, Vpu::pyDimensions		, METH_VARARGS, "dimensions() : Returns selected bitmap [ width, height ] "},
	{"disable"		, Vpu::pyDisable		, METH_VARARGS, "disable(layer) : Toggle off given vpu layer"},
	{"drawanim"		, Vpu::pyDrawAnimation	, METH_VARARGS, "drawanim(handle, x, y, rotation) : Draw animation identified by given handle onto given coordinates" },
	{"updateanim"	, Vpu::pyUpdateAnimation, METH_VARARGS, "updateanim(force_frame=None) : " },
	{"drawsprite"	, Vpu::pyDrawSprite		, METH_VARARGS, "drawsprite(handle, x, y, rotation) : Draw Sprite identified by given handle onto given coordinates" },
	{"drawsurf"		, Vpu::pyDrawSurface	, METH_VARARGS, "drawsurf(handle, x, y) : Draw surface identified by given handle onto given coordinates" },
	{"enable"		, Vpu::pyEnable			, METH_VARARGS, "enable(layer) : Toggle on  given vpu layer"},
	{"fadein"		, Vpu::pyFadeIn			, METH_VARARGS, "fadein() : Fade screen from black"},
	{"fadeout"		, Vpu::pyFadeOut		, METH_VARARGS, "fadeout() : Fade screen to black"},
	{"fading"		, Vpu::pyFading			, METH_VARARGS, "fading() : Return True is fade in / fade out is activated" },
	{"fill"			, Vpu::pyFill			, METH_VARARGS, "fill(r, g, b, a) : Fill selected bitmap with given color or default color"},
	{"fillrect"		, Vpu::pyFillRectangle	, METH_VARARGS, "filrect(x, y, dx, dy) : Draw a filled rectangle onto selected surface from x,y to dx,dy" },
	{"frames"		, Vpu::pyFrames			, METH_VARARGS, "frames() : Return actual frame count"},
	{"fullscreen"	, Vpu::pyFullScreen		, METH_VARARGS, "fullscreen(enabled) : Toggle fullscreen mode"},
	{"selectsprite"	, Vpu::pySelectSprite	, METH_VARARGS, "selectsprite(sprite_handle) : Select surface bound to given sprite"},
	{"rect"			, Vpu::pyRectangle		, METH_VARARGS, "rect(x, y, dx, dy) : Draw a rectangle onto selected surface from x,y to dx,dy" },
	{"line"			, Vpu::pyLine			, METH_VARARGS, "line(x, y, dx, dy) : Draw a line onto selected surface from x,y to dx,dy" },
	{"perlin"		, Vpu::pyPerlin			, METH_VARARGS, "perlin(surface_handle, r, g, b, scaling_factor, octave, random_seed) : Generate perlin noise using given color along given surface" },
	{"pset"			, Vpu::pyPutPixel		, METH_VARARGS, "pset(x, y) : Draw a pixel onto selected surface onto given coordinates" },
	{"getpixel"		, Vpu::pyGetPixel		, METH_VARARGS, "color = getpixel(x, y) : Read pixel from selected surface" },
	{"getsurfacedata",Vpu::pyGetSurfaceData , METH_VARARGS, "[ colors ] = getsurfacedata() : Read whole pixel data from selected surface in a linear list" },
	{"restart"		, Vpu::pyRestart		, METH_VARARGS, "restart() : Restart Video Processing Unit"},
	{"rotate"		, Vpu::pyRotate			, METH_VARARGS, "rotate(layer, angle) : Rotate specified layer (0-11) given degrees"},
	{"select"		, Vpu::pySelect			, METH_VARARGS, "select(layer) : Select given layer to perform next graphic operations onto it"},
	{"setframe"		, Vpu::pySetFrame		, METH_VARARGS, "setframe(handle, frame) : Sets active frame for specified animation"},
	{"setcolor"		, Vpu::pySetColor		, METH_VARARGS, "setcolor(r, g, b, a) : Sets current painting color"},
	{"setfont"		, Vpu::pySetFont		, METH_VARARGS, "setfont(font_handle) : Sets current font to the one identified by given handle"},
	{"setrotation"	, Vpu::pySetRotation	, METH_VARARGS, "setrotation(layer, angle) : Sets rotation for specified layer (0-11) at given degrees"},
	{"setscale"		, Vpu::pySetScale		, METH_VARARGS, "setscale(layer, scale_x, scale_y) : Sets scale for specified layer [0-11] given horizontal and vertical values"},
	{"scale"		, Vpu::pyScale			, METH_VARARGS, "scale(layer, scale_x, scale_y) : Change specified layer [0-11] given horizontal and vertical scale factor"},
	{"subsprite"	, Vpu::pySubSprite		, METH_VARARGS, "subsprite(sprite_handle, left, top, right, bottom) : Create sprite region or full copy from original sprite"},
	{"textout"		, Vpu::pyTextOut		, METH_VARARGS, "textout(text, x, y) : Print given text at given coordinates"},
	{"tintsprite"	, Vpu::pyTintSprite		, METH_VARARGS, "tintsprite(sprite_handle, original_palette, new_palette) : Change colors in sprite from original_palette to new_palette"},
	{"update"		, Vpu::pyUpdate			, METH_VARARGS, "update() : Allow blackbox engine to perform its rendering based input and output operations"},
	{NULL, NULL, 0, NULL}
};

void prepareTests() {
	Vpu::foreground.enabled = false;
	Vpu::background.enabled = true;		
	Vpu::select(Vpu::background);
	Vpu::subdivide(40, 40, 50, 255);
	//Vpu::randomize();
}

void Vpu::subdivide(int r, int g, int b, int alpha) {
	Vpu::paint(0, 0, 0, 0);
	int w = al_get_bitmap_width(Vpu::target->bitmap)/2;
	int h = al_get_bitmap_height(Vpu::target->bitmap)/2;
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN    , w, h, r		, g		 ,b		 , alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN    , w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN + h, w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN + h, w, h, r		, g		 ,b		 , alpha);
}

void Vpu::randomize() {	
	int w = al_get_bitmap_width( target->bitmap);
	int h = al_get_bitmap_height( target->bitmap );
	lock();
	for (int x = 0,xo=w; x < xo; x++) {
		for (int y = 0,yo=h; y < yo; y++) {
			int c = rand() * 255;
			al_put_pixel(x, y, al_map_rgb(c,c,c));
		}
	}
	unlock();
}

void Vpu::clear() {
	paint(0, 0, 0, 0);
}

void Vpu::clearAll() {
	select(background);
	paint(0, 0, 0, 0);
	select(foreground);
	paint(0, 0, 0, 0);
	select(overlay);
	paint(0, 0, 0, 0);
}

bool Vpu::start() {		
	width = Engine::width;
	height = Engine::height;
	int flags = ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
	al_set_new_display_flags(fullscreen ? flags | ALLEGRO_FULLSCREEN : flags);
	if (display) {
		al_unregister_event_source(Engine::queue, al_get_display_event_source(display));
		al_destroy_display(display);
		display = NULL;
	}
	display = al_create_display(width, height);
	if (!display) {
		Engine::printf("Cannot initialize %s %dx%d display\n", fullscreen?"fullscreen":"",width, height);
		return false;
	}
	al_register_event_source(Engine::queue, al_get_display_event_source(display));	
	return true;
}

void Vpu::initializeFonts() {
	ALLEGRO_FS_ENTRY* e = al_create_fs_entry("fonts/");
	if (al_open_directory(e)) {
		ALLEGRO_FS_ENTRY* file;
		while (file = al_read_directory(e)){
			std::string name = al_get_fs_entry_name(file);
			std::vector<std::string> temp  = explode(name, '\\');
			if(temp.size()>0)
				name = temp[temp.size() - 1];
			// Remove slashes
			std::vector<std::string> parts = explode(name, '/');
			if (parts.size() > 0) {
				name = parts[parts.size() - 1];
			}
			// Extract extension
			parts = explode(name, '.');
			al_destroy_fs_entry(file);
			if (parts.size() == 2) {
				if (!parts[1].compare("ttf")) {
					fonts.push_back(new Font(parts[0], 16));					
				}
			} else if (parts.size() == 3) {
				if (!parts[2].compare("ttf")) {
					int i = atoi(parts[1].c_str());
					fonts.push_back(new Font(parts[0] + "." + parts[1], i));
				}
			};
			// Reassign smallest and biggest font pointers 
			if (fonts.size()) {
				Font* last = fonts[fonts.size() - 1];
				if (!biggest_font)biggest_font = last;
				else if (biggest_font->height < last->height) biggest_font = last;
				if (!smallest_font)smallest_font = last;
				else if (smallest_font->height > last->height) smallest_font = last;
			}
		}
	}
	for (int i = 0; i < fonts.size(); i++) {
		font = fonts[i];
		if (font)return;
	}
	Engine::printf("WARNING: Failed to initialize system font.\nUsing default font.\n");
	font = legacy_font;	
	if (!biggest_font) biggest_font = legacy_font;
	if (!smallest_font) smallest_font = legacy_font;
}

bool Vpu::restart() {
	is_initialized = false;

	// overlay 
	overlay = createSurface(width, height);
	if(!overlay.enabled) return false;
	select(overlay);
	clear();		
	
	// foreground
	foreground = createSurface(width*2, height*2);
	if(!foreground.enabled) return false;
	select(foreground);
	clear();

	// background
	background= createSurface( width*2 , height*2);
	if(!background.enabled) return false;
	select(background);
	clear();
	
	// TODO: remove on production
	prepareTests();

	// (Re)initialize squash framebuffer
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	buffer = al_create_bitmap(width, height);
	if(!buffer) return false;			
	
	VfxProcessor::initialize(buffer);
	
	// Get pixel format to accelerate lock calls
	pixel_format = al_get_bitmap_format(buffer);

	// (Re)initialize console framebuffer
	destroySurface(console);
	destroySurface(Console::bitmap);	// This will be dinamically created at paint time by console if null
	destroySurface(Console::backdrop);	// This will be dinamically created at paint time by console if null
	console = createSurface(width, height);
	if(!console.enabled) return false;	
	Vpu::select(console);
	Vpu::paint(0, 0, 0, 0);

	// (Re)define default colors and reset flags
	transparent = al_map_rgba(0, 0, 0, 0);	
	setColor(255, 255, 255);
	redraw = true;
	is_initialized = true;
	al_set_window_title(display, "BlackBox v.3");		
	Curtain::render();
	return true;
}

bool Vpu::initialize() {
	try {
		if (!start())return false;		
		if (!al_init_font_addon()) return false;
		if (!al_init_ttf_addon()) return false;
		if (!al_init_primitives_addon())return false;
		if (!al_init_image_addon())return false;		
		legacy_font = new Font("legacy");
		font = legacy_font;	
		if (!restart()) return false;
		initializeFonts();
		ALLEGRO_BITMAP *icon;
		icon = al_load_bitmap("gfx/vendor.png");
		if (icon) {
			al_set_display_icon(display, icon);
		}
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::paint(int r, int g, int b, int a) {
	r = r >= 0 ? (r&255) : color.r * 255;
	g = g >= 0 ? (g&255) : color.g * 255;
	b = b >= 0 ? (b&255) : color.b * 255;
	a = a >= 0 ? (a&255) : color.a * 255;
	al_clear_to_color(al_map_rgba(r, g, b, a));
}

void Vpu::select(Surface &target) {
	Vpu::target = &target;
	al_set_target_bitmap(Vpu::target->bitmap);
}

void Vpu::deinitialize() {
	font = legacy_font;
	for (int i = 0; i < fonts.size(); i++) {
		delete fonts[i];
	}
	fonts.clear();
	if(display) al_destroy_display(display);
	destroySurface(console);
	for (int i = 0; i < 4; i++) {
		destroySurface(overlay);
		destroySurface(foreground);
		destroySurface(background);		
	}
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
		
}

#define ENSURETARGET 	if (!target) {														\
							std::printf("Error: Cannot scale target : No target selected");	\
							return false;													\
						};

#define XSCALE Vpu::width
#define YSCALE Vpu::height
//#define ZSCALE Vpu::depth

bool Vpu::setRotation(float sa) {
	ENSURETARGET
	std::printf("Vpu::setRotation(%f)\n", sa);
	target->rotation[0] = sa;
	return true;
}

bool Vpu::rotate(float sa) {
	ENSURETARGET
	return Vpu::setRotation(target->rotation[0] + sa);
}

bool Vpu::setScale(float sx, float sy, float sz) {
	ENSURETARGET
	if (target->width  * sx < XSCALE)return false;
	if (target->height * sy < YSCALE)return false;
	std::printf("Vpu::setScale(%f,%f,%f)\n", sx, sy, sz);
	//if (target.depth * sz < ZDEPTH)return false;
	target->scale[0] = sx;
	target->scale[1] = sy;
	target->scale[2] = sz;
	return true;
}

bool Vpu::scale(float sx, float sy, float sz) {
	ENSURETARGET
	if (target->width  * (target->scale[0]+sx) < XSCALE)return false;
	if (target->height * (target->scale[1]+sy) < YSCALE)return false;
	//if (target.depth * (target->scale[2]+sz) < ZSCALE)return false;
	return setScale(target->scale[0] + sx, target->scale[1] + sy, target->scale[2] + sz);
}

void Vpu::setColor(int r, int g, int b, int alpha) {
	color  = al_map_rgba(r, g, b, alpha);
	shadow = Vpu::alter(color, 0.5f, 0.5f, 0.5f);
}

void Vpu::setColor(ALLEGRO_COLOR _color) {
	Vpu::color  = _color;
	Vpu::shadow = Vpu::alter(_color, 0.5f, 0.5f, 0.5f);
}

ALLEGRO_COLOR Vpu::alter(ALLEGRO_COLOR _color, float qr, float qg, float qb, float qa) {
	ALLEGRO_COLOR ret = _color; 
	ret.r *= qr;
	ret.g *= qg;
	ret.b *= qb;
	ret.a *= qa;
	return ret;
}

static char _buffer[65535];

#include <iostream>
void Vpu::printf(const char* txt) {
	std::cout << txt;

}

void Vpu::printf(int  x, int y, int flags, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(_buffer, 2048, fmt, ap);
	va_end(ap);

	al_draw_textf(font->data, shadow, x + 1, y + 1, flags, _buffer);
	al_draw_textf(font->data, color, x, y, flags, _buffer);
}

void Vpu::print(std::string text, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, text.c_str());
	al_draw_textf(font->data, color , x  , y  , flags, text.c_str());
}
void Vpu::printInteger(std::string text, int d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%d").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%d").c_str(), d);
}
void Vpu::printFloat(std::string text, float d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printDouble(std::string text, double d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printBool(std::string text,bool d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%b").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%b").c_str(), d);
}

void Vpu::fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		al_map_rgba(r, g, b, alpha)
	);
}

void Vpu::qfillRectangle(int x, int y, int width, int height) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		color
	);
}

void Vpu::rectangle(int x, int y, int width, int height) {
	al_draw_rectangle(
		x, y,
		x + width, y + height,
		color, 1.0f
	);
}

void Vpu::fillSquircle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	Vpu::fillRectangle(x, y, width, height, r, g, b, alpha);
	al_put_pixel(x, y, transparent);
	al_put_pixel(x+width, y, transparent);
	al_put_pixel(x, y+height, transparent);
	al_put_pixel(x+width, y+height, transparent);	
}

void Vpu::circle(int x, int y, float radius, int r, int g, int b, int alpha){
	al_draw_circle(x, y, radius, al_map_rgba(r, g, b, alpha), 1.0f);
}

void Vpu::fillCircle(int x, int y, float radius, int r, int g, int b, int alpha) {
	al_draw_filled_circle(x, y, radius, al_map_rgba(r, g, b, alpha));
}

static inline void drawEntities() {
	al_set_target_bitmap(Vpu::foreground.bitmap);
	std::vector<Entity*>::iterator it = Engine::entities.begin();
	ALLEGRO_COLOR white = al_map_rgba(255, 255, 255, 64);
	for (; it != Engine::entities.end(); it++) {
		Vpu::color = white;
		(*it)->draw();
	}
}

void Vpu::render() {
	drawEntities();

	if (Vpu::active_map) {
		if(Vpu::active_map->target) al_set_target_bitmap(Vpu::active_map->target->bitmap);
		Vpu::active_map->redraw();
		Vpu::active_map->draw(-Vpu::active_map->scroll.x, -Vpu::active_map->scroll.y);
	}

	al_set_target_bitmap(buffer);
	/* Render enabled layers */

	#define renderlayer(l) {															\
								float OFFSETX=(-(Vpu::width/2)*l.scale[0] ) + ( (Vpu::width/2)  * ( l.scale[0] - 1.0f ) );	\
								float OFFSETY=(-(Vpu::height/2)*l.scale[1] ) + ( (Vpu::height/2) * ( l.scale[1] - 1.0f ) );	\
								if(l.enabled)											\
								 al_draw_scaled_rotated_bitmap(							\
									l.bitmap,											\
									l.width/2, l.height/2,								\
									Vpu::width + OFFSETX, Vpu::height + OFFSETY,		\
									l.scale[0],l.scale[1],								\
									l.rotation[0],										\
									0													\
								);														\
							}
		renderlayer(background);
		renderlayer(foreground);
	#undef renderlayer	
	
	Curtain::draw();
	
	al_draw_scaled_rotated_bitmap(
		overlay.bitmap,							
		overlay.width/2, overlay.height/2,	
		overlay.width/2, overlay.height,	
		overlay.scale[0]*2.0f,overlay.scale[1]*2.0f,	
		overlay.rotation[0],						
		0											
	);
	
	// Use fade if proceed
	if(fade_level != fade_target_level)
		fade_level += fade_delta;
	// Stop after reaching target level
	if((fade_target_level > 0.0f && fade_level > fade_target_level) 
	|| (fade_target_level < 1.0f && fade_level < fade_target_level))
		fade_level = fade_target_level;
	
	
	al_set_target_bitmap(buffer);
	al_draw_filled_rectangle(0, 0, width, height, al_map_rgba(fade_color[0], fade_color[1], fade_color[2], 255*fade_level));
	
	if (console.enabled)
		al_draw_scaled_bitmap(console.bitmap, 0, 0, width, height, 0, 0, al_get_display_width(display), al_get_display_height(display), 0);

	VfxProcessor::run();
	
	redraw = false;
}


void Vpu::pushFont() {
	Vpu::font_stack.push_back(font);
}
void Vpu::popFont() {
	if (Vpu::font_stack.size()) {
		font = Vpu::font_stack[0];
		Vpu::font_stack.pop_back();
	}
}
void Vpu::pushColor() {
	Vpu::color_stack.push_back(color);
	Vpu::color_stack.push_back(shadow);
}
void Vpu::popColor() {
	if (Vpu::color_stack.size()) {
		color  = Vpu::color_stack[0];
		shadow = Vpu::color_stack[1];
		Vpu::color_stack.pop_back();
	}
}

void Vpu::putpixel(int x, int y) {
	al_put_pixel(x, y, color);
}

Pixel* Vpu::getpixel(int x, int y) {
	if (!Vpu::target)return &Vpu::__getpixel;
	//al_lock_bitmap(Vpu::target->bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
	ALLEGRO_COLOR c = al_get_pixel(Vpu::target->bitmap, x, y);
	Vpu::__getpixel.r = c.r * 255.0f;
	Vpu::__getpixel.g = c.g * 255.0f;
	Vpu::__getpixel.b = c.b * 255.0f;
	Vpu::__getpixel.a = c.a * 255.0f;
	//
	al_unlock_bitmap(Vpu::target->bitmap);
	return &Vpu::__getpixel;
}

void Vpu::line(int x, int y, int dx, int dy) {
	al_draw_line(x, y, dx, dy, color, 1.0f);
}

Sprite &Vpu::destroySprite(Sprite &sprite) {
	return sprite;
}

Animation &Vpu::destroyAnimation(Animation &animation) {
	return animation;
}

Surface &Vpu::destroySurface(Surface &surface) {
	if (surface.bitmap) al_destroy_bitmap(surface.bitmap);
	surface.bitmap = NULL;
	surface.width = 0;
	surface.height = 0;
	surface.enabled = false;
	surface.rotation[0] = 0;
	surface.rotation[1] = 0;
	surface.rotation[2] = 0;
	surface.scale[0] = 0;
	surface.scale[1] = 0;
	surface.scale[2] = 0;
	return surface;
}

Font* Vpu::getFontByName(std::string name) {
	std::vector<Font*>::iterator it;
	for (it = fonts.begin(); it != fonts.end(); it++) {
		if (!(*it)->name.compare(name))return *it;
	}
	return NULL;
}

void Vpu::drawSurface(Surface& surface, float sx, float sy, float sw, float sh, float dx, float dy) {
	al_draw_bitmap_region(surface.bitmap, sx, sy, sw, sh, dx, dy, 0);
	//al_draw_bitmap(surface.bitmap, 0, 0, 0);
}

void Vpu::drawSurfaceRotated(Surface& surface, float dx, float dy, float angle){
	al_draw_rotated_bitmap(surface.bitmap, surface.width/2, surface.height/2, dx, dy, angle, 0);
}

void Vpu::drawSprite(Sprite &sprite, float dx, float dy) {
	Vpu::drawSurfaceRotated(sprite.picture, dx, dy, sprite.picture.rotation[0]);
	//sprite.draw(dx, dy);	
}

void Vpu::drawAnimation(Animation& animation, float dx, float dy) {
	if (animation.autoupdate)
		animation.run(animation.speed);
	animation.qdraw(dx, dy);
}

void Vpu::drawAnimationRotated(Animation& animation, float dx, float dy, float angle) {
	if (animation.autoupdate)
		animation.run(animation.speed);
	if (animation.frame.size() > 0)
		al_draw_rotated_bitmap(
			animation.frame[animation.current_frame], 
			animation.width >> 1,
			animation.height >> 1,
			dx, dy, 
			angle, 
			0
		);
}

Surface Vpu::createSurface(int width, int height) {
	Surface s;
	s.bitmap = al_create_bitmap(width, height);
	s.enabled = !(s.bitmap == NULL);
	if (s.enabled) {
		s.width  = width;
		s.height = height;
	}
	return s;
}

void Vpu::perlin(Surface& surface, int r, int g, int b, float scaling_bias, int octave, int random_seed) {
	const int		w				= surface.width;
	const int		h				= surface.height;
	int				area			= w * h;
	static float*	noise_seed		= nullptr;
	static float*	perlin_noise	= nullptr;
	static Surface* last			= nullptr;
	srand(random_seed);
	// Initialize buffers (use cache if same surface is used recurrently)
	if (&surface != last) {
		if(perlin_noise) delete perlin_noise;
		if(noise_seed) delete noise_seed;
		noise_seed = new float[area];
		perlin_noise = new float[area];
		last = &surface;
	}
	for (int i = 0; i < area; i++) 
		noise_seed[i] = (float)rand() / (float)RAND_MAX;
	
	// Calculate perlin noise
	Surface* push = Vpu::target;
	Vpu::select(surface);
	al_lock_bitmap(surface.bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octave; o++) {
				int nPitch = w >> o;
				if (nPitch <= 0)nPitch = 1;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % w;
				int nSampleY2 = (nSampleY1 + nPitch) % w;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * noise_seed[(nSampleY1 * w + nSampleX1)%area] + fBlendX * noise_seed[(nSampleY1 * w + nSampleX2)%area];
				float fSampleB = (1.0f - fBlendX) * noise_seed[(nSampleY2 * w + nSampleX1)%area] + fBlendX * noise_seed[(nSampleY2 * w + nSampleX2)%area];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / scaling_bias;
			}

			// Scale to seed range
			perlin_noise[y * w + x] = fNoise / fScaleAcc;
			int pixel_bw = (int)(perlin_noise[y * w + x] * 16.0f);
			al_put_pixel(
				x, y,
				al_map_rgb(
					(float(r) / 16.0f) * float(pixel_bw),
					(float(g) / 16.0f) * float(pixel_bw),
					(float(b) / 16.0f) * float(pixel_bw)
				)
			);		
		}
	}
	// Draw perlin noise
	/*
	Surface* push = Vpu::target;
	Vpu::select(surface);
	al_lock_bitmap(surface.bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
	for (int x = 0; x < w; x++){
		for (int y = 0; y < h; y++){
			int pixel_bw = (int)(perlin_noise[y * w + x] * 16.0f);
			al_put_pixel(
				x, y,
				al_map_rgb(
					(float(r) / 16.0f) * float(pixel_bw),
					(float(g) / 16.0f) * float(pixel_bw),
					(float(b) / 16.0f) * float(pixel_bw)
				)
			);			
		}
	}
	*/
	al_unlock_bitmap(surface.bitmap);
	Vpu::select(*push);		
}

Surface Vpu::loadBitmap(std::string filename) {
	Surface s;
	s.bitmap = al_load_bitmap((filename).c_str());	
	s.enabled = s.bitmap != NULL;
	if (s.enabled) {
		s.width  = al_get_bitmap_width(s.bitmap);
		s.height = al_get_bitmap_height(s.bitmap);
	}
	return s;
}

void Vpu::fadeout(int r, int g, int b) {
	fade_color[0] = r>0?r:fade_color[0];
	fade_color[1] = g>0?g:fade_color[1];
	fade_color[2] = b>0?b:fade_color[2];
	fade_level = 0.0f;
	fade_target_level = 1.0f;
	fade_delta = 1.0f / 120.0f;
}

void Vpu::fadein(int r, int g, int b) {
	fade_color[0] = r>0?r:fade_color[0];
	fade_color[1] = g>0?g:fade_color[1];
	fade_color[2] = b>0?b:fade_color[2];
	fade_level = 1.0f;
	fade_target_level = 0.0f;
	fade_delta = -(1.0f / 120.0f);
}

void Vpu::lock() {
	assert(target);
	assert(target->bitmap);
	//al_lock_bitmap( target->bitmap ,al_get_bitmap_format( target->bitmap ), ALLEGRO_LOCK_WRITEONLY);	
	al_lock_bitmap( target->bitmap , pixel_format , ALLEGRO_LOCK_WRITEONLY);	
}

void Vpu::unlock() {
	assert(target);
	assert(target->bitmap);
	al_unlock_bitmap(target->bitmap);
}
	
void Vpu::loadVars() {

}

Sprite Vpu::createSprite(std::string filename) {
	return Sprite(filename.c_str());
}

long int Vpu::createSubSprite(Sprite& s, int left, int top, int right, int bottom) {
	sprite_handle++;
	sprites.insert(std::pair<long int, Sprite>(sprite_handle, Sprite(s, left, top, right, bottom)));
	return sprite_handle;	
}

Animation Vpu::createAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical, bool autoupdate) {
	return Animation(s, width, height, sx, sy, dx, dy, vertical, autoupdate);	
}
	
long int Vpu::allocateSprite(std::string filename, int priority){
	#define SPRITE_PRIORITY_RANGE 1024
	sprite_handle++;
	sprite_handle += priority * SPRITE_PRIORITY_RANGE;
	sprites.insert( std::pair<long int, Sprite>(sprite_handle, createSprite(filename)) );
	sprite_handle -= priority * SPRITE_PRIORITY_RANGE;
	return sprite_handle + (priority * SPRITE_PRIORITY_RANGE);
	#undef SPRITE_PRIORITY_RANGE
}

void Vpu::deallocateSprite(long int handle) {
	if (sprites.empty())return;
	if (sprites.find(handle) != sprites.end()) {
		destroySprite(sprites.at(handle));
		sprites.erase(handle);
	}
}

long int Vpu::allocateAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical, bool autoupdate) {
	animation_handle++;
	animations.insert( std::pair<long int, Animation>(animation_handle, createAnimation(width, height, s,sx,sy,dx,dy, vertical, autoupdate)) );
	return animation_handle;
}

void Vpu::deallocateAnimation(long int handle) {
	if (animations.empty())return;
	if (animations.find(handle) != animations.end()) {
		destroyAnimation(animations.at(handle));
		animations.erase(handle);
	}
}

long int Vpu::allocateTiledMap(int width, int height, int layer_count, int tile_width, int tile_height) {
	tiledmap_handle++;
	tiledmaps.insert(std::pair<long int, TiledMap>(tiledmap_handle, TiledMap(width, height, layer_count, tile_width, tile_height)));
	return tiledmap_handle;
}

void Vpu::deallocateTiledMap(long int handle) {
	if (tiledmaps.empty())return;
	if (tiledmaps.find(handle) != tiledmaps.end()) {
		tiledmaps.erase(handle);
	}
}

long int Vpu::allocateSurface(int width, int height) {
	surface_handle++;
	surfaces.insert(std::pair<long int, Surface>(surface_handle, createSurface(width, height)));
	return surface_handle;
}

void Vpu::deallocateSurface(long int handle) {
	if (surfaces.empty())return;
	if (surfaces.find(handle) != surfaces.end()) {
		destroySurface(surfaces.at(handle));
		surfaces.erase(handle);
	}
}

void Vpu::setFont(Font *fnt) {
	Vpu::font = fnt;
}

void Vpu::tintSprite(Sprite& sprite, std::vector<Pixel>& src, std::vector<Pixel>& dst) {
	// Change colors from src to colors in dst over picture in sprite
	if (src.size() != dst.size()) {
		Console::printf("ERROR: Cannot tint Sprite, unmatched palette sizes: (%d != %d)\n", src.size(), dst.size());
		return;
	}
	select(sprite.picture);
	al_lock_bitmap(sprite.picture.bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
	for (int y = 0; y < sprite.picture.height; y++) {
		for (int x = 0; x < sprite.picture.width; x++) {
			ALLEGRO_COLOR c = al_get_pixel(sprite.picture.bitmap, x, y);
			Pixel p = {
				int(c.r * 255.0f),
				int(c.g * 255.0f),
				int(c.b * 255.0f)
			};
			if ((p.r == 0) && (p.g == 0) && (p.b == 0))continue;
			std::vector<Pixel>::iterator it = src.begin();
			int i = 0;
			while(it != src.end()) {
				Pixel* s = &src[i], * d = &dst[i];
				if (s->r == p.r) {
					if (s->g == p.g) {
						if (s->b == p.b) {
							al_put_pixel(x, y, al_map_rgb(d->r, d->g, d->b));
							break;
						}
					}
				}
				i++;
				it++;
			}
		}
	}
	al_unlock_bitmap(sprite.picture.bitmap);
}
Surface* getLayer(int index) {
	if (!Vpu::is_initialized) {
		printf("vpu is not initialized.\nYou must call vpu.restart() before operating.\n");
		return NULL;
	}
	if (index >= 3) {
		if (Vpu::surfaces.find(index - 3) != Vpu::surfaces.end())
			return &(Vpu::surfaces.at(index - 3));
		else index %= 3;
	}
	return
		index == 0 ? &Vpu::background :
		index == 1 ? &Vpu::foreground :
		&Vpu::overlay;
}







PyObject* Vpu::pyCreateAnimation(PyObject* self, PyObject* args) {
	float width;
	float height;
	int   sprite;
	float dx = 0;
	float dy = 0;
	float sx = 1;
	float sy = 0;
	bool  vertical = false;
	float speed = 1.0f;
	bool  autoupdate = false;
	if (!PyArg_ParseTuple(args, "ffi|ffffbfb", &width, &height, &sprite, &sx, &sy, &dx, &dy, &vertical, &speed, &autoupdate)) return NULL;
	if (Vpu::sprites.find(sprite) != Vpu::sprites.end()) {
		long int handle = Vpu::allocateAnimation(width, height, Vpu::sprites.at(sprite), sx, sy, dx, dy, vertical, autoupdate);
		Vpu::animations.at(handle).speed = speed;
		Vpu::animations.at(handle).bidirectional = false;
		return PyLong_FromLong(handle);
	}
	printf("ERROR: sprite_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyCreateSprite(PyObject* self, PyObject* args) {
	int priority = 0;
	char* filename;
	if (!PyArg_ParseTuple(args, "s|i", &filename, &priority)) return NULL;
	long int handle = Vpu::allocateSprite(filename, priority);
	return PyLong_FromLong(handle);
}
PyObject* Vpu::pyCreateSurface(PyObject* self, PyObject* args) {
	// Must add 3 units to handle (3 system layers to be selected which are not in <Vpu::surfaces> )
	int width;
	int height;
	if (!PyArg_ParseTuple(args, "ii", &width, &height)) return NULL;
	long int handle = Vpu::allocateSurface(width, height);
	return PyLong_FromLong(handle + 3);
}
PyObject* Vpu::pyEnable(PyObject * self, PyObject * args) {
	int index;
	if (!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(false);
	(*layer).enabled = true;
	return PyBool_FromLong(1);
}
PyObject* Vpu::pyDeleteAnimation(PyObject* self, PyObject* args) {
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::animations.find(handle) != Vpu::animations.end()) {
		Vpu::deallocateAnimation(handle);
		return PyBool_FromLong(true);
	}
	printf("ERROR: anim_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyDeleteSprite(PyObject* self, PyObject* args) {
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::sprites.find(handle) != Vpu::sprites.end()) {
		Vpu::deallocateSprite(handle);
		return PyBool_FromLong(true);
	}
	printf("ERROR: sprite_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyDeleteSurface(PyObject* self, PyObject* args) {
	// Must subtract 3 units from handle (3 system layers to be selected which are not in <Vpu::surfaces>)
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::surfaces.find(handle - 3) != Vpu::surfaces.end()) {
		Vpu::deallocateSurface(handle - 3);
		return PyBool_FromLong(true);
	}
	printf("ERROR: surface_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyDimensions(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	int dims[2] = { Vpu::target->width, Vpu::target->height };

	PyObject* list = PyList_New(2);
	if (!list) throw("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < 2; i++) {
		PyObject* num = PyFloat_FromDouble((double)dims[i]);
		if (!num) {
			Py_DECREF(list);
			throw("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(list, i, num);
	}
	return list;
}
PyObject* Vpu::pyDisable(PyObject * self, PyObject * args) {
	int index;
	if (!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(false);
	(*layer).enabled = false;
	return PyBool_FromLong(1);
}
PyObject* Vpu::pyDrawAnimation(PyObject* self, PyObject* args) {
	int handle;
	float x;
	float y;
	float rotation = 0.0f;
	if (!PyArg_ParseTuple(args, "iff|f", &handle, &x, &y, &rotation)) return NULL;
	if (Vpu::animations.find(handle) != Vpu::animations.end()) {
		Vpu::drawAnimationRotated(Vpu::animations.at(handle), x, y, rotation);
		return PyBool_FromLong(true);
	}
	printf("ERROR: anim_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyDrawSprite(PyObject* self, PyObject* args) {
	int handle;
	float x;
	float y;
	float angle = 0.0f;
	if (!PyArg_ParseTuple(args, "iff|f", &handle, &x, &y, &angle)) return NULL;
	if (Vpu::sprites.find(handle) != Vpu::sprites.end()) {
		Sprite* spr = &Vpu::sprites.at(handle);
		spr->picture.rotation[0] = angle;
		Vpu::drawSprite(*spr, x, y);
		return PyBool_FromLong(true);
	}
	printf("ERROR: sprite_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyDrawSurface(PyObject* self, PyObject* args) {
	// Must subtract 3 units from handle (3 system layers to be selected which are not in <surfaces>)
	int handle;
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "iff", &handle, &x, &y)) return NULL;
	Surface* s = getLayer(handle);
	Vpu::drawSurface(*s, 0, 0, s->width, s->height, x, y);
	return PyLong_FromLong(1);
}
PyObject* Vpu::pyFadeIn(PyObject * self, PyObject * args) {
	float r = -1;
	float g = -1;
	float b = -1;
	if (!PyArg_ParseTuple(args, "|fff", &r, &g, &b)) return NULL;
	Vpu::fadein(r, g, b);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyFadeOut(PyObject * self, PyObject * args) {
	float r = -1;
	float g = -1;
	float b = -1;
	if (!PyArg_ParseTuple(args, "|fff", &r, &g, &b)) return NULL;
	Vpu::fadeout(r, g, b);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyFading(PyObject * self, PyObject * args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	return PyBool_FromLong(Vpu::fade_level == Vpu::fade_target_level);
}
PyObject* Vpu::pyFillRectangle(PyObject * self, PyObject * args) {
	float x;
	float y;
	float dx;
	float dy;
	if (!PyArg_ParseTuple(args, "ffff", &x, &y, &dx, &dy)) return NULL;
	Vpu::qfillRectangle(x, y, dx, dy);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyFullScreen(PyObject* self, PyObject* args) {
	bool fullscreen;
	if (!PyArg_ParseTuple(args, "b", &fullscreen)) return NULL;
	Vpu::fullscreen = fullscreen;
	return PyBool_FromLong(Vpu::fullscreen);
}
PyObject* Vpu::pyFill(PyObject* self, PyObject* args) {
	float r = -1.0f;
	float g = -1.0f;
	float b = -1.0f;
	float a = -1.0f;
	if (!PyArg_ParseTuple(args, "|ffff", &r, &g, &b, &a)) return NULL;
	Vpu::paint(r >= 0.0f ? r : 0, g >= 0.0f ? g : 0.0f, b >= 0.0f ? b : 0.0f, a >= 0.0f ? a : 255.0f);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyFrames(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	return PyLong_FromLong(Vpu::total_frames);
}
PyObject* Vpu::pyGetPixel(PyObject* self, PyObject* args) {
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "ff", &x, &y)) return NULL;
	Pixel* p = Vpu::getpixel(x, y);
	return PyLong_FromLong(
		((int(p->a) & 0xFF) << 24) |
		((int(p->b) & 0xFF) << 16) |
		((int(p->g) & 0xFF) << 8) |
		(int(p->r) & 0xFF)
	);
}
PyObject* Vpu::pyGetSurfaceData(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	Surface* s = Vpu::target;
	ALLEGRO_BITMAP* bmp = s->bitmap;
	if(bmp) {
		PyObject* list = PyList_New(s->width * s->height);
		if (!list) throw("Unable to allocate memory for Python list");
		al_lock_bitmap(bmp, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
		int i = 0;
		for (int y = 0; y < s->height; y++) {
			for (int x = 0; x < s->width; x++) {
				ALLEGRO_COLOR p = al_get_pixel(bmp, x, y);
				int color = ((int(p.a*255.0f) & 0xFF) << 24) |
							((int(p.b*255.0f)  & 0xFF) << 16) |
							((int(p.g*255.0f)  & 0xFF) << 8) |
							(int(p.r * 255.0f) & 0xFF);
				PyList_SET_ITEM(list, i, PyLong_FromLong(color));
				i++;
			}
		}
		al_unlock_bitmap(bmp);
		return list;
	}
	return PyList_New(0);
}
PyObject* Vpu::pyLine(PyObject * self, PyObject * args) {
	float x;
	float y;
	float dx;
	float dy;
	if (!PyArg_ParseTuple(args, "ffff", &x, &y, &dx, &dy)) return NULL;
	Vpu::line(x, y, dx, dy);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyPerlin(PyObject * self, PyObject * args) {
	int handle;
	float r = 255.0f, g = 255.0f, b = 255.0f, octave = 7, scaling_factor = 2.0f;
	int random_seed = (rand() * RAND_MAX);
	if (!PyArg_ParseTuple(args, "i|fffffi", &handle, &r, &g, &b, &scaling_factor, &octave, &random_seed)) return NULL;
	Surface* l = getLayer(handle);
	if (l) {
		Vpu::perlin(*l, r, g, b, scaling_factor, octave, random_seed);
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyPutPixel(PyObject* self, PyObject* args) {
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "ff", &x, &y)) return NULL;
	Vpu::putpixel(x, y);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyRectangle(PyObject* self, PyObject* args) {
	float x;
	float y;
	float dx;
	float dy;
	if (!PyArg_ParseTuple(args, "ffff", &x, &y, &dx, &dy)) return NULL;
	Vpu::rectangle(x, y, dx, dy);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyReload(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	if (!Vpu::start())exit(1);
	if (!Vpu::restart())exit(1);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyRestart(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	bool r = true;
	r &= Vpu::start();
	r &= Vpu::restart();
	return PyBool_FromLong(r);
}
PyObject* Vpu::pyRotate(PyObject* self, PyObject* args) {
	int index;
	float rotation;
	if (!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] += rotation;
	return PyBool_FromLong(1);
}
PyObject* Vpu::pySelect(PyObject* self, PyObject* args) {
	int index;
	if (!PyArg_ParseTuple(args, "i", &index)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer)return PyBool_FromLong(false);
	Vpu::select(*layer);
	return PyBool_FromLong(true);
}
PyObject* Vpu::pySelectSprite(PyObject* self, PyObject* args) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::sprites.find(handle) != Vpu::sprites.end()) {
		Vpu::select(Vpu::sprites.at(handle).picture);
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}
PyObject* Vpu::pySetColor(PyObject* self, PyObject* args) {
	float r;
	float g;
	float b;
	float a = 255.0f;
	if (!PyArg_ParseTuple(args, "fff|f", &r, &g, &b, &a)) return NULL;
	Vpu::setColor(r, g, b, a);
	return PyBool_FromLong(1);
}
PyObject* Vpu::pySetFrame(PyObject* self, PyObject* args) {
	int handle;
	int frame;
	if (!PyArg_ParseTuple(args, "ii", &handle, &frame)) return NULL;
	if (Vpu::animations.find(handle) != Vpu::animations.end()) {
		Animation* a = &(Vpu::animations.at(handle));
		a->current_frame = frame % (a->frame.size());
		return PyBool_FromLong(true);
	}
	printf("ERROR: anim_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pySetFont(PyObject* self, PyObject* args) {
	char* fontname;
	if (!PyArg_ParseTuple(args, "s", &fontname)) return NULL;
	std::vector<Font*>::iterator it;
	bool found = false;
	Font* f = Vpu::getFontByName(fontname);
	if (f) {
		Vpu::setFont(f);
		return PyBool_FromLong(true);
	}
	printf(("ERROR: font not found:"+std::string(fontname)+"\n").c_str());
	return PyBool_FromLong(false);
}
PyObject* Vpu::pySetRotation(PyObject* self, PyObject* args) {
	int index;
	float rotation;
	if (!PyArg_ParseTuple(args, "if", &index, &rotation)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(1);
	(*layer).rotation[0] = rotation;
	return PyBool_FromLong(1);
}
PyObject* Vpu::pySetScale(PyObject* self, PyObject* args) {
	int index;
	float scale_x;
	float scale_y;
	if (!PyArg_ParseTuple(args, "iff", &index, &scale_x, &scale_y)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(1);
	(*layer).scale[0] = scale_x;
	(*layer).scale[1] = scale_y;
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyScale(PyObject* self, PyObject* args) {
	int index;
	float scale_x;
	float scale_y;
	if (!PyArg_ParseTuple(args, "iff", &index, &scale_x, &scale_y)) return NULL;
	Surface* layer = getLayer(index);
	if (!layer) return PyBool_FromLong(1);
	(*layer).scale[0] += scale_x;
	(*layer).scale[1] += scale_y;
	return PyBool_FromLong(1);
}
PyObject* Vpu::pySubSprite(PyObject* self, PyObject* args) {
	long int handle;
	float left = -1;
	float top = -1;
	float right = -1;
	float bottom = -1;
	if (!PyArg_ParseTuple(args, "i|ffff", &handle, &left, &top, &right, &bottom)) return NULL;
	// Get sprite object
	if (Vpu::sprites.find(handle) != Vpu::sprites.end()) {
		Sprite* s = &Vpu::sprites.at(handle);
		// Fill empty variables (-1) with sprite geometry (to allow a 1:1 copy if not specified)
		if (left == -1) left = 0;
		if (top == -1) top = 0;
		if (right == -1) right = s->picture.width - 1;
		if (bottom == -1) bottom = s->picture.height - 1;
		// Return handle from new sprite
		handle = Vpu::createSubSprite(*s, left, top, right, bottom);
		return PyLong_FromLong(handle);
	}
	printf("ERROR: sprite_handle out of range\n");
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyTextOut(PyObject* self, PyObject* args) {
	char* text;
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "sff", &text, &x, &y)) return NULL;
	Vpu::print(std::string(text), x, y);
	return PyBool_FromLong(true);
}
extern std::vector<Pixel> listToPalette(PyObject* incoming);
PyObject* Vpu::pyTintSprite(PyObject* self, PyObject* args) {
	long int handle;
	PyObject* original; //list
	PyObject* updated; //list
	if (!PyArg_ParseTuple(args, "iOO", &handle, &original, &updated)) return NULL;
	// Get palettes
	std::vector<Pixel> pal_o = listToPalette(original);
	std::vector<Pixel> pal_u = listToPalette(updated);
	// Colorize sprite data
	if (Vpu::sprites.find(handle) != Vpu::sprites.end()) {
		Sprite* s = &Vpu::sprites.at(handle);
		Vpu::tintSprite(*s, pal_o, pal_u);
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}
PyObject* Vpu::pyTransition(PyObject* self, PyObject* args) {
	int type = -1;
	if (!PyArg_ParseTuple(args, "|i", &type)) return NULL;
	Curtain::enabled = true;
	if (type >= 0) {
		type %= CURTAIN_TYPE_SIZE;
		Curtain::type = (CurtainType)type;
	}
	return PyBool_FromLong(true);
}
PyObject* Vpu::pyUpdate(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "")) return NULL;
	Engine::update();
	if (!Engine::run) exit(-1);
	static bool ready = false;
	ready = Vpu::ready;
	Vpu::ready = false;
	return PyBool_FromLong(ready);
}
PyObject* Vpu::pyUpdateAnimation(PyObject* self, PyObject* args) {
	int handle;
	int frame = -1;
	if (!PyArg_ParseTuple(args, "i|i", &handle, &frame)) return NULL;
	if (Vpu::animations.find(handle) != Vpu::animations.end()) {
		Animation* a = &(Vpu::animations.at(handle));
		a->run(a->speed);
		if (frame >= 0)a->current_frame = frame;
		return PyBool_FromLong(true);
	}
	printf("ERROR: anim_handle out of range\n");
	return PyBool_FromLong(false);
}
