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
	
	// Update TOC.py
	if (!fileExists("toc.py")) {
		checkupdate();
	}
	// Build dashboard titles described on TOC file data/toc.py (if present also)
	Dashboard::enabled = false;
	if(fileExists("toc.py")) {
		Script t("toc", "data");
		if (t.isLoaded()) t.call("load");
		else printf("Error parsing TOC File; Dashboard will be disabled.\n");
	} else printf("Cannot load TOC File. Dashboard will be disabled.\n");
	
	// Run default scripts
	Script::execute("from blackbox import *");
	Script::execute("import blackbox");
	Script::execute("import typewriter");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
	if (s.isLoaded()) if(!Dashboard::enabled) s.call("main");
	return true;
}

static int waitForEnter(int return_code) {
	std::string line;
	std::printf("\n---------------------------------------\n  *** Press [ ENTER ] to continue ***  \n---------------------------------------\n\n");
	std::getline(std::cin, line);
	return return_code;
}

int main(int argc, char **argv){
	
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

	if(! Script::initialize()	) return waitForEnter(100);	
	if(! initialize()			) return waitForEnter(200);
	Engine::loop();

	Engine::deinitialize();
	Script::deinitialize();

	return 0;
}