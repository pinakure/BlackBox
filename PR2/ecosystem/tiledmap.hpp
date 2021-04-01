#ifndef __TILEDMAP_C
#define __TILEDMAP_C

#include "script.hpp"
#include "surface.hpp"

class TiledLayer {
public:
	int						w;
	int						h;
	int*					*data;
	bool					redraw;

	TiledLayer(int w = 8, int h = 8);
	~TiledLayer();
	void destroy();
	void createDataRow(int index);
	void set(int x, int y, int tile_index, int tileset_index = 0);
	int  get(int x, int y);
	int  getTile(int x, int y);
	char getTileset(int x, int y);
	void fill(int value);
	int  count(int value);
	void clear() { this->fill(-1); }
};

class Boundaries {
public:
	int left    = 0;
	int right   = 0;
	int bottom  = 0;
	int top		= 0;

	Boundaries(int left=0, int top=0, int right=1, int bottom=1) {
		this->set(left, top, right, bottom);
	}
	void set(int left = 0, int top = 0, int right = 1, int bottom = 1) {
		this->left = left;
		this->right = right;
		this->bottom = bottom;
		this->top = top;
	}
};

class Scroll {
public:
	int x = 0;
	int y = 0;

	Scroll(int x=0, int y=0) {
		this->set(x,y);
	}
	void set(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}
};

class TiledMap {
public:
	static PyMethodDef		methods[];
	int						x = 0;
	int						y = 0;
	int						w;
	int						h;
	int						tile_width;
	int						tile_height;
	std::vector<Surface>    tileset;
	std::vector<TiledLayer> layers;
	Surface					*target;
	int						layer_count;
	Scroll					scroll;
	Boundaries				boundaries;
	

	TiledMap(int w, int h, int layer_count, int tile_width, int tile_height);
	void destroy();
	void draw(int x, int y);
	void update(double delta);
	int loadTileset(std::string tileset);
	void setScroll(int x, int y);
	void setBoundaries(int left, int top, int right, int bottom);
	void redraw();
	
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
	static PyObject* pySetSurface	(PyObject* self, PyObject* args);
	static PyObject* pyScroll		(PyObject* self, PyObject* args);
	static PyObject* pySetTarget	(PyObject* self, PyObject* args);
	static PyObject* pyUpdate		(PyObject* self, PyObject* args);

};

#endif
