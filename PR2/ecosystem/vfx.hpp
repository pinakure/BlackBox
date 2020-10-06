#ifndef __VFX_HPP
#define __VFX_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

class Vfx {
protected:
	bool initialized = false;
public:
	ALLEGRO_BITMAP *target = NULL;
	ALLEGRO_SHADER *shader;
	bool isInitialized() { return this->initialized; };
	virtual void run() = 0;
	virtual bool initialize() =0;
	Vfx(ALLEGRO_BITMAP *target);
};

class VfxRotoZoom : public Vfx{
public:
	void inline run();
	bool initialize();
	VfxRotoZoom(ALLEGRO_BITMAP *target);	
};
class VfxDummy : public Vfx{
public:
	void inline run();
	bool initialize();
	VfxDummy(ALLEGRO_BITMAP *target);	
};


typedef struct s_VfxEntry{
	Vfx *vfx;
	s_VfxEntry *next;
}VfxEntry;

class VfxProcessor {
private:
	//static Vfx *stack[16];
	static VfxEntry stack;
	static ALLEGRO_BITMAP *target;
public:
	static int width;
	static int height; 
	static int x;
	static int y;
	static bool initialize(ALLEGRO_BITMAP *target);
	static void addVfx(Vfx *obj);
	static void run();
	static ALLEGRO_SHADER *create_shader(const char* fileNameVert, const char* fileNameFrag);
};

#endif