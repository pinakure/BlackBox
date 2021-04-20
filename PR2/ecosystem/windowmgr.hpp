#ifndef __WINDOWMGR_HPP
#define __WINDOWMGR_HPP

#include <map>
#include <string>

class Window {
private:
	std::string caption = "";
	int			x		= 0;
	int			y		= 0;
	int			width	= 0;
	int			height	= 0;
	int			left	= 0;
	int			top		= 0;
	int			right   = 0;
	int			bottom  = 0;
	int			handle  = -1;
	int			flags  = 0x00;
	
	
public:
	bool redraw		 = false;
	bool hidden		 = false;
	
	Window(int handle=0, int x = 0, int y = 0, int width = 64, int height = 64, std::string caption="", int wndflags=0);
	void draw();
	void update();
	
	const char *getCaption(){ return this->caption.c_str();	}
	int  getHandle	 (){ return this->handle;	}
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