#include "console.hpp"
#include "cvar.hpp"
#include "color.hpp"
#include "integer.hpp"
#include "input.hpp"
#include <assert.h>
#include "linux.hpp"

std::map<std::string, CVar*> CVar::settings;
bool CVar::initialized = false;
unsigned int CVar::global_uuid;

void CVar::initialize(void) {
	if (initialized)return;
	loadCommands();
	loadVars();
	initialized = true;	
}

void CVar::findCandidates(std::vector<std::string> &candidates, std::string &cmd) {
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()) {
		if (Console::autoCompletionCheck(it->first, cmd)) {
			candidates.push_back(std::string(it->first));
		}
		it++;
	}	
}

void CVar::dumpVars(void) {
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()){
		std::string value = it->second?it->second->toString():"NULL";

		Console::printf("~b%s ~f= ~e'%s'", it->first.c_str(), value.c_str());
		it++;
	}	
}

void CVar::serialize(FILE *fp){	
	assert(fp);
	size_t q;
	
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()) {
		if (it->second) {
			std::string v = it->second->toString();
			while ((q = v.find(";")) != std::string::npos) {
				v.replace(q, 1, "|");
			}
			if (it->second->system_var)fprintf(fp, "%s %s\n", it->first.c_str(), v.c_str());
			else fprintf(fp, "set %s \"%s\"\n", it->first.c_str(), v.c_str());
		}
		it++;
	}	
}

void CVar::eraseAll(void){
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()) {
		if (it->second->system_var)it++;
		else settings.erase(it);
	}	
}

void CVar::erase(std::string name){
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()) {
		if (!it->first.compare(name)) {
			if (it->second->system_var) return;
			settings.erase(it);
			return;
		}
		it++;
	}
}

void Floating::showHelp(void){
	char buffer[64];
	sprintf_s(buffer, 64, "~b%s ~f= ~e'%.2f' ", name.c_str(), value);
	std::string help_str = buffer;
	if(clamped) {
		sprintf_s(buffer, 64, "~7( ~fmin: ~e%.2f ~7, ~fmax: ~e%.2f ~7) ", min, max);
		help_str.append(buffer);
	}
	sprintf_s(buffer, 64, "~fdefault: ~e'%.2f'\n", default_value);		
	help_str.append(buffer);
	Console::print(help_str.c_str());
	Console::printf("~d%s\n", help.c_str());
}

void Text::showHelp(void){
	char buffer[64];
	sprintf_s(buffer, 64, "~b%s ~f= ~e'%s' ", name.c_str(), value.c_str());
	std::string help_str = buffer;
	if(dictionary.size()) {
		help_str.append("~7{");
		for (size_t i = 0, o = dictionary.size(); i < o; i++) {
			help_str.append("~e");
			help_str.append(dictionary[i]);
			if(i+1 < o)help_str.append("~7|");
		}
		help_str.append("~7} ");
	}
	sprintf_s(buffer, 64, "~fdefault: ~e'%s'\n", default_value.c_str());
	help_str.append(buffer);
	Console::print(help_str.c_str());
	Console::printf("~d%s\n",help.c_str());	
}

CVar* CVar::getVar(std::string iname){
	std::map<std::string, CVar*>::iterator it = settings.begin();
	while (it != settings.end()) {
		if (!it->first.compare(iname))
		return it->second;
		it++;
	}
	return 0;
}

void CVar::invokeCallback(void){
	if(function) function();
}

Text::Text(std::string iname, std::string ihelp, std::string ivalue) : CVar(iname, ihelp) {
	value = ivalue;	
	type = CVAR_TEXT;
	default_value = value;
}

void Text::parseValue(std::string val){
	set(val);
}

void Text::set(std::string new_value){
	if(dictionary.size() > 0){
		for(size_t i=0, o=dictionary.size(); i<o; i++){
			if(!new_value.compare(dictionary[i])) {
				value = new_value;
				if (value.size() > max_length)value = value.substr(0, max_length);
				invokeCallback();
				return;
			}
		}		
	} else {
		value = new_value;
		if (value.size() > max_length)value = value.substr(0, max_length);
		invokeCallback();
	}
}

std::string Text::get(void){
	return value;
}

std::string Text::toString(void){
	return value;
}

#include <cstdarg>

void Text::defineDictionary(int number, ...){
	va_list ap;
	va_start(ap, number);
	for(int i=0; i< number; i++){
		dictionary.push_back(std::string(va_arg(ap, char*)));
	}
	va_end(ap);
}

void Text::addWordToDictionary(const char* word) {
	dictionary.push_back(std::string(word));
}

void Text::setMaxLength(int max_len) {
	max_length = max_len;
}


///////////////////////
void Floating::setMinMax(float imin, float imax){
	clamped = true;
	min = imin;
	max = imax;
}

void Floating::set(float new_value){
	if(!clamped||((value >= min)&&(value <= max))) {
		value = new_value;
		invokeCallback();
	}
}

float Floating::get(void){
	return value;
}

void Floating::parseValue(std::string new_value){
	set((float)atof(new_value.c_str()));
}

Floating::Floating(std::string iname, std::string ihelp, float ivalue) : CVar(iname, ihelp){
	type = CVAR_FLOATING;
	value = ivalue;
	clamped = false;
	default_value = ivalue;
}

std::string Floating::toString(void){
	char buffer[32];
	sprintf_s(buffer, 32, "%.2f", value);
	return std::string(buffer);
}

/* CONSOLE COMMAND CALLBACKS */

/* OPERATORS : These store the result of their operations in the variable $result */
COMMAND_CALLBACK(sumCommand) {
	if (args.size() < 2)return 0x1;
	int a = atoi(args[0].c_str());
	int b = atoi(args[1].c_str());
	char number[64];
	_itoa_s(a + b, number, 64, 10);
	Console::result->parseValue(number);
	return 0;
}

COMMAND_CALLBACK(multiplyCommand) {
	if (args.size() < 2)return 0x1;
	int a = atoi(args[0].c_str());
	int b = atoi(args[1].c_str());
	char number[64];
	_itoa_s(a*b, number, 64, 10);
	Console::result->parseValue(number);
	return 0;
}

COMMAND_CALLBACK(divideCommand) {
	if (args.size() < 2)return 0x1;
	int a = atoi(args[0].c_str());
	int b = atoi(args[1].c_str());
	if (b < 1) {
		Console::execute("echo division by zero!");
		return 0xFFFFFFFF;
	}
	char number[64];
	_itoa_s(a / b, number, 64, 10);
	Console::result->parseValue(number);
	return 0;
}

COMMAND_CALLBACK(subtractCommand) {
	if (args.size() < 2)return 0x1;
	int a = atoi(args[0].c_str());
	int b = atoi(args[1].c_str());
	char number[64];
	_itoa_s(a - b, number, 64, 10);
	Console::result->parseValue(number);
	return 0;
}

COMMAND_CALLBACK(xorCommand) {
	int a = atoi(args[0].c_str());
	int b = atoi(args[1].c_str());
	char number[64];
	_itoa_s(a^b, number, 64, 10);
	Console::result->parseValue(number);
	return 0;
}

COMMAND_CALLBACK(concatCommand) {
	if (args.size() < 2)return 0x1;
	std::string s = args[0];
	s.append(args[1]);
	Console::result->parseValue(s);
	return 0;
}

COMMAND_CALLBACK(aliasesCommand) {
	Console::dumpPropertyList(Console::aliases);
	return 0;
}

COMMAND_CALLBACK(vars) {
	CVar::dumpVars();
	return 0;
}

COMMAND_CALLBACK(aliasCommand) {
	std::string name = args[0].c_str();
	if (args.size() == 1) {
		Console::reportAlias(name.c_str());
		return 0x100;
	}

	std::string value = args[1];
	for (size_t i = 2; i < args.size(); i++) {
		value.append(" ");
		value.append(args[i]);
	}
	if (!name.compare(value)) {
		Console::print("~cAre you insane!?");
		return 0xFFFFFFFF;
	}
	Console::setAlias(name.c_str(), value.c_str());
	return 0;
}

COMMAND_CALLBACK(set) {
	std::string name = args[0];

	// Just report the variable's value
	if (args.size() == 1) {
		CVar *cv = CVar::getVar(name);
		if (!cv) {
			Console::printf("'%s' is not set", name.c_str());
			return 0x1;
		}
		switch (cv->getType()) {
		default: assert(0);
		case CVAR_FLOATING:	((Floating*)cv)->showHelp(); return 0x2;
		case CVAR_INTEGER: ((Integer*)cv)->showHelp(); return 0x2;
		case CVAR_BOOLEAN: ((Boolean*)cv)->showHelp(); return 0x2;
		case CVAR_COLOR: ((Color*)cv)->showHelp(); return 0x2;
		case CVAR_TEXT:	((Text*)cv)->showHelp(); return 0x2;
		}
	}

	// Compile all existing arguments in a single one
	std::string value = args[1];
	for (size_t i = 2; i < args.size(); i++) {
		value.append(" ");
		value.append(args[i]);
	}

	CVar *cv = CVar::getVar(name);
	// If CVar exists, update it
	if (cv) {
		cv->parseValue(value);
		return 0x3;
	}

	// Finally, if no cvar exists with given name, create a new one
	CVar::settings[std::string(name)] = CVar::create<Text>(name, "", value);
	return 0;
}

COMMAND_CALLBACK(unset) {
	CVar::erase(args[0]);
	return 0;
}

COMMAND_CALLBACK(unaliasCommand) {
	Console::unSetAlias(args[0].c_str());
	return 0;
}

COMMAND_CALLBACK(unsetallCommand) {
	CVar::eraseAll();
	return 0;
}

COMMAND_CALLBACK(writeconfig) {
	std::string filename = args[0].c_str();
	if (filename.find(".cfg") == std::string::npos) filename.append(".cfg");
	std::string s = "~aWritten settings to ~e";
	s += filename;
	FILE *fp = 0x00000000;
	fopen_s(&fp, filename.c_str(), "w");
	size_t q;

	if (fp) {
		fprintf(fp, "unbindall\nunsetall\n");
		CVar::serialize(fp);
		fprintf(fp, "vpu.restart\n");
		
		std::map<std::string, std::string>::iterator a_it= Console::aliases.begin();
		while (a_it != Console::aliases.end()) {
			std::string val = a_it->second;
			while ((q = val.find(";")) != std::string::npos) {
				val.replace(q, 1, "|");
			}
			fprintf(fp, "alias %s \"%s\"\n", a_it->first.c_str(), val.c_str());
			a_it++;
		}
		
		
		for (Trigger &t : InputDevice::trigger) {
			if (!t.callBack)
			while ((q = t.command.find(";")) != std::string::npos) {
				t.command.replace(q, 1, "|");
			}
			fprintf(fp, "bind %s \"%s\"\n", t.name.c_str(), t.command.c_str());
		}
		
		// Save current font
		//fprintf(fp, "font %d", Vpu::current_font_index);
		
		fclose(fp);

		Console::print(s.c_str());
	}
	return 0;
}

void CVar::sortVars(void) {
	std::map<std::string, CVar*> sorted;
	
	// Sort commands alphabetically
	for (char abc = 'a'; abc <= 'z'; abc++) {
		std::map<std::string, CVar*>::iterator pit = settings.begin();
		while(pit != settings.end()){
			if (pit->first.at(0) == abc)sorted[pit->first] = pit->second;
			pit++;
		}
	}

	settings = sorted;
	sorted.clear();
}

void CVar::loadCommands(void) {
	if (initialized)return; 
	Console::addCommand("unalias",		unaliasCommand,		"Unset given alias", "unalias <alias_name>");
	Console::addCommand("unset",		unset,				"Unset given variable", "unset <variable_name>");
	Console::addCommand("unsetall",		unsetallCommand,	"Unsets every variable set", "");
	Console::addCommand("vars",			vars,				"Show list of defined variables", "");
	Console::addCommand("alias",		aliasCommand,		"Set given alias for a command line", "alias <alias_name> [cmd_line]");
	Console::addCommand("aliases",		aliasesCommand,		"Show list of current aliases", "");
	Console::addCommand("divide",		divideCommand,		"Divide given operators and store the value in $result", "divide <op1> <op2>");
	Console::addCommand("subtract",		subtractCommand,	"Subtract given operators and store the value in $result", "substract <op1> <op2>");
	Console::addCommand("sum",			sumCommand,			"Sum given operators and store the value in $result", "sum <op1> <op2>");
	Console::addCommand("multiply",		multiplyCommand,	"Multiply given operators and store the value in $result", "multiply <op1> <op2>");
	Console::addCommand("set",			set,				"Set or view value of given variable", "set <variable_name> [variable_value]");
	Console::addCommand("concat",		concatCommand,		"Concatenate given arguments and store the value in $result", "concat <op1> <op2>");
	Console::addCommand("xor",			xorCommand,			"Apply XOR over given value and store value in $result", "xor <value>");
	Console::addCommand("writeconfig",	writeconfig,		"Stores variable settings under given filename", "writeconfig <config_name>");	
}

void CVar::loadVars(void) {
	if (initialized)return;
	Console::loadVars();
	InputDevice::loadVars();
	Vpu::loadVars();
	sortVars();
}

CVar* CVar::findByUUID(int uuid) {
	std::map<std::string, CVar*>::iterator it;
	for (it = settings.begin(); it != settings.end(); it++) {
		if (it->second) {
			if (it->second->uuid == uuid)return it->second;
		}
	}
	return NULL;
}

void CVar::setByUUID(int uuid, std::string value) {
	CVar* target = findByUUID(uuid);
	if (target) {
		target->parseValue(value);
	}
}
