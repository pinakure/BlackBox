#include "vpu.hpp"
#include "engine.hpp"

Surface Vpu::overlay[4];
Surface Vpu::background[4];
Surface Vpu::foreground[4];

ALLEGRO_BITMAP *Vpu::target = NULL;
int Vpu::frames = 0;
int Vpu::fps = 0;
int Vpu::width = 0;
int Vpu::height = 0;
int Vpu::scroll[2] = { 0, 0 };
bool Vpu::redraw = true;
ALLEGRO_DISPLAY *Vpu::display = NULL;
ALLEGRO_FONT *Vpu::font = NULL;

#define VPU_OVERSCAN 32

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
			overlay[i].bitmap    = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!overlay[i].bitmap) return false;
			foreground[i].bitmap = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!foreground[i].bitmap) return false;
			background[i].bitmap = al_create_bitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
			if(!background[i].bitmap) return false;
		}
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::select(Surface &target) {
	Vpu::target = target.bitmap;
	al_set_target_bitmap(Vpu::target);
}

void Vpu::deinitialize() {
	if(  font ) al_destroy_font(font);
	if (display) al_destroy_display(display);
	al_shutdown_primitives_addon();
}

void Vpu::print(std::string text, int  x, int y) {
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, text.c_str());	
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
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Last Frame Events: %d", Engine::cycles);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 8, 0, "Frames per Second: %d", Vpu::fps);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0,16, 0, "Engine Epoch time: %ld", Engine::epoch);
	al_flip_display();
	redraw = false;
}
