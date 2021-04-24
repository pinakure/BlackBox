#include "windowmgr.hpp"
#include "vpu.hpp"
#include "script.hpp"

int Button::button_id = 0;

std::map<int, Window> WindowManager::windows;
int WindowManager::window_handle = 0;

Window::Window(int handle, int x, int y, int width, int height, std::string caption, int wndflags) {
	this->handle = handle;
	this->flags = wndflags;
	this->setPosition(x, y);
	this->setSize(width, height);
	this->setCaption(caption.c_str());	
}

Button::Button(int id, int x, int y, int width, int height, std::string caption, Callback *callback) {
	this->id= id;
	this->callback = callback;
	this->setPosition(x, y);
	this->setSize(width, height);
	this->setCaption(caption.c_str());
}

void Widget::setCaption(const char *caption_){
	this->caption = caption_; 
	if(this->width < al_get_text_width(Vpu::smallest_font->data, this->caption.c_str())+8)
		this->width = al_get_text_width(Vpu::smallest_font->data, this->caption.c_str())+8;
}

void Widget::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
	this->left = x;
	this->top  = y;
	this->right  = this->left + this->width;
	this->bottom = this->top  + this->height;
	if (parent) {
		this->x		 += parent->x;
		this->left	 += parent->x;
		this->right	 += parent->x;
		this->y		 += parent->y;
		this->top	 += parent->y;
		this->bottom += parent->y;
	}
}

void Widget::setSize(int width, int height) {
	this->width  = width;
	this->height = height;
	this->right = this->left + this->width;
	this->bottom = this->top + this->height;
}

void Widget::setLeft(int left) {
	this->x		= left;
	this->width = (this->right - this->x >= this->min_width) 
				? this->right - this->x
				: this->min_width;
	this->left  = this->x + this->width;
}

void Widget::setTop(int top) {
	this->y		= top;
	this->height= (this->bottom - this->y >= this->min_height) 
				? this->bottom - this->y 
				: this->min_height;
	this->top	= this->y + this->height;
}

void Widget::setRight(int right) {
	this->width = (right - this->left >= this->min_width) 
				? right - this->left
				: this->min_width;
	this->right  = this->left + this->width;
}

void Widget::setBottom(int bottom) {
	this->height= (bottom - this->top >= this->min_height) 
				? bottom - this->top 
				: this->min_height;
	this->bottom = this->top + this->height;
}

int Window::drawCaption() {
	int h = Vpu::font->height+2;
	int w = this->width - 2;
	int x = this->x + 1;
	int y = this->y + 1;
	//Vpu::fillRectangle(x+4, y+4, w  , h, colors[4].r, colors[4].g, colors[4].b, 255);
	Vpu::fillRectangle(x  , y+1  , w , h-1,colors[2].r, 
												colors[2].g/2, 
												colors[2].b/2, 
												255);
	Vpu::fillRectangle(x  , y    , w , h-1,colors[0].r, 
												colors[0].g/2, 
												colors[0].b/2, 
												255);
	Vpu::fillRectangle(x  , y+1  , w -1 , h-2,colors[1].r, 
												colors[1].g/2, 
												colors[1].b/2, 
												255);
	Vpu::setColor(al_map_rgba(255,255,255,255));
	Vpu::print(this->caption.c_str(), x+(w>>1), y+1, ALLEGRO_ALIGN_CENTER);
	return h + 1;
}

void Widget::drawChildren() {
	std::vector<Widget*>::iterator it = this->children.begin();
	for (; it != this->children.end(); it++) {
		(*it)->draw();
	}
}

void Widget::updateChildren() {
	std::vector<Widget*>::iterator it = this->children.begin();
	for (; it != this->children.end(); it++) {
		(*it)->update();
	}
}

void Widget::addChildren(int type, int x, int y, int width, int height, std::string caption, Callback *callback) {
	Widget *w = nullptr;
	switch (type) {
		case WIDGET_BUTTON:
			w = new Button(Button::newId(), x, y, width, height, caption, callback);
			w->parent = this;
			this->children.push_back(w);
			break;
		default:
			break;
	}
	w->setPosition(x,y);
}

void Window::draw() {
	Vpu::pushFont();
	Vpu::font = Vpu::smallest_font;
	int caption_height = this->drawCaption();
	int x = this->x;
	int y = this->y + caption_height;
	int w = this->width;
	int h = this->height - caption_height;
	Vpu::fillRectangle(x+4, y+4, w  , h  , colors[4].r, colors[4].g, colors[4].b, colors[4].a);
	Vpu::fillRectangle(x  , y  , w  , h  , colors[2].r, colors[2].g, colors[2].b, colors[2].a);
	Vpu::fillRectangle(x+1, y+1, w-2, h-2, colors[0].r, colors[0].g, colors[0].b, colors[0].a);
	Vpu::fillRectangle(x+1, y+2, w-3, h-3, colors[1].r, colors[1].g, colors[1].b, colors[1].a);
	Vpu::setColor(colors[3].r, colors[3].g, colors[3].b, colors[3].a);
	Vpu::rectangle(this->x, this->y, this->width, this->height);
	this->drawChildren();
	Vpu::popFont();	

}

void Button::draw() {
	int x = this->x;
	int y = this->y;
	int w = this->width;
	int h = this->height;
	// Draw plate
	Vpu::fillRectangle(x  , y  , w  , h  , colors[2].r, colors[2].g, colors[2].b, colors[2].a);
	// Draw text
	Vpu::setColor(al_map_rgba(255,255,255,255));
	Vpu::pushFont();
	Vpu::font = Vpu::smallest_font;
	Vpu::print(this->caption.c_str(), x+(w>>1), y+1, ALLEGRO_ALIGN_CENTER);
	Vpu::popFont();	
	this->drawChildren();
}

void Button::handleMessages() {
	while(this->messages.size()){
		unsigned long int v = this->messages[0];
		this->messages.pop_front();
		switch (v & 0x00FF) {
			case 0x00: 
				break;
			case 0x10: 
				break;
			case 0x20: 
				break;
			default: 
				break;
		}
	}
	this->messages.clear();
}

void Button::update() {
	if(this->hidden)return;//should receive show message...
	this->handleMessages();
	this->updateChildren();
}

void Window::addComponent(WidgetType type, int x, int y, int w, int h, std::string caption, Callback *callback) {
	this->content_x = 3;
	this->content_y = Vpu::smallest_font->height+6;
	this->content_width  = this->width  - (this->content_x<<1);
	this->content_height = this->height - (this->content_y+3);
	this->addChildren(type, 
		x + this->content_x, 
		y + this->content_y, 
		this->content_width <  w ? this->content_width  : w,
		this->content_height < h ? this->content_height : h,
		caption,
		callback
	);
}

void Window::handleMessages() {
	while(this->messages.size()){
		unsigned long int v = this->messages[0];
		this->messages.pop_front();
		switch (v & 0x00FF) {
			case 0x00: 
				break;
			case 0x10: 
				break;
			case 0x20: 
				break;
			default: 
				break;
		}
	}
	this->messages.clear();
}

void Window::update() {
	if(this->hidden)return;//should receive show message...
	this->handleMessages();
	this->updateChildren();
}

void WindowManager::initialize() {
	Window *w = WindowManager::createWindow( 320-32, 120-32, 64, 64, "Test Window");
	w->addComponent(WIDGET_BUTTON, 0,0,128,128,"");
}

void WindowManager::render() {
	std::map<int, Window>::iterator it = WindowManager::windows.begin();
	Vpu::select(Vpu::windows);
	Vpu::clear();
	Window *w;
	for (; it != WindowManager::windows.end(); it++) {
		w = &((*it).second);
		if( w->isHidden() )continue;
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
	Script::execute(this->statement);
}

void Callback::typeError(Callback* victim) {
	printf("ERROR: Unexpected type for callback 0x%p, type = %d\n", victim, victim->type);
}