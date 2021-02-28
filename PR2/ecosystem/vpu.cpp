#include "utils.hpp"
#include "vpu.hpp"
#include "curtain.hpp"
#include "engine.hpp"
#include "console.hpp"
#include "vfx.hpp"

std::map<long int, Surface>			Vpu::surfaces;
std::map<long int, Animation>		Vpu::animations;
std::map<long int, Sprite>			Vpu::sprites;
	
long int							Vpu::animation_handle=0;
long int							Vpu::surface_handle=0;
long int							Vpu::sprite_handle=0;
float								Vpu::fade_target_level=0.0f;
float								Vpu::fade_level=0.0f;
float								Vpu::fade_delta=1.5f;
int									Vpu::fade_color[3] = {0, 0, 0};

int Vpu::pixel_format = 0;

Surface Vpu::console;
Surface Vpu::overlay;
Surface Vpu::background;
Surface Vpu::foreground;

std::vector<ALLEGRO_COLOR> Vpu::color_stack;
std::vector<Font*> Vpu::font_stack;
Surface* Vpu::target = NULL;
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
bool Vpu::ready= true;
ALLEGRO_COLOR Vpu::transparent;
std::vector<Font*> Vpu::fonts;

Font* Vpu::font = NULL;
Font* Vpu::biggest_font = NULL;
Font* Vpu::smallest_font = NULL;
Font* Vpu::legacy_font = NULL;


void prepareTests() {
	Vpu::foreground.enabled = false;
	Vpu::background.enabled = true;		
	Vpu::select(Vpu::background);
	Vpu::subdivide(40, 40, 50, 255);
	//Vpu::randomize();
}

void Vpu::subdivide(int r, int g, int b, int alpha) {
	Vpu::paint(0, 0, 0, 0);
	int w = al_get_bitmap_width(Vpu::target->bitmap)/2;
	int h = al_get_bitmap_height(Vpu::target->bitmap)/2;
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN    , w, h, r		, g		 ,b		 , alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN    , w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN    , VPU_OVERSCAN + h, w, h, r * 0.8, g * 0.8,b * 0.8, alpha);
	Vpu::fillRectangle(VPU_OVERSCAN + w, VPU_OVERSCAN + h, w, h, r		, g		 ,b		 , alpha);
}

void Vpu::randomize() {	
	int w = al_get_bitmap_width( target->bitmap);
	int h = al_get_bitmap_height( target->bitmap );
	lock();
	for (int x = 0,xo=w; x < xo; x++) {
		for (int y = 0,yo=h; y < yo; y++) {
			int c = rand() * 255;
			al_put_pixel(x, y, al_map_rgb(c,c,c));
		}
	}
	unlock();
}

void Vpu::clear() {
	paint(0, 0, 0, 0);
}

void Vpu::clearAll() {
	select(background);
	paint(0, 0, 0, 0);
	select(foreground);
	paint(0, 0, 0, 0);
	select(overlay);
	paint(0, 0, 0, 0);
}

bool Vpu::start() {		
	width = Engine::width;
	height = Engine::height;
	int flags = ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
	al_set_new_display_flags(fullscreen ? flags | ALLEGRO_FULLSCREEN : flags);
	if (display) {
		al_unregister_event_source(Engine::queue, al_get_display_event_source(display));
		al_destroy_display(display);
		display = NULL;
	}
	display = al_create_display(width, height);
	if (!display) {
		Engine::printf("Cannot initialize %s %dx%d display\n", fullscreen?"fullscreen":"",width, height);
		return false;
	}
	al_register_event_source(Engine::queue, al_get_display_event_source(display));	
	return true;
}

void Vpu::initializeFonts() {
	ALLEGRO_FS_ENTRY* e = al_create_fs_entry("fonts/");
	if (al_open_directory(e)) {
		ALLEGRO_FS_ENTRY* file;
		while (file = al_read_directory(e)){
			std::string name = al_get_fs_entry_name(file);
			std::vector<std::string> temp  = explode(name, '\\');
			if(temp.size()>0)
				name = temp[temp.size() - 1];
			// Remove slashes
			std::vector<std::string> parts = explode(name, '/');
			if (parts.size() > 0) {
				name = parts[parts.size() - 1];
			}
			// Extract extension
			parts = explode(name, '.');
			al_destroy_fs_entry(file);
			if (parts.size() == 2) {
				if (!parts[1].compare("ttf")) {
					fonts.push_back(new Font(parts[0], 16));					
				}
			} else if (parts.size() == 3) {
				if (!parts[2].compare("ttf")) {
					int i = atoi(parts[1].c_str());
					fonts.push_back(new Font(parts[0] + "." + parts[1], i));
				}
			};
			// Reassign smallest and biggest font pointers 
			if (fonts.size()) {
				Font* last = fonts[fonts.size() - 1];
				if (!biggest_font)biggest_font = last;
				else if (biggest_font->height < last->height) biggest_font = last;
				if (!smallest_font)smallest_font = last;
				else if (smallest_font->height > last->height) smallest_font = last;
			}
		}
	}
	for (int i = 0; i < fonts.size(); i++) {
		font = fonts[i];
		if (font)return;
	}
	Engine::printf("WARNING: Failed to initialize system font.\nUsing default font.\n");
	font = legacy_font;	
	if (!biggest_font) biggest_font = legacy_font;
	if (!smallest_font) smallest_font = legacy_font;
}

bool Vpu::restart() {
	is_initialized = false;

	// overlay 
	overlay = createSurface(width, height);
	if(!overlay.enabled) return false;
	select(overlay);
	clear();		
	
	// foreground
	foreground = createSurface(width*2, height*2);
	if(!foreground.enabled) return false;
	select(foreground);
	clear();

	// background
	background= createSurface( width*2 , height*2);
	if(!background.enabled) return false;
	select(background);
	clear();
	
	// TODO: remove on production
	prepareTests();

	// (Re)initialize squash framebuffer
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	buffer = al_create_bitmap(width, height);
	if(!buffer) return false;			
	
	VfxProcessor::initialize(buffer);
	
	// Get pixel format to accelerate lock calls
	pixel_format = al_get_bitmap_format(buffer);

	// (Re)initialize console framebuffer
	destroySurface(console);
	destroySurface(Console::bitmap);	// This will be dinamically created at paint time by console if null
	destroySurface(Console::backdrop);	// This will be dinamically created at paint time by console if null
	console = createSurface(width, height);
	if(!console.enabled) return false;	
	Vpu::select(console);
	Vpu::paint(0, 0, 0, 0);

	// (Re)define default colors and reset flags
	transparent = al_map_rgba(0, 0, 0, 0);	
	setColor(255, 255, 255);
	redraw = true;
	is_initialized = true;
	al_set_window_title(display, "BlackBox v.3");		
	Curtain::render();
	return true;
}

bool Vpu::initialize() {
	try {
		if (!start())return false;		
		if (!al_init_font_addon()) return false;
		if (!al_init_ttf_addon()) return false;
		if (!al_init_primitives_addon())return false;
		if (!al_init_image_addon())return false;		
		legacy_font = new Font("legacy");
		font = legacy_font;	
		if (!restart()) return false;
		initializeFonts();
		ALLEGRO_BITMAP *icon;
		icon = al_load_bitmap("gfx/vendor.png");
		if (icon) {
			al_set_display_icon(display, icon);
		}
		return true;
	} catch (int e) {
		e = e;
		return false;
	}
}

void Vpu::paint(int r, int g, int b, int a) {
	r = r >= 0 ? (r&255) : color.r * 255;
	g = g >= 0 ? (g&255) : color.g * 255;
	b = b >= 0 ? (b&255) : color.b * 255;
	a = a >= 0 ? (a&255) : color.a * 255;
	al_clear_to_color(al_map_rgba(r, g, b, a));
}

void Vpu::select(Surface &target) {
	Vpu::target = &target;
	al_set_target_bitmap(Vpu::target->bitmap);
}

void Vpu::deinitialize() {
	font = legacy_font;
	for (int i = 0; i < fonts.size(); i++) {
		delete fonts[i];
	}
	fonts.clear();
	if(display) al_destroy_display(display);
	destroySurface(console);
	for (int i = 0; i < 4; i++) {
		destroySurface(overlay);
		destroySurface(foreground);
		destroySurface(background);		
	}
	if (buffer) {
		al_destroy_bitmap(buffer);
		buffer = NULL;
	}
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
		
}

#define ENSURETARGET 	if (!target) {														\
							std::printf("Error: Cannot scale target : No target selected");	\
							return false;													\
						};

#define XSCALE Vpu::width
#define YSCALE Vpu::height
//#define ZSCALE Vpu::depth

bool Vpu::setRotation(float sa) {
	ENSURETARGET
	std::printf("Vpu::setRotation(%f)\n", sa);
	target->rotation[0] = sa;
	return true;
}

bool Vpu::rotate(float sa) {
	ENSURETARGET
	return Vpu::setRotation(target->rotation[0] + sa);
}

bool Vpu::setScale(float sx, float sy, float sz) {
	ENSURETARGET
	if (target->width  * sx < XSCALE)return false;
	if (target->height * sy < YSCALE)return false;
	std::printf("Vpu::setScale(%f,%f,%f)\n", sx, sy, sz);
	//if (target.depth * sz < ZDEPTH)return false;
	target->scale[0] = sx;
	target->scale[1] = sy;
	target->scale[2] = sz;
	return true;
}

bool Vpu::scale(float sx, float sy, float sz) {
	ENSURETARGET
	if (target->width  * (target->scale[0]+sx) < XSCALE)return false;
	if (target->height * (target->scale[1]+sy) < YSCALE)return false;
	//if (target.depth * (target->scale[2]+sz) < ZSCALE)return false;
	return setScale(target->scale[0] + sx, target->scale[1] + sy, target->scale[2] + sz);
}

void Vpu::setColor(int r, int g, int b, int alpha) {
	color  = al_map_rgba(r, g, b, alpha);
	shadow = Vpu::alter(color, 0.5f, 0.5f, 0.5f);
}

void Vpu::setColor(ALLEGRO_COLOR _color) {
	Vpu::color  = _color;
	Vpu::shadow = Vpu::alter(_color, 0.5f, 0.5f, 0.5f);
}

ALLEGRO_COLOR Vpu::alter(ALLEGRO_COLOR _color, float qr, float qg, float qb, float qa) {
	ALLEGRO_COLOR ret = _color; 
	ret.r *= qr;
	ret.g *= qg;
	ret.b *= qb;
	ret.a *= qa;
	return ret;
}

static char _buffer[65535];


void Vpu::printf(int  x, int y, int flags, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(_buffer, 2048, fmt, ap);
	va_end(ap);

	al_draw_textf(font->data, shadow, x + 1, y + 1, flags, _buffer);
	al_draw_textf(font->data, color, x, y, flags, _buffer);
}

void Vpu::print(std::string text, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, text.c_str());
	al_draw_textf(font->data, color , x  , y  , flags, text.c_str());
}
void Vpu::printInteger(std::string text, int d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%d").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%d").c_str(), d);
}
void Vpu::printFloat(std::string text, float d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printDouble(std::string text, double d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%f").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%f").c_str(), d);
}
void Vpu::printBool(std::string text,bool d, int  x, int y, int flags) {
	al_draw_textf(font->data, shadow, x+1, y+1, flags, (text + "%b").c_str(), d);
	al_draw_textf(font->data, color , x  , y  , flags, (text + "%b").c_str(), d);
}

void Vpu::fillRectangle(int x, int y, int width, int height, int r, int g, int b, int alpha) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		al_map_rgba(r, g, b, alpha)
	);
}

void Vpu::qfillRectangle(int x, int y, int width, int height) {
	al_draw_filled_rectangle(
		x, y,
		x + width, y + height,
		color
	);
}

void Vpu::rectangle(int x, int y, int width, int height) {
	al_draw_rectangle(
		x, y,
		x + width, y + height,
		color, 1.0f
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
	
	al_set_target_bitmap(buffer);
	/* Render enabled layers */
	
	#define renderlayer(l) {															\
								float OFFSETX=(-(Vpu::width/2)*l.scale[0] ) + ( (Vpu::width/2)  * ( l.scale[0] - 1.0f ) );	\
								float OFFSETY=(-(Vpu::height/2)*l.scale[1] ) + ( (Vpu::height/2) * ( l.scale[1] - 1.0f ) );	\
								if(l.enabled)											\
								 al_draw_scaled_rotated_bitmap(							\
									l.bitmap,											\
									l.width/2, l.height/2,								\
									Vpu::width + OFFSETX, Vpu::height + OFFSETY,		\
									l.scale[0],l.scale[1],								\
									l.rotation[0],										\
									0													\
								);														\
							}
		renderlayer(background);
		renderlayer(foreground);
	#undef renderlayer	
	
	Curtain::draw();
	
	al_draw_scaled_rotated_bitmap(
		overlay.bitmap,							
		overlay.width/2, overlay.height/2,	
		overlay.width/2, overlay.height,	
		overlay.scale[0]*2.0f,overlay.scale[1]*2.0f,	
		overlay.rotation[0],						
		0											
	);
	
	// Use fade if proceed
	if(fade_level != fade_target_level)
		fade_level += fade_delta;
	// Stop after reaching target level
	if((fade_target_level > 0.0f && fade_level > fade_target_level) 
	|| (fade_target_level < 1.0f && fade_level < fade_target_level))
		fade_level = fade_target_level;
	
	
	al_set_target_bitmap(buffer);
	al_draw_filled_rectangle(0, 0, width, height, al_map_rgba(fade_color[0], fade_color[1], fade_color[2], 255*fade_level));
	
	if (console.enabled)
		al_draw_scaled_bitmap(console.bitmap, 0, 0, width, height, 0, 0, al_get_display_width(display), al_get_display_height(display), 0);

	VfxProcessor::run();
	
	redraw = false;
}


void Vpu::pushFont() {
	Vpu::font_stack.push_back(font);
}
void Vpu::popFont() {
	if (Vpu::font_stack.size()) {
		font = Vpu::font_stack[0];
		Vpu::font_stack.pop_back();
	}
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
	}
}

void Vpu::putpixel(int x, int y) {
	al_put_pixel(x, y, color);
}

void Vpu::line(int x, int y, int dx, int dy) {
	al_draw_line(x, y, dx, dy, color, 1.0f);
}

Sprite &Vpu::destroySprite(Sprite &sprite) {
	return sprite;
}

Animation &Vpu::destroyAnimation(Animation &animation) {
	return animation;
}

Surface &Vpu::destroySurface(Surface &surface) {
	if (surface.bitmap) al_destroy_bitmap(surface.bitmap);
	surface.bitmap = NULL;
	surface.width = 0;
	surface.height = 0;
	surface.enabled = false;
	surface.rotation[0] = 0;
	surface.rotation[1] = 0;
	surface.rotation[2] = 0;
	surface.scale[0] = 0;
	surface.scale[1] = 0;
	surface.scale[2] = 0;
	return surface;
}

Font* Vpu::getFontByName(std::string name) {
	std::vector<Font*>::iterator it;
	for (it = fonts.begin(); it != fonts.end(); it++) {
		if (!(*it)->name.compare(name))return *it;
	}
	return NULL;
}

void Vpu::drawSurface(Surface &surface,float sx, float sy, float sw, float sh, float dx, float dy) {
	al_draw_bitmap_region(surface.bitmap, sx, sy, sw, sh, dx, dy, 0);
	//al_draw_bitmap(surface.bitmap, 0, 0, 0);
}

void Vpu::drawSprite(Sprite &sprite, float dx, float dy) {
	sprite.draw(dx, dy);	
}

void Vpu::drawAnimation(Animation &animation, float dx, float dy) {
	animation.run(1.0);
	animation.qdraw(dx, dy);	
}

Surface Vpu::createSurface(int width, int height) {
	Surface s;
	s.bitmap = al_create_bitmap(width, height);
	s.enabled = !(s.bitmap == NULL);
	if (s.enabled) {
		s.width  = width;
		s.height = height;
	}
	return s;
}

Surface Vpu::loadBitmap(std::string filename) {
	Surface s;
	s.bitmap = al_load_bitmap((filename).c_str());	
	s.enabled = s.bitmap != NULL;
	if (s.enabled) {
		s.width  = al_get_bitmap_width(s.bitmap);
		s.height = al_get_bitmap_height(s.bitmap);
	}
	return s;
}

void Vpu::fadeout(int r, int g, int b) {
	fade_color[0] = r>0?r:fade_color[0];
	fade_color[1] = g>0?g:fade_color[1];
	fade_color[2] = b>0?b:fade_color[2];
	fade_level = 0.0f;
	fade_target_level = 1.0f;
	fade_delta = 1.0f / 120.0f;
}

void Vpu::fadein(int r, int g, int b) {
	fade_color[0] = r>0?r:fade_color[0];
	fade_color[1] = g>0?g:fade_color[1];
	fade_color[2] = b>0?b:fade_color[2];
	fade_level = 1.0f;
	fade_target_level = 0.0f;
	fade_delta = -(1.0f / 120.0f);
}

void Vpu::lock() {
	assert(target);
	assert(target->bitmap);
	//al_lock_bitmap( target->bitmap ,al_get_bitmap_format( target->bitmap ), ALLEGRO_LOCK_WRITEONLY);	
	al_lock_bitmap( target->bitmap , pixel_format , ALLEGRO_LOCK_WRITEONLY);	
}

void Vpu::unlock() {
	assert(target);
	assert(target->bitmap);
	al_unlock_bitmap(target->bitmap);
}
	
void Vpu::loadVars() {

}

Sprite Vpu::createSprite(std::string filename) {
	return Sprite(filename.c_str());
}

long int Vpu::createSubSprite(Sprite& s, int left, int top, int right, int bottom) {
	sprite_handle++;
	sprites.insert(std::pair<long int, Sprite>(sprite_handle, Sprite(s, left, top, right, bottom)));
	return sprite_handle;	
}

Animation Vpu::createAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical) {
	return Animation(s, width, height, sx, sy, dx, dy, vertical);	
}
	
long int Vpu::allocateSprite(std::string filename, int priority){
	#define SPRITE_PRIORITY_RANGE 1024
	sprite_handle++;
	sprite_handle += priority * SPRITE_PRIORITY_RANGE;
	sprites.insert( std::pair<long int, Sprite>(sprite_handle, createSprite(filename)) );
	sprite_handle -= priority * SPRITE_PRIORITY_RANGE;
	return sprite_handle + (priority * SPRITE_PRIORITY_RANGE);
	#undef SPRITE_PRIORITY_RANGE
}

void Vpu::deallocateSprite(long int handle) {
	if (sprites.empty())return;
	if (sprites.find(handle) != sprites.end()) {
		destroySprite(sprites.at(handle));
		sprites.erase(handle);
	}
}

long int Vpu::allocateAnimation(int width, int height, Sprite &s, int sx, int sy, int dx, int dy, bool vertical) {
	animation_handle++;
	animations.insert( std::pair<long int, Animation>(animation_handle, createAnimation(width, height, s,sx,sy,dx,dy, vertical)) );
	return animation_handle;
}

void Vpu::deallocateAnimation(long int handle) {
	if (animations.empty())return;
	if (animations.find(handle) != animations.end()) {
		destroyAnimation(animations.at(handle));
		animations.erase(handle);
	}
}

long int Vpu::allocateSurface(int width, int height) {
	surface_handle++;
	surfaces.insert( std::pair<long int, Surface>(surface_handle, createSurface(width, height)) );
	return surface_handle;
}

void Vpu::deallocateSurface(long int handle) {
	if (surfaces.empty())return;
	if (surfaces.find(handle) != surfaces.end()) {
		destroySurface(surfaces.at(handle));
		surfaces.erase(handle);
	}
}

void Vpu::setFont(Font *fnt) {
	Vpu::font = fnt;
}