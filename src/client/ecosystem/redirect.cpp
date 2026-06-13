#include "redirect.hpp"
#include "linux.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <iostream>

// --- NUEVO: Compatibilidad multiplataforma para Pipes y File Descriptors ---
#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
    #include <string.h>
    // Mapear las funciones de Linux/POSIX a los nombres sin guion bajo
    #define _close  close
    #define _dup    dup
    #define _dup2   dup2
    #define _fileno fileno
    #define _read   read
    #define _pipe(fds, size, mode) pipe(fds) // Linux ignora el tamaño y modo en pipe()
    #define O_TEXT  0                        // No se necesita en Linux
#endif

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
    // Control de seguridad por si read devuelve un error (-1) en Linux
    if (nOutRead < 0) nOutRead = 0; 
    buffer[nOutRead] = '\0';
    return nOutRead;
}