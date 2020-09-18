#include "vpu.hpp"
#include "engine.hpp"

Surface Vpu::overlay[4];
Surface Vpu::background[4];
Surface Vpu::foreground[4];

std::vector<ALLEGRO_COLOR> Vpu::color_stack;
ALLEGRO_BITMAP *Vpu::target = NULL;
ALLEGRO_BITMAP *Vpu::buffer= NULL;
int Vpu::frames = 0;
int Vpu::fps = 0;
int Vpu::width = 0;
int Vpu::height = 0;
int Vpu::scroll[2] = { 0, 0 };
bool Vpu::redraw = true;
ALLEGRO_DISPLAY *Vpu::display = NULL;
ALLEGRO_COLOR Vpu::color;
ALLEGRO_COLOR Vpu::shadow;
ALLEGRO_FONT *Vpu::font = NULL;

#define VPU_OVERSCAN 32

void prepareTests() {
	Vpu::overlay[2].enabled = false;
	Vpu::overlay[3].enabled = false;
	Vpu::foreground[0].enabled = false;
	Vpu::foreground[1].enabled = false;
	Vpu::foreground[2].enabled = false;
	Vpu::foreground[3].enabled = false;
	Vpu::background[0].enabled = false;
	Vpu::background[1].enabled = false;
	Vpu::background[2].enabled = false;
	Vpu::background[3].enabled = false;
}

bool Vpu::initialize() {
	try {
		width = Engine::width;
		height = Engine::height;
		display = al_create_display(width, height);
		font = al_create_builtin_font();
		redraw = true;
		al_register_event_source(Engine::queue, al_get_display_event_source(display));
		if (!al_init_primitives_addon())return false;
		
		for (int i = 0; i < 4; i++) {
			overlay[i].bitmap = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!overlay[i].bitmap) return false;
			overlay[i].enabled = true;
			Vpu::select(overlay[i]);
			Vpu::paint(0, 0, 0, 0);
			al_convert_mask_to_alpha(overlay[i].bitmap, al_map_rgba(0, 0, 0,0));
			
			foreground[i].bitmap = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!foreground[i].bitmap) return false;
			foreground[i].enabled = true;
			Vpu::select(foreground[i]);
			Vpu::paint(255, 0,255);
			al_convert_mask_to_alpha(foreground[i].bitmap, al_map_rgba(255, 0, 255,255));
			
			background[i].bitmap = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!background[i].bitmap) return false;
			background[i].enabled = true;
			Vpu::select(background[i]);
			Vpu::paint(255, 0,255);
			al_convert_mask_to_alpha(background[i].bitmap, al_map_rgba(255, 0, 255,255));
			prepareTests();
		}
		buffer = al_create_bitmap(width, height);
		if(!buffer) return false;			

		setColor(255, 255, 255);

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
	for (int i = 0; i < 4; i++) {
		if (overlay[i].bitmap) {
			al_destroy_bitmap(overlay[i].bitmap);
			overlay[i].bitmap = NULL;
		}
		if (foreground[i].bitmap) {
			al_destroy_bitmap(foreground[i].bitmap);
			foreground[i].bitmap = NULL;
		}
		if (background[i].bitmap) {
			al_destroy_bitmap(background[i].bitmap);
			background[i].bitmap = NULL;
		}
	}
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	al_shutdown_primitives_addon();
}

void Vpu::setColor(int r, int g, int b, int alpha) {
	color  = al_map_rgba(r, g, b, alpha);
	shadow = al_map_rgba(r/2, g/2, b/2, alpha);
}

void Vpu::print(std::string text, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, text.c_str());	
	al_draw_textf(font, color , x  , y  , 0, text.c_str());	
}
void Vpu::printInteger(std::string text, int d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%d").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%d").c_str(), d);
}
void Vpu::printFloat(std::string text, float d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%f").c_str(), d);
}
void Vpu::printDouble(std::string text, double d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%f").c_str(), d);
}
void Vpu::printBool(std::string text,bool d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%b").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%b").c_str(), d);
}

void Vpu::fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		al_map_rgba(r, g, b, alpha)
	);
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
	if(background[0].enabled) al_draw_bitmap(background[0].bitmap, 0, 0, 0);
	if(background[1].enabled) al_draw_bitmap(background[1].bitmap, 0, 0, 0);
	if(background[2].enabled) al_draw_bitmap(background[2].bitmap, 0, 0, 0);
	if(background[3].enabled) al_draw_bitmap(background[3].bitmap, 0, 0, 0);	
	
	if(foreground[0].enabled) al_draw_bitmap(foreground[0].bitmap, 0, 0, 0);
	if(foreground[1].enabled) al_draw_bitmap(foreground[1].bitmap, 0, 0, 0);
	if(foreground[2].enabled) al_draw_bitmap(foreground[2].bitmap, 0, 0, 0);
	if(foreground[3].enabled) al_draw_bitmap(foreground[3].bitmap, 0, 0, 0);	
	
	if(overlay[0].enabled) al_draw_bitmap(overlay[0].bitmap, 0, 0, 0);
	if(overlay[1].enabled) al_draw_bitmap(overlay[1].bitmap, 0, 0, 0);
	if(overlay[2].enabled) al_draw_bitmap(overlay[2].bitmap, 0, 0, 0);
	if(overlay[3].enabled) al_draw_bitmap(overlay[3].bitmap, 0, 0, 0);

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
