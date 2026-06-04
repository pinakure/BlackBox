#include "font.hpp"
#include "engine.hpp"
#include "utils.hpp"
#include "vpu.hpp"

Font::Font(std::string filename, int size) {
	if (!filename.compare("legacy")) {
		data = al_create_builtin_font();
		height = 8;
		return;
	}
	std::vector<std::string> parts = explode(filename, '.');
	if (parts.size()) name = parts[0];
	else name = filename;
	if (parts.size() > 1) height = atoi(parts[1].c_str());
	else height = 16;
	std::string file = "fonts/" + filename +".ttf";
	data = al_load_ttf_font(
		file.c_str(), 
		-height, 
		ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING
	);
	if (!data) {
		Engine::printf("WARNING: Failed to initialize font '%s.ttf'.\n", filename.c_str());
		return;
	}
}

Font::~Font() {
	if (data) {
		al_destroy_font(data);
		data = NULL;
	}
}