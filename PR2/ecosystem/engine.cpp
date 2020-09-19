#include "engine.hpp"
#include "hud.hpp"

int Engine::width = 640;
int Engine::height = 480;
bool Engine::run = true;
int Engine::cycles = 0;
long int Engine::epoch= 0;
ALLEGRO_TIMER *Engine::timer = NULL;
ALLEGRO_TIMER *Engine::clock = NULL;
ALLEGRO_EVENT_QUEUE *Engine::queue = NULL;
ALLEGRO_EVENT Engine::event;	
	
bool Engine::initialize() {
	try{
		al_init();
		al_install_keyboard();
		clock   = al_create_timer(1.0);
		timer	= al_create_timer(1.0 / 60.0);
		queue	= al_create_event_queue();
		
		if (!Vpu::initialize()) return false;
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_timer_event_source(timer));
		al_register_event_source(queue, al_get_timer_event_source(clock));
		al_start_timer(timer);
		al_start_timer(clock);
		TypeWriter::initialize();
		TypeWriter::enqueue("Hello world!");
		TypeWriter::enqueue("Hello world!");
		TypeWriter::enqueue("Hello world!");
		TypeWriter::enqueue("Hello world!");
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Engine::deinitialize() {
	Vpu::initialize();
	if(timer) al_destroy_timer(timer);
	if(queue) al_destroy_event_queue(queue);
}

void Engine::tick() {
	epoch++;
	Vpu::fps = Vpu::frames;
	Vpu::frames = 0;
}

void Engine::handleEvents() {
	al_wait_for_event(queue, &event);
	switch (event.type) {
		case ALLEGRO_EVENT_TIMER:			
			if (event.timer.source == clock) return(tick());
			if(event.timer.source == timer) Vpu::redraw = true;
			break;
		//case ALLEGRO_EVENT_KEY_DOWN:
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			run = false;
			break;
		default:
			break;
	}		
}

void Engine::render() {
	if (Vpu::redraw && al_is_event_queue_empty(queue)) {
		Hud::draw();
		Vpu::render();		
		Vpu::frames++;
		cycles = 0;
	}
}

void Engine::loop() {
	while(run){
		Hud::update();
		handleEvents();		
		render();
		cycles++;
	}	
}