#include "engine.hpp"
#include "input.hpp"
#include "hud.hpp"
#include "console.hpp"
#include "world.hpp"
#include "camera.hpp"

int Engine::width = 640;
int Engine::height = 480;
bool Engine::run = true;
int Engine::cycles = 0;
long int Engine::epoch= 0;
ALLEGRO_TIMER *Engine::timer = NULL;
ALLEGRO_TIMER *Engine::clock = NULL;
ALLEGRO_EVENT_QUEUE *Engine::queue = NULL;
ALLEGRO_EVENT Engine::event;	

#define WORLD_SIZE 65535
#define NPC_COUNT 128

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
		TypeWriter::initialize();
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue("Welcome to BlackBox");
		TypeWriter::enqueue(" ");

		if (!World::initialize()) {
			std::printf("ERROR: Cannot initialize World.\n");
			return false;
		}
		int mon = (Sector::size * (WORLD_SIZE/2) * World::cell_size)+(Sector::size/2);
		/*World::setOrigin(new Sector(
			mon/(Sector::size*World::cell_size), 
			mon/(Sector::size*World::cell_size), 
			mon/(Sector::size*World::cell_size)
		));*/
		Camera::target_x = World::origin->subjective_x;
		Camera::target_y = World::origin->subjective_y;
		Camera::target_z = World::origin->subjective_z;
		World::origin->requestRedraw();

		return true;
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
		Vpu::paint(0, 64, 0, 64);	
		// -----------------------
		World::draw();
		//World::drawMiniMap();
		Hud::draw();
		InputDevice::draw(11);
		Console::draw(16);
		Vpu::render();		
		Vpu::frames++;
		Vpu::total_frames++;
		cycles = 0;
	}
}

void Engine::update() {
	if(int(_rotation*100))
		rotate(_rotation);
	if(int(_scale[0]*100))
		scale(
			_scale[0], 
			_scale[1],
			_scale[2]
		);
	World::update(1.0);
	Camera::update();
	Hud::update();
	InputDevice::update(1);
	Console::update();
	render();
	handleEvents();
	cycles++;
}

void Engine::loop() {
	while(run){
		update();
		if (Engine::epoch > 10) {
			if(World::origin) {
				World::origin->destroy(true);
				delete(World::origin);
				World::origin = new Sector(0, 0, 0);
			}
			Engine::epoch= 0;
		}
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
