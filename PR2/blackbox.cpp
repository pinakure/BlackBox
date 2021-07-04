#include <physfs.h>
#include <allegro5/allegro_physfs.h>
#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <string>
#include <iostream>

#include "ecosystem.h"
#include "ecosystem/dashboard.hpp"

bool fileExists(std::string filename) {
	ALLEGRO_FILE* f; 
	f = al_fopen(filename.c_str(), "r");
	if (f) {
		al_fclose(f);
		return true;
	}
	return false;
}

static void checkupdate() {
	ALLEGRO_FILE* f = al_fopen("toc.py", "r");
	if (f) {
		printf("TOC File is present. No forced update is required.\n");
		al_fclose(f);
	}
	else {
		printf("TOC File Missing. Trying to download...\n");
		Engine::download("toc.py");		
	}
}

bool initialize() {
	// Call configure function, if existing in main.py (if present also)
	Script s("main");
	if(s.isLoaded()) s.call("configure");
	
	// Initialize Engine
	if (!Engine::initialize()) {
		Engine::deinitialize();
		std::printf("ERROR: Engine cannot run.\n\tCritical Modules could not be loaded.\n\tIt may be some files missing.\n");
		return false;
	}
	return true;
}

static void postinit_dependencies() {
	Script::execute("from blackbox import *");
	Script::execute("import blackbox");
	Script::execute("import typewriter");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
}

static void postinit_dashboard() {
	// Update TOC.py
	if (!fileExists("toc.py"))
		checkupdate();

	// Build dashboard titles described on TOC file data/toc.py (if present also)
	Dashboard::enabled = false;
	if(fileExists("toc.py")) {
		Script t("toc", "data");
		if (t.isLoaded()) 
			t.call("load");
		else 
			printf("Error parsing TOC File; Dashboard will be disabled.\n");
	} else 
		printf("Cannot load TOC File. Dashboard will be disabled.\n");
	postinit_dependencies();
}

extern void _extractScripts(), _deleteScripts();

static void postinit_package(const char* package) {
	std::string filename = "data/" + std::string(package);
	filename += ".zip";
	PHYSFS_addToSearchPath(filename.c_str(), 1);
	// Run game.py ( NOTE: It MUST be located at 'data/scripts/game.py' )
	_extractScripts();
	Script s = Script("game", "data.scripts");
	if (s.isLoaded()) {
		s.execute("from data.scripts.game import Game");
		InputDevice::control_c = false;
		s.execute("Game.setup()");
		s.execute("Game.loop()");
		InputDevice::control_c = false;
		s.execute("Game.destroy()");
		s.execute("Game = 1");		
		Engine::destroyEntities();
		Engine::music.randomMenuMusic();
	} else {
		printf("ERROR: Cannot call game.main\n");
	}
	PHYSFS_removeFromSearchPath(("data/" + std::string(package)).c_str());
	_deleteScripts();
}

static void postinit_module(const char *script) {
	// Run default scripts
	Script s("main");
	postinit_dependencies();
	printf("Calling Script %s()...\n", script);
	if (s.isLoaded()) s.call(script);	
	else printf("Cannot exec Script %s()\n", script);
}

static int waitForEnter(int return_code) {
	std::string line;
	std::printf("\n---------------------------------------\n  *** Press [ ENTER ] to continue ***  \n---------------------------------------\n\n");
	std::getline(std::cin, line);
	return return_code;
}

int main(int argc, char **argv){
	/*
	Initialize Virtual FileSystem Environment
	*/
	PHYSFS_init(argv[0]);
	ALLEGRO_FILE *f = al_fopen("data/system.zip", "r");
	if (!f) {
		_mkdir("data");
		Engine::download("system.zip");
		PHYSFS_addToSearchPath("data/system.zip", 1);		
	} else {
		al_fclose(f);
		PHYSFS_addToSearchPath("data/system.zip", 1);		
	}	
	PHYSFS_addToSearchPath("data/", 1);
	/*
	Initialize Engine
	*/
	if(! Script::initialize()	) return waitForEnter(100);	
	if(! initialize()			) return waitForEnter(200);
	
	/* 
	Parse arguments 
	*/
	const char *mode;
	const char *script;
	switch (argc) {
		case 2:
			script = argv[1];
			postinit_module(script);
			break;
		case 3:
			mode = argv[1];
			if (!strcmp(argv[1], "-p")) {
				const char *package = argv[2];
				postinit_package(package);
				break;
			}
		default:
			postinit_dashboard();
			Engine::loop();
			break;
	}
	/*
	Engine deinitialization
	*/
	Engine::deinitialize();
	Script::deinitialize();

	return 0;
}