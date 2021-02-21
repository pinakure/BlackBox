#include "engine.hpp"
#include "input.hpp"
#include "hud.hpp"
#include "console.hpp"
#include "world.hpp"
#include "camera.hpp"

#define WORLD_SIZE					65535
#define NPC_COUNT					128
int Engine::width					= 640;
int Engine::height					= 480;
bool Engine::run					= true;
int Engine::cycles					= 0;
long int Engine::epoch				= 0;
ALLEGRO_TIMER *Engine::timer		= NULL;
ALLEGRO_TIMER *Engine::clock		= NULL;
ALLEGRO_EVENT_QUEUE *Engine::queue	= NULL;
ALLEGRO_EVENT Engine::event;	
/* 
---------------------------------------------------------------------------------------- 
 Point to the showcase we want to test on runtime 
---------------------------------------------------------------------------------------- 
 - dummy_showcase : do nothing
 - typewriter_showcase : test typewriter display box
 - sandbox_showcase : initialize world, update and draw , also calls typewriter_showcase






*/
#include "showcase.cpp"

Showcase *showcase = 
	&dummy_showcase
//	&sandbox_showcase		
	;
/* 
---------------------------------------------------------------------------------------- 
#
#
#
#
#
#
#
#
#
#
#
#
---------------------------------------------------------------------------------------- 
*/
bool Engine::initialize() {
	try{
		al_init();
		clock   = al_create_timer(1.0);
		timer	= al_create_timer(1.0 / 120.0);
		queue	= al_create_event_queue();
		
		if (!Vpu::initialize()) return false;
		CVar::initialize();
		Console::initialize();
		if (!InputDevice::initialize())return false;
		al_register_event_source(queue, al_get_timer_event_source(timer));
		al_register_event_source(queue, al_get_timer_event_source(clock));
		al_start_timer(timer);
		al_start_timer(clock);
		Hud::initialize();
		Script::execute("from scripts.main import test");
		Script::execute("from scripts.main import menu");
		return showcase->initialize();
	} catch (int e) {
		e = e;
		return false;
	}
}

void Engine::deinitialize() {
	World::deinitialize();
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
			al_acknowledge_resize(Vpu::display);
			Engine::width = event.display.width;
			Engine::height = event.display.height;
			if (!Vpu::start()) run = false;
			if (!Vpu::restart()) run = false;
			break;
		
		case ALLEGRO_EVENT_KEY_DOWN:
			if(InputDevice::in_keyboard) InputDevice::handleEvent(event);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			if(InputDevice::in_keyboard) InputDevice::handleEvent(event);
			break;
			
		default:
			break;
	}		
}

void Engine::render() {
	if (Vpu::redraw && al_is_event_queue_empty(queue)) {
		// Clean before drawing HUD
		Vpu::select(Vpu::overlay);
		Vpu::paint(0, 0, 0, 16);	
		// -----------------------
		showcase->draw();
		Hud::draw();
		InputDevice::draw(11);
		Console::draw(16);
		Vpu::render();		
		Vpu::frames++;
		Vpu::total_frames++;
		cycles = 0;
	}
}

#include <chrono>
auto tp1 = std::chrono::system_clock::now();
auto tp2 = std::chrono::system_clock::now();

void Engine::update() {
	
	
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
    float fElapsedTime = elapsedTime.count();
    // From this line on, every value must be multiplied by fElapsedTime
    
	if(int(_rotation*100))
		rotate(_rotation);
	if(int(_scale[0]*100))
		scale(
			_scale[0], 
			_scale[1],
			_scale[2]
		);
	showcase->update(fElapsedTime);
	Camera::update(fElapsedTime);
	Hud::update(fElapsedTime);
	InputDevice::update(fElapsedTime);
	Console::update();//!!!
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



float Engine::_rotation = 0.0f;
void Engine::rotate(float r) {
	Vpu::select(Vpu::background);
	_rotation = r;
	Vpu::target->rotation[0]+=_rotation;
}

float Engine::_scale[3] = {
	0.0f, 0.0f, 0.0f,
};
void Engine::scale(float x, float y, float z) {
	Vpu::select(Vpu::background);
	_scale[0] = x;
	_scale[1] = y;
	_scale[2] = z;
	if (x > 2.0f)x = 2.0f;
	if (y > 2.0f)y = 2.0f;	
	if((Vpu::target->scale[0] + _scale[0] >= 0.5f)&&(Vpu::target->scale[0] + _scale[0] <= 2.0f)) Vpu::target->scale[0] += _scale[0];
	if((Vpu::target->scale[1] + _scale[1] >= 0.5f)&&(Vpu::target->scale[1] + _scale[1] <= 2.0f)) Vpu::target->scale[1] += _scale[1];
	if((Vpu::target->scale[2] + _scale[2] >= 0.5f)&&(Vpu::target->scale[2] + _scale[2] <= 2.0f)) Vpu::target->scale[2] += _scale[2];
	
	std::printf("Vpu::setScale(%f,%f,%f)\n", Vpu::target->scale[0],Vpu::target->scale[1],Vpu::target->scale[2]);
}
