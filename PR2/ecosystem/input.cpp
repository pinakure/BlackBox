#include "input.hpp"
#include "input.hpp"
#include "engine.hpp"
#include "console.hpp"
char					InputDevice::keyrepeat_rate = 32;
Surface					InputDevice::joystick_image;
Surface					InputDevice::joystick_bitmap;
Surface					InputDevice::keyboard_bitmap[2];

bool					InputDevice::controller[INPUT_MAX];
E_InputButton			InputDevice::remap[INPUT_MAX];
int						InputDevice::axis_x[2];
int						InputDevice::axis_y[2];

static int num_joysticks = 0;

std::forward_list<Trigger> InputDevice::trigger;

int						InputDevice::mouse_w;
int						InputDevice::mouse_x;
int						InputDevice::mouse_y;

float					InputDevice::aim_x;
float					InputDevice::aim_y;

int						InputDevice::movement_x;
int						InputDevice::movement_y;

bool					InputDevice::left;
bool					InputDevice::right;
bool					InputDevice::up;
bool					InputDevice::down;

bool					InputDevice::readKeyboard;
bool					InputDevice::readMouse;
bool					InputDevice::readJoystick;

bool					InputDevice::joystick;
bool					InputDevice::caps = false;
bool					InputDevice::shift = false;
bool					InputDevice::alt = false;
bool					InputDevice::control = false;

int						InputDevice::keyRepeat;
int						InputDevice::keyRepeatDelay;

Demo					InputDevice::demo;

Boolean					*InputDevice::in_mouse;
Boolean					*InputDevice::in_keyboard;
Boolean					*InputDevice::in_joystick;
Floating				*InputDevice::in_joysens;
Boolean					*InputDevice::aim_invert_y;
Boolean					*InputDevice::aim_invert_x;
Boolean					*InputDevice::aim_swap_axis;
Boolean					*InputDevice::aim_acceleration;
Floating				*InputDevice::aim_sensitivity;
Boolean					*InputDevice::debug_joystick;
Boolean					*InputDevice::debug_keyboard;
Boolean					*InputDevice::aim_backwards;
Boolean					*InputDevice::debug_crosshair;

#include "codepage.cpp"  // <- codepage enum, value and key string name arrays, keys contained inside codepage array will be caught by console prompt
#include "trigger.cpp"   //  
#include "snapshot.cpp"

												// "><", "()", "[]", "/\", "L1", "R1", "L2", "R2", "SL", "ST", "+U", "+D", "+L", "+R", "L3", "R3" };
const char *controller_button_names[INPUT_MAX] = { "B1", "B2", "B3", "B4", "T1", "T2", "T3", "T4", "SL", "ST", "+U", "+D", "+L", "+R", "L3", "R3" };

InputDevice::InputDevice(){
	
} 
InputDevice::~InputDevice(void){
	disableTimer();		
}		

void InputDevice::enableTimer(void){
	//install_int_ex(readKeyboardInterrupt, MSEC_TO_TIMER(30));
}

void InputDevice::disableTimer(void){
	//remove_int(readKeyboardInterrupt);
}


const char *InputDevice::getControllerButtonName(E_InputButton button){
	return controller_button_names[button];
}

void InputDevice::describeJoystick(int index){
	Engine::printf("Describing Joystick #%d:", index);

	bool digital	= false;// joy[index].flags & JOYFLAG_DIGITAL ? true : false;
	bool analog		= false;// joy[index].flags & JOYFLAG_ANALOGUE ? true : false;
	bool calibrate	= false;// joy[index].flags & JOYFLAG_CALIBRATE ? true : false;

	if (digital)Engine::printf(">> ~bCapabilities :~a%s%s%s", digital ? " ~e[~aDIGITAL~e]" : "", analog ? " ~e[~aANALOGIC~e]" : "", calibrate ? " ~e[~aNEED CALIBRATION~e]" : "");

	int button_count = 0; // joy[index].num_buttons;
	Engine::printf(">> ~a% 2d ~bbuttons found:", button_count);

	for (int i = 0; i < button_count; i++){
		std::string button_name = 0;// joy[index].button[i].name;
		Engine::printf("  >> ~bButton ~f% 2d ~e'~a%s~e'", i, button_name.c_str());
	}
	
	int stick_count = 0;// joy[index].num_sticks;
	Engine::printf(">> ~a% 2d ~bsticks found:", stick_count);
	
	for (int i = 0; i < stick_count; i++){
		std::string stick_name = "No name";// joy[index].stick[i].name;
		bool sign = false;// joy[index].stick[i].flags & JOYFLAG_SIGNED ? true : false;
		int axis_count = 0;// joy[index].stick[i].num_axis;
		Engine::printf("    >> ~bStick ~f~a% 2d ~b: ~a%s ~baxes ~e'~a%s~e'", i, sign ? "signed" : "unsigned", stick_name.c_str());
		for (int o = 0; o < axis_count; o++){
			std::string axis_name = "No name";// joy[index].stick[i].axis[o].name;
			Engine::printf("    >> ~bAxis ~f~a% 2d ~e'~a%s~e'", o, stick_name.c_str());
		}
	}
	
}

/**
@param &keyBuf Reference to a char[0x80] containing a string which maximum length can be 0x7F, and each one of the chars until a zero is found represents the keycodes being pressed.
@bug When pressing simultaneously two or more keys, there is no keyrepeat. I dont know if this is really a bug or a cool feature. 
**/
typedef struct s_Point {
	int x;
	int y;
}Point;

typedef struct s_KeyInfo {
	Point position;
	Point size;
}KeyInfo;

#include <map>
float bitmap_keys_size =4.0f;
static std::map<int, KeyInfo> bitmap_keys;

static bool initializeBitmapKeys(void) {
	
	bitmap_keys[ALLEGRO_KEY_ESCAPE  ] = { {  0, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F1		] = { {  4, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F2		] = { {  6, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F3		] = { {  8, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F4		] = { { 10, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F5		] = { { 13, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F6		] = { { 15, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F7		] = { { 17, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F8		] = { { 19, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F9		] = { { 22, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F10		] = { { 24, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F11		] = { { 26, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F12		] = { { 28, 0 },{ 2, 2 } };
	
	bitmap_keys[ALLEGRO_KEY_PRINTSCREEN] = { { 31, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_SCROLLLOCK ] = { { 33, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_PAUSE	] = { { 35, 0 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_INSERT	] = { { 31, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_HOME	] = { { 33, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_PGUP	] = { { 35, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_DELETE	] = { { 31, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_END		] = { { 33, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_PGDN	] = { { 35, 4 },{ 2, 2 } };

	bitmap_keys[ALLEGRO_KEY_TILDE	] = { {  0, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_1		] = { {  2, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_2		] = { {  4, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_3		] = { {  6, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_4		] = { {  8, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_5		] = { { 10, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_6		] = { { 12, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_7		] = { { 14, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_8		] = { { 16, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_9		] = { { 18, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_0		] = { { 20, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_BACKQUOTE] = { { 22, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_QUOTE]= { { 22, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_EQUALS	] = { { 24, 2 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_BACKSPACE]= { { 26, 2 },{ 4, 2 } };
	
	bitmap_keys[ALLEGRO_KEY_TAB		] = { {  0, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_Q		] = { {  3, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_W		] = { {  5, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_E		] = { {  7, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_R		] = { {  9, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_T		] = { { 11, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_Y		] = { { 13, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_U		] = { { 15, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_I		] = { { 17, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_O		] = { { 19, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_P		] = { { 21, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_OPENBRACE ] = { { 23, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_CLOSEBRACE] = { { 25, 4 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_ENTER	] = { { 27, 4 },{ 3, 4 } };

	bitmap_keys[ALLEGRO_KEY_CAPSLOCK] = { {  0, 6 },{ 4, 2 } };
	bitmap_keys[ALLEGRO_KEY_A		] = { {  4, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_S		] = { {  6, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_D		] = { {  8, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_F		] = { { 10, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_G		] = { { 12, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_H		] = { { 14, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_J		] = { { 16, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_K		] = { { 18, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_L		] = { { 20, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_SEMICOLON]= { { 22, 6 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_QUOTE	] = { { 24, 6 },{ 2, 2 } };
	//bitmap_keys[ALLEGRO_KEY_] = { { 24, 6 },{ 2, 2 } };

	bitmap_keys[ALLEGRO_KEY_LSHIFT	] = { {  0, 8 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_Z		] = { {  3, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_X		] = { {  5, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_C		] = { {  7, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_V		] = { {  9, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_B		] = { { 11, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_N		] = { { 13, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_M		] = { { 15, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_COMMA	] = { { 17, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_START	] = { { 19, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_MINUS	] = { { 21, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_CIRCUMFLEX] = { { 21, 8 },{ 2, 2 } };

	bitmap_keys[ALLEGRO_KEY_UP		] = { { 33, 8 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_RSHIFT	] = { { 23, 8 },{ 8, 2 } };

	bitmap_keys[ALLEGRO_KEY_LCTRL   ] = { {  0, 10 },{ 4, 2 } };
	bitmap_keys[ALLEGRO_KEY_LWIN	] = { {  4, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_ALT		] = { {  7, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_ALTGR	] = { { 18, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_SPACE	] = { { 10, 10 },{ 8, 2 } };
	bitmap_keys[ALLEGRO_KEY_RWIN	] = { { 21, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_MENU	] = { { 24, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_RCTRL   ] = { { 27, 10 },{ 3, 2 } };
	bitmap_keys[ALLEGRO_KEY_LEFT	] = { { 31, 10 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_DOWN	] = { { 33, 10 },{ 2, 2 } };
	bitmap_keys[ALLEGRO_KEY_RIGHT	] = { { 35, 10 },{ 2, 2 } };	
	return true;
}

void InputDevice::restart() {
	
	if (in_mouse->integer()) {
		al_install_mouse();
		al_register_event_source(Engine::queue, al_get_mouse_event_source());
		//al_disable_hardware_cursor();
	} else {
		//TODO: deinit code
	}

	if (in_keyboard->integer()) {
		al_install_keyboard();
		al_register_event_source(Engine::queue, al_get_keyboard_event_source());
	} else {
		//TODO: deinit code
	}

	if (in_joystick->integer()) {
		al_install_joystick();
		al_register_event_source(Engine::queue, al_get_joystick_event_source());
		// TODO: Allow changing to another driver:
		// JOY_TYPE_DIRECTX
		// JOY_TYPE_WIN32
		// JOY_TYPE_NONE
		

		// clear controller button array			
		for (int i = 0; i<INPUT_MAX; i++) {
			controller[i] = false;
			remap[i] = (E_InputButton)i;
		}
		
		//if (num_joysticks == 0) {
		//	in_joystick->set(false);
		//	useJoystick = false;
		//}
		//for (int i = 0; i < num_joysticks; i++) {
		//	describeJoystick(i);
		//}
	} else {
		//TODO: deinit code
	}
	
	keyRepeat = 0;
	keyRepeatDelay = 0;

	InputDevice::mouse_w = 0;
	InputDevice::mouse_x = 0;
	InputDevice::mouse_y = 0;
	aim_x = 1;
	aim_y = 1;
	movement_x = 0;
	movement_y = 0;
	Engine::print("Initialized input");
}


bool InputDevice::initialize(bool useKeyboard, bool useMouse, bool useJoystick){
	readJoystick = false;
	readKeyboard = false;
	readMouse = false;

	// Set cvars
	in_keyboard		 = Boolean::cast(	CVar::settings["in_keyboard"]);
	in_mouse		 = Boolean::cast(	CVar::settings["in_mouse"]);
	in_joystick		 = Boolean::cast(	CVar::settings["in_joystick"]);
	in_joysens		 = Floating::cast(	CVar::settings["in_joysens"]);
	aim_swap_axis	 = Boolean::cast(	CVar::settings["aim_swap_axis"]);
	aim_invert_x	 = Boolean::cast(	CVar::settings["aim_invert_x"]);
	aim_invert_y	 = Boolean::cast(	CVar::settings["aim_invert_y"]);
	aim_acceleration = Boolean::cast(	CVar::settings["aim_acceleration"]);
	aim_sensitivity  = Floating::cast(	CVar::settings["aim_sensitivity"]);
	debug_joystick	 = Boolean::cast(	CVar::settings["debug_joystick"]);
	debug_keyboard	 = Boolean::cast(	CVar::settings["debug_keyboard"]);
	aim_backwards	 = Boolean::cast(	CVar::settings["aim_backwards"]);
	debug_crosshair  = Boolean::cast(	CVar::settings["debug_crosshair"]);

	in_joystick->set(useJoystick);
	in_mouse->set(useMouse);
	in_keyboard->set(useKeyboard);
	
	CVar::settings["in_joystick"]->function = InputDevice::restart;
	CVar::settings["in_keyboard"]->function = InputDevice::restart;
	CVar::settings["in_mouse"	]->function = InputDevice::restart;

	restart();
	for (int i = 0; i < 256; i++) {
		key[i] = 0;
		lap[i] = 1;
	}
	demo.clear();
	
	joystick_image = Vpu::loadBitmap("data/gfx/hud/joypad.png");
	if (!joystick_image.enabled){
		Engine::printf("Error: %d", errno);
		Engine::error("Cannot load joystick debugging picture 'data/gfx/hud/joypad.png'");
		return false;
	}
	joystick_bitmap   = Vpu::createBitmap(96, 32);

	Surface keyboard_image = Vpu::loadBitmap("data/gfx/hud/keyboard.png");
	if(!keyboard_image.enabled){
		Engine::error("Cannot load keyboard debugging picture 'data/gfx/hud/keyboard.png'");
		return false;
	}
	keyboard_bitmap[0] = Vpu::createBitmap(keyboard_image.width, keyboard_image.height / 2);
	keyboard_bitmap[1] = Vpu::createBitmap(keyboard_image.width, keyboard_image.height / 2);
	Vpu::select(keyboard_bitmap[0]);
	Vpu::drawSurface(keyboard_image, 0, 0,  keyboard_image.width, keyboard_image.height / 2, 0, 0);
	Vpu::select(keyboard_bitmap[1]);
	Vpu::drawSurface(keyboard_image, 0, keyboard_image.height/2, keyboard_image.width, keyboard_image.height / 2, 0, 0);
	Vpu::destroySurface(keyboard_image);
	initializeBitmapKeys();
	return true;
}

void InputDevice::deinitialize(void){
	Vpu::destroySurface(joystick_image);
	Vpu::destroySurface(joystick_bitmap);
	Vpu::destroySurface(keyboard_bitmap[0]);
	Vpu::destroySurface(keyboard_bitmap[1]);	
}

void right_callback(int i) {};
void left_callback(int i) {};
void down_callback(int i) {};
void up_callback(int i) {};
void jump_callback(int i) {};
void fire_callback(int i) {}
void action_callback(int i) {};
void matrixmode_callback(int i) {};
void walk_callback(int i) {};

void InputDevice::updateController(void){
	
	//PROFILE_START();
	int sens = InputDevice::in_joysens->get();
	if (aim_swap_axis->get()){
		int aim_swap = axis_x[1];
		axis_x[1] = axis_y[1];
		axis_y[1] = aim_swap;
	}

	if (aim_invert_x->get()) {
		axis_x[1] = 255 - axis_x[1];
	}
	if (aim_invert_y->get()){
		axis_y[1] = 255 - axis_y[1];
	}

	aim_x = ((float(axis_x[1])/256.0f)-0.5f)*sens;
	aim_y = ((float(axis_y[1])/256.0f)-0.5f)*sens;

	
	right_callback(axis_x[0]>128?1:3);
	left_callback(axis_x[0]<128?1:3);

	walk_callback(axis_x[0] > 128 && axis_x[0] < 192 ? 1 : axis_x[0] < 128 && axis_x[0] > 64 ? 1 : 3); 

	//up_callback(axis_y[0]<10?1:3);//jetpack! remap!
	up_callback(controller[INPUT_R1]?1:3);//jetpack! remap!
	down_callback(axis_y[0]>240?1:3);

	jump_callback(controller[INPUT_R2]?1:3);
	fire_callback(controller[INPUT_L2]?1:3);
	matrixmode_callback(controller[INPUT_L1]?1:3);
	action_callback(controller[INPUT_CROSS]?1:3);


	//TODO: switch weapons
	//TODO: handle inventary
	//TODO: handle menu
	//

	//PROFILE_END("joystick");
	
}


void InputDevice::updateJoystick(void){
	/*
	PROFILE_START();

	if(!in_joystick->get())return;

	poll_joystick();

	controller[remap[INPUT_CROSS]   ] = joy[0].button[2].b>0;
	controller[remap[INPUT_CIRCLE  ]] = joy[0].button[1].b>0;
	controller[remap[INPUT_TRIANGLE]] = joy[0].button[0].b>0;
	controller[remap[INPUT_SQUARE]] = joy[0].button[3].b>0;
	controller[remap[INPUT_L1]] = joy[0].button[4].b>0;
	controller[remap[INPUT_R1]] = joy[0].button[5].b>0;
	controller[remap[INPUT_L2]] = joy[0].button[6].b>0;
	controller[remap[INPUT_R2]] = joy[0].button[7].b>0;
	controller[INPUT_SELECT  ] = joy[0].button[8].b>0;
	controller[INPUT_START	 ] = joy[0].button[9].b>0;
	controller[INPUT_UP		 ] = joy[0].stick[2].axis[1].pos<0;
	controller[INPUT_DOWN	 ] = joy[0].stick[2].axis[1].pos>0;
	controller[INPUT_LEFT	 ] = joy[0].stick[2].axis[0].pos<0;
	controller[INPUT_RIGHT	 ] = joy[0].stick[2].axis[0].pos>0;
	controller[INPUT_L3		 ] = joy[0].button[10].b>0;
	controller[INPUT_R3		 ] = joy[0].button[11].b>0;
	axis_x[0] = joy[0].stick[0].axis[0].pos+128;
	axis_y[0] = joy[0].stick[0].axis[1].pos+128;
	axis_x[1] = joy[0].stick[0].axis[2].pos + 128;
	axis_y[1] = joy[0].stick[1].axis[0].pos;

	updateController();

	PROFILE_END("joystick");
	*/
}



void InputDevice::drawJoystick(int x, int y) {
	//if (!joystick)return;
	/* PROFILE_START(); */
	static ALLEGRO_COLOR alpha = al_map_rgba(128, 128, 128, 16);

	Vpu::select(Vpu::overlay);
	al_draw_tinted_bitmap_region(joystick_image.bitmap, alpha, 0, 0, 96,64, x, y, 0);

	for (int i = 0; i<INPUT_MAX; i++) {
		if (controller[i]) al_draw_tinted_bitmap_region(joystick_image.bitmap, alpha, 0, 64 + (i * 64), 96,64, x, y, 0);
	}

	/* Draw Analog Velocity lines */
	/*
	int lcx = x + 36;
	int rcx = x + 58;
	int lcy = y + 45;

	int rx, ry;

	rx = lcx + ((float(axis_x[0]) / 256.0f)*8.0f) - 4;
	ry = lcy + ((float(axis_y[0]) / 256.0f)*8.0f) - 4;
	line(bmp, lcx, lcy, rx, ry, makecol(255, 0, 0));

	rx = rcx + ((float(axis_x[1]) / 256.0f)*8.0f) - 4;
	ry = lcy + ((float(axis_y[1]) / 256.0f)*8.0f) - 4;
	Vpu::line(bmp, rcx, lcy, rx, ry, makecol(255, 0, 0));
	*/
	Vpu::pushColor();
	Vpu::setColor(180, 196, 240);
	Vpu::printf(x + 8, y + 56, 0, "% 4d <-LX  RX-> % 4d", axis_x[0], axis_x[1]);
	Vpu::printf(x + 8, y + 64, 0, "% 4d <-LY  RY-> % 4d", axis_y[0], axis_y[1]);
	Vpu::popColor();
	/* PROFILE_END("joystick"); */
}

void InputDevice::drawKeyboard(int x, int y) {
	/* PROFILE_START(); */
	static ALLEGRO_COLOR alpha = al_map_rgba(128, 128, 128, 16);

	Vpu::select(Vpu::overlay);
	al_draw_tinted_bitmap_region(keyboard_bitmap[0].bitmap, alpha, 0, 0, keyboard_bitmap[0].width, keyboard_bitmap[0].height, x, y, 0);
	
	for (size_t i = 0; i<255; i++) {
		if(key[i] && (bitmap_keys.find(i) != bitmap_keys.end())) {
			KeyInfo ki = bitmap_keys[i];
			al_draw_tinted_bitmap_region(
				keyboard_bitmap[1].bitmap,
				alpha,
				ki.position.x*bitmap_keys_size, ki.position.y*bitmap_keys_size,
				ki.size.x*bitmap_keys_size, ki.size.y*bitmap_keys_size,
				x + (ki.position.x*bitmap_keys_size), y + (ki.position.y*bitmap_keys_size),
				0
			);
		}
	}
	/* PROFILE_END("input.drawkeyboard"); */
}


/** 
@param delta Time elapsed since last call to this function
**/

// Draws debugging information (demo, joystick, keystrokes)
void InputDevice::draw(int layer_index){
	layer_index %= 3;
	Surface *surfs[3] = { &Vpu::background, &Vpu::foreground, &Vpu::overlay };
	Vpu::select(*surfs[layer_index]);
	int x_key = (Vpu::target->width/2) - (keyboard_bitmap[0].width/2);
	int x_joy = (Vpu::target->width/2) - (joystick_bitmap.width/2);
	drawKeyboard(x_key,   5);
	drawJoystick(x_joy, -12);
	if (demo.isInitialized()) demo.draw();
}

int InputDevice::findKeyByString(std::string &skey){
	const char *p;
	int index = 0;
	std::string k;
	
	while((p = keyNames[index])!=0){
		k = p;
		if(!k.compare(skey)) return index;
		index++;
	}

	return -1;
}

int InputDevice::waitForJoystick(void){
	if (num_joysticks == 0)return -1;

	updateJoystick();

	for (int i = 0; i < INPUT_SELECT; i++){
		if (controller[i])return i;
	}
	return INPUT_MAX;
}

void InputDevice::setBind(const char* skey, const char* value, int n, bool keydown, bool keyrepeat, bool keyup){
	if (!unSetBind(skey)){
		if (!Console::initialized)return;
		return Engine::printf("~cERROR: ~e%s~c is reserved for the system", skey);
	}

	void(*c)(int) = NULL;

	std::string v = value;

	if (v.size() > 0){
		if (v.c_str()[0] == '+'){
			// Handle '+' prefix as keyrepeat bind, with keyup notification
			v = v.substr(1, v.length() - 1);
			keyrepeat = true;
			keyup = true;
		}
		else if (v.c_str()[0] == '-'){
			// Or just a keyup bind, to stop a status
			v = v.substr(1, v.length() - 1);
			keydown = false;
			keyrepeat = false;
			keyup = true;
		}
	}

	if (Console::initialized){
		std::vector<SpecialCommand>::iterator it;
		for (SpecialCommand &s: Console::special_commands) {
			if (s.isCommand(v)) {
				c = s.callBack;
				break;
			}
		}		
	}
	if(!v.compare("fire")) c = fire_callback;

	Trigger t = Trigger(n, 0, c, keydown, keyrepeat, keyup);
	t.command = c ? value : v.c_str();
	t.name = skey;
	trigger.push_front(t);
}

std::string InputDevice::getBind(const char *name){
	for (Trigger &t : trigger) {
		if (!strcmp(t.name.c_str(), name)){
			return t.command;
		}
	}
    return "0";
}

bool InputDevice::unSetBind(const char* name){
	for (Trigger &t : trigger) {
		if (!strcmp(t.name.c_str(), name)){
			if (!t.callBack){
				trigger.remove_if([name](Trigger const& value){
					return !value.name.compare(name);
				});
				return true;
			}
			return false;
		}
	}
	return true;
}

void InputDevice::loadVars() {
	#define CREATE_CVAR(name, type, help, default_value, system_var)	CVar::settings[name] = CVar::create<type>(name, help, default_value, system_var)

	CREATE_CVAR("in_keyboard"		, Boolean	, "Control Keyboard input interaction"	, true	, true);
	CREATE_CVAR("in_mouse"			, Boolean	, "Control Mouse input interaction"		, false	, true);
	CREATE_CVAR("in_joystick"		, Boolean	, "Control Joystick input interaction"	, false	, true);
	CREATE_CVAR("in_joysens"		, Floating	, "Joystick axis sensibility"			, 1.0f	, true);
	CREATE_CVAR("aim_swap_axis"		, Boolean	, "Aim Swap X - Y Axes"					, false	, true);
	CREATE_CVAR("aim_invert_x"		, Boolean	, "Invert only X Axis"					, false	, true);
	CREATE_CVAR("aim_invert_y"		, Boolean	, "Invert only Y Axis"					, false	, true);
	CREATE_CVAR("aim_acceleration"	, Boolean	, "Enable aiming acceleration"			, false	, true);
	CREATE_CVAR("aim_sensitivity"	, Floating	, "Aiming sensitivity"					, 1.0f	, true);
		((Floating*)CVar::settings["aim_sensitivity"])->setMinMax(0.0001f, 100.0f);
	CREATE_CVAR("debug_joystick"	, Boolean	, "Show joystick debugging information"	, false	, true);
	CREATE_CVAR("debug_keyboard"	, Boolean	, "Show keyboard debugging information"	, false	, true);
	CREATE_CVAR("debug_crosshair"	, Boolean	, "Show crosshair debugging information", false	, true);
	CREATE_CVAR("aim_backwards"		, Boolean	, "Aim backwards enable"				, false	, true);	
}

void InputDevice::handleEvent(ALLEGRO_EVENT &event) {
	switch(event.keyboard.keycode){
		
		case ALLEGRO_KEY_LSHIFT:
		case ALLEGRO_KEY_RSHIFT:
		case 116:
			shift = event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN;
			break;

		case ALLEGRO_KEY_F1:
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Console::enabled ^= 1;
			}
			break;

		case ALLEGRO_KEY_F2:
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::print("Erasing Demo...");
				demo.clear();
			}
			break;
			
		case ALLEGRO_KEY_F3:
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::print("Playing back Demo...");
				demo.start();
			}
			break;

		case ALLEGRO_KEY_F4: 
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::print("Rewinding Demo...");
				demo.rewind();
			}
			break;

		case ALLEGRO_KEY_F5: 
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::scale(-0.01, -0.01, 0.0);				
			} else if (event.keyboard.type == ALLEGRO_EVENT_KEY_UP) Engine::scale(0.0, 0.0, 0.0);
			break;

		case ALLEGRO_KEY_F6: //f6
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::scale(0.01, 0.01, 0.0);
			} else if (event.keyboard.type == ALLEGRO_EVENT_KEY_UP) Engine::scale(0.0, 0.0, 0.0);
			break;

		case ALLEGRO_KEY_F7: //f7
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::rotate(-0.01);
			} else if (event.keyboard.type == ALLEGRO_EVENT_KEY_UP) Engine::rotate(0.0);
			break;

		case 54: //f8
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN) {
				Engine::rotate(0.01);
			} else if (event.keyboard.type == ALLEGRO_EVENT_KEY_UP) Engine::rotate(0.0);
			break;

		case 55: //f9
		case 56: //f10
		case 57: //f11
		case 58: //f11
			break;

		case 93: //pause / break
		case 92: //print screen

		case 223: // menu key
		case 224: // scroll lock
		case 225: // num lock
		case 222://windows right
		case 221://windows left
		case 120://dunno
		case 130://ctrl+scroll_lock
		case ALLEGRO_KEY_ALT:
			break;

		case 226: // capslock
			InputDevice::caps ^= event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN;
			break;

		case ALLEGRO_KEY_ALTGR:
			alt = event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN;
			break;
		
		case ALLEGRO_KEY_LCTRL:
		case ALLEGRO_KEY_RCTRL:
			control = event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN;
			break;
				
		case ALLEGRO_KEY_PAD_ENTER:
			event.keyboard.keycode = ALLEGRO_KEY_ENTER;
		default :			
			if (event.keyboard.type == ALLEGRO_EVENT_KEY_DOWN)
				key[event.keyboard.keycode] = 1;
			else if (event.keyboard.type == ALLEGRO_EVENT_KEY_UP)
				key[event.keyboard.keycode] = -1;
			if (Console::enabled) {
				if (KEYDOWN(key[event.keyboard.keycode])) {
					Console::messages.push_back(Message(CONSOLE_CHAR, event.keyboard.keycode));					
				}				
			}	
			
	}
}

void InputDevice::update(int delta){	

	if(demo.isInitialized()){
		
	
		if(demo.isPlaying()){
			if(!demo.play()) {
				demo.rewind();				
			};
			//PROFILE_START();
			updateController();
			//PROFILE_END("demo_k");
			return;
		}
		//PROFILE_START();
		updateController();
		//PROFILE_END("demo_j");
	}

	if (demo.isRecording())
		demo.record();

	//PROFILE_START();

	// TODO: update local member at callback when var changes only!
	int aim_radius = 1024 - InputDevice::aim_sensitivity->get();

	if(readMouse){
		/*
		int mx, my;
		get_mouse_mickeys(&mx, &my);
		InputDevice::mouse_w = mouse_w;
		position_mouse(GPU::width/2, GPU::height/2); 
		
		// TODO: update local member at callback when var changes only!
		if(InputDevice::aim_acceleration->get()){
			// accel
			movement_x = float(mx + movement_x) * 0.90f;
			movement_y = float(my + movement_y) * 0.90f;
		} else {
			// no accel
			movement_x = mx;
			movement_y = my;
		}
		// Calculate angle
		aim_x += movement_x;
		aim_y += movement_y;
		float t = atan2(aim_y, aim_x);
		aim_x = aim_radius * cos(t);
		aim_y = aim_radius * sin(t);
		
		InputDevice::mouse_x += movement_x;
		InputDevice::mouse_y += movement_y;
		InputDevice::mouse_x = InputDevice::mouse_x >= 0 ? InputDevice::mouse_x < (GPU::width*GPU::scale) ? InputDevice::mouse_x : (GPU::width*GPU::scale) -1 : 0;
		InputDevice::mouse_y = InputDevice::mouse_y >= 0 ? InputDevice::mouse_y < (GPU::height*GPU::scale) ? InputDevice::mouse_y : (GPU::height*GPU::scale)-1: 0;
		*/
		readMouse = false;
	}
	
	if(joystick)updateJoystick();

	for (Trigger &t : trigger) {
		t.update();
	}

	for (int i = 0; i < 255; i++) {
		if (KEYDOWN(key[i])||KEYPRESS(key[i])) 
			key[i]++; // Convert keyDown in keyPress
		if (KEYUP(key[i])) {
			key[i] = 0; // Convert keyUp   in keyOff
			lap[i] = 1; // Reset lap retrigger divider
		} if ((Console::enabled)||( (i >= ALLEGRO_KEY_F5 ) &&  (i <= ALLEGRO_KEY_F9 ))) {
			//Keyrepeat if console is enabled
			if (key[i] > InputDevice::keyrepeat_rate / lap[i])
				key[i] = -2;
			if (key[i] < 0) {
				key[i] = 1;
				lap[i]++;
				Console::messages.push_back(Message(CONSOLE_CHAR, i));
			}
		}
	}

	readKeyboard = false;
	readMouse	 = false;
	readJoystick = false;	

	//PROFILE_END("input");
}

#undef gpu
