#ifndef CORE_TYPEWRITER
#define CORE_TYPEWRITER


#include <map>
#include <vector>
#include <queue>
#include <string>
#include "cvar.hpp"
#include "font.hpp"

class TypeWriter {
private:
public:
	static bool next;
	static int wait_time;
	static int x;
	static int y;
	static int a;
	static int r;
	static int g;
	static int b;
	static Font* font;
	static bool enabled;
	static double width;
	static double height;
	static double final_width;
	static double final_height;
	static int current_end;
	static double current_position;
	static bool needs_redraw;
	static int bullet_radius;
	static std::vector<std::string> display;
	static std::string current;
	static std::queue<std::string> queue;
	static int active_option; //volatile 
	static int active_choice; //volatile 
	static std::string answer; //volatile 
	static std::string question; //volatile 
	static std::map<std::string, std::string> choices;
	static std::vector<CVar*> options;
	static int line_height;
	static void initialize();
	static void draw();
	static void drawChoices();
	static void drawText();
	static void clearTextBox(size_t max_length = 255, std::string placeholder = "");
	static void update(double delta);
	static void updateChoices(double delta);
	static void updateOptions(double delta);
	static void updateText(double delta);
	static void loadPicture(std::string filename, int x=0, int y=0, int w=-1, int h=-1);
	static void clearPicture();
	
	static void nextChoice();
	static void prevChoice();
	static void selectChoice();
	
	static void prevOption();
	static void nextOption();
	static void increaseValue();
	static void decreaseValue();
	
	static void enqueue(const char *text);	
	static std::string getAnswer();
};

class GetTextBox {
public:
	static const int STATUS_DISABLED = 0;
	static const int STATUS_ENABLED = 1;
	static const int STATUS_FINISHED = 2;
	static int x;
	static int y;
	static int cursor_x;
	static int cursor_y;
	static int caret_pos;
	static int status;
	static int max_length;
	static bool caps;
	static std::string text;
	static const char chars[80];
	static Font* font;
	static void initialize();
	static void draw();
	static void moveCaretLeft();
	static void moveCaretRight();
	static void finalize();
	static void moveCursorUp();
	static void moveCursorDown();
	static void moveCursorLeft();
	static void moveCursorRight();
	static void putchar();
	static void clear(std::string placeholder = "");
	static void toggleCaps();
	static void backspace();
	static void update();
};

#endif