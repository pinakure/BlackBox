#include "vpu.hpp"
#include "engine.hpp"
#include "console.hpp"

Surface Vpu::console;
Surface Vpu::overlay[4];
Surface Vpu::background[4];
Surface Vpu::foreground[4];
Surface *Vpu::__layers[12] = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL
};

std::vector<ALLEGRO_COLOR> Vpu::color_stack;
ALLEGRO_BITMAP *Vpu::target = NULL;
ALLEGRO_BITMAP *Vpu::buffer= NULL;
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
ALLEGRO_COLOR Vpu::transparent;
ALLEGRO_FONT *Vpu::font = NULL;
ALLEGRO_FONT *Vpu::legacy_font = NULL;

void prepareTests() {
	for (int i = 0; i < 4; i++) {
		Vpu::foreground[i].enabled = false;
	}
	/*Vpu::background[0].enabled = true;		

	Vpu::select(Vpu::background[0]);
	Vpu::subdivide(200, 120, 0, 64);
	Vpu::randomize();*/
}

void Vpu::subdivide(int r, int g, int b, int alpha) {
	Vpu::paint(0, 0, 0, 0);
	int w = al_get_bitmap_width(Vpu::target)/2;
	int h = al_get_bitmap_height(Vpu::target)/2;
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN    , w, h, r		, g		 ,b		 , alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN    , w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN + h, w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN + h, w, h, r		, g		 ,b		 , alpha);
}

void Vpu::randomize() {	
	int w = al_get_bitmap_width( target );
	int h = al_get_bitmap_height( target );
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
	for (int i = 0; i < 12; i++) {
		select(*__layers[i]);
		paint(0, 0, 0, 0);
	}
}

bool Vpu::start() {		
	width = Engine::width*2;
	height = Engine::height*2;
	int flags = ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
	al_set_new_display_flags(fullscreen ? flags | ALLEGRO_FULLSCREEN : flags);
	if (display) {
		al_unregister_event_source(Engine::queue, al_get_display_event_source(display));
		al_destroy_display(display);
		display = NULL;
	}
	display = al_create_display(width/2, height/2);
	if (!display) {
		Engine::printf("Cannot initialize %s %dx%d display\n", fullscreen?"fullscreen":"",width/2, height/2);
		return false;
	}
	al_register_event_source(Engine::queue, al_get_display_event_source(display));	
	return true;
}

void Vpu::initializeFonts() {
	font = al_load_ttf_font("data/fonts/big.ttf",16,0 );
	if (!font){
		Engine::printf("WARNING: Failed to initialize font 'data/fonts/big.ttf'.\nUsing default font.\n");
		font = legacy_font;
	}
}

bool Vpu::restart() {
	for (int i = 0; i < 12; i++) {
		if(__layers[i]){
			destroySurface(*__layers[i]);
			__layers[i] = NULL;
		}
	}
	is_initialized = false;
	for (int i = 0; i < 4; i++) {
		overlay[i] = createBitmap((width/2)+(VPU_OVERSCAN*2), (height/2)+(VPU_OVERSCAN*2));
		if(!overlay[i].enabled) return false;
		select(overlay[i]);
		clear();
		__layers[i+8] = &overlay[i];
		foreground[i]= createBitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
		if(!foreground[i].enabled) return false;
		select(foreground[i]);
		clear();
		__layers[i+4] = &foreground[i];
		background[i]= createBitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
		if(!background[i].enabled) return false;
		select(background[i]);
		clear();
		__layers[ i ] = &background[i];
		prepareTests();
	}

	// (Re)initialize squash framebuffer
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	buffer = al_create_bitmap(width, height);
	if(!buffer) return false;			
	// (Re)initialize console framebuffer
	destroySurface(console);
	destroySurface(Console::bitmap);
	destroySurface(Console::backdrop);
	console = createBitmap(width/2, height/2);
	if(!console.enabled) return false;	
	Vpu::select(console);
	Vpu::paint(0, 0, 0, 0);

	// (Re)define default colors and reset flags
	transparent = al_map_rgba(0, 0, 0, 0);	
	setColor(255, 255, 255);
	redraw = true;
	is_initialized = true;
	al_set_window_title(display, "BlackBox v.3");		
	return true;
}

bool Vpu::initialize() {
	try {
		if (!start())return false;		
		if (!al_init_font_addon()) return false;
		if (!al_init_ttf_addon()) return false;
		if (!al_init_primitives_addon())return false;
		if (!al_init_image_addon())return false;		
		legacy_font = al_create_builtin_font();
		font = legacy_font;	
		if (!restart()) return false;
		initializeFonts();			
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::paint(int r, int g, int b, int alpha) {
	al_clear_to_color(al_map_rgba(r, g, b, alpha));
}

void Vpu::select(Surface &target) {
	Vpu::target = target.bitmap;
	al_set_target_bitmap(Vpu::target);
}

void Vpu::deinitialize() {
	if(  font ) al_destroy_font(font);
	if(display) al_destroy_display(display);
	destroySurface(console);
	for (int i = 0; i < 4; i++) {		
		destroySurface(overlay[i]);
		destroySurface(foreground[i]);
		destroySurface(background[i]);		
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

void Vpu::printf(int  x, int y, int flags, const char *fmt, ...) {
	char buffer[1024];
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buffer, 1024, fmt, ap);
	va_end(ap);

	al_draw_textf(font, shadow, x+1, y+1, flags, buffer);	
	al_draw_textf(font, color , x  , y  , flags, buffer);	
}

void Vpu::print(std::string text, int  x, int y, int flags) {
	al_draw_textf(font, shadow, x+1, y+1, flags, text.c_str());	
	al_draw_textf(font, color , x  , y  , flags, text.c_str());	
}
void Vpu::printInteger(std::string text, int d, int  x, int y, int flags) {
	al_draw_textf(font, shadow, x+1, y+1, flags, (text + "%d").c_str(), d);
	al_draw_textf(font, color , x  , y  , flags, (text + "%d").c_str(), d);
}
void Vpu::printFloat(std::string text, float d, int  x, int y, int flags) {
	al_draw_textf(font, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printDouble(std::string text, double d, int  x, int y, int flags) {
	al_draw_textf(font, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printBool(std::string text,bool d, int  x, int y, int flags) {
	al_draw_textf(font, shadow, x+1, y+1, flags, (text + "%b").c_str(), d);
	al_draw_textf(font, color , x  , y  , flags, (text + "%b").c_str(), d);
}

void Vpu::fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		al_map_rgba(r, g, b, alpha)
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

void Vpu::render() {
	
	al_set_target_bitmap(buffer);
	/* Render enabled layers */
	#define renderlayer(l,i) if(l[i].enabled)							\
								 al_draw_scaled_rotated_bitmap(			\
									l[i].bitmap,						\
									l[i].width/2, l[i].height/2,		\
									l[i].width/4, l[i].height/4,		\
									l[i].scale[0],l[i].scale[1],		\
									l[i].rotation[0],					\
									0									\
								);
		renderlayer(background, 0);
		renderlayer(background, 1);
		renderlayer(background, 2);
		renderlayer(background, 3);
		renderlayer(foreground, 0);
		renderlayer(foreground, 1);
		renderlayer(foreground, 2);
		renderlayer(foreground, 3);
	#undef renderlayer	
	#define renderlayer(l,i) if(l[i].enabled)							\
								 al_draw_scaled_rotated_bitmap(			\
									l[i].bitmap,						\
									l[i].width/2, l[i].height/2,		\
									l[i].width/2, l[i].height/2,		\
									l[i].scale[0],l[i].scale[1],		\
									l[i].rotation[0],					\
									0									\
								);
		renderlayer(overlay, 0);
		renderlayer(overlay, 1);
		renderlayer(overlay, 2);
		renderlayer(overlay, 3);
		
	#undef renderlayer	
	if (console.enabled)
		al_draw_scaled_bitmap(console.bitmap, 0, 0, width/2, height/2, 0, 0, al_get_display_width(display), al_get_display_height(display), 0);

	al_set_target_backbuffer(display);
	al_draw_bitmap(buffer, 0, 0, 0);	
	al_flip_display();
	redraw = false;
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
		Vpu::color_stack.pop_back();
	}
}

void Vpu::putpixel(int x, int y) {
	al_put_pixel(x, y, color);
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

void Vpu::drawSurface(Surface &surface,float sx, float sy, float sw, float sh, float dx, float dy) {
	al_draw_bitmap_region(surface.bitmap, sx, sy, sw, sh, dx, dy, 0);
	//al_draw_bitmap(surface.bitmap, 0, 0, 0);
}

Surface Vpu::createBitmap(int width, int height) {
	Surface s;
	s.bitmap = al_create_bitmap(width, height);
	s.enabled = !(s.bitmap == NULL);
	if (s.enabled) {
		s.width  = width;
		s.height = height;
	}
	return s;
}

Surface Vpu::loadBitmap(std::string filename) {
	Surface s;
	s.bitmap = al_load_bitmap(("./"+filename).c_str());	
	s.enabled = s.bitmap != NULL;
	if (s.enabled) {
		s.width  = al_get_bitmap_width(s.bitmap);
		s.height = al_get_bitmap_height(s.bitmap);
	}
	return s;
}

void Vpu::fadeout() {
	select(*__layers[11]);
	al_draw_bitmap(buffer, 0, 0, 0);
	float alpha = 0.0f;
	while (alpha < 255.0f) {
		al_set_target_bitmap(buffer);
		al_draw_bitmap((*__layers[11]).bitmap, 0, 0, 0);	
		al_draw_filled_rectangle(0, 0, width, height, al_map_rgba(0, 0, 0, alpha));
		al_set_target_backbuffer(display);
		al_draw_bitmap(buffer, 0, 0, 0);	
		al_flip_display();
		alpha += 1.5f;
	}	
}

void Vpu::fadein() {
	select(*__layers[11]);
	al_draw_bitmap(buffer, 0, 0, 0);
	float alpha = 255.0f;
	while (alpha > 0.0f) {
		al_set_target_bitmap(buffer);
		al_draw_bitmap((*__layers[11]).bitmap, 0, 0, 0);	
		al_draw_filled_rectangle(0, 0, width, height, al_map_rgba(0, 0, 0, alpha));
		al_set_target_backbuffer(display);
		al_draw_bitmap(buffer, 0, 0, 0);	
		al_flip_display();
		alpha -= 1.5f;
	}	
}

void Vpu::lock() {
	al_lock_bitmap( target ,al_get_bitmap_format( target ), ALLEGRO_LOCK_WRITEONLY);	
}

void Vpu::unlock() {
	al_unlock_bitmap(target);
}
	
/*
Example layer alterations:
	overlay[0].rotation[0] += 0.01f;
	overlay[1].rotation[0] += 0.0125f;
	background[0].rotation[0] -= 0.05f;
	background[0].scale[0] += 0.1f;
	background[0].scale[1] += 0.1f;
	if (background[0].scale[0] > 200000.0f)background[0].scale[0] = -200000.0f;
	if (background[0].scale[1] > 200000.0f)background[0].scale[1] = -200000.0f;
	//overlay[0].rotation[0] = 3.141519f; // 180 deg
	//overlay[1].rotation[0] = 3.141519f; // 180 deg
	if(overlay[0].scale[0]>0.0f) overlay[0].scale[0] -= 0.001f;
	if(overlay[0].scale[1]>0.0f) overlay[0].scale[1] -= 0.001f;
	if(overlay[1].scale[0]>0.0f) overlay[1].scale[0] -= 0.00125f;
	if(overlay[1].scale[1]>0.0f) overlay[1].scale[1] -= 0.00125f;
*/

void Vpu::loadVars() {

}
