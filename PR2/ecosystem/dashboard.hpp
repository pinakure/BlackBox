#ifndef _DASHBOARD_HPP
#define	_DASHBOARD_HPP

#include <string>
#include <vector>
#include "vpu.hpp"

class DashboardTitle {
public:
	std::string name		= "noname";
	std::string description = "nodescription";
	std::string developer   = "nodev";
	std::string url			= "nourl";
	std::string picturename	= "nopicture";
	std::string genre		= "nogenre";
	Font *font 				= NULL;
	bool		multiplayer = false;
	bool		cooperative = false;
	bool		joystick	= false;
	bool		mouse		= false;
	bool		keyboard	= false;
	int			rating		= 5;
	bool		downloaded	= false;
	Surface		picture;
	std::vector<Surface>	snapshots;
	std::string released	= "01-01-2022";
	DashboardTitle* parent  = NULL;
	
	DashboardTitle() {};
	void draw(int x, int y, int width, int height, bool active = false);
	
	void download();
	void load();

	DashboardTitle(
		std::string name,
		std::string description,
		std::string developer,
		std::string url,
		std::string picturename, 
		std::string genre,
		std::string font,
		bool multiplayer,
		bool cooperative,
		bool joystick,
		bool mouse,
		bool keyboard,
		int rating, 
		std::string released,
		char* parent
	);
};

class Dashboard {
public:
	static bool enabled;
	static int active_index;
	static int cursor_x;
	static int cursor_y;
	static int columns;
	static int rows;
	static float offset;
	static size_t max_offset;
	static std::vector<DashboardTitle> titles;
	static DashboardTitle* selected;
	static void addTitle(
		std::string name,
		std::string description,
		std::string developer,
		std::string url,
		std::string picture,
		std::string genre,
		std::string font,
		bool multiplayer,
		bool cooperative,
		bool joystick,
		bool mouse,
		bool keyboard,		
		int rating,
		std::string released,
		char* parent
	);
	static void draw();
	static void update(double delta);
};

#endif 