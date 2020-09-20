#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>

typedef int (*callback)(std::vector<std::string>&);

class Command {
	public: 
		callback function;
		std::string usage;
		std::string help;
		std::string command;
		
		Command(std::string name, callback func, std::string help="", std::string usage="");
		virtual void call(std::vector<std::string> &arguments);
		bool isCommand(std::string comm);
		
};

class SpecialCommand : public Command {
	public:
		void(*callBack)(int);

		SpecialCommand(std::string name, void(*func)(int), std::string help="");		
		void call(int button_status);
};

#endif