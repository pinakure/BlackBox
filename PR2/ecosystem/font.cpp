#include "font.hpp"
#include "engine.hpp"
#include "utils.hpp"

Font::Font(std::string filename, int size) {
	std::vector<std::string> parts = explode(filename, '.');
	if (parts.size()) name = parts[0];
	else name = filename;
	if (parts.size() > 1) size = atoi(parts[1].c_str());
	else size = 16;
	std::string file = "data/fonts/" + filename +".ttf";
	data = al_load_ttf_font(
		file.c_str(), 
		-size, 
		ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING
	);
	if (!data) {
		Engine::printf("WARNING: Failed to initialize font 'data/fonts/%s.ttf'.\n", filename.c_str());
		return;
	}
}

Font::~Font() {
	if (data) {
		al_destroy_font(data);
		data = NULL;
	}
}