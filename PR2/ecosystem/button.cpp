#include "windowmgr.hpp"
#include "script.hpp"

int Button::button_id = 0;

Button::Button(int id, int x, int y, int width, int height, std::string caption, Callback *callback) {
	this->id= id;
	this->callback = callback;
	this->setPosition(x, y);
	this->setSize(width, height);
	this->setCaption(caption.c_str());
}

void Button::draw() {
	int x = this->x;
	int y = this->y;
	int w = this->width;
	int h = this->height;
	// Draw plate
	Vpu::fillRectangle(x  , y  , w  , h  , colors[2].r, colors[2].g, colors[2].b, colors[2].a);
	Vpu::fillRectangle(x+1, y+1, w-2, h-2, colors[0].r, colors[0].g, colors[0].b, colors[0].a);
	Vpu::fillRectangle(x+1, y+2, w-3, h-3, colors[1].r, colors[1].g, colors[1].b, colors[1].a);
		
	if (this->mouse_hold) {
		Vpu::gradientRectangle(
			x+1  , y+2,
			w-3  , h-3, 
			float(colors[2].r)*1, float(colors[2].g)*.75f, float(colors[2].b)*1, float(colors[2].a),
			float(colors[1].r)*1, float(colors[1].g)*.75f, float(colors[1].b)*1, float(colors[1].a) 
		);		
		Vpu::setColor(al_map_rgba(255,255,64,255));	
	} else {
		Vpu::gradientRectangle(
			x+1  , y+2,
			w-3  , h-3, 
			colors[1].r, colors[1].g, colors[1].b, colors[1].a,
			colors[2].r, colors[2].g, colors[2].b, colors[2].a 
		);
		Vpu::setColor(al_map_rgba(255,255,255,255));	
	}
	
	// Draw text
	Vpu::pushFont();
	Vpu::font = Vpu::smallest_font;
	Vpu::print(this->caption.c_str(), x+(w>>1), y+(h>>1)-(Vpu::font->height>>1), ALLEGRO_ALIGN_CENTER);
	Vpu::popFont();	
	this->drawChildren();
}

void Button::update() {
	if(this->hidden)return;//should receive show message...
	this->handleMessages();
	if (this->mouse_down) {
		if(this->callback){
			if (this->callback->type == CALLBACK_INTEGER) this->callback->trigger(this->id);
			else if (this->callback->type == CALLBACK_STRING) this->callback->trigger(this->caption);
			else this->callback->trigger();
		}
	}
	this->updateChildren();
}
