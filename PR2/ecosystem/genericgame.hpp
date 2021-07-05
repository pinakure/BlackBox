#ifndef __GenericGame_HPP
#define __GenericGame_HPP

#include "Script.hpp"
#include "volumetricmap.hpp"
#include "tiledmap.hpp"
#include "vpu.hpp"

typedef enum e_maptypes {
	MAPTYPE_TILED,
	MAPTYPE_VOLUMETRIC,
	MAPTYPE_RAW
}MapType;

class RawMap {
public:
	int		width  = 0; 
	int		height = 0;
	Surface data;
	RawMap() {}
	RawMap(int width, int height){ this->width = width; this->height = height; this->data = Vpu::createSurface(width, height); }
	~RawMap(){ Vpu::destroySurface(this->data); }
	void draw();
	bool update(double delta);
};

class GenericGame;

class MapHandler {
public:
    bool(*create)(GenericGame*, int, int)	= nullptr;
    void(*draw)(GenericGame*)				= nullptr;
    bool(*update)(GenericGame*, double)		= nullptr;
    void(*destroy)(GenericGame*)			= nullptr;
};

class GenericGame {
private: 
	int width  = 0;
	int height = 0;
	MapType		map_type;
public:
	MapHandler map_handler;
	GenericGame();
	GenericGame(int width, int height, MapType type=MAPTYPE_TILED);
	~GenericGame();
	bool update					(float delta);
	void draw					();
	
	VolumetricMap				*volumetric_map = nullptr;
	TiledMap					*tiled_map		= nullptr;
	RawMap						*raw_map		= nullptr;

	void setType(MapType type);

	static PyObject *pyCreate	(PyObject* self, PyObject* args);
	static PyObject *pyDestroy  (PyObject* self, PyObject* args);
	static PyObject *pyDraw		(PyObject* self, PyObject* args);
	static PyObject *pyUpdate	(PyObject* self, PyObject* args);
	static PyMethodDef			methods[];
};

#endif
