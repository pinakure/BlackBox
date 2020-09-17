#include "engine.hpp"

bool Engine::run = true;
bool Engine::redraw = true;
ALLEGRO_TIMER *Engine::timer;
ALLEGRO_EVENT_QUEUE *Engine::queue;
ALLEGRO_DISPLAY *Engine::display;
ALLEGRO_FONT *Engine::font;
ALLEGRO_EVENT Engine::event;	
	
bool Engine::initialize() {
	try{
		al_init();
		al_install_keyboard();
		timer	= al_create_timer(1.0 / 60.0);
		queue	= al_create_event_queue();
		display = al_create_display(320, 200);
		font	= al_create_builtin_font();
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_display_event_source(display));
		al_register_event_source(queue, al_get_timer_event_source(timer));
		redraw = true;
		al_start_timer(timer);
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Engine::deinitialize() {
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
}

void Engine::handleEvents() {
	al_wait_for_event(queue, &event);
	switch (event.type) {
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			run = false;
			break;
		default:
			break;
	}		
}

void Engine::render() {
	if (redraw && al_is_event_queue_empty(queue)) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
		al_flip_display();
		redraw = false;
	}
}

void Engine::loop() {
	while(run){
		handleEvents();
		render();
	}	
}