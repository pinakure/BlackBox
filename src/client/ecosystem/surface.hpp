#ifndef __SURFACE_HPP
#define __SURFACE_HPP

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

typedef struct s_Surface {
	ALLEGRO_BITMAP *bitmap = NULL;
	bool  enabled = false;
	float rotation[3] = { 0.0f, 0.0f, 0.0f };	
	float scale[3]	  = { 1.0f, 1.0f, 1.0f };	
	int width  = 0;
	int height = 0;
}Surface;

#endif