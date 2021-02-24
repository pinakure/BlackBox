#include <physfs.h>
#include <allegro5/allegro_physfs.h>
#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <string>
#include <iostream>

#include "ecosystem.h"

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
	
	// Check TOC.py
	checkupdate();
	// Build dashboard titles described on TOC file data/toc.py (if present also)
	Script t("toc", "data");
	if (t.isLoaded())t.call("load");
	
	// Run default scripts
	Script::execute("from blackbox import *");
	Script::execute("import blackbox");
	Script::execute("import typewriter");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
	if (s.isLoaded()) s.call("main");
	return true;
}

static int waitForEnter(int return_code) {
	std::string line;
	std::printf("\n---------------------------------------\n  *** Press [ ENTER ] to continue ***  \n---------------------------------------\n\n");
	std::getline(std::cin, line);
	return return_code;
}

int main(int argc, char **argv)
{
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath("data/system.zip", 1);
	PHYSFS_addToSearchPath("data/", 1);

	if(! Script::initialize()	) return waitForEnter(100);	
	if(! initialize()			) return waitForEnter(200);
	Engine::loop();

	Engine::deinitialize();
	Script::deinitialize();

	return 0;
}