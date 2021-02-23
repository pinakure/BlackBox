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

typedef enum E_CurtainType {
	CURTAIN_TYPE_PLAIN,
	CURTAIN_TYPE_DIAGONAL_NW,
	CURTAIN_TYPE_DIAGONAL_SE,
	CURTAIN_TYPE_DIAGONAL_NE,
	CURTAIN_TYPE_DIAGONAL_SW,
	CURTAIN_TYPE_VERTICAL_S,
	CURTAIN_TYPE_VERTICAL_N,
	CURTAIN_TYPE_VERTICAL_IN,
	CURTAIN_TYPE_VERTICAL_OUT,
	CURTAIN_TYPE_HORIZONTAL_E,
	CURTAIN_TYPE_HORIZONTAL_W,
	CURTAIN_TYPE_HORIZONTAL_OUT,
	CURTAIN_TYPE_HORIZONTAL_IN,
	CURTAIN_TYPE_VERTICAL_STRIPS,
	CURTAIN_TYPE_HORIZONTAL_STRIPS,
	CURTAIN_TYPE_SQUARE_IN,
	CURTAIN_TYPE_SQUARE_OUT,
	CURTAIN_TYPE_CIRCLE_IN,
	CURTAIN_TYPE_CIRCLE_OUT,
	CURTAIN_TYPE_CHECKER,
	CURTAIN_TYPE_DIAGONAL_IN_VERTICAL,
	CURTAIN_TYPE_DIAGONAL_IN_HORIZONTAL,
	CURTAIN_TYPE_DIAGONAL_OUT_VERTICAL,
	CURTAIN_TYPE_DIAGONAL_OUT_HORIZONTAL,
	CURTAIN_TYPE_SPIRAL,
	CURTAIN_TYPE_SIZE
}CurtainType;

class Curtain {
private:
public:
	static bool forward;
	static bool enabled;
	static int time;
	static float scale;
	static CurtainType type;
	static int *grid;
	static Surface *begin_surface;
	static Surface *end_surface;
	static int delay;
	static CurtainStatus status;
	static Surface frames[10];

	static void prepare(Surface* target);
	static CurtainStatus run(double delta);
	static void render();
	static void reset(bool invert = false);
	static void draw();	
};

#endif