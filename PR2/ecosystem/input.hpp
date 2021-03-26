#ifndef __INPUT_HPP
#define __INPUT_HPP

#include <forward_list>
#include "vpu.hpp"
#include "cvar.hpp"
#include "integer.hpp"

/*! @brief
 * Module which manages user input, and responds with input events, callbacks
 */

enum E_InputButton {
	INPUT_CROSS,    // >< - button0
	INPUT_CIRCLE,   // () - button1
	INPUT_SQUARE,   // [] - button2	  
	INPUT_TRIANGLE, // /\ - button3
	INPUT_R1,		// button5
	INPUT_L1,		// button4
	INPUT_R3,		// button7
	INPUT_L3,		// button6
	INPUT_SELECT,	// button8
	INPUT_START,	// button9
	INPUT_RIGHT,	// hat_x++
	INPUT_LEFT,		// hat_x--
	INPUT_DOWN,		// hat_y++
	INPUT_UP,		// hat_y--
	INPUT_L2,		// button10
	INPUT_R2,		// button11
	INPUT_MAX
};

#include "trigger.hpp"
#include "snapshot.hpp"
#include "python.h"

class InputDevice {
	private:
		static Demo				demo;
	public:
		static PyMethodDef methods[];
		///////////////////////
		// CVARS				//
		///////////////////////		
		static bool				control_c;
		static Boolean			*in_keyboard;
		static Boolean			*in_mouse;
		static Boolean			*in_joystick;
		static Floating			*in_joysens;
		static Boolean			*aim_acceleration;
		static Floating			*aim_sensitivity;
		static Boolean			*aim_invert_x;
		static Boolean			*aim_invert_y;
		static Boolean			*aim_swap_axis;
		static Boolean			*debug_joystick;
		static Boolean			*debug_keyboard;
		static Boolean			*aim_backwards;
		static Boolean			*debug_crosshair;


		// Bitmaps to draw joystick and keyboard monitors
		static Surface			joystick_image;
		static Surface 			joystick_bitmap;
		static Surface			keyboard_bitmap[2];

		static int				controller[INPUT_MAX]; // array holding the input buttons, where all controls converge but binds
		static E_InputButton	remap[INPUT_MAX];
		static int				axis_x[2];
		static int				axis_y[2];

		static std::forward_list		<Trigger>	trigger; //!< List of trigger pointers

		static char				keyrepeat_rate;

		static int				mouse_w;
		static int				mouse_x;
		static int				mouse_y;

		static float			aim_x;
		static float			aim_y;

		static int				movement_x;
		static int				movement_y;
		
		static bool				left;
		static bool				right;
		static bool				up;
		static bool				down;

		//!< true if the keyboard has to be polled next frame
		static bool				readKeyboard;		
		//!< true if the mouse has to be polled next frame
		static bool				readMouse;			
		//!< true if the joystick has to be polled next frame
		static bool				readJoystick;		
		
		static bool				joystick;
		//!< true if shift is being pressed
		static bool				shift;				
		//!< true if alt is being pressed
		static bool				alt;				
		static bool				control;				
		static bool				caps;				

		static int				keyRepeat;
		static int				keyRepeatDelay;
		static bool				initialized;

		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////

		InputDevice(void); //!< Initializes an INPUT module, setting up use of mouse and or keyboard

		static bool			initialize(bool useKeyboard=true, bool useMouse=false, bool useJoystick = false);
		static void			deinitialize(void);

		static const char	*getControllerButtonName(E_InputButton button);

		static void			update(int); //!< Update status for each one of the triggers registered

		static int			findKeyByString(std::string &key);

		static void			setBind(const char* key, const char* value, int n, bool keydown = true, bool keypress = false, bool keyup = false);
		static	std::string getBind(const char* name);
		static bool			unSetBind(const char* keyname);

		static void			drawKeyboard(int x, int y);
		static void			drawJoystick(int x, int y);
		static void			updateJoystick(void);
		static void			updateController(void);

		static void			disableTimer(void);	//!< Disable periodic callback, which disables the polling functions
		static void			enableTimer(void);	//!< Enable periodic callback, which enables the polling functions

		static void			describeJoystick(int index);

		static void			draw(int layer_index);

		static int			waitForJoystick(void);

		static bool			playing(void) { return demo.isPlaying(); }
		static bool			recording(void) { return demo.isRecording(); }

		static void			loadVars();
		static void			restart();
		static void			handleEvent(ALLEGRO_EVENT &event);

		static PyObject* pyA(PyObject* self, PyObject* args);
		static PyObject* pyB(PyObject* self, PyObject* args);
		static PyObject* pyX(PyObject* self, PyObject* args);
		static PyObject* pyY(PyObject* self, PyObject* args);
		static PyObject* pyL1(PyObject* self, PyObject* args);
		static PyObject* pyL2(PyObject* self, PyObject* args);
		static PyObject* pyL3(PyObject* self, PyObject* args);
		static PyObject* pyR1(PyObject* self, PyObject* args);
		static PyObject* pyR2(PyObject* self, PyObject* args);
		static PyObject* pyR3(PyObject* self, PyObject* args);
		static PyObject* pyUp(PyObject* self, PyObject* args);
		static PyObject* pyDown(PyObject* self, PyObject* args);
		static PyObject* pyLeft(PyObject* self, PyObject* args);
		static PyObject* pyRight(PyObject* self, PyObject* args);
		static PyObject* pyMenu(PyObject* self, PyObject* args);
		static PyObject* pySelect(PyObject* self, PyObject* args);
		static PyObject* pyStart(PyObject* self, PyObject* args);

		~InputDevice(void);
};

extern int codepage[];

#define KEYDOWN(keyvalue)	(keyvalue == 1)
#define KEYPRESS(keyvalue)	(keyvalue > 1)
#define KEYUP(keyvalue)		(keyvalue ==-1)

#endif
