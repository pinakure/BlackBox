#include "showcase.hpp"
#include "sector.hpp"
#include "engine.hpp"
#include "world.hpp"
#include "camera.hpp"
#include <string>

DummyShowcase		dummy_showcase;
TypeWriterShowcase	typewriter_showcase;
SandboxShowcase		sandbox_showcase;

bool SandboxShowcase::initialize() {
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
}

void SandboxShowcase::update(float delta) {
	World::update(delta);
	if (Engine::epoch > 10) {
		if(World::origin) {
			World::origin->destroy(true);
			delete(World::origin);
			World::origin = new Sector(0, 0, 0);
		}
		Engine::epoch= 0;
	}
}

void SandboxShowcase::draw() {		
	World::draw();
	World::drawMiniMap();
}
