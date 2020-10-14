#include "engine.hpp"
#include "hud.hpp"
#include "vpu.hpp"
#include <string>

bool Hud::initialize() {
	return true;
}


void Hud::draw() {
	drawDebugInfo();
	TypeWriter::draw();	
}

void Hud::drawDebugInfo() {
	Vpu::select(Vpu::overlay);
	int x = Vpu::target->width  >> 2;
	int y = Vpu::target->height >> 4;
	int right = Vpu::overlay.width - (Vpu::overlay.width >> 2);
	Vpu::printf(right,  0, ALLEGRO_ALIGN_RIGHT, "FPS %#6d", Vpu::fps);
	Vpu::printf(right,  8, ALLEGRO_ALIGN_RIGHT, "CPS %#6d", Engine::cycles);
	Vpu::printf(right, 16, ALLEGRO_ALIGN_RIGHT, " T  %#6d", Vpu::total_frames);	
}

void Hud::update(float delta) {
	TypeWriter::update(delta);	
}