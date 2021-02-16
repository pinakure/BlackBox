#ifndef CORE_TYPEWRITER
#define CORE_TYPEWRITER


#include <map>
#include <vector>
#include <queue>
#include <string>

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
	static std::map<std::string, int&> options;
	static int line_height;
	static void initialize();
	static void draw();
	static void drawChoices();
	static void drawText();
	static int  get_text;
	static int	get_text_pos;
	static int	get_text_x;
	static int	get_text_y;
	static std::string _get_text;
	static void clearTextBox(size_t max_length = 255, std::string placeholder = "");
	static void drawGetTextBox();
	static void update(double delta);
	static void updateChoices(double delta);
	static void updateOptions(double delta);
	static void updateText(double delta);
	static void updateGetTextBox(double delta);
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

#endif