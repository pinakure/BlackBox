#ifndef __CVAR_HPP
#define __CVAR_HPP

#include <map>
#include <vector>
#include <string>

typedef enum E_EvalTypes {
	EVAL_NOT_EQUAL,
	EVAL_GREATER_OR_EQUAL,
	EVAL_LESSER_OR_EQUAL,
	EVAL_EQUAL,
	EVAL_GREATER,
	EVAL_LESSER,
	EVAL_EXPRESSION
}EvalType;

enum CVarType {
	CVAR_INTEGER,
	CVAR_FLOATING,
	CVAR_BOOLEAN,
	CVAR_TEXT,
	CVAR_COLOR
};

class CVar {
	protected:
		std::string name;
		std::string help;
		CVar(std::string name, std::string help){ 
			system_var = false; 
			function = NULL; 
			this->name = name; 
			this->help = help; 
			this->uuid = CVar::global_uuid;
			CVar::global_uuid++;
		}
		CVarType type;
		unsigned int uuid;
		static unsigned int global_uuid;
	public:
		unsigned int getUUID() {return uuid;}
		static CVar* findByUUID(int uuid);
		static void setByUUID(int uuid, std::string value);
		static std::map<std::string, CVar*>	settings;
		static std::vector<CVar*>	variables;
		static void initialize(void);
		static bool initialized;

		bool system_var;
		CVarType getType(void){ return type; };
		void (*function)(void);

		std::string getName(void){ return name; }
		std::string getHelp(void){ return help; }
		void invokeCallback(void);
		virtual int increase(void) = 0;
		virtual int decrease(void) = 0;
		virtual int integer(void) = 0;
		virtual void increase()=0;
		virtual void decrease()=0;
		virtual void parseValue(std::string value) = 0;
		virtual std::string toString(void) = 0;

		static void dumpVars(void);
		static void findCandidates(std::vector<std::string> &candidates, std::string &cmd);
		static CVar* getVar(std::string name);
		static void erase(std::string name);
		static void eraseAll(void);
		static void serialize(FILE *fp);

		static void sortVars();
		static void loadCommands(void);
		static void loadVars(void);

		template <typename T, typename X> static T* create(std::string iname, std::string ihelp, const X &ivalue, bool sys_var = false) {
			T *var = new T(iname, ihelp, ivalue);
			var->system_var = sys_var;
			return var;
		}
};


class Text : public CVar {
	private:
		std::string value;
		std::string default_value;
		std::vector<std::string> dictionary;

	public:
		int max_length = 65535;
		void showHelp(void);
		
		Text(std::string name, std::string help, std::string value = "");
		void defineDictionary(int number, ...);
		void addWordToDictionary(const char *word);
		void setMaxLength(int max_len);
		void parseValue(std::string value);
		std::string toString(void);
		void set(std::string new_value);
		void increase() {};
		void decrease() {};
		std::string get(void);
		int integer(void){ return 0; };
		static std::string get(CVar *cvar) { return ((Text*)cvar)->get(); }
		static Text* cast(CVar *cvar) { return ((Text*)cvar); }		
};

class Floating : public CVar {
	private:
		float value;
		float default_value;
		
	public:
		bool clamped;
		float min;
		float max;
		void showHelp(void);
		
		Floating(std::string name, std::string help, float value = 0.0f);
		void parseValue(std::string value);
		std::string toString(void);
		void increase() { value += 0.125f; if (value > max)value = clamped ? value - max : max; };
		void decrease() { value -= 0.125f; if (value < min)value = clamped ? max - value : min; };
		void setMinMax(float imin, float imax);
		void set(float new_value);
		float get(void);
		int integer(void){ return (int)get(); };
		static float get(CVar *cvar) { return ((Floating*)cvar)->get(); }
		static Floating* cast(CVar *cvar) { return ((Floating*)cvar); }		
};

#define SYNCFLOAT(a,b)	a = ((Floating*)CVar::settings[b])->get()
#define SYNCBOOL(a,b)	a = ((Boolean*)CVar::settings[b])->get()
#define SYNCINT(a,b)	a = CVar::settings[b]->integer()
#define SYNCSTR(a,b)	a = ((Text*)CVar::settings[b])->get()

#endif