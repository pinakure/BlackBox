#include "engine.hpp"
#include "input.hpp"
#include "hud.hpp"
#include "console.hpp"

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
		clock   = al_create_timer(1.0);
		timer	= al_create_timer(1.0 / 60.0);
		queue	= al_create_event_queue();
		
		if (!Vpu::initialize()) return false;
		if (!InputDevice::initialize())return false;
		al_register_event_source(queue, al_get_timer_event_source(timer));
		al_register_event_source(queue, al_get_timer_event_source(clock));
		al_start_timer(timer);
		al_start_timer(clock);
		CVar::initialize();
		Console::initialize();
		TypeWriter::initialize();
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue("Welcome to BlackBox");
		TypeWriter::enqueue(" ");
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Engine::deinitialize() {
	//TypeWriter::deInitialize();
	Console::deInitialize();
	InputDevice::deinitialize();
	Vpu::deinitialize();
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
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			{
				al_acknowledge_resize(Vpu::display);
				Engine::width = event.display.width;
				Engine::height = event.display.height;
				if (!Vpu::start()) run = false;
				if (!Vpu::restart()) run = false;
				break;
			}
		default:
			break;
	}		
}

void Engine::render() {
	if (Vpu::redraw && al_is_event_queue_empty(queue)) {
		Hud::draw();
		Console::draw(12);
		Vpu::render();		
		Vpu::frames++;
		Vpu::total_frames++;
		cycles = 0;
	}
}

void Engine::update() {
	Hud::update();
	Console::update();
	render();
	handleEvents();
	cycles++;
}

void Engine::loop() {
	while(run){
		update();
	}	
}
void Engine::error(std::string text) {
	if (Console::initialized) {
		Console::print(text);
		return;
	}
	fprintf(stderr, "%s\n", text.c_str());
}

void Engine::printf(const char *fmt, ...){
	char buffer[1024];

	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buffer, 1024, fmt, ap);
	va_end(ap);

	Engine::print(buffer);
}

void Engine::print(std::string text) {	
	if (Console::initialized) {
		Console::print(text);
		return;
	}
	fprintf(stdout, "%s\n", text.c_str());
}