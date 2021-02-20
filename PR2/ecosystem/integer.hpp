#ifndef __INTEGER_HPP
#define __INTEGER_HPP

#include <string>
#include "cvar.hpp"

class Integer : public CVar {
	private:
		
	protected:
		int default_value;
		int	value;
		
	public:		
		bool clamped;
		int min;
		int max;
		void showHelp(void);
		void increase(); 
		void decrease(); 

		Integer(std::string name, std::string help, int value = 0);
		std::string toString(void);
		void parseValue(std::string value);
		void setMinMax(int min, int max);
		void set(int new_value);
		int get(void);
		int integer(void){ return get(); };
		static int get(CVar *cvar) { return ((Integer*)cvar)->get(); }
		static Integer* cast(CVar *cvar) { return (Integer*)cvar; }
};

class Boolean : public Integer {
	public:
		void showHelp(void);
		
		Boolean(std::string iname, std::string ihelp, bool ivalue);
		void increase() { value ^= 1; };
		void decrease() { value ^= 1; };
		bool get(void);
		void parseValue(std::string value);
		std::string toString(void);
		int integer(void){ return get() ? 1 : 0; };
		static Boolean* cast(CVar *cvar) { return (Boolean*)cvar; }
		static bool get(CVar *cvar) { return ((Boolean*)cvar)->get(); }
};

#endif
