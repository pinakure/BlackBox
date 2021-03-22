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

class TiledMap {
public:
	static PyMethodDef		methods[];
	int						w;
	int						h;
	int						tile_width;
	int						tile_height;
	std::vector<Surface>    tileset;
	std::vector<TiledLayer> layers;
	Surface					*target;
	int						layer_count;

	TiledMap(int w, int h, int layer_count, int tile_width, int tile_height);
	void destroy();
	void draw(int x, int y);
	void update(double delta);
	bool loadTileset(std::string tileset);
	
	static PyObject* pyCreate		(PyObject* self, PyObject* args);
	static PyObject* pyClear		(PyObject* self, PyObject* args);
	static PyObject* pyCount		(PyObject* self, PyObject* args);
	static PyObject* pyDestroy		(PyObject* self, PyObject* args);
	static PyObject* pyDraw			(PyObject* self, PyObject* args);
	static PyObject* pyFill			(PyObject* self, PyObject* args);
	static PyObject* pyGet			(PyObject* self, PyObject* args);
	static PyObject* pyGetData		(PyObject* self, PyObject* args);
	static PyObject* pyLoadTileset	(PyObject* self, PyObject* args);
	static PyObject* pyRedraw		(PyObject* self, PyObject* args);
	static PyObject* pySet			(PyObject* self, PyObject* args);
	static PyObject* pySetData		(PyObject* self, PyObject* args);
	static PyObject* pySetTarget	(PyObject* self, PyObject* args);
	static PyObject* pyUpdate		(PyObject* self, PyObject* args);

};

#endif
