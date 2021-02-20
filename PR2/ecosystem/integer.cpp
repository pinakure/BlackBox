#include "integer.hpp"
#include "console.hpp"

void Integer::setMinMax(int imin, int imax){
	min = imin;
	max = imax;
}

void Integer::set(int new_value){
	if((value >= min)&&(value <= max)) {
		value = new_value;
		invokeCallback();
	}
}

int Integer::get(void){
	return value;
}

void Integer::parseValue(std::string new_value){
	set(atoi(new_value.c_str()));
}

Integer::Integer(std::string iname, std::string ihelp, int ivalue) : CVar(iname, ihelp){
	type = CVAR_INTEGER;
	value = ivalue;
	default_value = ivalue;
	min = INT_MIN;
	max = INT_MAX;
	clamped = false;
}

std::string Integer::toString(void){
	char buffer[32];
	sprintf_s(buffer, 32, "%d", value);
	return std::string(buffer);
}

Boolean::Boolean(std::string iname, std::string ihelp, bool ivalue) : Integer(iname, ihelp, ivalue) {
	type = CVAR_BOOLEAN;
	default_value = ivalue;
}

bool Boolean::get(void){
	return value != 0;
}

void Boolean::parseValue(std::string val){
	if( !val.compare("0") || !val.compare("false") || !val.compare("FALSE")){
		set(0);
		return;
	}
	set(1);
}

std::string Boolean::toString(void){
	return value ? "TRUE" : "FALSE";
}

void Boolean::showHelp(void){
	char buffer[64];
	sprintf_s(buffer, 64, "~b%s ~f= ~e'%s' ", name.c_str(), value?"TRUE":"FALSE");
	std::string help_str = buffer;
	sprintf_s(buffer, 64, "~fdefault: ~e'%s'\n", default_value ?"TRUE":"FALSE");		
	help_str.append(buffer);
	Console::print(help_str.c_str());
	Console::printf("~d%s\n", help.c_str());	
}

void Integer::showHelp(void){
	char buffer[64];
	sprintf_s(buffer, 64, "~b%s ~f= ~e'%d' ", name.c_str(), value);
	std::string help_str = buffer;
	if((min != INT_MIN)||(max != INT_MAX)) {
		sprintf_s(buffer, 64, "~7( ~fmin: ~e%d ~7, ~fmax: ~e%d ~7) ", min, max);
		help_str.append(buffer);
	}
	sprintf_s(buffer, 64, "~fdefault: ~e'%d'\n", default_value);
	help_str.append(buffer);
	Console::print(help_str.c_str());
	Console::printf("~d%s\n", help.c_str());	
}

void Integer::increase() { value += 1; if (value > max)value = clamped ? value - max : max; };
void Integer::decrease() { value -= 1; if (value < min)value = clamped ? max - value : min; };