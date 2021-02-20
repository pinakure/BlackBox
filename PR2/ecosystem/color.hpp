#ifndef __COLOR_HPP
#define __COLOR_HPP

#include "integer.hpp"

class Color : public Integer {
	public:
		int lightness;
		void showHelp(void);
		void increase() { invokeCallback(); };
		void decrease() { invokeCallback(); };
		Color(std::string name, std::string help, int value = 0);
		void parseValue(std::string value);
		std::string toString(void);
		int darker(int factor, bool complimentary = false);
		int lighter(int factor, bool complimentary = false);
		int alter(int r, int g, int b);
		int integer(void){ return value; };
		static Color* cast(CVar *cvar) { return (Color*)cvar; }
		static int get(CVar *cvar) { return ((Color*)cvar)->integer(); }
};

#endif