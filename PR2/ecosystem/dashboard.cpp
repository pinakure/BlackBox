#include "dashboard.hpp"
#include "vpu.hpp"
#include "input.hpp"
#include "typewriter.hpp"
#include "console.hpp"
extern char key[256];

int  Dashboard::active_index = 0;
bool Dashboard::enabled = false;
bool Dashboard::downloading= false;
int Dashboard::cursor_x = 0;
int Dashboard::cursor_y = 0;
int Dashboard::columns = 4;
int Dashboard::rows = 3;
float Dashboard::offset = -16.0f;
size_t Dashboard::max_offset = 0;
DashboardTitle* Dashboard::selected = NULL;
std::vector<DashboardTitle> Dashboard::titles;

using namespace std;
void Dashboard::addTitle(string name,string description, string developer, string url,string picture,string genre,string font,
						bool multiplayer,bool cooperative,bool joystick,bool mouse,bool keyboard,
						int rating, string released, char* parent) {
	if (titles.size() == 0) {
		printf("Titles found at data/toc.py, dashboard enabled.\n");
		Dashboard::enabled = true;
	}
	Dashboard::titles.push_back(
		DashboardTitle(
			name,
			description,
			developer,
			url,
			picture,
			genre,
			font,
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
	printf(" * Added Title '%s' to the dashboard.\n", name.c_str());
}

void Dashboard::draw(){
	if (!enabled || downloading )return;
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
	Vpu::select(Vpu::overlay);
	Vpu::clear();
	
	// Draw title boxes area
	/*Vpu::fillRectangle(
		title_area_left	 , title_area_top,
		title_area_width , title_area_height,
		TypeWriter::r >> 3,
		TypeWriter::g >> 3,
		TypeWriter::b >> 3,
		TypeWriter::a >> 3
	);*/
	// Draw titles area
	int title_index = 0;
	selected = NULL;
	if (titles.size()) {
		for (int y = 0; y < title_rows; y++) {
			for (int x = 0; x < title_columns; x++, title_index++) {
				if (title_index >= Dashboard::titles.size())break;
				int dy = title_area_top + (y * title_height) + (title_area_padding * y);
				DashboardTitle* title = &Dashboard::titles[title_index];
				int dx = title_area_left + (x * title_width) + (title_area_padding * x);
				title->draw(dx, dy, title_width, title_height, title_index == active_index);
				if (title_index == active_index)selected = title;
			}
			if (title_index >= Dashboard::titles.size())break;
		}
	}
	if (selected) {
		// Draw title art graphic
		Vpu::fillRectangle(
			left, top,
			width, bar_height,
			TypeWriter::r >> 4,
			TypeWriter::g >> 4,
			TypeWriter::b >> 4,
			TypeWriter::a >> 4
		);
		Vpu::pushFont();
		Vpu::pushColor();
		Vpu::setFont(Vpu::biggest_font);
		Vpu::setColor(al_map_rgb(255, 255, 255));
		Vpu::print(selected->name, left+(width/2), top, ALLEGRO_ALIGN_CENTER);
		Vpu::popColor();
		Vpu::popFont();

		// Draw information scrolling marquee
		Vpu::fillRectangle(
			left, footer,
			width, bar_height,
			0, 0, 0, 255
		);
		Vpu::pushColor();
		Vpu::setColor(al_map_rgb(255, 255, 255));
		max_offset = al_get_text_width(Vpu::font->data, selected->description.c_str());
		offset += 0.25f;
		if (int(offset) >= int(max_offset)) {
			offset = -width;
		}
		Vpu::print(selected->description, (left)-offset, footer , 0);
		Vpu::popColor();
	}
}

#include "input.hpp"

void Dashboard::update(double delta) {
	int temp_x = cursor_x;
	int temp_y = cursor_y;
	int temp_i = active_index;
	float temp_o = offset;
	static bool return_lock = false;
	// Dont update input if any of the interfaces is enabled
	if (Console::enabled
		|| !enabled
		|| downloading
		|| TypeWriter::enabled
		|| (TypeWriter::choices.size() > 0)
		|| (TypeWriter::options.size() > 0)
		|| (GetTextBox::status != GetTextBox::STATUS_DISABLED)
		) {
		return_lock = true;
		return;
	}
	if (return_lock) {
		InputDevice::update(1);
		return_lock = false;
	}

	if (KEYDOWN(key[ALLEGRO_KEY_ESCAPE])) {
		Script::execute("menu()");
	}
	if (KEYDOWN(key[ALLEGRO_KEY_LEFT])) {
		offset = -16.0f;
		cursor_x--;
		if (cursor_x < 0)cursor_x = columns - 1;		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_RIGHT])) {
		offset = -16.0f;
		cursor_x++;
		if (cursor_x >= columns)cursor_x = 0;		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_UP])) {
		offset = -16.0f;
		cursor_y--;
		if (cursor_y < 0)cursor_y = rows - 1;		
	}
	else if (KEYDOWN(key[ALLEGRO_KEY_DOWN])) {
		offset = -16.0f;
		cursor_y++;
		if (cursor_y >= rows)cursor_y = 0;
	}
	// Calculate new active_index
	active_index = (cursor_y * columns) + cursor_x;
	// If the movement is forbidden, restore variables to previous values
	if (active_index >= titles.size()) {
		cursor_x = temp_x;
		cursor_y = temp_y;
		active_index = temp_i;
		offset = temp_o;
	}
	// Update selected 
	if(titles.size()>0)
		selected = &titles[active_index];
	if (KEYDOWN(key[ALLEGRO_KEY_ENTER])) {
		if (selected) selected->load();
	}
	printf("Updating Dashboard %f                    \r", delta);
}


/*---------------------------------------------------------------------------------------------------------*/
extern bool fileExists(std::string filename);
#include "engine.hpp"

DashboardTitle::DashboardTitle(string name, string description, string developer, string url, string picture_url, string genre, string font,
	bool  multiplayer, bool	cooperative, bool joystick, bool mouse, bool	keyboard,
	int	rating, string released, char* parent) {
	this->name = name;
	this->description = description;
	this->developer = developer;
	this->url = url;
	this->picturename = picture_url;
	this->genre = genre;
	this->multiplayer = multiplayer;
	this->cooperative = cooperative;
	this->joystick = joystick;
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->rating = rating;
	this->parent = NULL;//TODO: create hierarchy

	// Load title picture
	this->picture = Vpu::loadBitmap("titles/" + picturename + ".png");
	if (!this->picture.enabled)
		this->picture = Vpu::loadBitmap("gfx/vendor.png");

	// Load title snapshots
	// todo

	// Load (initialize) font by name
	this->font = Vpu::getFontByName(font);
	if (!this->font)this->font = Vpu::biggest_font;

	// Check if game files are present and set up downloaded attribute
	downloaded = false; 
	if (fileExists(this->url)) downloaded = true;	
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
	if(picture.enabled)
		al_draw_tinted_scaled_bitmap(
			picture.bitmap, 
			al_map_rgba(
				downloaded ? 128 : 32,
				downloaded ? 128 : 32,
				downloaded ? 128 : 128,
				downloaded ? 240 : 200
			),
			0, 0, 
			picture.width, picture.height,
			x + 1, y + 1,
			width - 2, height - 2,
			0
		);
		
}

void DashboardTitle::download() {
	Dashboard::downloading = true;
	Engine::download((url).c_str());
	printf("\n");
	if (fileExists(url)) {
		downloaded = true;
	}
	Dashboard::downloading = false;
}

#include "utils.hpp"


static void _extractScript(std::string name) {
	_mkdir("data\\scripts");
	ALLEGRO_FILE* f = al_fopen(("scripts/" + name).c_str(), "r");
	if (!f) {
		printf("Cannot execute. main function or game.py not present.\n");
	}
	else {
		printf(("Reading " + name + " from datafile...\n").c_str());
		al_fseek(f, SEEK_SET, SEEK_END);
		size_t size = al_ftell(f);
		al_fseek(f, SEEK_SET, 0);
		char* contents;
		contents = (char*)malloc(sizeof(char) * size);
		if (contents) {
			memset(contents, sizeof(char) * size, 0);
			al_fread(f, contents, size);
		}
		al_fclose(f);
		if (contents) {
			printf(("Writing data/scripts/" + name+"...\n").c_str());
			FILE* output = 0;
			fopen_s(&output, ("data/scripts/" + name).c_str(), "w");
			if (output) {
				fwrite(contents, size, 1, output);
				fclose(output);
			}
		}
	}
}

static void _deleteFolder(std::string path, std::string folder) {
	std::string dir = path + folder + "/";
	printf(("Deleting folder " + dir + " contents...\n").c_str());
	ALLEGRO_FS_ENTRY* e = al_create_fs_entry(dir.c_str());
	if (al_open_directory(e)) {
		ALLEGRO_FS_ENTRY* file;
		while (file = al_read_directory(e)) {
			std::string name = al_get_fs_entry_name(file);
			std::vector<std::string> temp = explode(name, '\\');
			if (temp.size() > 0)
				name = temp[temp.size() - 1];
			// Remove slashes
			std::vector<std::string> parts = explode(name, '/');
			if (parts.size() > 0) {
				name = parts[parts.size() - 1];
			}
			// Extract extension
			parts = explode(name, '.');
			al_destroy_fs_entry(file);
			std::string filename = "data/" + dir +  name;
			if (parts.size() == 1) {
				_deleteFolder(dir, name);
			}
			else {
				printf(("Deleting "+filename+"\n").c_str());
				_unlink(filename.c_str());
			}
		}
	}
	printf(("Deleting folder data/" + dir + "\n").c_str());
	_rmdir(("data/"+dir).c_str());
}

static void _deleteScripts() {
	_deleteFolder("", "scripts");
}

static void _extractScripts() {
	ALLEGRO_FS_ENTRY* e = al_create_fs_entry("scripts/");
	if (al_open_directory(e)) {
		ALLEGRO_FS_ENTRY* file;
		while (file = al_read_directory(e)) {
			std::string name = al_get_fs_entry_name(file);
			std::vector<std::string> temp = explode(name, '\\');
			if (temp.size() > 0)
				name = temp[temp.size() - 1];
			// Remove slashes
			std::vector<std::string> parts = explode(name, '/');
			if (parts.size() > 0) {
				name = parts[parts.size() - 1];
			}
			// Extract extension
			parts = explode(name, '.');
			al_destroy_fs_entry(file);
			if (parts.size() == 2) {
				if (!parts[1].compare("py")) {
					_extractScript(name);
				}
			}			
		}
	}
}

#include <physfs.h>
void DashboardTitle::execute() {
	PHYSFS_addToSearchPath(("data/" + url).c_str(), 1);
	// Run game.py ( NOTE: It MUST be located at 'data/scripts/game.py' )
	_extractScripts();
	Script s = Script("game", "data.scripts");
	if (s.isLoaded()) {
		Dashboard::enabled = false;
		s.execute("from data.scripts.game import Game");
		InputDevice::control_c = false;
		s.execute("Game.setup()");
		s.execute("Game.loop()");
		InputDevice::control_c = false;
		s.execute("Game.destroy()");
		Dashboard::enabled = true;
	} else {
		printf("ERROR: Cannot call game.main\n");
	}
	PHYSFS_removeFromSearchPath(("data/" + url).c_str());
	_deleteScripts();
}

void DashboardTitle::load() {
	if (!downloaded) {
		download();
		if (downloaded) {
			execute();
			return;
		} else {
			printf(("Cannot execute " + name + " :Title is not downloaded. ").c_str());
			return;
		}
	}
	execute();
}

