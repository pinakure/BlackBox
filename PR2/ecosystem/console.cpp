#include "cvar.hpp"
#include "integer.hpp"
#include "vpu.hpp"
#include "color.hpp"
#include "engine.hpp"
#include "console.hpp"
#include "input.hpp"
#include <sstream>
#include <algorithm>

std::map<std::string, std::string> Console::pyhelp;

#define BUFFERSIZE 1024

StdOutRedirect				Console::_stdout;
char						Console::char_buffer[16536];
bool						Console::remember_syntax_errors = true;
std::map<std::string, std::string>	Console::aliases;
std::vector<Toggle*>		Console::toggles;
std::vector<std::string>	Console::lines;				
std::vector<Message>		Console::messages;
int							Console::height = 29;		//!< Recalculated in runtime // (GPU::height / 8) - 1;
int							Console::visibleHeight = 960;//0;	//!< Visible size of the console, to draw it full sized, it must match CONSOLE_HEIGHT macro.
int							Console::speed = 24;

Text*						Console::result = 0;
Color*						Console::con_fgcolor = 0;
Color*						Console::con_bgcolor = 0;

Boolean*					Console::debug_script = 0;
//Script					Console::script;
//bool						Console::scriptInput = false;//!< true if script is receiving command line result

int							Console::fgcolor = 0xC000FF00;
int							Console::bgcolor = 0xC0104010;

Surface						Console::backdrop;
Surface						Console::bitmap;			

float						Console::opacity=1.0f;

bool						Console::redirect = false;	//!< true when console cannot use graphical routines and must send output to stdout (WIP)
bool						Console::enabled = false;	//!< true if console is visible and manipulable, false if hidden, and oneliner will show in top line of the screen
bool						Console::redraw = true;		//!< true if the console buffer must be redrawn (so render would be called)
bool						Console::echo = true;		//!< true if inputted console commands are shown ingame
bool						Console::ctrlc = false; 	//!< true if was pressed
bool						Console::initialized = false;

std::string					Console::command = "";		//!< Command being built on the console (input)
std::vector<std::string>	Console::consoleHistory;
std::vector<std::string>	Console::commandHistory;
size_t						Console::commandPosition = 0;	
size_t						Console::consoleHistoryLimit  = 1024;
size_t						Console::consoleHistoryCursor = 0;
size_t						Console::commandHistoryLimit  = 1000;
size_t						Console::commandHistoryCursor = 0;
std::vector<Command>		Console::commands;
std::vector<SpecialCommand> Console::special_commands;	//!< Commands which require keypress status will be initialized with this function

std::vector<std::string>	Console::files;
std::string					Console::cwd = "./";				//!< Current working path
std::string					Console::initial_cwd = "";		//!< Current working path

bool						Console::wait_for_window = false;
int							Console::wait = 0;				//!< Number of cycles to wait before executing next sentence

int							Console::palette[16];

#include <direct.h> /* for _getcwd() and _chdir() */
#define getcwd _getcwd
#define getcwd _getcwd


void Console::initialize(void){
	if (initialized) return;
	
	char cCurrentPath[FILENAME_MAX];
	getcwd(cCurrentPath, sizeof(cCurrentPath));
	cwd = cCurrentPath;

	if (!initial_cwd.compare(""))initial_cwd = cwd;

	setAlias("quit", "exit");
	setAlias("ls", "dir");

	std::replace(cwd.begin(), cwd.end(), '\\', '/');
	clear();

	loadCommands();

	// Set pointers to CVars
	Console::result = Text::cast(CVar::settings["result"]);
	Console::con_fgcolor = Color::cast(CVar::settings["con_fgcolor"]);
	Console::con_bgcolor = Color::cast(CVar::settings["con_bgcolor"]);
	Console::debug_script = Boolean::cast(CVar::settings["debug_script"]);

	Console::palette[ 0] = 0xff000000;
	Console::palette[ 1] = 0xff000080;
	Console::palette[ 2] = 0xff008000;
	Console::palette[ 3] = 0xff008080;
	Console::palette[ 4] = 0xff800000;
	Console::palette[ 5] = 0xff800080;
	Console::palette[ 6] = 0xff808000;
	Console::palette[ 7] = 0xff808080;
	Console::palette[ 8] = 0xff404040;
	Console::palette[ 9] = 0xff0000f0;
	Console::palette[10] = 0xff00f000;
	Console::palette[11] = 0xff00f0f0;
	Console::palette[12] = 0xfff00000;
	Console::palette[13] = 0xfff000f0;
	Console::palette[14] = 0xfff0f000;
	Console::palette[15] = 0xfff0f0f0;

	initialized = true;
	redirect = false;

	Console::execute("import blackbox"	, true);
	Console::execute("import vpu"		, true);
	Console::execute("import console"	, true);

	//_stdout.initialize();
	//_stdout.Start();
}

void Console::deInitialize(void){
	// Restore stdout
	//_stdout.Stop();
}

const std::string Console::getBind(const char *name){
	return name;
	return (InputDevice::getBind(name));
}

void Console::setBind(const char *key_name, const char *cmd_line, int key_code){
	InputDevice::setBind(key_name, cmd_line, key_code);
}

void Console::unSetBind(const char *name){
	InputDevice::unSetBind(name);
}

std::vector<std::string> extractArguments(std::string haystack){
	std::vector<std::string> ls;
	
	std::stringstream ss(haystack);
	std::string s;
	
	bool quoted = false;
	while(!ss.eof()){
		getline(ss, s, '"');
		if(s.compare(" ")&&s.compare("")){
			if(!quoted){
				//unquoted argument, separate by spaces
				std::stringstream sss(s);
				std::string p;
				while(!sss.eof()){
					getline(sss, p, ' ');
					if(p.compare(" ")&&p.compare("")){
						ls.push_back(p);
					}
				}
			} else {
				//quoted argument, leave it alone
				ls.push_back(s);
			}
		}
		quoted ^= 1;
	}
	
	return ls;
}

Console::Console(void){
	initialized = false;
}

void Console::addCommand(const char *cmdname, callback function, const char *help, const char *usage){
	commands.push_back(Command(cmdname, function, help, usage));
}

void Console::addSpecial(const char *cmdname, void(*function)(int), const char *help){
	special_commands.push_back(SpecialCommand(cmdname, function, help));
}

void Console::reportAlias(const char *var){
	std::string s = "echo ";
	s += var;
	s += " = '";
	s += getAlias(var);
	s += "'";
	execute(s.c_str());
}

void Console::toggleVariable(const char* name){
	CVar *cv = CVar::getVar(std::string(name));
	if (cv){
		((Boolean*)cv)->set(((Boolean*)cv)->get() ^ 1);
		Console::printf("~b%s ~f= ~7'~e%s~7'"
			  , name
			  , ((Boolean*)cv)->get() ? "enabled" : "disabled");
	}
}

void Console::reportBind(const char *var){
	Console::printf("~b%s ~f= ~7'~e%s~7'", var, getBind(var).c_str());
}

Command *Console::findCommand(std::string name){
	if(commands.size()== 0) {
		print("WARNING: No commands on console!");
		return NULL;
	}
		
	// Find command
	Command *c;
	size_t i=0;
	while(i<commands.size()){
		c = &commands[i];
		if(c->isCommand(name)) {
			return c;
		}
		i++;
	}
	return NULL;
}

bool Console::autoCompletionCheck(std::string candidate, std::string autocomplete){
	size_t candlen = candidate.length();
	size_t autolen = autocomplete.length();
	char *c = (char*)candidate.c_str();
	char *a = (char*)autocomplete.c_str();
	
	// Skip candidates shorter or equal than autocomplete target
	if(candlen <= autolen) return false;
	
	for(int o=0; o < autolen; o++){
		if(c[o] != a[o]){
		    return false;
		}
	}
	return true;
}

std::string Console::autoCompletionMinMatch(std::vector <std::string> &candidates){
	size_t minsize = 0;
	size_t maxsize = 0;
	size_t len;
	
	// Find the longest candidate and note its length
	for (size_t i = 0, o = candidates.size(); i < o; i++) {
		len = candidates[i].length();
		if( len > maxsize) maxsize = len;
	}
	// Now we know the maximum value minsize can take, we can look
	// for lower ones until finding the length of shortiest std::string.
	minsize = maxsize;
	for (size_t i = 0, o = candidates.size(); i < o; i++) {
		len = candidates[i].length();
		if( len < minsize) minsize = len;
	}
	
	// Reduce minsize to minimal common content length
	std::string s = candidates[0];
	for(size_t m = 0; m<minsize; m++){
		for (size_t o = 0, op = candidates.size(); o<op; o++) {
			std::string t = candidates[o];
			if(s.at(m) != t.at(m)) {
				minsize = m;
				break;
			}
		}
	}
	
	return candidates[0].substr(0, minsize);
}

void Console::autoCompletion(void){
	std::vector <std::string> candidates;
	
	size_t  count;
	
	std::string candidate;
	std::string prefix;		// Part prepended to the autocompleted text
	std::string autocomp;	// Part to be autocompleted
	
	// Autocomplete just the last parameter, and find out whenever
	// autocompletion is performed on a command or a parameter
	if(command.length() == 0)return;
	
	// Get candidate keyword list
	candidates = extractArguments(command);
	
	// But abort if list is empty
	if(!candidates.size()) {
		candidates.clear();
		return;
	}
	
	prefix="";
	count = candidates.size();
	for(int i=0; i < count-1; i++){
		prefix.append(candidates[i].c_str());
		prefix.append( " " );
	}
	autocomp = candidates[count-1].c_str();
	candidates.clear();

	// Time to ask for the candidate list
	candidates = autoCompletionGetCandidates(autocomp, prefix.length()==0);
	
	// Abort if no candidates are found
	if(!candidates.size()){
		candidates.clear();
		return;
	}
	
	// Replace command line if there is just one candidate, 
	// but print candidate list and fill up to first unshared character
	command = prefix;
	if(candidates.size()==1){
		candidate = candidates[0];
		command.append(candidate.c_str());
		command.append(" ");
	} else {
		std::string minmatch = autoCompletionMinMatch(candidates);
		command.append(minmatch);
		
		for (size_t  i = 0, o = candidates.size(); i < o; i++) {
			candidate = candidates[i];
			print(candidate.c_str());
		}
	}
	candidates.clear();

	commandPosition = command.length();		
}

void Console::setAlias(const char *name, const char *value){
	unSetAlias(name);	
	aliases[name] = value;
}

std::string Console::getAlias(const char *name){
	std::map<std::string, std::string>::iterator pit = aliases.begin();
	
	while(pit != aliases.end()){
		if(!pit->first.compare(name)) return pit->second;
		pit++;
	}
	return "0";
}

void Console::unSetAlias(const char *name){
	std::map<std::string, std::string>::iterator pit = aliases.begin();

	while (pit != aliases.end()) {
		if(!strcmp(pit->first.c_str(), name)){
			aliases.erase(pit);
			return;
		}
		pit++;
	}
}

std::string &stringRemChar(std::string &str, size_t &position){
	if(position > str.length()) position = str.length();
	if(position == 0) return str;

	std::string a = str.substr(0, position-1);
	std::string b = str.substr(position, str.length() - position);

	str = a;
	str.append(b);

	position--;
	position = position<0?0:position;

	return str;
}

std::string &stringAddChar(std::string &str, size_t &position, const char c[2]){
	if(position > str.length()) position = str.length();
	
	std::string a = str.substr(0, position);
	std::string b = str.substr(position, str.length() - position);

	str = str.substr(0, position).append(c);
	str.append(b);

	position++;

	return str;
}

void Console::scroll(int i) {
	if (i < 0) {
		for (int t = 0; t > i; t--) {
			if (consoleHistoryCursor > 0) consoleHistoryCursor--;
		}
	}else {
		for (int t = 0; t < i; t++) {
			if (consoleHistoryCursor < consoleHistoryLimit) consoleHistoryCursor++;
		}
	}
}

void Console::historyBegin(void){
	
}

void Console::historyEnd(void){
}

void Console::historyAdd(std::string line){
	if (unsigned(commandHistory.size()) >= commandHistoryLimit) {
		commandHistory.erase(commandHistory.begin());
	}
	commandHistory.push_back(line);
	commandHistoryCursor = commandHistory.size();
}

void Console::historyNext(void){
	if (commandHistory.size() > 0) {
		if (commandHistoryCursor < (unsigned)commandHistory.size()- 1) {
			commandHistoryCursor++;
			command = commandHistory[commandHistoryCursor].c_str();
		}
		else {
			commandHistoryCursor = commandHistory.size();
			clearPrompt();
		}
	}
	commandPosition = command.length();
}

void Console::historyPrev(void){
	if (commandHistory.size() > 0) {
		if (commandHistoryCursor > 0) commandHistoryCursor--;
		command = commandHistory[commandHistoryCursor].c_str();
	}
	commandPosition = command.length();
}

void Console::addSpace(void){
	stringAddChar(command, commandPosition, " ");
}

void Console::removeChar(void) { //backspace key
	stringRemChar(command, commandPosition);
}

void Console::deleteChar(void) { //del key
	if (commandPosition == command.length()) return;
	commandPosition++;
	stringRemChar(command, commandPosition);
}

extern const char *keyNames[];

void Console::addChar(int k){
	
	std::string a, b;
	// leave ctrl + ascii to special functions...
	if ((!(InputDevice::alt)) && (InputDevice::control)) {
		if (k == ALLEGRO_KEY_C) ctrlc = true;
		return;
	}
	char st[2] = " ";

	int i,
		t = codepage[0],
		t_ascii = codepage[1],
		t_shift = codepage[2],
		t_alt = codepage[3];
	i = 0;
	while (t != 0) {
		if (k == t) {
			if (InputDevice::alt) st[0] = t_alt;
			else if (InputDevice::shift) {
				if (InputDevice::caps) st[0] = t_ascii;
				else st[0] = t_shift;
			}
			else if (InputDevice::caps) st[0] = t_shift;
			else st[0] = t_ascii;

			command = stringAddChar(command, commandPosition, st);

			redraw = true;
			return;
		}
		i += 4;
		t = codepage[i];
		t_ascii = codepage[i + 1];
		t_shift = codepage[i + 2];
		t_alt = codepage[i + 3];
	}
	Console::printf("%s is not bound", keyNames[k]);	
}

void Console::moveLeft(void){
	if (commandPosition>0)commandPosition--;
}

void Console::moveRight(void){
	if (commandPosition<command.length())commandPosition++;
}

void Console::moveBegin(void){
	commandPosition = 0;
}

void Console::moveEnd(void){
	commandPosition = command.length();
}

void Console::sendPrompt(void){
	// Execute command
	if (command.length() == 0)return;
	messages.push_back(Message(CONSOLE_EXEC, command.c_str()));
	clearPrompt();
}

void Console::readKeyboard(int k){
	if(enabled) {
		redraw = true;
		switch(k){
			case ALLEGRO_KEY_ESCAPE:	return clearPrompt();				
			case ALLEGRO_KEY_ENTER:		return sendPrompt();
			case ALLEGRO_KEY_DELETE:	return deleteChar();
			case ALLEGRO_KEY_BACKSPACE:	return removeChar();
			case ALLEGRO_KEY_TAB:		return autoCompletion();
			case ALLEGRO_KEY_LEFT:		return moveLeft();
			case ALLEGRO_KEY_RIGHT:		return moveRight();					
			case ALLEGRO_KEY_HOME:		return moveBegin();
			case ALLEGRO_KEY_END:		return moveEnd();
			case ALLEGRO_KEY_UP:		return historyPrev();
			case ALLEGRO_KEY_DOWN:		return historyNext();
			case ALLEGRO_KEY_SPACE:		return addSpace();
			case ALLEGRO_KEY_PGDN:		return scroll(-1);
			case ALLEGRO_KEY_PGUP:		return scroll(1);					
			default:					return addChar(k);
		}			
	}	
}

void Console::update(){	
	static char szBuffer[512];
    int nOutRead = _stdout.GetBuffer(szBuffer);
    if(nOutRead)
        Console::printf("%s",szBuffer);
	handleMessages();
	//if((wait==0)&&(!wait_for_window)) script.run();		
}

// Handle messages
void Console::handleMessages(void){
	if(messages.size() > 0){
		Message *msg;
		std::string str;
		if((msg = &messages[0])!=0){
			switch(msg->message){
				case CONSOLE_EXEC:
					str = msg->cdata;
					if (!ctrlc) {
						messages.erase(messages.begin());
						execute(str.c_str());
						return;
					}
					break;
					
				case CONSOLE_CHAR:
					if(!enabled) break;
					readKeyboard(msg->data);
					break;
					
				case CONSOLE_TOGGLE:
					enabled ^= 1;
					break;
			}
		}
		messages.erase(messages.begin());
	} 
}

void Console::clearPrompt(void){
	commandPosition = 0;
	command.clear();
}

void Console::clear(void){
	for(size_t  i=lines.size(); i>0; i--){
		lines.erase(lines.begin());
	}
	while(lines.size() < (unsigned)height){
		lines.push_back("");
	}
	return;
}

bool Console::evaluate(const char *expr){
	E_EvalTypes type;
	std::string t = expr;
	std::string a, b;
	size_t n;

	/*
		 if (!t.compare("dialog")) return (UI::initialized && UI::dialog && (UI::dialog->preloaded || UI::dialog->isOpen())) ? true : false;
	else if (!t.compare("!dialog"))return (UI::initialized && UI::dialog && (UI::dialog->preloaded || UI::dialog->isOpen())) ? false : true;
	*/
	
		 if( (n = t.find("!=")) != std::string::npos) type = EVAL_NOT_EQUAL;
	else if( (n = t.find(">=")) != std::string::npos) type = EVAL_GREATER_OR_EQUAL;
	else if( (n = t.find("<=")) != std::string::npos) type = EVAL_LESSER_OR_EQUAL;
	else if( (n = t.find("=") ) != std::string::npos) type = EVAL_EQUAL;
	else if( (n = t.find(">") ) != std::string::npos) type = EVAL_GREATER;
	else if( (n = t.find("<") ) != std::string::npos) type = EVAL_LESSER;
	else {
		n = 0;
		t = (translateVariables(std::string(expr)));
		type = EVAL_EXPRESSION;
	}

	switch(type){
		case EVAL_NOT_EQUAL:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+2, t.length()-n));
			return (a.compare(b)==0)?false:true;
			
		case EVAL_LESSER_OR_EQUAL:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+2, t.length()-n));
			return atoi(a.c_str()) <= atoi(b.c_str());
	
		case EVAL_GREATER_OR_EQUAL:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+2, t.length()-n));
			return atoi(a.c_str()) >= atoi(b.c_str());

		case EVAL_EQUAL:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+1, t.length()-n));
			return (a.compare(b)==0)?true:false;

		case EVAL_LESSER:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+1, t.length()-n));
			return atoi(a.c_str()) < atoi(b.c_str());
		
		case EVAL_GREATER:
			a = translateVariables(t.substr(0, n));
			b = translateVariables(t.substr(n+1, t.length()-n));
			return atoi(a.c_str()) > atoi(b.c_str());
		
		case EVAL_EXPRESSION:
			return atoi(t.c_str()) != 0;
	}
	return false;
}

void Console::paintBackdrop(float variance) {	
	variance = 0.0005f;
	Color *bgc = Console::con_bgcolor; 
		
	float c = float(bgc->lightness) / float(Vpu::console.height );
	int i = 0;
	Vpu::select(backdrop);
	Vpu::lock();
	float factor = (Console::opacity * 255.0f);
	for(int y= 0 ; y < Vpu::console.height; y++){
		float _opacity = factor - ((float(Vpu::console.height-y) / float(Vpu::height)) * factor);
		for(int x= 0 ; x < Vpu::console.width; x++){
			if (i & 1) {
				c = ((int(float(x*y*y / (y + 1))*variance) & 0x000000ff));
				Vpu::setColor(
					c / 2,
					c / 2,
					c / 1.8,
					_opacity
				);
			} else {
				c = ((int(float(y*x*x / (y + 1))*variance) & 0x000000ff));
				Vpu::setColor(
					c / 4,
					c / 4,
					c / 3.6,
					_opacity
				);
			}
			Vpu::putpixel(x, y);			
			i++;
		}
	}		
	Vpu::unlock();
}
	
void Console::draw(int h){
	if (!enabled){
		if (visibleHeight > 0) {
			visibleHeight =-32;
			render(0, false);
		} else return;
	} 
	
	if(!bitmap.bitmap) bitmap = Vpu::createSurface(Vpu::console.width, Vpu::console.height );	
	if (!backdrop.bitmap) {
		backdrop = Vpu::createSurface(Vpu::console.width, Vpu::console.height );
		Console::paintBackdrop();
	}
	
	static int cursor = CONSOLE_BLINK_SPEED;
	if(cursor < -CONSOLE_BLINK_SPEED) cursor = CONSOLE_BLINK_SPEED;
	else cursor--;
	
	static bool showCursor=false;
	if((cursor == CONSOLE_BLINK_SPEED)||(cursor == 0)){
		showCursor ^= 1;
		redraw = true;
	}
	
	
	if (visibleHeight < Vpu::console.height) {
		visibleHeight += speed;
		redraw = true;
	} 
	else visibleHeight = Vpu::console.height ;
	
	if (redraw) {
		render(h, showCursor);
	}
	
}

void Console::render(int h, bool drawCursor){
	size_t n = consoleHistoryCursor;
	if (!bitmap.enabled)return;
	Vpu::select(bitmap);
	Vpu::clear();
	// If edit point is not first, move cursor...
	std::string a = "";
	if(command.length() > 0) {
		a = command.substr(0, commandPosition);
		a.append(drawCursor?"_":command.substr(commandPosition, 1));
		if(commandPosition < command.length())
			a.append(command.substr(commandPosition+1, command.length() - (commandPosition+1)));
	} else {
		a.append(drawCursor?"_":" ");
	}
	
	int H = ((Engine::epoch / 3600) % 24);
	int m = (Engine::epoch / 60) % 60;
	int s = Engine::epoch % 60;

	Vpu::setColor(200, 200, 0,250);
	static std::string truncate;
	if (a.length() > 2048-12) {//asuming 2048 byte vpu.printf static buffer
		truncate = a;
		a = truncate.substr(0, 2048-12).c_str();		
	}
	if (messages.size() > 1) {
		Vpu::printf(0, 0, 0, "%02d Event Messages in queue", messages.size());
	}
	Vpu::printf(0, (Vpu::console.height - 4) - h, 0, "%02d:%02d:%02d > %s", H, m, s, a.c_str());
	
	// Render lines
	if(h>0)
	for(int u = (Vpu::console.height-4)-(h*2); u>=0; u-= h){
		if (!lines.size())continue;		
#ifndef CONSOLE_COLOR		
		//textout_ex(bitmap, font, lines.get(n)->c_str(), 0, i, config.con_fgcolor->get(), -1);
		Vpu::print(lines[n], 0, i);
#else
		Color *fgc = Console::con_fgcolor; 
		std::string li = lines[n].c_str();			
		if(li.find("~")== std::string::npos){			
			Vpu::print(li, 0, u);
		} else {
			if(li.length() > 0){		
				std::string templine(li.c_str());
				std::stringstream cmdstream(templine.c_str());
				std::string piece;
				size_t x = 0;
				bool firstPiece = true;
				int color = Color::get(fgc);
				while(!cmdstream.eof()){
					getline(cmdstream, piece, '~');
					if(piece.length() > 0){
						const char *data = piece.c_str() + (firstPiece?0:1);
						color = Console::palette[strtol(piece.substr(0,1).c_str(), NULL, 16)];
						Vpu::setColor(
							((color & 0x00ff0000)	>>16),
							((color & 0x0000ff00)	>>8	),
							((color & 0x000000ff)		),
							((color & 0xff000000)	>>24)
						);
						Vpu::print(std::string(data), (int)x, u);
								
						x += 8*strlen(piece.c_str());//replace by font width//x += Vpu::current_font->width(data);
					}
					firstPiece = false;
				}
			}
		}
#endif

		n++;
		if(n >= lines.size()) {
			lines.push_back("");
		}
	}

	height = (int)n;

	n++;
	
	Vpu::select(Vpu::console);
	Vpu::clear();
	Vpu::drawSurface(backdrop, 0, Vpu::console.height -visibleHeight,							
							   Vpu::console.width, visibleHeight);	
	Vpu::drawSurface(bitmap, 0, Vpu::console.height -visibleHeight,							
							 Vpu::console.width, visibleHeight);

	redraw = false;
}

void Console::printColoredNameValuePairFloat(const char *name, float value){
	Console::printf("~b%s ~f= ~e%f", name, value);
}

void Console::printColoredNameValuePairDir(const char *name, int value){
	std::string direction;

	if(value & 1) direction.append(" DOWN ");
	if(value & 2) direction.append(" UP ");
	if(value & 4) direction.append(" LEFT ");
	if(value & 8) direction.append(" RIGHT ");
	if(!direction.length())direction = "NONE";
	
	Console::printf("~b%s ~f= ~e%s", name, direction.c_str());
}
		
void Console::printColoredNameValuePairInt(const char *name, int value){
	Console::printf("~b%s ~f= ~e%d", name, value);
}

void Console::printColoredNameValuePairBool(const char *name, bool value){
	Console::printf("~b%s ~f= ~e%s", name, value ? "TRUE" : "FALSE");
}

void Console::printColoredNameValuePair(const char *name, const char *value){
	Console::printf("~b%s ~f= ~e'%s'", name, value);
}

#include <cstdarg>

void Console::printf(const char *fmt, ...){
	char buffer[1024];

	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buffer, 1024, fmt, ap);
	va_end(ap);

	print(buffer);
}

void Console::debug(std::string section, std::string s) {
	print(s.c_str());
}

void Console::print(std::string s){
	print(s.c_str());
}

void Console::print(const char *text){
		
	if(redirect) return;
	
		
	if(lines.size() < (unsigned)height){
		lines.push_back(std::string(text));
	} else {
		lines.insert(lines.begin(), std::string(text));
		
		std::string s = std::string(lines[lines.size()-1].c_str());
		if(consoleHistoryCursor == lines.size()) consoleHistoryCursor++;
		consoleHistory.push_back(s);
		
		if(consoleHistory.size() >= (unsigned)consoleHistoryLimit) 
			consoleHistory.erase(consoleHistory.end()-1);

		if(lines.size() >= (unsigned)consoleHistoryLimit) {
			lines.erase(lines.end()-1);
			//delete s;
		}
	}
	std::string line_without_color = stripColorCodes(lines[0]);
	
	//UI::setOneLiner(line_without_color);
	
	/*
	?????
	if (Editor::isOpen()) {
		int index = GetWindowTextLength(Editor::console_output);
		//SetFocus(hEdit); // set focus
		std::string wincmd = line_without_color.c_str() + std::string("\r\n");
		SendMessageA(Editor::console_output, EM_SETSEL, (WPARAM)index, (LPARAM)index); // set selection - end of text
		SendMessageA(Editor::console_output, EM_REPLACESEL, 0, (LPARAM)wincmd.c_str()); // append!		
	}
	*/
	redraw = true;
}

std::string Console::stripColorCodes(const std::string &l){
	std::string::const_iterator it;
	std::string output = "";
	for(it=l.begin(); it < l.end(); it++){
		if(*it == '~'){
			it++;
			if(it >= l.end()) return output;			
		} else output += *it;
	}
	return output;
}

std::string Console::translateVariables(std::string cmd){
	size_t  l = cmd.length();
	std::string newCommand = "";
	char c[2];
	for(int i=0; i < l; i++) {
		
		c[1] = '\0';
					
		c[0] = cmd.at(i);
		if(c[0] != '$') {
			newCommand.append(c);
		} else {
			std::string foundVar="$";
			i++; //skip % sign
			while(i<l){
				c[0]=cmd.at(i);// get var name
				if(c[0]==' ') break;
				else foundVar.append(c);
				i++;
			}
				
			if(foundVar.length() == 0){
				newCommand.append("$");
			} else {			
				// test against var names
				if(!foundVar.substr(0,1).compare("$")){
					CVar *cv;				
					foundVar = foundVar.substr(1, foundVar.length()-1);
					cv = CVar::getVar(foundVar);
					// In case the variable didnt exist, just print out the text
					if(!cv){
						newCommand.append(foundVar);						
					} else {						
						newCommand.append(cv->toString());
						//if this is a list of vars, append spaces!
					}
					if(i<l-1)newCommand.append(" ");
					
				} else newCommand.append(foundVar);
			}
		}
	}
	cmd = newCommand;

	return std::string(cmd);
}		

void Console::execute(std::string commandline, bool nohist){
	
	// Add command to history
	if(commandline.length() > 0){		
		std::string tempcommand(commandline.c_str());
		std::stringstream cmdstream(tempcommand.c_str());
		std::string cmd;
		while(!cmdstream.eof()){
			getline(cmdstream, cmd, ';');
	
			// First of all remove prepending spaces
			while(cmd.length() > 0){
				if(cmd.at(0) != ' ')break;
				else cmd.erase(0, 1);
			}

			// Replace commas by command separators
			size_t q;
			while((q = cmd.find("|")) != std::string::npos){
				cmd.replace(q, 1, ";");
			}	

			std::string hist = cmd;
	
			cmd = translateVariables(cmd);
	
			if(!parse(cmd)){
				if (!Script::execute(cmd)) {
					std::string tmp = "~4";
					tmp.append(hist);
					tmp.append("~c::~4Syntax Error~e");
					print(tmp.c_str());
					if(Console::remember_syntax_errors) historyAdd(hist); 
					//9-2020//if (!echo) UI::oneLinerTimer = 0;
				} else {
					if(nohist){ 
						continue;
					}
					historyAdd(hist);
				}				
			} else {
				if(nohist) continue;				
				// *hist is not deleted in this branch, it'll be deleted on ~commandHistory()
				// notice commandHistory refers to the input, consoleHistory is the output
				historyAdd(hist);				
			}
			
		}
		//SendMessage(Editor::console, WM_PAINT, 0, 0);
	}
}

#include <assert.h>

bool Console::parse(std::string &comond){
	
	std::vector<std::string> args;
	
	// Second, replace all variables by their values in the command line when prepended with a % sign
	
	std::string cmd;

	// separate command line into command and arguments
	size_t pos = comond.find(' ');
	if(pos == std::string::npos) {
		cmd = comond;
	} else {
		cmd = comond.substr(0, pos);
		std::vector<std::string> ls = extractArguments(comond.substr(pos + 1));
		for(size_t u=0; u<ls.size(); u++){
			std::string arg = ls[u];
			args.push_back(arg.c_str());
		}		
	}

	// find in list of commands the command given
	for(size_t i=0; i<commands.size(); i++){
		Command *c = &commands[i];
		
		// Skip empty commands, but dont report them as unknown commands
		if(cmd.length() == 0) return true;
		
		// Findout if c is the command we are looking for
		if(c->isCommand(cmd)) {
			// And call its callback if it is indeed
			c->call(args);
			return true;
		}
	}
	
	// Check if alias exist
	size_t i=0;
	std::map<std::string, std::string>::iterator pit = aliases.begin();
	while(pit != aliases.end()){
		if(!strcmp(pit->first.c_str(), cmd.c_str())){
			// Mix alias value with current arguments 
			std::string compound_command = pit->second;
			for(size_t u=0, v=args.size(); u<v; u++){	
				compound_command.append(" ");
				compound_command.append(args[u]);
			}
			execute(compound_command.c_str());
			return true;
		}
		pit++;
	}
	
	// Look into cvars...
	CVar *cv = CVar::getVar(cmd);
	if(cv){
		if(!args.size()) {
			switch(cv->getType()){
				default: assert(0);
				case CVAR_FLOATING: ((Floating*)cv)->showHelp(); break;
				case CVAR_INTEGER:	((Integer*)cv)->showHelp(); break;
				case CVAR_BOOLEAN:	((Boolean*)cv)->showHelp(); break;
				case CVAR_COLOR:	((Color*)cv)->showHelp(); break;
				case CVAR_TEXT:		((Text*)cv)->showHelp(); break;
			}
			return true;
		}
		switch(cv->getType()){
			default: assert(0);
			case CVAR_FLOATING: ((Floating*)cv)->parseValue(args[0]); break;
			case CVAR_INTEGER:	((Integer*)cv)->parseValue(args[0]); break;
			case CVAR_BOOLEAN:	((Boolean*)cv)->parseValue(args[0]); break;
			case CVAR_COLOR:	((Color*)cv)->parseValue(args[0]); break;
			case CVAR_TEXT:		
				std::string s;
				for(size_t u=0, v=args.size(); u<v; u++){	
					s.append(args[u]);
					if(u+1<v)s.append(" ");
				}
				cv->parseValue(s);
				break;
		}
		return true;
	}
	
	return false;
}

Console::~Console(void){	
	commandHistory.clear();	
	backdrop = Vpu::destroySurface(backdrop);
	bitmap	 = Vpu::destroySurface(bitmap);	
}

void ToggleConsole(int){
	Console::enabled ^= 1;	
}

void Console::updateColors(void) {
	Console::fgcolor = Console::con_fgcolor->integer();
	Console::bgcolor = Console::con_bgcolor->integer();
}

void Console::updateSpeed(void) {
	Console::speed = CVar::settings["speed"]->integer();
}

void Console::dumpList(std::vector<std::string> &s){
	for (size_t i = 0, o = s.size(); i < o; i++) {
		Console::printf("~a%s", s[i].c_str());
	}
}

void Console::dumpHeader(const char *header){
	Console::print("~8--------------------------------------------------");
	Console::printf("~7%s", header);
	Console::print("~8--------------------------------------------------");
}

void Console::dumpBindList(void){
	char buffer[2048];
	
	tabulate(buffer, 2048, 15, "sss", "CALLBACK", "KEY NAME", "COMMAND");
	dumpHeader(buffer);
		
	for(Trigger &t : InputDevice::trigger){
		tabulate(buffer, 2048, 15, "sss", t.callBack ? "FUNCTION" : "   --   ", t.name.c_str(), t.command.c_str());
		Console::printf("~f%s", buffer);
	}
}

void Console::dumpPropertyList(std::map<std::string, std::string> &pl){
	std::map<std::string, std::string>::iterator it = pl.begin();
	while (it != pl.end()) {
		Console::printf("~b%s ~f= ~e'%s'", it->first.c_str(), it->second.c_str());
		it++;
	}		
}

void Console::dumpToggleList(std::vector<Toggle*> &tl){
	
	for (size_t i = 0, o = tl.size(); i < o; i++) {
		Toggle *p = tl[i];
		if (!p) continue;
		char n[16];
		_itoa_s(p->id, n, 16, 10);
		std::string vr = p->name;
		vr.append(" (");
		vr.append(n);
		vr.append(") : ");
		vr.append(p->status ? "ON" : "OFF");
		print(vr);
	}
}

std::vector<std::string> split(std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

std::string Console::stripColors(const char *colored_string) {
	std::string s = "";

	for (size_t  i = 0, o = strlen(colored_string); i < o; i++) {
		if (colored_string[i] == '~')i++;
		else s += colored_string[i];
	}

	return s;
}

#include <windows.h>

BOOL DirectoryExists(const char *szPath) {
	DWORD dwAttrib = GetFileAttributesA(szPath);
	return	(dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


COMMAND_CALLBACK(cdCommand) {
	
	std::string path = Console::cwd;
	std::vector<std::string> tokens;
	
	// Check if target dir is a valid dir
	if (!strcmp(args[0].c_str(), ".")) return 0;
	if (!strcmp(args[0].c_str(), "..")) {
		// --> Go back a level
		tokens = split(path, '/');

		// Check if .. directory exists
		path += "/";
		path += args[0].c_str();

		if (!DirectoryExists(path.c_str())) return 0x1;

		// Change dir to previous dir
		if (tokens.size() == 1) return 0; // ALREADY AT ROOT 
		Console::cwd = "";
		for (size_t i = 0, o = tokens.size() - 1; i < o; i++) {
			Console::cwd += tokens[i].c_str();
			if (i < o - 1) Console::cwd += "/";
		}
		return 0;
	}
	if (!strcmp(args[0].c_str(), "/")) {
		tokens = split(path, '/');
		Console::cwd = tokens[0];
		//go to root
		return 0;
	}
	//go to specified path (if exists)
	path += "/";
	path += args[0].c_str();
	if (DirectoryExists(path.c_str())) {
		Console::cwd = path;
		return 0;
	}
	
	Console::print("~cCannot open directory");
	return 0;
}

COMMAND_CALLBACK(cwdCommand) {
	Console::print(Console::cwd);
	return 0;
}

COMMAND_CALLBACK(dirCommand) {
	std::string v = "Showing files in ";
	v += Console::cwd;
	Console::print("");
	Console::print(v);
	

	HANDLE hFind;
	WIN32_FIND_DATAA data;

	v = "";
	v = Console::cwd;
	v += "/";

	if (args.size() >0) {
		v += args[0].c_str();
	}
	else {
		v += "*.*";
	}
	std::string w = v.c_str();
	hFind = FindFirstFileA(w.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {

		do {
			v = data.cFileName;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				v = "~a<" + v + ">";
				Console::print(v.c_str());
			}

		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}

	hFind = FindFirstFileA(w.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {

		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
			v = data.cFileName;
			Console::printf("~b%s", v.c_str());
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}
	
	return 0;
}

#include <fstream>

COMMAND_CALLBACK(view) {
	std::string file = Console::cwd;
	file.append("/");
	file.append(args[0].c_str());

	std::ifstream t(file.c_str());
	if (!t) {
		Console::print("~cFile not found!");
		return 0x1;
	}
	std::stringstream buffer;
	buffer << t.rdbuf();

	// Read rest of parameters, if 			
	std::string line;
	while (!buffer.eof()) {
		getline(buffer, line);
		Console::printf("~d%s", line.c_str());
	}
	return 0;
}

COMMAND_CALLBACK(dump) {
	std::string file = Console::cwd;
	file.append("/");
	file.append(args[0].c_str());

	FILE *fp = 0x00000000;
	fopen_s(&fp, file.c_str(), "w");
	for (size_t  i = Console::lines.size() - 1; i >= 0; i--) {
		std::string st = Console::lines[i];
		if (st.empty())continue;
		if (!st.compare("\n"))continue;
		  fprintf(fp, "%s\n", Console::stripColorCodes(st.c_str()).c_str());
	}
	fclose(fp);

	Console::printf("~aDumped console buffer into ~e'%s'", args[0].c_str());
	return 0;
}

COMMAND_CALLBACK(cls) {
	Console::clear();
	return 0;
}

COMMAND_CALLBACK (toggleCon) {
	Console::enabled ^= 1;
	return 0;
}

COMMAND_CALLBACK(help) {
	Command *topic;
	std::map<std::string, std::string>::iterator pit;
	size_t pos;

	std::string helpstr = "~cNo help for requested command.";
	if (!strcmp(args[0].c_str(), "-full") || !strcmp(args[0].c_str(), "-f")) {
		char b[1024];
		Console::print("");
		std::string hlp;
		for (size_t i = 0, o = Console::commands.size(); i<o; i++) {
			topic = &Console::commands[i];
			hlp = "~e";
			hlp.append(topic->help.c_str());
			tabulate(b, 1024, 25, "ss", topic->command.c_str(), hlp.c_str());
			Console::printf("~f%s", b);
		}
		Console::print("~7-----------------------------------------------");
		Console::print("~f SCRIPTING API. Use ~ehelp <term>~f for details");
		Console::print("~7-----------------------------------------------");
		pit = Console::pyhelp.begin();
		while (pit != Console::pyhelp.end()) {
			pos = pit->second.find(" : ");
			hlp = "~e";
			std::string fullname = pit->second.substr(0, pos);
			size_t pos2 = fullname.find('.')+1;
			std::string name = fullname.substr(pos2, fullname.length() - (pos2));
			name = name.substr(0, name.find("("));
			hlp.append((pit->second.substr(pos + 3, pit->second.length() - (pos + 3))).c_str());
			tabulate(b, 1024, 25, "ss", name.c_str(), hlp.c_str());
			Console::printf("~f%s", b);
			pit++;
		}
		return 0;
	}
	if (!strcmp(args[0].c_str(), "-toc") || !strcmp(args[0].c_str(), "-t")) {
		char b[1024];
		Console::print("");
		Console::print("~eAvailable Commands:");
		for (size_t i = 0, o = Console::commands.size(); i<o; i += 4) {
			Command *t1, *t2, *t3, *t4;
			t1 = ((i) < o) ? &Console::commands[i] : NULL;
			t2 = ((i + 1) < o) ? &Console::commands[i + 1] : NULL;
			t3 = ((i + 2) < o) ? &Console::commands[i + 2] : NULL;
			t4 = ((i + 3) < o) ? &Console::commands[i + 3] : NULL;
			tabulate(b, 1024, 20, "ssss", t1 ? t1->command.c_str() : "", t2 ? t2->command.c_str() : "", t3 ? t3->command.c_str() : "", t4 ? t4->command.c_str() : "");
			Console::printf("~f%s", b);
		}
		Console::print("~aUse -full to get a comprehensive command list, including scripting API");
		return 0;
	}
	topic = Console::findCommand(args[0]);
	if (!topic) {
		// Look into pyhelp for matches
		pit = Console::pyhelp.begin();
		while (pit != Console::pyhelp.end()) {
			if (!pit->first.compare(args[0])) {
				pos = pit->second.find(" : ");
				Console::printf("~d%s", (pit->second.substr(0, pos)).c_str());
				Console::printf("  ~e%s", (pit->second.substr(pos+3, pit->second.length() - (pos+3))).c_str());
				return 0;
			}
			pit++;
		}
		Console::print(helpstr); //Unmodified (no help for requested command)
	}
	else {
		Console::printf("~d%s", topic->help.c_str());
		if (topic->usage.length() > 0) {			
			Console::printf("~b Usage : ~f%s", topic->usage.c_str());
		}
	}
	return 0;
}

COMMAND_CALLBACK(echoCommand) {
	std::string msg = "";
	for (size_t i = 0; i<args.size(); i++) {
		msg += args[i];
		if (i + 1 < args.size()) msg += " ";
	}
	msg.append("");
	Console::print(msg);
	return 0;
}

COMMAND_CALLBACK(debug_addtoggle) {
	if (args.size() < 2) {
		Console::execute("help debug_addtoggle");
		return 0x1;
	}
	Toggle *t = new Toggle;
	t->id = atoi(args[0].c_str());
	t->name = args[1].c_str();
	t->status = false;
	Console::toggles.push_back(t);
	return 0;
}

COMMAND_CALLBACK(toggleCmd) {
	Toggle *t;
	int id = atoi(args[0].c_str());
	for (size_t  i = 0, o = Console::toggles.size(); i<o; i++) {
		t = Console::toggles[i];
		if (t->id == id) {
			t->status ^= 1;
			return 0;
		}
	}
	return 0;
}

COMMAND_CALLBACK(toggle_list) {
	Console::dumpToggleList(Console::toggles);
	return 0;
}

COMMAND_CALLBACK(memDumpCommand) {
	/*
	//	LEAK_Detect();
	if (!args.size())return 0;

	unsigned ptr;
	sscanf_s(args[0].c_str(), "%x", &ptr);

	uint8_t *p = (uint8_t *)ptr;
	char buffer[256];
	std::string b;

	for (int i = 0; i<16; i++) {
		sprintf_s(buffer, 256, "~b%02X ", *p++);
		b.append(buffer);
	}
	Console::printf("%s", b.c_str());
	*/
	Console::printf("Not Implemented");
	return 0;
}

void Console::sortCommands(void) {
	std::vector<Command> sorted;
	std::vector<SpecialCommand> sorted_special;

	// Sort commands alphabetically
	for (char abc = 'a'; abc <= 'z'; abc++) {
		for (size_t i = 0, o = commands.size(); i < o; i++) {
			if (commands[i].command.at(0) == abc)sorted.push_back(commands[i]);
		}
	}

	commands = sorted;
	sorted.clear();

	// Sort specialCommands alphabetically
	for (char abc = 'a'; abc <= 'z'; abc++) {
		for (size_t i = 0, o = special_commands.size(); i < o; i++) {
			if (special_commands[i].command.at(0) == abc)sorted_special.push_back(special_commands[i]);
		}
	}

	special_commands = sorted_special;
	sorted_special.clear();
}

//#include "windows.h"

std::vector<std::string> Console::autoCompletionGetCandidates(std::string cmd, bool isCmd) {
	std::vector<std::string> candidates;

	// Do not try to get candidates for empty texts
	if (cmd.length() == 0) return candidates;

	// If prefix is empty, look for candidates in existing commands
	Command *c;
	for (size_t i = 0, o = commands.size(); i<o; i++) {
		c = &commands[i];

		if (autoCompletionCheck(c->command, cmd)) {
			candidates.push_back(c->command.c_str());
		}
	}
	

	// Iterate existing variables
	std::vector < std::string> vcand;
	for (size_t i = 0, o = candidates.size(); i<o; i++) {
		vcand.push_back(candidates[i]);
	}

	CVar::findCandidates(vcand, cmd);

	std::vector<std::string>::iterator it = vcand.begin();
	while (it != vcand.end()) {
		candidates.push_back(*it);
		it++;
	}

	// Look into aliases for matches
	std::map<std::string, std::string>::iterator pit = aliases.begin();
	while (pit != aliases.end()) {
		if (autoCompletionCheck(pit->first, cmd)) {
			std::string s = "";
			//if(isCmd) s.append("alias ");
			s.append(pit->first);
			//s.append(" ");
			candidates.push_back(s.c_str());
		}
		pit++;
	}

	// Look into pyhelp for matches
	pit = pyhelp.begin();
	while (pit != pyhelp.end()) {
		if (autoCompletionCheck(pit->first, cmd)) {
			std::string s = "";
			//if(isCmd) s.append("alias ");
			s.append(pit->first);
			//s.append(" ");
			candidates.push_back(s.c_str());
		}
		pit++;
	}

	if (!isCmd) {
		// Iterate existing filenames, only when using as parameters
		
		HANDLE hFind;
		WIN32_FIND_DATAA data;
		std::string v = cmd;
		v.append("*.*");
		std::string w = v.c_str();
		hFind = FindFirstFileA(w.c_str(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				v = data.cFileName;
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)continue;
				candidates.push_back(v.c_str());
			} while (FindNextFileA(hFind, &data));
			FindClose(hFind);
		}
	}

	// Remove duplicates
	std::vector<std::string> candidatemp;
	std::string candidate;
	for (size_t i = 0, o = candidates.size(); i < o; i++) {
		candidate = candidates[i];
		if (std::find(candidatemp.begin(), candidatemp.end(), candidate) != candidatemp.end())continue;
		candidatemp.push_back(candidate);
	}
	return candidatemp;
}

/* EXTERNAL HELPERS */
std::vector<std::string> tokenize(std::string set, char split) {
	std::vector<std::string> list;
	if (!strcmp(set.c_str(), ""))return list;

	// tokenize
	for (size_t i = 0; i < set.size();) {
		size_t o = set.find(split, i);
		if (o == std::string::npos)o = set.size();
		list.push_back(set.substr(i, o - i));
		i = o + 1;
	}
	return list;
}

std::vector<std::string> findFiles(const char *path, const char *extension) {
	std::vector<std::string> files;
	
	HANDLE hFind;
	WIN32_FIND_DATAA data;
	
	
	std::string filename = path;
	filename.append("/*.");
	filename.append(extension);
	hFind = FindFirstFileA(filename.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {

		do {
			filename = data.cFileName;
			files.push_back(filename.c_str());
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}
	return files;
}

void tabulate(char *dest, size_t dest_size, int tabsize, const char *fmt, ...) {
	std::string output = "";

	int i = 0;
	size_t  o = strlen(fmt);
	va_list ap;
	va_start(ap, fmt);

	const char *str;
	int integer;
	bool boolean;
	char number[16];

	size_t  lack = 0;
	std::string ss;

	while (i<o) {

		switch (fmt[i]) {
		case 's':
			str = va_arg(ap, char*);

			// if string is too long, truncate it 				
			if ((int)strlen(str) > tabsize) {
				if (i<o - 1) {
					ss = str;
					ss = ss.substr(0, tabsize - 3);
					ss.append("...");
					str = ss.c_str();
				}
			}

			lack = tabsize - strlen(str);
			output.append(str);
			break;
		case 'b':
			boolean = va_arg(ap, bool);
			_itoa_s(boolean, number, 16, 2);
			lack = tabsize - 3;
			output.append(boolean ? "~7[~f*~7]~f" : "~7[ ]~f");
			break;

		case 'i':
			integer = va_arg(ap, int);
			_itoa_s(integer, number, 16, 10);
			lack = tabsize - strlen(number);
			output.append(number);
			break;
		}
		// insert tab spaces
		for (int u = 0; u<lack; u++) output.append(" ");

		i++;
	}
	va_end(ap);

	strcpy_s(dest, dest_size, output.c_str());
}

std::string replace(std::string haystack, std::string needle, std::string replace_for_needle) {
	std::string::size_type n = 0;
	while ((n = haystack.find(needle, n)) != std::string::npos)
	{
		haystack.replace(n, needle.size(), replace_for_needle);
		n += replace_for_needle.size();
	}

	return haystack;
}

void Console::loadCommands(void) {
	if (initialized)return;

	CVar::loadCommands();
	// Commands defined with last parameter as "" wont drop an error if no arguments are given along
	addCommand("cd", cdCommand, "Changes active working directory", "cd <folder_name>");
	addCommand("cls", cls, "Clear console buffer", "");
	addCommand("cwd", cwdCommand, "Shows active working directory", "");
	addCommand("dir", dirCommand, "Shows files and folders in current working directory", "");
	addCommand("dump", dump, "Dump console buffer into given filename", "dump <output_filename>");
	addCommand("echo", echoCommand, "Dump given text on the console buffer", "");
	addCommand("help", help, "Show help about requested command", "help <help_term | -toc | -full>");
	addCommand("memdump", memDumpCommand, "Dump heap blocks to the filesystem", "");
	addCommand("addtoggle", debug_addtoggle, "Adds a toggle with given id and name", "debug_addtoggle <id> <name>");
	addCommand("toggle", toggleCmd, "Set or toggle given in-game toggle", "toggle <toggle_id> [toggle_status]");
	addCommand("toggleconsole", toggleCon, "Open or close the console interface", "");
	addCommand("togglelist", toggle_list, "Show list of current in-game toggles", "");
	addCommand("view", view, "Dump given filename over the console", "view <input_filename>");

	sortCommands();
}
#include "typewriter.hpp"
static void updateUIColors() {
	TypeWriter::r = CVar::settings["ui.r"]->integer();
	TypeWriter::g = CVar::settings["ui.g"]->integer();
	TypeWriter::b = CVar::settings["ui.b"]->integer();
}

void Console::loadVars(void) {
	if (initialized)return;
	CVar::settings["ui.r"] = CVar::create<Integer>("ui.r", "UI Color Red Channel"	, 128 + (rand() % 128), true);
	CVar::settings["ui.g"] = CVar::create<Integer>("ui.g", "UI Color Green Channel"	, 128 + (rand() % 128), true);
	CVar::settings["ui.b"] = CVar::create<Integer>("ui.b", "UI Color Blue Channel"	, 128 + (rand() % 128), true);
	((Integer*)CVar::settings["ui.r"])->setMinMax(0, 255);
	((Integer*)CVar::settings["ui.g"])->setMinMax(0, 255);
	((Integer*)CVar::settings["ui.b"])->setMinMax(0, 255);
	CVar::settings["ui.r"]->function = &updateUIColors;
	CVar::settings["ui.g"]->function = &updateUIColors;
	CVar::settings["ui.b"]->function = &updateUIColors;
	updateUIColors();

	CVar::settings["con_speed"] = CVar::create<Integer>("con_speed", "Console slide transition speed", 24, true);
	((Integer*)CVar::settings["con_speed"])->setMinMax(1, 600);
	CVar::settings["con_speed"]->function = &Console::updateSpeed;
	
	CVar::settings["con_fgcolor"] = CVar::create<Color>("con_fgcolor", "Console default font color", 0, true);
	CVar::settings["con_fgcolor"]->parseValue("#808080");
	CVar::settings["con_fgcolor"]->function = &Console::updateColors;
	// :( not working yet! -> ((Color*)CVar::settings["con_fgcolor"])->set(Bitmap::makecolor(0x80, 0x80, 0x80));

	CVar::settings["con_bgcolor"] = CVar::create<Color>("con_bgcolor", "Console backdrop main color", 0, true);
	CVar::settings["con_bgcolor"]->parseValue("#000040");
	CVar::settings["con_bgcolor"]->function = &Console::updateColors;
	// :( not working yet! -> ((Color*)CVar::settings["con_bgcolor"])->set(Bitmap::makecolor(0x40, 0, 0x10));

	CVar::settings["result"] = CVar::create<Text>("result", "Stores operation results", "0", true);
}

void Console::addHelp(std::string name, std::string hlp) {
	pyhelp.insert(std::pair<std::string, std::string>(name, hlp));
}