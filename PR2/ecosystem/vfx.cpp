#include "vfx.hpp"
#include "vpu.hpp"
#include "input.hpp"
#include "engine.hpp"
#include <stdio.h>

/* 
# --------------------------------------------------------------------------
#   VFX BASE CLASS
# -------------------------------------------------------------------------- 
*/
Vfx::Vfx(ALLEGRO_BITMAP *target) {
	if(target) {
		initialized = true;
	} 	
}
/* 
# --------------------------------------------------------------------------
#   DUMMY VFX
# -------------------------------------------------------------------------- 
*/
VfxDummy::VfxDummy(ALLEGRO_BITMAP *target) : Vfx(target) {
	this->target = target;
}
bool VfxDummy::initialize() {
	const char * myShaderVertFileName("data/shaders/vert.glsl");
	const char * myShaderFragFileName("data/shaders/dummy_frag.glsl");
	shader = VfxProcessor::create_shader(myShaderVertFileName,myShaderFragFileName);
	return true;
}
inline void VfxDummy::run() {
	al_set_shader_sampler("tex", target, 0);
	float resolution[2] = { VfxProcessor::width,VfxProcessor::height };
	al_set_shader_float_vector("resolution" , 2 , &resolution[0] , 1);

}

/* 
# --------------------------------------------------------------------------
#   ROTOZOOM VFX
# -------------------------------------------------------------------------- 
*/
VfxRotoZoom::VfxRotoZoom(ALLEGRO_BITMAP *target) : Vfx(target) {
	this->target = target;
}
bool VfxRotoZoom::initialize() {
	const char * myShaderVertFileName("data/shaders/vert.glsl");
	const char * myShaderFragFileName("data/shaders/wave_frag.glsl");
	shader = VfxProcessor::create_shader(myShaderVertFileName,myShaderFragFileName);
	return true;
}
inline void VfxRotoZoom::run() {
	static int w;
	static int h;
	w = VfxProcessor::width;
	h = VfxProcessor::height;

	static float radius(0);
    static float maxRadius(400);
	static bool warp = false;
	static float dt = 0.0f;
	static float speed(0);
	static const float max_speed = 10.0f;
	static const float decay = 0.97f;
	static long int ii = 0;
	if ((ii % 2048) == 0) {
		warp = true;
        radius = 0;
        speed = max_speed;
		ii = 0;
	}
	static const float pi = 3.14159265;
	static const float tau  = pi * 2.0f;
	float t = float(Vpu::total_frames) / 60.0f;
	InputDevice::mouse_x = (w/2)+((sin(t))*((w)/2));
	InputDevice::mouse_y = (h/2)+((cos(t))*((t+h)/2));
		

	ii++;
	radius += speed;
	speed *= decay;
    if (speed<0.0f)
    {        
        //radius = 0;
        //speed = 0;
        //warp = false;
    }
	float quad = /*sin(Engine::epoch) / 2.0f + 0.5f;//*/2.0f;
    dt = quad * 3.141519 * (radius/maxRadius);

	al_set_shader_sampler("tex", target, 0);

	int tw = 0;
	int th = 0;
	//al_get_opengl_texture_size(myImage , &tw , &th);

	float dim[2] = {(float)al_get_bitmap_width(target) , (float)al_get_bitmap_height(target)};

	float ms[2] = {(float)InputDevice::mouse_x,(float)(Vpu::height - InputDevice::mouse_y)};

	float r = radius;
            
	al_set_shader_bool("warp" , warp);
	al_set_shader_float("radius", r);
	al_set_shader_float("dt", dt);
	al_set_shader_float_vector("mouse" , 2 , &ms[0] , 1);
	al_set_shader_float_vector("dim", 2, &dim[0], 1);

				
}

/* 
# --------------------------------------------------------------------------
#   VFXPROCESSOR SINGLETON
# -------------------------------------------------------------------------- 
*/
int VfxProcessor::x = 0;
int VfxProcessor::y = 0;
VfxEntry VfxProcessor::stack;
ALLEGRO_BITMAP *VfxProcessor::target = NULL;
int VfxProcessor::width;
int VfxProcessor::height; 
bool VfxProcessor::initialize(ALLEGRO_BITMAP *target) {
	if (!target)return false;
	VfxProcessor::target = target;
	VfxProcessor::width = al_get_bitmap_width(target);
	VfxProcessor::height =al_get_bitmap_height(target); 
	VfxProcessor::stack.vfx = NULL;
	VfxProcessor::stack.next = NULL;
	// return true;
	
	// Now its Initialized, test'em
	//VfxProcessor::addVfx(new VfxRotoZoom(target));
	//VfxProcessor::addVfx(new VfxDummy(target));
	//stack.vfx->initialize();
	
	return true;
}
void VfxProcessor::addVfx(Vfx *obj) {
	if (!stack.vfx) {
		// First entry, no entries yet in stack, so use first 'slot'
		stack.vfx  = obj;
		stack.next = NULL;
	} else {
		VfxEntry *p = &stack;
		while (p->next) {
			p = p->next;
		}
		p->next = new VfxEntry;			
		p->next->vfx = obj;
		p->next->next = NULL;
	}
}

void VfxProcessor::run() {
	VfxEntry *p = &stack;
	//al_lock_bitmap( target, al_get_bitmap_format(target), ALLEGRO_LOCK_READWRITE);	
	if (!p->vfx) {
		al_set_target_backbuffer(Vpu::display);
		al_draw_bitmap(Vpu::buffer, 0, 0, 0);	
		al_flip_display();
		return;
	}
	while(p) {
		// Use shader here before blit buffer to display !!
		al_use_shader(p->vfx->shader?p->vfx->shader:NULL);
				
		p->vfx->run();
		al_set_target_backbuffer(Vpu::display);
		al_draw_bitmap(p->vfx->target, 0,0,0);

		al_use_shader(NULL);
		p = p->next;	
	}
	//al_unlock_bitmap( target);	
	
	al_flip_display();
}

ALLEGRO_SHADER *VfxProcessor::create_shader(const char* fileNameVert, const char* fileNameFrag )
{
    ALLEGRO_SHADER* shader = al_create_shader(ALLEGRO_SHADER_GLSL);

    if(!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, fileNameVert))
    {
        printf("%s\n", al_get_shader_log(shader));
        return NULL;
    }
    if(!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, fileNameFrag))
    {
        printf("%s\n", al_get_shader_log(shader));
        return NULL;
    }
    if(!al_build_shader(shader))
    {
        printf("%s\n", al_get_shader_log(shader));
        return NULL;
    }
    return shader;
}

