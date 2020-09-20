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
	return;
	Vpu::select(Vpu::overlay[0]);
	Vpu::paint(0, 64, 0, 255);
	/*
	Vpu::select(Vpu::overlay[0]);
	Vpu::subdivide(200, 0, 64, 64);
	
	Vpu::select(Vpu::overlay[1]);
	Vpu::subdivide(0, 0, 200, 64);
	*/
	Vpu::select(Vpu::overlay[2]);
	Vpu::clear();
	Vpu::printInteger("Last Frame Events:", Engine::cycles	,  0,  0);
	Vpu::printInteger("Frames per Second:", Vpu::fps		,  0, 20);
	Vpu::printInteger("Engine Epoch time:", Engine::epoch	,  0, 40);		
	Vpu::printFloat("Rotation[0]:", Vpu::overlay[0].rotation[0], 0, 60);
	Vpu::printFloat("Rotation[1]:", Vpu::overlay[1].rotation[0], 0, 80);
	Vpu::printFloat("Bg.Scale[0]:", Vpu::background[0].scale[0], 0, 100);
	Vpu::printFloat("Bg.Scale[1]:", Vpu::background[1].scale[0], 0, 120);
	
}

void Hud::update() {
	static double delta = 1.0;
	TypeWriter::update(delta);	
}