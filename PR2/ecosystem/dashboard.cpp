#include "dashboard.hpp"

bool Dashboard::enabled = false;
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

}

void Dashboard::update(double delta) {
	printf("Updating Dashboard %f                    \r", delta);
}