#ifndef __SCRIPT_HPP
#define __SCRIPT_HPP
#define PY_SSIZE_T_CLEAN

#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif
#include <string>
#include <vector>

class Script {
	private:
		unsigned int execution_counter = 0;
		unsigned int program_size = 0;
		PyObject *__module__ = NULL;
		bool loaded		= false;
		bool finalized	= false;
		std::string path;
		std::vector<std::string> err;
		std::vector<std::string> out;
		std::vector<std::string> code;		
		bool run_sentence();
		void print(std::string txt);
		void error(std::string txt);
		bool load();
		
	public:
		Script(std::string path);
		~Script();
		bool isLoaded();
		bool call(std::string function_name);

		static bool execute(const char *python_code);
		static bool execute(std::string &python_code);
		static bool initialize();
		static void deinitialize();
};

#endif 
