#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>

#include "ecosystem.h"



bool initialize() {
	Script s("main");
	if(s.isLoaded()) s.call("configure");	
	
	if (!Engine::initialize()) {
		printf("ERROR: Cannot initialize allegro backend.");
		return false;
	}
	Script::execute("import blackbox");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
	
	if (s.isLoaded()) s.call("main");
	return true;
}

int main(int argc, char **argv)
{
	if(! Script::initialize() ) {
		printf("ERROR: Cannot initialize scripting machine.");
		return(100);
	}
	
	if (!initialize()) return 200;

	
	Engine::loop();

	Engine::deinitialize();
	Script::deinitialize();
	
	return 0;
}