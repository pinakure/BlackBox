#include "windowmgr.hpp"
#include "vpu.hpp"

std::map<int, Window> WindowManager::windows;
int WindowManager::window_handle = 0;

Window::Window(int handle, int x, int y, int width, int height, std::string caption, int wndflags) {
	this->handle = handle;
	this->flags = wndflags;
	this->setPosition(x, y);
	this->setSize(width, height);
	this->caption = caption;
}

void Window::setCaption(const char *caption_){
	this->caption = caption_; 
}

void Window::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	this->left = x;
	this->top  = y;
	this->right  = this->left + this->width;
	this->bottom = this->top  + this->height;
}

void Window::setSize(int width, int height) {
	this->width  = width;
	this->height = height;
	this->right = this->left + this->width;
	this->bottom = this->top + this->height;
}

void Window::setLeft(int left) {
	this->x		= left;
	this->width = (this->right - this->x > 0) 
				? this->right - this->x
				: 1;
	this->left  = this->x + this->width;
}

void Window::setTop(int top) {
	this->y		= top;
	this->height= (this->bottom - this->y > 0) 
				? this->bottom - this->y 
				: 1;
	this->top	= this->y + this->height;
}

void Window::setRight(int right) {
	this->width = (right - this->left> 0) 
				? right - this->left
				: 1;
	this->right  = this->left + this->width;
}

void Window::setBottom(int bottom) {
	this->height= (bottom - this->top > 0) 
				? bottom - this->top 
				: 1;
	this->bottom = this->top + this->height;
}

void Window::draw() {
	//Vpu::fillRectangle(this->x, this->y, this->width, this->height,32,32,32,128);
	Vpu::select(Vpu::overlay);
	Vpu::setColor(0,255,0,255);
	Vpu::rectangle(this->x, this->y, this->width, this->height);
}

void Window::update() {
	if(this->hidden)return;
}

void WindowManager::initialize() {
	Window *w = WindowManager::createWindow( 160, 120, 64, 64, "Test Window");
}

void WindowManager::render() {
	std::map<int, Window>::iterator it = WindowManager::windows.begin();
	Window *w;
	for (; it != WindowManager::windows.end(); it++) {
		w = &((*it).second);
		if( w->hidden )continue;
		w->draw();
	}
}

void WindowManager::update() {
	std::map<int, Window>::iterator it = WindowManager::windows.begin();
	Window *w;
	for (; it != WindowManager::windows.end(); it++) {
		w = &((*it).second);
		w->update();
	}
}

Window* WindowManager::createWindow(int x, int y, int width, int height, std::string caption, int wndflags){
	int handle = WindowManager::newHandle();
	WindowManager::windows[handle] = Window(handle, x, y, width, height, caption, wndflags);
	return &WindowManager::windows[handle];
}

int WindowManager::newHandle() {
	return WindowManager::window_handle++;
}