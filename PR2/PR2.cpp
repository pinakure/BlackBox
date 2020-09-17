#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <cmath>

#include "ecosystem.h"

void run() {
	Script::execute("import emb");
	Script::execute("version = '0.3'");
	Script::execute("print(f'Welcome to BlackBox v.{version}')");
	Script::execute("print(emb.numargs())");
	Script s("main");
	s.call("main");	
	
	Engine::loop();
}

int main(int argc, char **argv)
{
	if(! Script::initialize() ) {
		printf("ERROR: Cannot initialize scripting machine.");
		return(100);
	}
	
	if (!Engine::initialize()) {
		printf("ERROR: Cannot initialize allegro backend.");
		return(200);
	}

	run();	

	Engine::deinitialize();
	Script::deinitialize();
	
	return 0;
}