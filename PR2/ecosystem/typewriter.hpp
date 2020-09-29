#ifndef CORE_TYPEWRITER
#define CORE_TYPEWRITER


#include <vector>
#include <queue>
#include <string>

class TypeWriter {
private:
public:
	static bool next;
	static int x;
	static int y;
	static double width;
	static double height;
	static double final_width;
	static double final_height;
	static int current_end;
	static double current_position;
	static std::vector<std::string> display;
	static std::string current;
	static std::queue<std::string> queue;
	static void initialize();
	static void draw();
	static void update(double delta);
    static void enqueue(const char *text);
	static bool needs_redraw;
	static int bullet_radius;
};

#endif