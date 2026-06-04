#include "command.hpp"
#include <string>
#include <vector>
#include "console.hpp"

SpecialCommand::SpecialCommand(std::string name, void(*func)(int), std::string hlp) : Command(name, NULL, hlp, "") {
	callBack = func;
}

void SpecialCommand::call(int button_status){
	callBack(button_status);
}

Command::Command(std::string name, callback func, std::string hlp, std::string usag){
	function = func;
	command = name;
	usage = usag;
	help = hlp;
}

void Command::call(std::vector<std::string> &args){
	if(usage.compare("")){
		if((!args.size())||(!strcmp(args[0].c_str(), ""))) {
			std::string usageString = "Usage: " + usage;
			Console::print(usageString.c_str());
			return;
		} 
	}
	function(args);
}

bool Command::isCommand(std::string comm){
	if(!comm.compare(command)) return true;
	return false;
}