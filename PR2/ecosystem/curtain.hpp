#ifndef CORE_CURTAIN
#define CORE_CURTAIN

#include "surface.hpp"

typedef enum E_CurtainStatus {
	CURTAIN_STATUS_NOTREADY,
	CURTAIN_STATUS_READY,
	CURTAIN_STATUS_CLIMAX,
	CURTAIN_STATUS_FINISHED,
	CURTAIN_STATUS_MAX
}CurtainStatus;

class Curtain {
private:
public:
	static bool forward;
	static bool enabled;
	static int time;
	static int *grid;
	static Surface *begin_surface;
	static Surface *end_surface;
	static int delay;
	static CurtainStatus status;
	static Surface frames[10];

	static void prepare(Surface* target);
	static CurtainStatus run(double delta);
	static void render();
	static void reset();
	static void draw();	
};

#endif