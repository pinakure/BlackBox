#include "dashboard.hpp"
#include "vpu.hpp"
#include "input.hpp"
#include "typewriter.hpp"
#include "console.hpp"
extern char key[256];

int  Dashboard::active_index = 0;
bool Dashboard::enabled = false;
int Dashboard::cursor_x = 0;
int Dashboard::cursor_y = 0;
int Dashboard::columns = 4;
int Dashboard::rows = 3;

std::vector<DashboardTitle> Dashboard::titles;

void Dashboard::addTitle(std::string name,std::string url,std::string picture,std::string genre,
						bool multiplayer,bool cooperative,bool joystick,bool mouse,bool keyboard,
						int rating, std::string released, char* parent) {
	Dashboard::titles.push_back(
		DashboardTitle(
			name,
			url,
			picture,
			genre,
			multiplayer,
			cooperative,
			joystick,
			mouse,
			keyboard,
			rating,
			released,
			parent
		)
	);
	if (titles.size() == 0) {
		printf("Titles found at data/toc.py, dashboard enabled.\n");
		Dashboard::enabled = true;
	}
	printf(" * Added Title '%s' to the dashboard.\n", name.c_str());
}

DashboardTitle::DashboardTitle(std::string name, std::string url, std::string picture, std::string genre,
								bool  multiplayer, bool	cooperative, bool joystick,	bool mouse,bool	keyboard,
								int	rating, std::string released, char* parent){
	this->name = name;
	this->url = url;
	this->picture = picture;
	this->genre = genre;
	this->multiplayer = multiplayer;
	this->cooperative = cooperative;
	this->joystick = joystick;
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->rating = rating;
	this->parent = NULL;//TODO: create hierarchy
}

void Dashboard::draw(){
	const int bar_height = 32;
	const int left = Vpu::width / 4;
	const int footer = (Vpu::height / 2) - TypeWriter::line_height;
	const int top = 0;;
	const int width = Vpu::width / 2;
	const int title_area_padding = 8;
	const int title_area_width  = width - (title_area_padding << 1);
	const int title_area_height = footer-(top+bar_height)-(title_area_padding << 1);
	const int title_area_left = left + title_area_padding;
	const int title_area_top = top + bar_height + title_area_padding;
	const int title_columns = columns;
	const int title_rows = rows;
	const int title_width  = (title_area_width  - (title_area_padding * (title_columns - 1))) / title_columns;
	const int title_height = (title_area_height - (title_area_padding * (title_rows - 1))) / title_rows;



	Vpu::clear();
	// Draw title art graphic
	Vpu::fillRectangle(
		left, top, 
		width, bar_height, 
		0, 255, 0, 128
	);
	// Draw title boxes area
	Vpu::fillRectangle(
		title_area_left	 , title_area_top,
		title_area_width , title_area_height,
		TypeWriter::r >> 3,
		TypeWriter::g >> 3,
		TypeWriter::b >> 3,
		TypeWriter::a >> 3
	);
	// Draw titles area
	int title_index = 0;
	if (titles.size()) {
		for (int y = 0; y < title_rows; y++) {
			for (int x = 0; x < title_columns; x++, title_index++) {
				if (title_index >= Dashboard::titles.size())break;
				int dy = title_area_top + (y * title_height) + (title_area_padding * y);
				DashboardTitle* title = &Dashboard::titles[title_index];
				int dx = title_area_left + (x * title_width) + (title_area_padding * x);
				title->draw(dx, dy, title_width, title_height, title_index == active_index);
			}
			if (title_index >= Dashboard::titles.size())break;
		}
	}

	// Draw information scrolling marquee
	Vpu::fillRectangle(
		left, footer,
		width, bar_height,
		0, 0, 0, 255
	);
	Vpu::pushColor();
	Vpu::setColor(al_map_rgb(255, 255, 255));
	std::string msg = "Welcome to the Dashboard";
	static float offset = 0.0f;
	size_t max_offset = al_get_text_width(Vpu::font->data, msg.c_str());
	offset += 0.25f;
	if (int(offset) >= int(max_offset)) {
		offset = -width;
	}
	Vpu::print(msg, (left)-offset, footer , 0);
	Vpu::popColor();
}

void Dashboard::update(double delta) {
	int temp_x = cursor_x;
	int temp_y = cursor_y;
	int temp_i = active_index;
	if (Console::enabled)return;
	if (KEYDOWN(key[ALLEGRO_KEY_LEFT])) {
		cursor_x--;
		if (cursor_x < 0)cursor_x = columns - 1; 		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_RIGHT])) {
		cursor_x++;
		if (cursor_x > columns)cursor_x = 0;		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_UP])) {
		cursor_y--;
		if (cursor_x < 0)cursor_y = rows - 1;		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_DOWN])) {
		cursor_y++;
		if (cursor_y > rows)cursor_y = 0;		
	}
	active_index = (cursor_y * columns) + cursor_x;
	if (active_index >= titles.size()) {
		cursor_x = temp_x;
		cursor_y = temp_y;
		active_index = temp_i;
	}
	printf("Updating Dashboard %f                    \r", delta);
}

void DashboardTitle::draw(int x, int y, int width, int height, bool active) {
	static float i = 0.0f;
	i += 0.75f;
	const int r = TypeWriter::r;
	const int g = TypeWriter::g;
	const int b = TypeWriter::b;
	const int a = (TypeWriter::a) - (active ? int(i) % (TypeWriter::a >> 1) : 0);
	Vpu::fillRectangle(
		x-1, y,
		width+2, height,
		r, g, b, a
	);
	Vpu::fillRectangle(
		x, y-1,
		width, height+2,
		r, g, b, a
	);
}