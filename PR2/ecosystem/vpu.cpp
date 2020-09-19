#include "vpu.hpp"
#include "engine.hpp"

Surface Vpu::overlay[4];
Surface Vpu::background[4];
Surface Vpu::foreground[4];
Surface *Vpu::__layers[12] = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL
};

std::vector<ALLEGRO_COLOR> Vpu::color_stack;
ALLEGRO_BITMAP *Vpu::target = NULL;
ALLEGRO_BITMAP *Vpu::buffer= NULL;
int Vpu::frames = 0;
unsigned long int Vpu::total_frames = 0;
int Vpu::fps = 0;
int Vpu::width = 0;
int Vpu::height = 0;
int Vpu::scroll[2] = { 0, 0 };
bool Vpu::redraw = true;
bool Vpu::is_initialized = false;
ALLEGRO_DISPLAY *Vpu::display = NULL;
ALLEGRO_COLOR Vpu::color;
ALLEGRO_COLOR Vpu::shadow;
bool Vpu::fullscreen = false;
ALLEGRO_COLOR Vpu::transparent;
ALLEGRO_FONT *Vpu::font = NULL;
ALLEGRO_FONT *Vpu::legacy_font = NULL;

void prepareTests() {
	Vpu::foreground[0].enabled = false;
	Vpu::foreground[1].enabled = false;
	Vpu::foreground[2].enabled = false;
	Vpu::foreground[3].enabled = false;
	Vpu::background[1].enabled = false;
	Vpu::background[2].enabled = false;
	Vpu::background[3].enabled = false;		

	Vpu::select(Vpu::background[0]);
	int w = Vpu::background[0].width/2;
	int h = Vpu::background[0].height/2;
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN    , w, h, 64, 200,   0);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN    , w, h, 64,   0, 200);
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN + h, w, h, 64,   0, 200);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN + h, w, h, 64, 200,   0);
	al_lock_bitmap(Vpu::background[0].bitmap,al_get_bitmap_format(Vpu::background[0].bitmap), ALLEGRO_LOCK_WRITEONLY);
	for (int x = 0,xo=w*2; x < xo; x++) {
		for (int y = 0,yo=h*2; y < yo; y++) {
			int c = rand() * 255;
			al_put_pixel(x, y, al_map_rgb(c,c,c));
		}
	}
	al_unlock_bitmap(Vpu::background[0].bitmap);
}

bool Vpu::start() {
		
	width = Engine::width*2;
	height = Engine::height*2;
	al_set_new_display_flags(fullscreen ? ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN : ALLEGRO_OPENGL);
	if (display) {
		al_unregister_event_source(Engine::queue, al_get_display_event_source(display));
		al_destroy_display(display);
		display = NULL;
	}
	display = al_create_display(width/2, height/2);
	if (!display) {
		printf("Cannot initialize %s %dx%d display\n", fullscreen?"fullscreen":"",width/2, height/2);
		return false;
	}
	al_register_event_source(Engine::queue, al_get_display_event_source(display));	
	return true;
}

void Vpu::initializeFonts() {
	font = al_load_ttf_font("data/fonts/small.ttf",16,0 );
	if (!font){
		std::printf("WARNING: Failed to initialize font 'data/fonts/small.ttf'.\nUsing default font.\n");
		font = legacy_font;
	}
}

bool Vpu::restart() {
	for (int i = 0; i < 12; i++) {
		if(__layers[i]){
			if((*__layers[i]).bitmap){
				al_destroy_bitmap((*__layers[i]).bitmap);
				(*__layers[i]).bitmap = NULL;
			}
			__layers[i] = NULL;
		}
	}
	is_initialized = false;
	for (int i = 0; i < 4; i++) {
		overlay[i] = createBitmap((width/2)+(VPU_OVERSCAN*2), (height/2)+(VPU_OVERSCAN*2));
		if(!overlay[i].enabled) return false;
		Vpu::select(overlay[i]);
		Vpu::paint(0, 0, 0, 0);
		__layers[i+8] = &overlay[i];
		foreground[i]= createBitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
		if(!foreground[i].enabled) return false;
		Vpu::select(foreground[i]);
		Vpu::paint(255, 0,255);
		__layers[i+4] = &foreground[i];
		background[i]= createBitmap(width+(VPU_OVERSCAN*2), height+(VPU_OVERSCAN*2));
		if(!background[i].enabled) return false;
		Vpu::select(background[i]);
		Vpu::paint(255, 0,255);			
		__layers[ i ] = &background[i];
		prepareTests();
	}
	// (Re)initialize squash framebuffer
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	buffer = al_create_bitmap(width, height);
	if(!buffer) return false;			

	// (Re)define default colors and reset flags
	transparent = al_map_rgba(0, 0, 0, 0);	
	setColor(255, 255, 255);
	redraw = true;
	is_initialized = true;
	initializeFonts();
	return true;

}

bool Vpu::initialize() {
	try {
		if (!start())return false;		
		if (!al_init_font_addon()) return false;
		if (!al_init_ttf_addon()) return false;
		if (!al_init_primitives_addon())return false;
		legacy_font = al_create_builtin_font();
		font = legacy_font;	
		if (!restart()) return false;
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::paint(int r, int g, int b, int alpha) {
	al_clear_to_color(al_map_rgba(r, g, b, alpha));
}

void Vpu::select(Surface &target) {
	Vpu::target = target.bitmap;
	al_set_target_bitmap(Vpu::target);
}

void Vpu::deinitialize() {
	if(  font ) al_destroy_font(font);
	if(display) al_destroy_display(display);
	for (int i = 0; i < 4; i++) {
		if (overlay[i].bitmap) {
			al_destroy_bitmap(overlay[i].bitmap);
			overlay[i].bitmap = NULL;
		}
		if (foreground[i].bitmap) {
			al_destroy_bitmap(foreground[i].bitmap);
			foreground[i].bitmap = NULL;
		}
		if (background[i].bitmap) {
			al_destroy_bitmap(background[i].bitmap);
			background[i].bitmap = NULL;
		}
	}
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	al_shutdown_primitives_addon();
}

void Vpu::setColor(int r, int g, int b, int alpha) {
	color  = al_map_rgba(r, g, b, alpha);
	shadow = al_map_rgba(r/2, g/2, b/2, alpha);
}

void Vpu::print(std::string text, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, text.c_str());	
	al_draw_textf(font, color , x  , y  , 0, text.c_str());	
}
void Vpu::printInteger(std::string text, int d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%d").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%d").c_str(), d);
}
void Vpu::printFloat(std::string text, float d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%f").c_str(), d);
}
void Vpu::printDouble(std::string text, double d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%f").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%f").c_str(), d);
}
void Vpu::printBool(std::string text,bool d, int  x, int y) {
	al_draw_textf(font, shadow, x+1, y+1, 0, (text + "%b").c_str(), d);
	al_draw_textf(font, color , x  , y  , 0, (text + "%b").c_str(), d);
}

void Vpu::fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		al_map_rgba(r, g, b, alpha)
	);
}
void Vpu::fillSquircle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	Vpu::fillRectangle(x, y, width, height, r, g, b, alpha);
	al_put_pixel(x, y, transparent);
	al_put_pixel(x+width, y, transparent);
	al_put_pixel(x, y+height, transparent);
	al_put_pixel(x+width, y+height, transparent);	
}

void Vpu::circle(int x, int y, float radius, int r, int g, int b, int alpha){
	al_draw_circle(x, y, radius, al_map_rgba(r, g, b, alpha), 1.0f);
}

void Vpu::fillCircle(int x, int y, float radius, int r, int g, int b, int alpha) {
	al_draw_filled_circle(x, y, radius, al_map_rgba(r, g, b, alpha));
}

void Vpu::render() {
	/*overlay[0].rotation[0] += 0.01f;
	overlay[1].rotation[0] += 0.0125f;
	background[0].rotation[0] -= 0.05f;
	background[0].scale[0] += 0.1f;
	background[0].scale[1] += 0.1f;
	if (background[0].scale[0] > 200000.0f)background[0].scale[0] = -200000.0f;
	if (background[0].scale[1] > 200000.0f)background[0].scale[1] = -200000.0f;
	//overlay[0].rotation[0] = 3.141519f; // 180 deg
	//overlay[1].rotation[0] = 3.141519f; // 180 deg
	if(overlay[0].scale[0]>0.0f) overlay[0].scale[0] -= 0.001f;
	if(overlay[0].scale[1]>0.0f) overlay[0].scale[1] -= 0.001f;
	if(overlay[1].scale[0]>0.0f) overlay[1].scale[0] -= 0.00125f;
	if(overlay[1].scale[1]>0.0f) overlay[1].scale[1] -= 0.00125f;
	*/
	al_set_target_bitmap(buffer);
	/* Render enabled layers */
	#define renderlayer(l,i) if(l[i].enabled)							\
								 al_draw_scaled_rotated_bitmap(			\
									l[i].bitmap,						\
									l[i].width/2, l[i].height/2,		\
									l[i].width/4, l[i].height/4,		\
									l[i].scale[0],l[i].scale[1],		\
									l[i].rotation[0],					\
									0									\
								);
		renderlayer(background, 0);
		renderlayer(background, 1);
		renderlayer(background, 2);
		renderlayer(background, 3);
		renderlayer(foreground, 0);
		renderlayer(foreground, 1);
		renderlayer(foreground, 2);
		renderlayer(foreground, 3);
	#undef renderlayer	
	#define renderlayer(l,i) if(l[i].enabled)							\
								 al_draw_scaled_rotated_bitmap(			\
									l[i].bitmap,						\
									l[i].width/2, l[i].height/2,		\
									l[i].width/2, l[i].height/2,		\
									l[i].scale[0],l[i].scale[1],		\
									l[i].rotation[0],					\
									0									\
								);
		renderlayer(overlay, 0);
		renderlayer(overlay, 1);
		renderlayer(overlay, 2);
		renderlayer(overlay, 3);
	#undef renderlayer	
	al_set_target_backbuffer(display);
	al_draw_bitmap(buffer, 0, 0, 0);	
	al_flip_display();
	redraw = false;
}

void Vpu::pushColor() {
	Vpu::color_stack.push_back(color);
	Vpu::color_stack.push_back(shadow);
}

void Vpu::popColor() {
	if (Vpu::color_stack.size()) {
		color  = Vpu::color_stack[0];
		shadow = Vpu::color_stack[1];
		Vpu::color_stack.pop_back();
		Vpu::color_stack.pop_back();
	}
}

Surface Vpu::createBitmap(int width, int height) {
	Surface s;
	s.bitmap = al_create_bitmap(width, height);
	s.enabled = s.bitmap != NULL;
	if (s.enabled) {
		s.width  = width;
		s.height = height;
	}
	return s;
}

Surface Vpu::loadBitmap(std::string filename) {
	Surface s;
	s.bitmap = al_load_bitmap(filename.c_str());	
	s.enabled = s.bitmap != NULL;
	if (s.enabled) {
		s.width  = al_get_bitmap_width(s.bitmap);
		s.height = al_get_bitmap_height(s.bitmap);
	}
	return s;
}