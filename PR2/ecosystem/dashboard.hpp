#ifndef _DASHBOARD_HPP
#define	_DASHBOARD_HPP

#include <string>
#include <vector>

class DashboardTitle {
public:
	std::string name		= "noname";
	std::string url			= "nourl";
	std::string picture		= "nopicture";
	std::string genre		= "nogenre";
	bool		multiplayer = false;
	bool		cooperative = false;
	bool		joystick	= false;
	bool		mouse		= false;
	bool		keyboard	= false;
	int			rating		= 5;
	std::string released	= "01-01-2022";
	DashboardTitle* parent  = NULL;
	
	DashboardTitle() {};
	void draw(int x, int y, int width, int height, bool active = false);

	DashboardTitle(std::string name, std::string url, std::string picture, std::string genre,
				    bool multiplayer,bool cooperative,bool joystick,bool mouse,bool keyboard,
					int rating, std::string released,char* parent);
};

class Dashboard {
public:
	static bool enabled;
	static int active_index;
	static int cursor_x;
	static int cursor_y;
	static int columns;
	static int rows;
	static std::vector<DashboardTitle> titles;
	static void addTitle(
		std::string name,
		std::string url,
		std::string picture,
		std::string genre,
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