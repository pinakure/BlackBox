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
	static std::string answer; //volatile 
	static std::string _question; //volatile 
	static std::map<std::string, std::string> choices;
	static void initialize();
	static void draw();
	static void loadPicture(std::string filename, int x=0, int y=0, int w=-1, int h=-1);
	static void clearPicture();
	static void update(double delta);
	static void question(std::string question, std::map<std::string, std::string> choices);
    static void enqueue(const char *text);	
	static std::string getAnswer();
};

#endif