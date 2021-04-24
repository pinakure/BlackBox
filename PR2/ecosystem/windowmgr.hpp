#ifndef __WINDOWMGR_HPP
#define __WINDOWMGR_HPP

#include <map>
#include <deque>
#include <string>
#include "vpu.hpp"

typedef enum t_WidgetType {
	WIDGET_BUTTON,
	WIDGET_LABEL,
	WIDGET_TEXTBOX,
	WIDGET_MAX
}WidgetType;

typedef enum t_CallbackType {
	CALLBACK_STRING,
	CALLBACK_INTEGER,
	CALLBACK_STATEMENT,
}CallbackType;

class Callback {
	CallbackType type=CALLBACK_STATEMENT;
	void(*string_callback)(std::string)=nullptr;
	void(*integer_callback)(int)=nullptr;
	std::string	statement = "";
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
	std::deque<unsigned long int> messages;
public:
	Widget *parent	 = nullptr;
	Callback *callback= nullptr;
	std::vector<Widget*> children;
	const	char *getCaption(){ return this->caption.c_str();	}
	virtual void draw()			= 0;
	virtual void update()		= 0;
	virtual void handleMessages() = 0;
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
	void setSize	 (int width, int height);
	void setLeft	 (int left);
	void setRight	 (int right);
	void setTop		 (int top);
	void setBottom	 (int bottom);
	void updateChildren();
	void drawChildren();
	void addChildren(int type=WIDGET_BUTTON, int x=0, int y=0, int width=64, int height=64, std::string caption="", Callback *callback=nullptr);
	
};

class Window : public Widget {
private:
	int handle  = -1;	
	int content_height = 0;
	int content_width = 0;
	int content_x = 0;
	int content_y = 0;
public:
	int  getHandle	 (){ return this->handle;	}
	Window(int handle=0, int x = 0, int y = 0, int width = 64, int height = 64, std::string caption="", int wndflags=0);
	void draw();
	int drawCaption();
	void update();
	void handleMessages();
	void addComponent(WidgetType type, int x, int y, int w, int h, std::string caption="", Callback *callback=nullptr);
};

class Button : public Widget {
private:
	int id = -1;
public:
	Button(int id=0, int x=0, int y=0, int width = 64, int height=64, std::string caption="", Callback *callback=nullptr);
	void draw();
	void update();
	void handleMessages();
	static int newId() { return Button::button_id++; }
	static int button_id;
};


class WindowManager {
private:
	static int window_handle;
	friend class Window;
public:
	static std::map<int, Window> windows;
	static void initialize();
	static Window *createWindow(int x, int y, int width, int height, std::string caption, int wndflags=0);
	static int newHandle();
	static void render();
	static void update();
};

#endif