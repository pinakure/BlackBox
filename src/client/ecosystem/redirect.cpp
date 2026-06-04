#include "redirect.hpp"
#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

#define READ_FD 0
#define WRITE_FD 1

#define CHECK(a) if ((a)!= 0) return -1;

StdOutRedirect::~StdOutRedirect(){
	if (initialized) {
		_close(fdStdOut);
		_close(fdStdOutPipe[WRITE_FD]);
		_close(fdStdOutPipe[READ_FD]);
	}
}
StdOutRedirect::StdOutRedirect(){}
void StdOutRedirect::initialize() {
	if (_pipe(fdStdOutPipe, 512, O_TEXT)!=0)
    {
		initialized = false;
		return;
        //treat error eventually
    }
    fdStdOut = _dup(_fileno(stdout));
	initialized = true;
}

int StdOutRedirect::Start() {
    fflush( stdout );
    CHECK(_dup2(fdStdOutPipe[WRITE_FD], _fileno(stdout)));
    ios::sync_with_stdio();
    setvbuf( stdout, NULL, _IONBF, 0 ); // absolutely needed
    return 0;
}

int StdOutRedirect::Stop(){
    CHECK(_dup2(fdStdOut, _fileno(stdout)));
    ios::sync_with_stdio();
    return 0;
}

int StdOutRedirect::GetBuffer(char *buffer){
	if (!initialized)return 0;
    int nOutRead = _read(fdStdOutPipe[READ_FD], buffer, 512);
    buffer[nOutRead] = '\0';
    return nOutRead;
}
