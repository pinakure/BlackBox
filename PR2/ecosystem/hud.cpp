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
	Vpu::select(Vpu::overlay[0]);
	Vpu::paint(0, 0, 0);
	Vpu::printInteger("Last Frame Events:", Engine::cycles	,  0,  0);
	Vpu::printInteger("Frames per Second:", Vpu::fps		,  0,  8);
	Vpu::printInteger("Engine Epoch time:", Engine::epoch	,  0, 16);	
	Vpu::printInteger("Color_Stack.len():", Vpu::color_stack.size(),  0, 24);	
}

void Hud::update() {
	static double delta = 1.0;
	TypeWriter::update(delta);	
}