#ifndef MODULES_CON
#define MODULES_CON

class InputDevice;

enum E_MessageTypes {
	MESSAGE_NONE = -1,
	MESSAGE_CHAR = 1024,
	MESSAGE_MAX,
	MESSAGE_UPDATE = 0xFFFFFFF0,
};

#include "cvar.hpp"
#include "color.hpp"
#include "vpu.hpp"
#include <vector>
#include <string>
#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <map> 
#include "redirect.hpp"

class Message {
public:
	int message;
	int data;
	std::string  cdata;

	Message(void) {
		set(MESSAGE_NONE, MESSAGE_NONE);
	}
	
	Message(int message, int data) {
		set(message, data);
	}
	
	Message(int message, std::string data) {
		set(message, data);
	}

	void set(int m, int d) {
		message = m;
		data = d;		
	}
		
	void set(int m, std::string d) {
		message = m;
		cdata = d;
		data = MESSAGE_CHAR;
	}
};


#include "script.hpp"
#include "command.hpp"

#define CONSOLE_BLINK_SPEED		16
#define CONSOLE_ONELINER_TIME	250
#define CONSOLE_COLOR

// Kind of messages accepted by Console
enum E_ConsoleMessages {
	CONSOLE_CHAR = 1,
	CONSOLE_TOGGLE = 2,
	CONSOLE_EXEC = 3,
	CONSOLE_MAX
};

typedef struct t_Toggle {
	std::string name;
	int id;
	bool status;
} Toggle;

class Console{
	public:		
		static PyMethodDef methods[];
		////////////////////////////////////////////
		// CVARS										//
		////////////////////////////////////////////
		static StdOutRedirect			_stdout;    
		static char						char_buffer[16536];
		
		static Text						*result;
		static Color					*con_fgcolor;
		static Color					*con_bgcolor;
		static Boolean					*debug_script;
		static float					opacity;

		////////////////////////////////////////////
		// MEMBERS										//
		////////////////////////////////////////////
		static bool						initialized;
		static int						fgcolor;
		static int						bgcolor;
			
		static Surface					backdrop;
		static Surface					bitmap;				

		static bool						remember_syntax_errors;
		static bool						redirect; 			//!< true when console cannot use graphical routines and must send output to stdout (WIP)
		//static bool						scriptInput;		//!< true when script is going to receive the command line
		//static Script					script;
		static bool						enabled;			//!< true if console is visible and manipulable, false if hidden, and oneliner will show in top line of the screen
		static bool						redraw;				//!< true if the console buffer must be redrawn (so render would be called)
		static bool						echo;				//!< true if inputted console commands are shown ingame
		static bool						ctrlc;				//!< true if was pressed
		
		static std::vector<std::string> lines;				//!< List of every line in the console buffer @note Currently, just the visible lines are stored, when console height changes, the number of lines is readjusted while rendering them
		static std::vector<std::string> consoleHistory;
		
		// turn into deque!
		static std::vector<Message>		messages;			//!< List for incoming messages for this entity
		
		static std::vector<std::string>	files;
		
		static void						updateSpeed(void);// Reload value from CVAR con_speed
		static int						speed;
		static int						visibleHeight; //!< Visible size of the console, to draw it full sized, it must match CONSOLE_HEIGHT macro.

		static size_t					consoleHistoryLimit;
		static size_t					consoleHistoryCursor;
		static size_t					commandHistoryLimit;
		static size_t					commandHistoryCursor;
		static size_t					commandPosition;	//!< Cursor input position

		static std::vector<std::string>				commandHistory;
		static std::map<std::string, std::string>	aliases;
		static std::vector<Toggle*>					toggles;
		static std::vector<Command>					commands;
		static std::vector<SpecialCommand>			special_commands;//!< Commands which require keypress status will be initialized with this function


		static std::string				command;			//!< Command being built on the console (input)
		static std::string				cwd;				//!< Current working path
		static std::string				initial_cwd;		//!< Initial working path
		static int						height;				//!< Maximum number of lines for the console buffer		
		static bool						wait_for_window;
		static int						wait;				//!< Number of cycles to wait before executing next sentence
		
		static int						palette[16];		

		////////////////////////////////////////////
		// METHODS										//
		////////////////////////////////////////////
		Console(void);
		static void						init(void);//merge with initialize
		static void						initialize(void);
		
		static void						sortCommands(void);
		static void						loadCommands(void);//delete, this should be done by each class which interacts via console!
		static void						loadVars(void);//delete, this should be done by each class which interacts via console!
		
		static void						updateColors(void); //Reload values from CVAR (Optimization)
		static void						update(void);
		static void						render(int h, bool cursor);
		static void						deInitialize(void);
		~Console(void);
	
		static void						printColoredNameValuePair(const char *name, const char *value);
		static void						printColoredNameValuePairFloat(const char *name, float value);
		static void						printColoredNameValuePairInt(const char *name, int value);
		static void						printColoredNameValuePairDir(const char *name, int value);
		static void						printColoredNameValuePairBool(const char *name, bool value);
		static void						printf(const char *fmt, ...);
		static void						print(const char *text);
		static void						print(std::string s);
		static void						debug(std::string section, std::string s);
	
		static bool						evaluate(const char *expr);
		static void						draw(int h);
		static void						readKeyboard(int k);
		static void						handleMessages(void);

		static void						autoCompletion(void);
		static std::vector<std::string>	autoCompletionGetCandidates(std::string command, bool isCmd);
		static bool						autoCompletionCheck(std::string candidate, std::string autocomplete);
		static std::string				autoCompletionMinMatch(std::vector<std::string> &candidates);//caution here <- there was a pointer instead of referecnce

		static void						clear(void);
		static void						clearPrompt(void);
	
		static bool						parse(std::string &comond);
	
		static void						execute(std::string commandline, bool nohist=false);
	
		static Command*					findCommand(std::string name);

		static void						dumpList(std::vector<std::string> &s);
		static void						dumpHeader(const char *header);
		static void						dumpBindList(void);
		static void						dumpPropertyList(std::map<std::string, std::string> &pl);
		static void						dumpToggleList(std::vector<Toggle*> &tl);

		static void						reportAlias(const char *var);
			
		static void						setAlias(const char* name, const char* value);
		static std::string				getAlias(const char* name);
		static void						unSetAlias(const char* name);

		static void						reportBind(const char *var);
		static void						toggleVariable(const char* name);

		static const std::string		getBind(const char *name);
		static void						setBind(const char *key_name, const char *cmd_line, int key_code);
		static void						unSetBind(const char *name);

		static std::string				stripColorCodes(const std::string &line);
		static std::string				translateVariables(std::string cmd);
	
		static void						addCommand(const char *cmdname, callback function, const char *help, const char *usage);
		static void						addSpecial(const char *cmdname, void(*function)(int), const char *help);


		static void						paintBackdrop(float variance=0.05f);

		// PUBLIC API
		static void 					scroll(int i);
		static void 					historyBegin(void);
		static void 					historyEnd(void);
		static void 					historyAdd(std::string line);
		static void 					historyNext(void);
		static void 					historyPrev(void);
		static void 					addSpace(void);
		static void 					removeChar(void);
		static void 					deleteChar(void);
		static void 					addChar(int chr);
		static void						addHelp(std::string name, std::string hlp);
		static std::map<std::string, std::string> pyhelp;
		static void 					moveLeft(void);
		static void 					moveRight(void);
		static void 					moveBegin(void);
		static void 					moveEnd(void);
		static void 					sendPrompt(void);
		
		static std::string				stripColors(const char *colored);

		static PyObject* pyCls(PyObject* self, PyObject* args);
		static PyObject* pyToggle(PyObject* self, PyObject* args);
		static PyObject* pyPrint(PyObject* self, PyObject* args);
		static PyObject* pyExec(PyObject* self, PyObject* args);
};

#define COMMAND_CALLBACK(a)			int a(std::vector<std::string> &args)
#define ACTION_CALLBACK(a)			void a(int i)



// Function Exports
void ToggleConsole(int);

// External Helper Function Exports
std::vector<std::string> split(std::string &text, char sep);
std::vector<std::string> tokenize(std::string set, char split);
std::vector<std::string> findFiles(const char *path, const char *extension);
void tabulate(char *dest, size_t dest_size, int tabsize, const char *fmt, ...);
std::string replace(std::string haystack, std::string needle, std::string replace_for_needle);

#define WM_CONSOLE_PRINT				0xFFFF00FF

#endif
