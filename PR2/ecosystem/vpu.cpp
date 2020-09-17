#include "vpu.hpp"
#include "engine.hpp"

int Vpu::frames = 0;
int Vpu::fps = 0;
int Vpu::width = 0;
int Vpu::height = 0;
bool Vpu::redraw = true;
ALLEGRO_DISPLAY *Vpu::display = NULL;
ALLEGRO_FONT *Vpu::font = NULL;

bool Vpu::initialize() {
	try {
		width = Engine::width;
		height = Engine::height;
		display = al_create_display(width, height);
		font = al_create_builtin_font();
		redraw = true;
		al_register_event_source(Engine::queue, al_get_display_event_source(display));
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::deinitialize() {
	if(  font ) al_destroy_font(font);
	if (display) al_destroy_display(display);
}

void Vpu::render() {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Last Frame Events: %d", Engine::cycles);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 8, 0, "Frames per Second: %d", Vpu::fps);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0,16, 0, "Engine Epoch time: %ld", Engine::epoch);
	al_flip_display();
	redraw = false;
}
