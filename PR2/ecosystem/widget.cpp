#include "windowmgr.hpp"

void Widget::sendMessage(Widget* w, MessageType msg, long long parameter) {
	w->messages.push_back((parameter << 16) | ((unsigned int) msg&0x00000000FFFF));
}

void Widget::setCaption(const char *caption_){
	this->caption = caption_; 
	this->min_width = al_get_text_width(Vpu::smallest_font->data, this->caption.c_str())+8;
	if(this->width < min_width)
		this->width = min_width;
}

void Widget::move(int x, int y) {
	if(this->x+x < 0) x = 0;
	if(this->y+y < 0) y = 0;
	this->x		 += x;
	this->y      += y;
	this->left   += x;
	this->top    += y;
	this->right  = this->left+this->width;
	this->bottom = this->top+this->height;
	std::vector<Widget*>::iterator it=children.begin();
	for(;it!=children.end(); it++){
		(*it)->move(x, y);
	}	
}

void Widget::setPosition(int x, int y) {
	int delta_x = x - this->x;
	int delta_y = y - this->y;
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
	std::vector<Widget*>::iterator it=children.begin();
	for(;it!=children.end(); it++){
		(*it)->move(delta_x, delta_y);
	}
}

void Widget::setSize(int width, int height) {
	this->width  = width < min_width ? min_width : width;
	this->height = height < min_height ? min_height : height;
	this->right = this->left + this->width;
	this->bottom = this->top + this->height;
}

void Widget::setLeft(int left) {
	this->x		= left;
	this->setSize((this->right - this->x >= this->min_width) 
				? this->right - this->x
				: this->min_width,
				this->height);
	this->left  = this->x;
}

void Widget::setTop(int top) {
	this->y		= top;
	this->setSize(this->width,
				(this->bottom - this->y >= this->min_height) 
				? this->bottom - this->y 
				: this->min_height);
	this->top	= this->y;
}

void Widget::setRight(int right) {
	this->setSize((right - this->left >= this->min_width) 
					? right - this->left
					: this->min_width,
					this->height);
	this->right  = this->x+this->width;
}

void Widget::setBottom(int bottom) {
	this->height= (bottom - this->top >= this->min_height) 
				? bottom - this->top 
				: this->min_height;
	this->bottom = this->top + this->height;
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

Widget *Widget::addChildren(int type, int x, int y, int width, int height, std::string caption, Callback *callback, int flags) {
	Widget *w = nullptr;
	switch (type) {
		case WIDGET_BUTTON:
			w = new Button(Button::newId(), x, y, width, height, caption, callback, flags);
			w->handle = WindowManager::widget_handle++;
			w->parent = this;
			this->children.push_back(w);
			break;
		default:
			break;
	}
	w->setPosition(x,y);
	return w;
}

void Widget::handleMessages() {
	while(this->messages.size()){
		unsigned long long msg = this->messages[0];
		this->messages.pop_front(); // dispose message asap
		unsigned int parameter = (msg & 0xffffffff0000)>>16;
		MessageType type = MessageType(msg & 0x00000000ffff);
		std::vector<Widget*>::iterator it = children.begin();
		int mx = parameter&0x0000ffff;
		int my = (parameter&0xffff0000)>>16;				
		switch (type) {
			case MSG_MOUSE_DOWN: 
				printf("MouseDown: %d, %d        \n", mx,my);
				this->mouse_up = false;
				this->mouse_hold = false;
				this->mouse_down = true;
				WindowManager::redraw = true;
				break;
			case MSG_MOUSE_HOLD: 
				//printf("MouseHold: %d, %d        \n", mx,my);
				this->mouse_up = false;
				this->mouse_down = false;
				this->mouse_hold = true;
				WindowManager::redraw = true;
				break;
			case MSG_MOUSE_UP: 
				this->mouse_up = true;
				this->mouse_down = false;
				this->mouse_hold = false;
				printf("MouseUp: %d, %d          \n", mx,my);
				WindowManager::redraw = true;
				break;
			case MSG_MOUSE_IN:
				printf("MouseIn: %d, %d          \n", mx,my);
				mouse_in  = true;
				mouse_out = false;
				break;
			case MSG_MOUSE_OUT:
				printf("MouseOut: %d, %d          \n", mx,my);
				mouse_out = true;
				mouse_in  = false;
				break;
			default: 
				break;
		}
		// Propagate message to children
		for (; it != children.end(); it++) {
			Widget *w = *it;
			if(w->contains(mx,my)){
				Widget::sendMessage(w, type, parameter);
			}
		}		
	}
	this->messages.clear();
}
