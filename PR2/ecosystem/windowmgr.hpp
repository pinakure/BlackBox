#ifndef __WINDOWMGR_HPP
#define __WINDOWMGR_HPP

#include <map>
#include <deque>
#include <string>
#include "vpu.hpp"
#include "input.hpp"

typedef enum t_MessageType {
	MSG_MOUSE_DOWN,
	MSG_MOUSE_HOLD,
	MSG_MOUSE_UP,
	MSG_MOUSE_IN,
	MSG_MOUSE_OUT,
	MSG_SHOW,
	MSG_HIDE,
	MSG_MAX
}MessageType;

typedef enum t_WidgetType {
	WIDGET_BUTTON,
	WIDGET_LABEL,
	WIDGET_TEXTBOX,
	WIDGET_MAX
}WidgetType;

typedef enum t_DNDAction {
	DRAGNDROP_NONE		= 0x00,
	DRAGNDROP_MOVE		= 0x01,
	DRAGNDROP_RESIZE_NW = 0x02,
	DRAGNDROP_RESIZE_N  = 0x03,
	DRAGNDROP_RESIZE_NE = 0x04,
	DRAGNDROP_RESIZE_E  = 0x05,
	DRAGNDROP_RESIZE_SE = 0x06,
	DRAGNDROP_RESIZE_S  = 0x07,
	DRAGNDROP_RESIZE_SW = 0x08,
	DRAGNDROP_RESIZE_W  = 0x09
}DNDAction;

typedef enum t_CallbackType {
	CALLBACK_STRING,
	CALLBACK_INTEGER,
	CALLBACK_STATEMENT,
}CallbackType;

class Callback {
private:
	void(*string_callback)(std::string)=nullptr;
	void(*integer_callback)(int)=nullptr;
	std::string	statement = "";
public:
	CallbackType type=CALLBACK_STATEMENT;
	Callback(std::string statement){ this->statement = statement; this->type = CALLBACK_STATEMENT; }
	Callback(void(*callback)(std::string)){ this->string_callback = callback; this->type = CALLBACK_STRING; }
	Callback(void(*callback)(int)){ this->integer_callback = callback; this->type = CALLBACK_INTEGER; }
	void trigger(std::string);
	void trigger(int);
	void trigger(void);
	static void typeError(Callback *victim);
};

class Widget {
private:
protected:
	WidgetType	type;
	std::string caption = "";
	int			x		= 0;
	int			y		= 0;
	int			width	= 0;
	int			height	= 0;
	int			left	= 0;
	int			top		= 0;
	int			right   = 0;
	int			bottom  = 0;
	int			flags  = 0x00;
	int			min_height = 32;
	int			min_width  = 32;
	int alpha = 255;
	Pixel colors[5] = {
		{ 250,250,255, 255   }, // highlight
		{ 128,128,132, alpha }, // bright
		{  64, 64, 64, alpha }, // middle
		{  32, 32, 32, alpha }, // dark
		{  12, 12, 16, 200   }, // shadow
	};	
	bool redraw		 = false;
	bool hidden		 = false;


	std::deque<unsigned long long> messages;
public:
	bool mouse_in	 = false; // Changes to true on MSG_MOUSE_IN  if mouse_in = false
	bool mouse_out	 = false; // Changes to true on MSG_MOUSE_OUT if mouse_out = false
	bool mouse_up	 = false; // Changes to true on MSG_MOUSE_IN  if mouse_in = false
	bool mouse_down	 = false; // Changes to true on MSG_MOUSE_OUT if mouse_out = false
	bool mouse_hold	 = false; // Changes to true on MSG_MOUSE_IN  if mouse_in = false
	
	Widget *parent	 = nullptr;
	Callback *callback= nullptr;
	std::vector<Widget*> children;
	static void sendMessage(Widget *w, MessageType msg, long long parameter=0);
	const	char *getCaption(){ return this->caption.c_str();	}
	virtual void draw()			= 0;
	virtual void update()		= 0;
	void handleMessages();
	int  isHidden	 (){ return this->hidden;	}
	int  getX		 (){ return this->x;		}
	int  getY		 (){ return this->y;		}
	int	 getFlags	 (){ return this->flags;	}
	int  getWidth	 (){ return this->width;	}
	int  getHeight	 (){ return this->height;	}
	int  getLeft	 (){ return this->left;		}
	int  getRight	 (){ return this->right;	}
	int  getTop		 (){ return this->top;		}
	int  getBottom	 (){ return this->bottom;	}
	void setCaption	 (const char *caption_);
	void setPosition (int x, int y);
	void move		 (int x, int y);
	void setSize	 (int width, int height);
	void setLeft	 (int left);
	void setRight	 (int right);
	void setTop		 (int top);
	void setBottom	 (int bottom);
	void updateChildren();
	void drawChildren();
	void addChildren(int type=WIDGET_BUTTON, int x=0, int y=0, int width=64, int height=64, std::string caption="", Callback *callback=nullptr);
	bool contains(int cx, int cy){ 
		return (cx >= x) && (cx <= x + width) && (cy >= y) && (cy <= y + height);
	}
};

class Anchor {
private:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
public:
	Anchor(int x = 0, int y = 0, int w = 0, int h = 0) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	bool contains(int x, int y) {
		return ((x >= this->x) && (y >= this->y) && (x <= (this->x + this->w)) && (y <= (this->y + this->h)));
	}
	void move(int x, int y) {
		if (this->x + x < 0) x = 0;
		if (this->y + y < 0) y = 0;
		this->x += x;
		this->y += y;
	}
	void resize(int width, int height) {
		this->w = width;
		this->h = height;
	}
};

class Window : public Widget {
private:
	int handle  = -1;	
	int content_height = 0;
	int content_width = 0;
	int content_x = 0;
	int content_y = 0;
	bool hover = false;
public:
	Anchor anchor_nw;
	Anchor anchor_n;
	Anchor anchor_ne;
	Anchor anchor_sw;
	Anchor anchor_s;
	Anchor anchor_se;
	Anchor anchor_e;
	Anchor anchor_w;
	int  getHandle	 (){ return this->handle;	}
	Window(int handle=0, int x = 0, int y = 0, int width = 64, int height = 64, std::string caption="", int wndflags=0);
	void draw();
	int drawCaption();
	void update();
	void addComponent(WidgetType type, int x, int y, int w, int h, std::string caption="", Callback *callback=nullptr);
};

class Button : public Widget {
private:
	int id = -1;
public:
	Button(int id=0, int x=0, int y=0, int width = 64, int height=64, std::string caption="", Callback *callback=nullptr);
	void draw();
	void update();
	static int newId() { return Button::button_id++; }
	static int button_id;
};


class DragNDrop {
public:
	Point			begin	= { 0, 0 };
	Point			end		= { 0, 0 }; 
	Point			delta	= { 0, 0 };
	Window			*target = nullptr;
	DNDAction		action	= DRAGNDROP_NONE; // Move, resize left, resize right...etc

	void clear() {
		begin.x = 0;
		begin.y = 0;
		delta.x = 0;
		delta.y = 0;
		end.x = 0;
		end.y = 0;
		target = nullptr;
		action = DRAGNDROP_NONE;
	}
	
	void start(Window *target, int x, int y, DNDAction action) {
		clear();
		this->target = target;
		this->action = action;
		begin.x = x;
		begin.y = y;
		update(x,y);
	}

	void update(int x, int y) {
		end.x = x;
		end.y = y;
		delta.x = end.x - begin.x;
		delta.y = end.y - begin.y;
		switch (action) {
			case DRAGNDROP_MOVE:
				target->move(delta.x, delta.y);
				target->anchor_nw.move(delta.x, delta.y);
				target->anchor_n.move(delta.x, delta.y);
				target->anchor_ne.move(delta.x, delta.y);
				target->anchor_sw.move(delta.x, delta.y);
				target->anchor_s.move(delta.x, delta.y);
				target->anchor_se.move(delta.x, delta.y);
				target->anchor_w.move(delta.x, delta.y);
				target->anchor_e.move(delta.x, delta.y);
				printf("target->move(%d, %d);\n", delta.x, delta.y);
				break;
			case DRAGNDROP_RESIZE_NW:
				target->setLeft(target->getLeft() + delta.x);
				target->setTop(target->getTop() + delta.y);
				target->anchor_nw.move(delta.x, delta.y);
				target->anchor_n.move(0, delta.y);
				target->anchor_n.resize(target->getWidth() - 16, 8);
				target->anchor_ne.move(0,  delta.y);
				target->anchor_w.move(delta.x, delta.y);
				target->anchor_w.resize(8, target->getHeight() - 16);
				target->anchor_e.move(0, delta.y);
				target->anchor_e.resize(8, target->getHeight() - 16);
				target->anchor_sw.move(delta.x, 0);
				target->anchor_s.resize(target->getWidth() - 16, 8);
				printf("target->resize(%d, %d);\n", delta.x, delta.y);
				break;
		}
		begin.x = x;
		begin.y = y;
		
	}

	void finalize(int x, int y, bool cancel = false) {
		update(x,y);
		if(!cancel){
			switch (action) {
				case DRAGNDROP_MOVE:
					target->move(delta.x, delta.y);
					printf("target->move(%d, %d);\n", delta.x, delta.y);
					break;
			}
		}
		clear();
	}
};


class WindowManager {
private:
	static int window_handle;
	static int mouse_x;
	static int mouse_y;
	friend class Window;
	static Window *hover;
	static DragNDrop dnd;
public:
	static bool redraw;
	static std::map<int, Window> windows;
	static void initialize();
	static Window *createWindow(int x, int y, int width, int height, std::string caption, int wndflags=0);
	static int newHandle();
	static void render();
	static void update();
};

#endif