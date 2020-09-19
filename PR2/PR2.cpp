#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>

#include "ecosystem.h"



void run() {
	Script::execute("import blackbox");
	Script::execute("print(f'Welcome to BlackBox v.{blackbox.version()}')");
	Script s("main");
	s.call("main");	
	
}

int main(int argc, char **argv)
{
	if(! Script::initialize() ) {
		printf("ERROR: Cannot initialize scripting machine.");
		return(100);
	}
	
	run();	

	if (!Engine::initialize()) {
		printf("ERROR: Cannot initialize allegro backend.");
		return(200);
	}
	
	run();

	Engine::loop();

	Engine::deinitialize();
	Script::deinitialize();
	
	return 0;
}