#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <string>
#include <iostream>

#include "ecosystem.h"



bool initialize() {
	Script s("main");
	if(s.isLoaded()) s.call("configure");	
	
	if (!Engine::initialize()) {
		Engine::deinitialize();
		std::printf("ERROR: Engine cannot run.\n\tCritical Modules could not be loaded.\n\tIt may be some files missing.\n");
		return false;
	}
	Script::execute("from blackbox import *");
	Script::execute("import blackbox");
	Script::execute("import typewriter");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
	Engine::download("1GB.zip");
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
	if(! Script::initialize()	) return waitForEnter(100);	
	if(! initialize()			) return waitForEnter(200);
		
	Engine::loop();

	Engine::deinitialize();
	Script::deinitialize();

	return 0;
}