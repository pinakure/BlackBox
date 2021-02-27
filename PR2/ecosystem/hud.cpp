#include "engine.hpp"
#include "hud.hpp"
#include "vpu.hpp"
#include <string>

bool Hud::initialize() {
	TypeWriter::initialize();
	GetTextBox::initialize();
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
	Vpu::pushColor();
	Vpu::setColor(128, 128, 0, 64);
	Vpu::pushFont();
	Vpu::setFont(Vpu::smallest_font);
	Vpu::printf(right,  0, ALLEGRO_ALIGN_RIGHT, "FPS %#6d", Vpu::fps);
	//Vpu::printf(right, Vpu::font->height, ALLEGRO_ALIGN_RIGHT, "CPS %#6d", Engine::cycles);
	Vpu::printf(right, Vpu::font->height, ALLEGRO_ALIGN_RIGHT, " T  %#6d", Vpu::total_frames);
	Vpu::popFont();
	Vpu::popColor();
}

void Hud::update(float delta) {
	TypeWriter::update(delta);	
}