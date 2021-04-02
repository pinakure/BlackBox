#include "tiledmap.hpp"
#include "vpu.hpp"
#include "direction.hpp"

PyMethodDef TiledMap::methods[] = {
	{"create"		, TiledMap::pyCreate		, METH_VARARGS, "create(width,height,layer_count=1,tile_width=8,tile_height=8) : " },
	{"clear"		, TiledMap::pyClear			, METH_VARARGS, "clear(handle,layer_index=-1) : " },
	{"count"		, TiledMap::pyCount			, METH_VARARGS, "count(handle,value,layer_index=-1) : " },
	{"destroy"		, TiledMap::pyDestroy		, METH_VARARGS, "destroy(tiledmap_handle) : " },
	{"draw"			, TiledMap::pyDraw			, METH_VARARGS, "destroy(handle,x=0,y=0) : " },
	{"fill"			, TiledMap::pyFill			, METH_VARARGS, "set(handle,tile_index,layer_index=-1) : " },
	{"get"			, TiledMap::pyGet			, METH_VARARGS, "get(handle,x,y,layer_index=0) : " },
	{"getscroll"	, TiledMap::pyGetScroll		, METH_VARARGS, "[x, y] = getscroll(handle): " },
	{"getdata"		, TiledMap::pyGetData		, METH_VARARGS, "getdata(handle,layer_index=0) : " },
	{"loadtileset"	, TiledMap::pyLoadTileset	, METH_VARARGS, "loadtileset(handle,tileset_filename) : " },
	{"redraw"		, TiledMap::pyRedraw		, METH_VARARGS, "redraw(handle,layer_index=-1) : " },
	{"needsredraw"	, TiledMap::pyNeedsRedraw	, METH_VARARGS, "needsredraw(handle) : " },
	{"set"			, TiledMap::pySet			, METH_VARARGS, "set(handle,x,y,value,layer_index=0) : " },
	{"setdata"		, TiledMap::pySetData		, METH_VARARGS, "setdata(handle,value,layer_index=0) : " },
	{"setactive"	, TiledMap::pySetActive		, METH_VARARGS, "setactive(handle, disable) : " },
	{"setsurface"	, TiledMap::pySetSurface	, METH_VARARGS, "setsurface(tiledmap_handle,surface_handle) : " },
	{"settarget"	, TiledMap::pySetTarget		, METH_VARARGS, "settarget(tiledmap_handle,surface_handle) : " },
	{"setboundaries", TiledMap::pySetBoundaries , METH_VARARGS, "setboundaries(tiledmap_handle,left, top, right, bottom) : " },
	{"setscroll"	, TiledMap::pySetScroll		, METH_VARARGS, "setscroll(tiledmap_handle,x, y) : " },
	{"setposition"	, TiledMap::pySetPosition	, METH_VARARGS, "setposition(tiledmap_handle,x, y) : " },
	{"scroll"		, TiledMap::pyScroll		, METH_VARARGS, "scroll(tiledmap_handle, delta, direction) : " },
	{"update"		, TiledMap::pyUpdate		, METH_VARARGS, "update(handle,delta=1.0) : " },
	{NULL, NULL, 0, NULL}
};

void TiledMap::destroy() {
	// Destroy layers
	std::vector<TiledLayer>::iterator layer = this->layers.begin();
	while (layer != this->layers.end()) {
		layer->destroy();
		layer++;
	}
	this->layers.clear();
	// Destroy tilesets
}

TiledMap::TiledMap(int w, int h, int layer_count, int tile_width, int tile_height) {
	this->w = w;
	this->h = h;
	this->layer_count = layer_count;
	this->tile_height = tile_height;
	this->tile_width  = tile_width;
	this->tileset	  = std::vector<Surface>();
	this->target	  = &Vpu::background;
	while (this->layers.size() < layer_count) {
		this->layers.push_back(TiledLayer(w, h));
	}
}

void TiledMap::draw(int ix, int iy) {
	int v = 0;
	std::vector<TiledLayer>::iterator layer = this->layers.begin();
	int dy = iy	, dx = ix;
	int tile_index, tileset_index;
	ALLEGRO_COLOR grid = al_map_rgba(0, 64, 255, 128);
	ALLEGRO_COLOR text = al_map_rgba(192,192,192,255);
	
	Vpu::select(*this->target);
	//al_lock_bitmap(this->target->bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
	Surface* ts = this->tileset.size() == 0 ? nullptr : &this->tileset[0];
	int tileset_width = ts->width / this->tile_width;
	int tileset_height = ts->height / this->tile_height;
	int tileset_size = tileset_width * tileset_height;
	while (layer != this->layers.end()) {
		dy = iy;
		if(layer->redraw)
			for (int y = 0; y < this->h; y++) {
			dx = ix;
			int* p = layer->data[y];
			for (int x = 0; x < this->w; x++) {
				if (!ts){
					/*Vpu::fillRectangle(
						dx, dy,
						this->tile_width, this->tile_height,
						Vpu::color.r * 255.0f,
						Vpu::color.g * 255.0f,
						Vpu::color.b * 255.0f,
						Vpu::color.a * 255.0f
					); */
					Vpu::color = grid;
					Vpu::rectangle(
						dx, dy,
						this->tile_width, this->tile_height
					);
					Vpu::color = text;
					Vpu::printf(
						dx + (this->tile_width >> 1),
						dy - (this->tile_height >> 1),
						ALLEGRO_ALIGN_CENTER,
						tile_index > 0 ? " " : "#",
						tile_index > 0 ? 1 : 8
					);
				} else {
					v = *p;
					tile_index = (v & 0x00ffffff) % tileset_size;
					tileset_index = ((v & 0xff000000) >> 24) % this->tileset.size();
					int oy = int(tile_index / tileset_width) * this->tile_height;
					int ox = (tile_index % tileset_width) * this->tile_width;
					Vpu::drawSurface(
						*ts,
						ox, oy,
						this->tile_width, this->tile_height,
						dx, dy
					);
					
				}
				dx += this->tile_width;
				p++;
			}
			dy += this->tile_height;
		}
		layer->redraw = false;
		layer++;
	}
	//al_unlock_bitmap(this->target->bitmap);
}

void TiledMap::update(double delta) {

}

int TiledMap::loadTileset(std::string filename) {
	Surface s = Vpu::loadBitmap("tilesets/" + filename+".png");
	if (s.enabled) {
		this->tileset.push_back(s);
		return (int)this->tileset.size();
	}
	return -1;
}


PyObject* TiledMap::pyCreate(PyObject* self, PyObject* args) {
	int width;
	int height;
	int layercount = 1;
	int tilewidth  = 8;
	int tileheight = 8;
	if (!PyArg_ParseTuple(args, "ii|iii", &width, &height, &layercount, &tilewidth, &tileheight)) return NULL;
	long int handle = Vpu::allocateTiledMap(width, height, layercount, tilewidth, tileheight);
	return PyLong_FromLong(handle);
}

PyObject* TiledMap::pyDestroy(PyObject* self, PyObject* args) {
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		Vpu::deallocateTiledMap(handle);
		return True;
	}
	printf("ERROR @ pyDestroy : tiledmap_handle out of range\n");
	return False; 	
}

PyObject* TiledMap::pyDraw(PyObject* self, PyObject* args) {
	int handle;
	float x = -9999;
	float y = -9999;
	if (!PyArg_ParseTuple(args, "i|ff", &handle, &x, &y)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		x = x > -9999 ? x : tm->x;
		y = y > -9999 ? y : tm->y;
		tm->draw(x-tm->scroll.x, y - tm->scroll.y);
		return True;
	}
	printf("ERROR @ pyDraw : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyUpdate(PyObject* self, PyObject* args) {
	int handle;
	float delta=1.0f;
	if (!PyArg_ParseTuple(args, "i|f", &handle, &delta)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		tm->update(delta);
		return True;
	}
	printf("ERROR @ pyUpdate : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyClear(PyObject* self, PyObject* args) {
	int handle;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "i|i", &handle, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			tm->layers[i].clear();
		}
		else {
			tm->layers[layer].clear();
		}
		return True;
	}
	printf("ERROR @ pyClear : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyCount(PyObject* self, PyObject* args) {
	int handle;
	int value;
	int layer = -1;
	
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &value, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		int count = 0;
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			count += tm->layers[i].count(value);
		} else {
			count += tm->layers[layer].count(value);
		}
		return PyLong_FromLong(count);
	}
	printf("ERROR @ pyFill : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyFill(PyObject* self, PyObject* args) {
	int handle;
	int value;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &value, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			tm->layers[i].fill(value);
		} else {
			tm->layers[layer].fill(value);
		}
		return True;
	}
	printf("ERROR @ pyFill : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySet(PyObject* self, PyObject* args) {
	int handle;
	int x;
	int y;
	int value;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "iiii|i", &handle, &x, &y, &value, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		if (y < 0) return False;
		else if (x < 0) return False;
		else if (x >= tl->w) return False;
		else if (y >= tl->h) return False;
		tl->data[y][x] = value;
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

extern Surface* getLayer(int index);

PyObject* TiledMap::pySetTarget(PyObject* self, PyObject* args) {
	int handle;
	int target;
	if (!PyArg_ParseTuple(args, "ii", &handle, &target)) return NULL;
	Surface* tgt = getLayer(target);
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		tm->target = tgt;
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyScroll(PyObject* self, PyObject* args) {
	int handle;
	float delta;
	int direction;
	if (!PyArg_ParseTuple(args, "ifi", &handle, &delta, &direction)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		int x = tm->scroll.x;
		int y = tm->scroll.y;
		// see direction.py
		switch (direction) {
			case 0: tm->setScroll(x, y - delta); break;
			case 2: tm->setScroll(x, y + delta); break;
			case 3: tm->setScroll(x - delta, y); break;
			case 1: tm->setScroll(x + delta, y); break;
		}		
		printf("SCROLL: %d,%d   DELTA:%f  \r", tm->scroll.x, tm->scroll.y, delta);
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetScroll(PyObject* self, PyObject* args) {
	int handle;
	int x;
	int y;
	if (!PyArg_ParseTuple(args, "iii", &handle, &x, &y)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		tm->setScroll(x, y);
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetSurface(PyObject* self, PyObject* args) {
	int handle;
	int surface;
	if (!PyArg_ParseTuple(args, "ii", &handle, &surface)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (surface >= 0) {
			Surface* srf = getLayer(surface);
			tm->target = srf;
		}
		else {
			tm->target = nullptr;
		}
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetBoundaries(PyObject* self, PyObject* args) {
	int handle;
	int left;
	int top;
	int right;
	int bottom;
	if (!PyArg_ParseTuple(args, "iiiii", &handle, &left, &top, &right, &bottom)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		tm->setBoundaries(left, top, right, bottom);
		return True;
	}
	printf("ERROR @ pySet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetData(PyObject* self, PyObject* args) {
	int handle;
	char* data = nullptr;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &data, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		if (strlen(data) % 4 == 0) {
			for (int i = 0, x = 0, y = 0; i < strlen(data); i += 4) {
				tl->data[y][x] = (int(data[i]) << 24)
					| (int(data[i + 1]) << 16)
					| (int(data[i + 2]) << 8)
					| (int(data[i + 3]))
					;
				if (y == tl->h) break;
				if (x == tl->w) {
					x = 0;
					y++;
				}
				else x++;
			}
		}
		else {
			printf("ERROR @ pySetData : data.size() not multiple of 4\n");
			return False;
		}
		return True;
	}
	printf("ERROR @ pySetData : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetActive(PyObject* self, PyObject* args) {
	int handle;
	bool disable=false;
	if (!PyArg_ParseTuple(args, "i|b", &handle, &disable)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (disable)  
			Vpu::active_map = nullptr; 
		else Vpu::active_map = tm;
		return True;
	}
	printf("ERROR @ pySetData : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyGet(PyObject* self, PyObject* args) {
	int handle;
	unsigned int x;
	unsigned int y;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "iii|i", &handle, &x, &y, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		y %= tl->h;
		x %= tl->w;
		return PyLong_FromLong(tl->data[y][x]);
	}
	printf("ERROR @ pyGet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pySetPosition(PyObject* self, PyObject* args) {
	int handle;
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "iff", &handle, &x, &y)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		tm->x = x;
		tm->y = y;
		return True;
	}
	printf("ERROR @ pyGet : tiledmap_handle out of range\n");
	return False;
}


PyObject* TiledMap::pyGetScroll(PyObject* self, PyObject* args) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		PyObject* list = PyList_New(2);
		if (!list) throw("Unable to allocate memory for Python list");
		PyList_SET_ITEM(list, 0, PyFloat_FromDouble((double)tm->scroll.x));
		PyList_SET_ITEM(list, 1, PyFloat_FromDouble((double)tm->scroll.y));
		return list;
	}
	printf("ERROR @ pyGet : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyGetData(PyObject* self, PyObject* args) {
	int handle;
	std::string data = "";
	int layer = 0;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &data, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		const int data_size = tl->w * tl->h;
		for (int i = 0, x = 0, y = 0; i < data_size; i += 4) {
			data += (char((tl->data[y][x] & 0xFF000000) >> 24	));
			data += (char((tl->data[y][x] & 0x00FF0000) >> 16	));
			data += (char((tl->data[y][x] & 0x0000FF00) >> 8	));
			data += (char((tl->data[y][x] & 0x000000FF)			));
			if (y == tl->h) break;
			if (x == tl->w) {
				x = 0;
				y++;
			} else x++;
		}
		return Py_BuildValue("s", data.
			c_str());
	}
	printf("ERROR @ pyGetData : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyLoadTileset(PyObject* self, PyObject* args) {
	int handle;
	char *name;
	if (!PyArg_ParseTuple(args, "is", &handle, &name)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		return PyLong_FromLong(tm->loadTileset(name));
	}
	printf("ERROR @ pyRedraw : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyRedraw(PyObject* self, PyObject* args) {
	int handle;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "i|i", &handle, &layer)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			tm->layers[i].redraw = true;
		}
		else {
			layer %= tm->layer_count;
			TiledLayer* tl = &tm->layers[layer];
			tl->redraw = true;
		}
		return True;
	}
	printf("ERROR @ pyRedraw : tiledmap_handle out of range\n");
	return False;
}

PyObject* TiledMap::pyNeedsRedraw(PyObject* self, PyObject* args) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::tiledmaps.find(handle) != Vpu::tiledmaps.end()) {
		TiledMap* tm = &Vpu::tiledmaps.at(handle);
		for (int i = 0; i < tm->layers.size(); i++) {
			if (tm->layers[i].redraw) return True;
		}
		return False;
	}
	printf("ERROR @ pyRedraw : tiledmap_handle out of range\n");
	return False;
}

void TiledMap::setScroll(int x, int y) {
	this->scroll.set(x, y);
	if (this->scroll.y >= this->boundaries.bottom) this->scroll.y = this->boundaries.bottom - 1;
	if (this->scroll.x >= this->boundaries.right ) this->scroll.x = this->boundaries.right - 1;
	if (this->scroll.x <  this->boundaries.left  ) this->scroll.x = this->boundaries.left;
	if (this->scroll.y <  this->boundaries.top   ) this->scroll.y = this->boundaries.top;	
	this->redraw();
}

void TiledMap::redraw() {
	for (int i = 0; i < this->layers.size(); i++) {
		this->layers[i].redraw = true;
	}
}

void TiledMap::setBoundaries(int left, int top, int right, int bottom) {
	this->boundaries.set(left, top, right, bottom);
}

TiledLayer::TiledLayer(int w, int h) {
	this->w		= w;
	this->h		= h;
	this->data	= (int**)malloc(sizeof(int*)* this->h);
	this->redraw = true;
	for (int i = 0; i < h; i++) {
		this->createDataRow(i);
	}
}

TiledLayer::~TiledLayer() {
	
}

void TiledLayer::destroy() {
	for (int i = 0; i < h; i++) {
		free(this->data[i]);
	}
	free(this->data);
	this->data = nullptr;
}

void TiledLayer::createDataRow(int index) {
	this->data[index] = (int*)malloc(sizeof(int)* this->w);
	memset(this->data[index], 0, sizeof(int)*this->w);
}

void TiledLayer::set(int x, int y, int tile_index, int tileset_index) {
	this->data[y][x] = ((tileset_index & 0xFF) << 24) | (tile_index & 0x00FFFFFF);
}

int TiledLayer::get(int x, int y) {
	return this->data[y][x];
}

int TiledLayer::getTile(int x, int y) {
	return this->data[y][x] & 0x00FFFFFF;
}

char TiledLayer::getTileset(int x, int y) {
	return (this->data[y][x] & 0xFF000000) >> 24;
}

void TiledLayer::fill(int value) {
	for (int y = 0; y < this->h; y++) {
		for (int x = 0; x < this->w; x++) {
			this->data[y][x] = value;
		}
	}
}

int TiledLayer::count(int value) {
	int count = 0;
	for (int y = 0; y < this->h; y++) {
		for (int x = 0; x < this->w; x++) {
			count += this->data[y][x] == value ? 1 : 0;
		}
	}
	return count;
}

