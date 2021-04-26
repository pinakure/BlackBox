#include "windowmgr.hpp"

Window::Window(int handle, int x, int y, int width, int height, std::string caption, int wndflags) {
	this->handle = handle;
	this->flags = wndflags;
	this->setPosition(x, y);
	this->setSize(width, height);
	this->setCaption(caption.c_str());	
	this->anchor_nw = Anchor(x, y, 8, 8);
	this->anchor_n = Anchor(x + 8, y, width - 16, 8);
	this->anchor_ne = Anchor(x + width - 8, y, 8, 8);
	this->anchor_w = Anchor(x, y + 8, 8, height - 16);
	this->anchor_e = Anchor(x+width-8, y + 8, 8, height - 16);
	this->anchor_sw = Anchor(x, y + height - 8, 8, 8);
	this->anchor_s = Anchor(x + 8, y + height - 8, width - 16, 8);
	this->anchor_se = Anchor(x + width - 8, y + height - 8, 8, 8);
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
	Vpu::gradientRectangle(
		x+1  , y+2,
		w-3  , h-3, 
		colors[1].r, colors[1].g, colors[1].b, colors[1].a, 
		colors[2].r, colors[2].g, colors[2].b, colors[2].a
	);
	if(this!=WindowManager::hover) 
		Vpu::setColor(colors[3].r, colors[3].g, colors[3].b, colors[3].a);
	else 
		Vpu::setColor(128,0,128,255);
	Vpu::rectangle(this->x, this->y, this->width, this->height);
	this->drawChildren();
	Vpu::popFont();	
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

int Window::drawCaption() {
	int h = Vpu::font->height+2;
	int w = this->width - 2;
	int x = this->x;
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

void Window::update() {
	if(this->hidden)return;//should receive show message...
	this->hover = this == WindowManager::hover;
	this->handleMessages();
	this->updateChildren();
}
