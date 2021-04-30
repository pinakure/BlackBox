#include "windowmgr.hpp"
#include "input.hpp"
int		Anchor::size = 3;
std::map<int, Window> WindowManager::windows;
int		WindowManager::window_handle = 0;
int		WindowManager::mouse_x = 0;
int		WindowManager::mouse_y = 0;
bool	WindowManager::redraw = true;
Window *WindowManager::hover = nullptr;
DragNDrop WindowManager::dnd;

PyMethodDef WindowManager::methods[] = {
	{"createwindow" , WindowManager::pyCreateWindow , METH_VARARGS, "createwindow(x, y, width, height, caption, wndflags) : "},
	{"setposition"	, WindowManager::pySetPosition  , METH_VARARGS, "setposition(handle, x, y) : "},
	{"setsize"		, WindowManager::pySetSize		, METH_VARARGS, "setsize(handle, width, height) : "},
	{"setcaption"	, WindowManager::pySetCaption	, METH_VARARGS, "setcaption(handle, caption) : "},
	{NULL, NULL, 0, NULL}
};

PyObject* WindowManager::pyCreateWindow(PyObject* self, PyObject* args) {
	float x;
	float y;
	float w;
	float h;
	char* caption = nullptr;
	int flags = 0;
	if (!PyArg_ParseTuple(args, "ffff|si", &x, &y, &w, &h, &caption, &flags)) return NULL;
	Window *win = WindowManager::createWindow(x,y,w,h,caption?caption:"UnnamedWindow", flags);
	return PyLong_FromLong(win->getHandle());
}


PyObject* WindowManager::pySetPosition(PyObject* self, PyObject* args) {
	float x;
	float y;
	int handle;
	if (!PyArg_ParseTuple(args, "iff", &handle, &x, &y)) return NULL;
	
	Window *win = WindowManager::getWindow(handle);
	if(win){
		win->setPosition(x,y);
		win->resetAnchors();
		return IntegerList(2, win->getX(), win->getY());// Must return [ x, y ], because they can be corrected in setposition
	}
	return PyBool_FromLong(false);
}
PyObject* WindowManager::pySetSize(PyObject* self, PyObject* args) {
	float x;
	float y;
	int handle;
	if (!PyArg_ParseTuple(args, "iff", &handle, &x, &y)) return NULL;	
	Window *win = WindowManager::getWindow(handle);
	if(win){
		win->setSize(x,y);
		win->resetAnchors();
		return IntegerList(2, win->getWidth(), win->getHeight());// Must return list => [ width, height ], because they can be corrected at setsize		
	}
	return PyBool_FromLong(false);
}

PyObject* WindowManager::pySetCaption(PyObject* self, PyObject* args) {
	char *caption;
	int handle;
	if (!PyArg_ParseTuple(args, "is", &handle, &caption)) return NULL;	
	Window *win = WindowManager::getWindow(handle);
	if(win){
		win->setCaption(caption);
		win->resetAnchors();
		return PyBool_FromLong(true);
	}
	return PyBool_FromLong(false);
}

Window* WindowManager::getWindow(int handle) {
	if (windows.find(handle) != windows.end()) {
		return &(windows.at(handle));
	}
	return nullptr;
}

void Callback::trigger(int argument) {
	if (this->type != CALLBACK_INTEGER) return Callback::typeError(this);
	this->integer_callback(argument);
}
void Callback::trigger(std::string argument) {
	if (this->type != CALLBACK_STRING) return Callback::typeError(this);
	this->string_callback(argument);
}
void Callback::trigger(void) {
	if (this->type != CALLBACK_STATEMENT) return Callback::typeError(this);
	std::string code = this->statement;
	Script::execute(code);
}

void Callback::typeError(Callback* victim) {
	printf("ERROR: Unexpected type for callback 0x%p, type = %d\n", victim, victim->type);
}

void WindowManager::initialize() {
	dnd.clear();
	Window*
	w = WindowManager::createWindow( 120-32, 100-32, 64, 64, "Test Window 2");
	w = WindowManager::createWindow( 220-32, 110-32, 64, 64, "Test Window 1");
	w = WindowManager::createWindow( 320-32, 120-32, 64, 64, "Test Window 3");
	Callback *bcb = new Callback("import console; console.echo('hello');");
	w->addComponent(WIDGET_BUTTON, 0,0,128,128,"Button",bcb);
}

void WindowManager::render() {
	if(!WindowManager::redraw)return;
	std::map<int, Window>::iterator it = WindowManager::windows.begin();
	Vpu::select(Vpu::windows);
	Vpu::clear();
	Window *w;
	for (; it != WindowManager::windows.end(); it++) {
		w = &((*it).second);
		if( w->isHidden() )continue;
		w->draw();
	}
	WindowManager::redraw = false;
}

#include <windows.h>

void WindowManager::update() {
	al_get_mouse_cursor_position(&WindowManager::mouse_x, &WindowManager::mouse_y);
	int offset_x=0, offset_y=0;
	al_get_window_position(Vpu::display, &offset_x, &offset_y);
	offset_y += GetSystemMetrics(SM_CYFRAME) + (GetSystemMetrics(SM_CYCURSOR))-2/*tempfix*/;//+GetSystemMetrics(SM_CYSIZE)
	offset_x += 11;/*tempfix*/
	WindowManager::mouse_x-=offset_x;
	WindowManager::mouse_y-=offset_y;
	int wx=WindowManager::hover?WindowManager::hover->getX():0;
	int wy=WindowManager::hover?WindowManager::hover->getY():0;
	Window *w;
	Window *_hover = nullptr;
	std::reverse_iterator<std::map<int, Window>::iterator> it = WindowManager::windows.rbegin();
	for (; it != WindowManager::windows.rend(); it++) {
		w = &((*it).second);
		// Find out which window is the first being below the mouse cursor (must use reverse update ordering method)
		int x		= w->getX();
		int y		= w->getY();
		int width	= w->getWidth();
		int height	= w->getHeight();

		if((!_hover)
		   &&( (WindowManager::mouse_x >= x) 
		      &(WindowManager::mouse_y >= y) 
		      &(WindowManager::mouse_x <= x + width) 
		      &(WindowManager::mouse_y <= y + height)
		     )
		  ){
			if (InputDevice::mouse_button[0]== 1) {
				if (w->anchor_nw.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_NW);
				else if (w->anchor_n.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_N);
				else if (w->anchor_ne.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_NE);
				else if (w->anchor_n.contains( mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_N);
				else if (w->anchor_ne.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_NE);
				else if (w->anchor_w.contains( mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_W);
				else if (w->anchor_e.contains( mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_E);
				else if (w->anchor_sw.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_SW);
				else if (w->anchor_s.contains( mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_S);
				else if (w->anchor_se.contains(mouse_x, mouse_y)) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_RESIZE_SE);
				else if (mouse_y < y + Vpu::font->height + 2) dnd.start(w, mouse_x, mouse_y, DRAGNDROP_MOVE);
				Widget::sendMessage(w, MSG_MOUSE_DOWN, (mouse_y << 16) | mouse_x);
				//WindowManager::bringToTop(_hover);		
			} else if(InputDevice::mouse_button[0]== 2) {
				dnd.update(mouse_x, mouse_y);
				Widget::sendMessage(w, MSG_MOUSE_HOLD, (mouse_y << 16) | mouse_x);				
			} else if(InputDevice::mouse_button[0]==-1) {
				dnd.finalize(mouse_x, mouse_y);
				Widget::sendMessage(w, MSG_MOUSE_UP  , (mouse_y << 16) | mouse_x);
			}
			_hover = w;
			
		} else switch(dnd.action){
			case DRAGNDROP_NONE: 
				break;
			default:
				if (InputDevice::mouse_button[0] == 2) {
					dnd.update(mouse_x, mouse_y);
					Widget::sendMessage(w, MSG_MOUSE_HOLD, (mouse_y << 16) | mouse_x);
				} else if (InputDevice::mouse_button[0] == -1) {
					dnd.finalize(mouse_x, mouse_y);
					Widget::sendMessage(w, MSG_MOUSE_UP, (mouse_y << 16) | mouse_x);
				}
				break;
		}

		// Generate mouse_in & mouse_out events
		if(w->contains(mouse_x, mouse_y)) {
			if (!w->mouse_in) {
				Widget::sendMessage(w, MSG_MOUSE_IN, (mouse_y << 16) | mouse_x);				
			}
		} else {
			if (!w->mouse_out) {
				Widget::sendMessage(w, MSG_MOUSE_OUT, (mouse_y << 16) | mouse_x);				
			}
		}

		// and ultimately, update the window...
		w->update();
	}
	// Update window_hover reference
	if (WindowManager::hover != _hover) {
		WindowManager::hover = _hover;
		WindowManager::redraw = true;
	}	
}

void WindowManager::bringToTop(Window* window) {
	if(!window) return;
	int handle = window->getHandle();
	std::map<int, Window> new_windows;
	std::map<int, Window>::iterator it=windows.begin();
	for (; it != windows.end(); it++) {
		if (it->second.getHandle() != handle) {
			new_windows[it->first] = it->second;
		} 
	}
	new_windows[handle] = *window;
	windows = new_windows;	
}

Window* WindowManager::createWindow(int x, int y, int width, int height, std::string caption, int wndflags){
	int handle = WindowManager::newHandle();
	WindowManager::windows[handle] = Window(handle, x, y, width, height, caption, wndflags);
	WindowManager::redraw = true;
	return &WindowManager::windows[handle];
}

int WindowManager::newHandle() {
	return WindowManager::window_handle++;
}

