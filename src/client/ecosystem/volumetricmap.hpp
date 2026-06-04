#ifndef __VOLUMETRICMAP_C
#define __VOLUMETRICMAP_C

#include "script.hpp"
#include "surface.hpp"

class VolumetricMap {
public:
	static PyMethodDef		methods[];
	int						x = 0;
	int						y = 0;
	int						w;
	int						h;
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	int						tileset_width = 0x20;
	int						tileset_size;
	Surface				    tileset;
	Surface					*target;
	long long				*data;
	bool					redraw = true;
	int						offset_x=0, offset_y=0;
	//Scroll					scroll;
	//Boundaries				boundaries;
	
	static VolumetricMap* find(int handle);
	VolumetricMap(int w, int h, int tile_width, int tile_height, std::string tileset);
	void destroy();
	void draw(int x, int y);
	bool update(double delta);
	void setScroll(int x, int y);
	void setBoundaries(int left, int top, int right, int bottom);
	void loadData(int width, int height,long int *data);
	void initialize(int width, int height);
	void deinitialize();
	
	static PyObject* pyLoad(PyObject* self, PyObject* args);
	/*
	static PyObject* pyCreate		(PyObject* self, PyObject* args);
	static PyObject* pyClear		(PyObject* self, PyObject* args);
	static PyObject* pyCount		(PyObject* self, PyObject* args);
	static PyObject* pyDestroy		(PyObject* self, PyObject* args);
	static PyObject* pyDraw			(PyObject* self, PyObject* args);
	static PyObject* pyFill			(PyObject* self, PyObject* args);
	static PyObject* pyGet			(PyObject* self, PyObject* args);
	static PyObject* pyGetData		(PyObject* self, PyObject* args);
	static PyObject* pyLoadTileset	(PyObject* self, PyObject* args);
	static PyObject* pyNeedsRedraw	(PyObject* self, PyObject* args);
	static PyObject* pyRedraw		(PyObject* self, PyObject* args);
	static PyObject* pySet			(PyObject* self, PyObject* args);
	static PyObject* pySetActive	(PyObject* self, PyObject* args);
	static PyObject* pySetBoundaries(PyObject* self, PyObject* args);
	static PyObject* pySetData		(PyObject* self, PyObject* args);
	static PyObject* pySetPosition	(PyObject* self, PyObject* args);
	static PyObject* pyGetScroll	(PyObject* self, PyObject* args);
	static PyObject* pySetScroll	(PyObject* self, PyObject* args);
	static PyObject* pySetOffset	(PyObject* self, PyObject* args);
	static PyObject* pySetSurface	(PyObject* self, PyObject* args);
	static PyObject* pyScroll		(PyObject* self, PyObject* args);
	static PyObject* pySetTarget	(PyObject* self, PyObject* args);
	static PyObject* pyUpdate		(PyObject* self, PyObject* args);
	*/
};

#endif
