#include "volumetricmap.hpp"
#include "vpu.hpp"
#include "direction.hpp"

PyMethodDef VolumetricMap::methods[] = {
	/*
	{"create"		, VolumetricMap::pyCreate		, METH_VARARGS, "create(width,height,layer_count=1,tile_width=8,tile_height=8) : " },
	{"clear"		, VolumetricMap::pyClear		, METH_VARARGS, "clear(handle,layer_index=-1) : " },
	{"count"		, VolumetricMap::pyCount		, METH_VARARGS, "count(handle,value,layer_index=-1) : " },
	{"destroy"		, VolumetricMap::pyDestroy		, METH_VARARGS, "destroy(VolumetricMap_handle) : " },
	{"draw"			, VolumetricMap::pyDraw			, METH_VARARGS, "destroy(handle,x=0,y=0) : " },
	{"fill"			, VolumetricMap::pyFill			, METH_VARARGS, "set(handle,tile_index,layer_index=-1) : " },
	{"get"			, VolumetricMap::pyGet			, METH_VARARGS, "get(handle,x,y,layer_index=0) : " },
	{"getscroll"	, VolumetricMap::pyGetScroll	, METH_VARARGS, "[x, y] = getscroll(handle): " },
	{"getdata"		, VolumetricMap::pyGetData		, METH_VARARGS, "getdata(handle,layer_index=0) : " },
	{"loadtileset"	, VolumetricMap::pyLoadTileset	, METH_VARARGS, "loadtileset(handle,tileset_filename) : " },
	{"redraw"		, VolumetricMap::pyRedraw		, METH_VARARGS, "redraw(handle,layer_index=-1) : " },
	{"needsredraw"	, VolumetricMap::pyNeedsRedraw	, METH_VARARGS, "needsredraw(handle) : " },
	{"set"			, VolumetricMap::pySet			, METH_VARARGS, "set(handle,x,y,value,layer_index=0) : " },
	{"setdata"		, VolumetricMap::pySetData		, METH_VARARGS, "setdata(handle,value,layer_index=0) : " },
	{"setactive"	, VolumetricMap::pySetActive	, METH_VARARGS, "setactive(handle, disable) : " },
	{"setsurface"	, VolumetricMap::pySetSurface	, METH_VARARGS, "setsurface(VolumetricMap_handle,surface_handle) : " },
	{"setoffset"	, VolumetricMap::pySetOffset	, METH_VARARGS, "setoffset(VolumetricMap_handle,x, y, layer) : " },
	{"settarget"	, VolumetricMap::pySetTarget	, METH_VARARGS, "settarget(VolumetricMap_handle,surface_handle) : " },
	{"setboundaries", VolumetricMap::pySetBoundaries, METH_VARARGS, "setboundaries(VolumetricMap_handle,left, top, right, bottom) : " },
	{"setscroll"	, VolumetricMap::pySetScroll	, METH_VARARGS, "setscroll(VolumetricMap_handle,x, y) : " },
	{"setposition"	, VolumetricMap::pySetPosition	, METH_VARARGS, "setposition(VolumetricMap_handle,x, y) : " },
	{"scroll"		, VolumetricMap::pyScroll		, METH_VARARGS, "scroll(VolumetricMap_handle, delta, direction) : " },
	{"update"		, VolumetricMap::pyUpdate		, METH_VARARGS, "update(handle,delta=1.0) : " },
	*/
	{NULL, NULL, 0, NULL}
};

void VolumetricMap::destroy() {
	// Destroy data
	// Destroy tilesets
}

VolumetricMap::VolumetricMap(int w, int h, int tile_width, int tile_height, std::string tileset) {
	this->w				= w;
	this->h				= h;
	this->tile_height	= tile_height;
	this->tile_width	= tile_width;
	this->target		= &Vpu::background;
	
	// Initialize data
	this->data = nullptr;
	
	// Load tileset
	Surface temp = Vpu::loadBitmap("tilesets/" + tileset + ".png");
	if(temp.enabled) {
		this->tileset_width = temp.width / tile_width;
		int w = ((temp.height/tile_height)*temp.width);
		int h = tile_height; 
		this->tileset = Vpu::createSurface(w, h);
		Vpu::select(this->tileset);
		int y = 0;
		for (int i = 0; i < w; i+=temp.width) {
			Vpu::drawSurface(temp, 0, y, temp.width, tile_height, i, 0);
			y+=tile_height;
		}
		this->tileset_size = (temp.height / tile_height) * this->tileset_width;
	} else printf("ERROR: Cannot load tileset '%s'! \n", tileset.c_str());
	// Now we have a linear tileset, a single row bitmap containing all the tiles 
	// in sequential order so we can achieve the fastest speed locating them by index
}

void VolumetricMap::deinitialize() {
	if(!this->data)return;
	free(this->data);
	this->data   = nullptr;
	this->width  = 0;
	this->height = 0;
}

void VolumetricMap::initialize(int width, int height) {
	if(this->data) this->deinitialize();
	this->data   = (long long*)malloc(sizeof(long long) * width * height);
	this->width  = width;
	this->height = height;
}

void VolumetricMap::loadData(int width, int height,long int *data) {
	this->initialize(width, height);
	int i=0;
	// Clear data buffer
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++,i++) {
			this->data[i] = ((this->tileset_width-1)<<24) |
							((this->tileset_width-1)<<16) |
							((this->tileset_width-1)<<8 ) |
							((this->tileset_width-1)	) ;
		}
	}
	// Load layers
	i=0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++,i++) {
			this->data[i] = (this->data[i]&0xffffff00) | data[i];
			this->data[i] = (this->data[i]&0xffff00ff) | (data[i]<<8);
			if(i>=this->width*1) this->data[i-(this->width*1)] = (this->data[i-(this->width*1)]&0xff00ffff) | (data[i]<<16);
			if(i>=this->width*2) this->data[i-(this->width*2)] = (this->data[i-(this->width*2)]&0x00ffffff) | (data[i]<<24);
		}
	}
}

PyObject* VolumetricMap::pyLoad(PyObject* self, PyObject* args) {
	int handle;
	int width  = 8;
	int height = 8;
	
    PyObject *int_list;
    int pr_length;
    long  *data;

    if (!PyArg_ParseTuple(args, "iiiO", &handle, &width, &height, &int_list))
        return NULL;
    pr_length = PyObject_Length(int_list);
    if (pr_length < 0)
        return NULL;
    data = (long  *) malloc(sizeof(long *) * pr_length);
    if (data == NULL)
        return NULL;
    for (int index = 0; index < pr_length; index++) {
        PyObject *item;
        item = PyList_GetItem(int_list, index);
        if (!PyLong_Check(item))
            data[index] = 0;
        data[index] = PyLong_AsLong(item);
    }
	
	if (Vpu::volumetricmaps.find(handle) != Vpu::volumetricmaps.end()) {
		VolumetricMap* map = &Vpu::volumetricmaps.at(handle);
		map->loadData(width, height, data);
	}
	free(data);
	return PyLong_FromLong(1);
}

void VolumetricMap::draw(int ix, int iy) {
	int v = 0;
	int dy = iy;
	int dx = ix;
	
	Vpu::select(*this->target);
	//al_lock_bitmap(this->target->bitmap, Vpu::pixel_format, ALLEGRO_LOCK_READWRITE);
	Surface* ts = &this->tileset;
	if(this->redraw){
		dy = iy+this->offset_y;
		for (int y = 0; y < this->h; y++) {
			dx = ix + this->offset_x;
			long long *p = &this->data[y];
			for (int x = 0; x < this->w; x++) {
				v = *p;
				int bg_4 = ((v & 0xff000000) >> 24) % this->tileset_size;
				int bg_3 = ((v & 0x00ff0000) >> 16) % this->tileset_size;
				int bg_2 = ((v & 0x0000ff00) >>  8) % this->tileset_size;
				int bg_1 = ((v & 0x000000ff)      ) % this->tileset_size;
				// High wall
				Vpu::drawSurface(
					this->tileset,
					bg_4 * this->tile_width, 0,
					this->tile_width, this->tile_height,
					dx, dy-(this->tile_height*2)
				);
				// Mid wall
				Vpu::drawSurface(
					this->tileset,
					bg_3 * this->tile_width, 0,
					this->tile_width, this->tile_height,
					dx, dy-(this->tile_height)
				);
				// Low wall
				Vpu::drawSurface(
					this->tileset,
					bg_2 * this->tile_width, 0,
					this->tile_width, this->tile_height,
					dx, dy
				);
				// Floor
				Vpu::drawSurface(
					this->tileset,
					bg_1 * this->tile_width, 0,
					this->tile_width, this->tile_height,
					dx, dy
				);	
				dx += this->tile_width;
				p++;
			}
			dy += this->tile_height;
		}
		this->redraw = false;
	}
	//al_unlock_bitmap(this->target->bitmap);
}

void VolumetricMap::update(double delta) {

}

void VolumetricMap::setScroll(int x, int y) {
	/*this->scroll.set(x, y);
	if (this->scroll.y >= this->boundaries.bottom) this->scroll.y = this->boundaries.bottom - 1;
	if (this->scroll.x >= this->boundaries.right ) this->scroll.x = this->boundaries.right - 1;
	if (this->scroll.x <  this->boundaries.left  ) this->scroll.x = this->boundaries.left;
	if (this->scroll.y <  this->boundaries.top   ) this->scroll.y = this->boundaries.top;	
	this->redraw();*/
}

void VolumetricMap::setBoundaries(int left, int top, int right, int bottom) {
	//this->boundaries.set(left, top, right, bottom);
}

/*
PyObject* VolumetricMap::pyCreate(PyObject* self, PyObject* args) {
	int width;
	int height;
	int layercount = 1;
	int tilewidth  = 8;
	int tileheight = 8;
	if (!PyArg_ParseTuple(args, "ii|iii", &width, &height, &layercount, &tilewidth, &tileheight)) return NULL;
	long int handle = Vpu::allocateVolumetricMap(width, height, layercount, tilewidth, tileheight);
	return PyLong_FromLong(handle);
}

PyObject* VolumetricMap::pyDestroy(PyObject* self, PyObject* args) {
	long int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		Vpu::deallocateVolumetricMap(handle);
		return True;
	}
	printf("ERROR @ pyDestroy : VolumetricMap_handle out of range\n");
	return False; 	
}

PyObject* VolumetricMap::pyDraw(PyObject* self, PyObject* args) {
	int handle;
	float x = -9999;
	float y = -9999;
	if (!PyArg_ParseTuple(args, "i|ff", &handle, &x, &y)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		x = x > -9999 ? x : tm->x;
		y = y > -9999 ? y : tm->y;
		tm->draw(x-tm->scroll.x, y - tm->scroll.y);
		return True;
	}
	printf("ERROR @ pyDraw : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyUpdate(PyObject* self, PyObject* args) {
	int handle;
	float delta=1.0f;
	if (!PyArg_ParseTuple(args, "i|f", &handle, &delta)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->update(delta);
		return True;
	}
	printf("ERROR @ pyUpdate : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyClear(PyObject* self, PyObject* args) {
	int handle;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "i|i", &handle, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			tm->layers[i].clear();
		}
		else {
			tm->layers[layer].clear();
		}
		return True;
	}
	printf("ERROR @ pyClear : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyCount(PyObject* self, PyObject* args) {
	int handle;
	int value;
	int layer = -1;
	
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &value, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		int count = 0;
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			count += tm->layers[i].count(value);
		} else {
			count += tm->layers[layer].count(value);
		}
		return PyLong_FromLong(count);
	}
	printf("ERROR @ pyFill : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyFill(PyObject* self, PyObject* args) {
	int handle;
	int value;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &value, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		if (layer == -1)for (int i = 0; i < tm->layers.size(); i++) {
			tm->layers[i].fill(value);
		} else {
			tm->layers[layer].fill(value);
		}
		return True;
	}
	printf("ERROR @ pyFill : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySet(PyObject* self, PyObject* args) {
	int handle;
	int x;
	int y;
	int value;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "iiii|i", &handle, &x, &y, &value, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		if (y < 0) return False;
		else if (x < 0) return False;
		else if (x >= tl->w) return False;
		else if (y >= tl->h) return False;
		tl->data[y][x] = value;
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

extern Surface* getLayer(int index);

PyObject* VolumetricMap::pySetTarget(PyObject* self, PyObject* args) {
	int handle;
	int target;
	if (!PyArg_ParseTuple(args, "ii", &handle, &target)) return NULL;
	Surface* tgt = getLayer(target);
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->target = tgt;
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyScroll(PyObject* self, PyObject* args) {
	int handle;
	float delta;
	int direction;
	if (!PyArg_ParseTuple(args, "ifi", &handle, &delta, &direction)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
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
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetScroll(PyObject* self, PyObject* args) {
	int handle;
	int x;
	int y;
	if (!PyArg_ParseTuple(args, "iii", &handle, &x, &y)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->setScroll(x, y);
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetSurface(PyObject* self, PyObject* args) {
	int handle;
	int surface;
	if (!PyArg_ParseTuple(args, "ii", &handle, &surface)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		if (surface >= 0) {
			Surface* srf = getLayer(surface);
			tm->target = srf;
		}
		else {
			tm->target = nullptr;
		}
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetOffset(PyObject* self, PyObject* args) {
	int handle;
	int x, y, layer;
	if (!PyArg_ParseTuple(args, "iiii", &handle, &x, &y, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->layers[layer].offset_x = x;
		tm->layers[layer].offset_y = y;
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetBoundaries(PyObject* self, PyObject* args) {
	int handle;
	int left;
	int top;
	int right;
	int bottom;
	if (!PyArg_ParseTuple(args, "iiiii", &handle, &left, &top, &right, &bottom)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->setBoundaries(left, top, right, bottom);
		return True;
	}
	printf("ERROR @ pySet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetData(PyObject* self, PyObject* args) {
	int handle;
	char* data = nullptr;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &data, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
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
	printf("ERROR @ pySetData : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetActive(PyObject* self, PyObject* args) {
	int handle;
	bool disable=false;
	if (!PyArg_ParseTuple(args, "i|b", &handle, &disable)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		if (disable)  
			Vpu::active_map = nullptr; 
		else Vpu::active_map = tm;
		return True;
	}
	printf("ERROR @ pySetData : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyGet(PyObject* self, PyObject* args) {
	int handle;
	unsigned int x;
	unsigned int y;
	int layer = 0;
	if (!PyArg_ParseTuple(args, "iii|i", &handle, &x, &y, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		layer %= tm->layer_count;
		TiledLayer* tl = &tm->layers[layer];
		y %= tl->h;
		x %= tl->w;
		return PyLong_FromLong(tl->data[y][x]);
	}
	printf("ERROR @ pyGet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pySetPosition(PyObject* self, PyObject* args) {
	int handle;
	float x;
	float y;
	if (!PyArg_ParseTuple(args, "iff", &handle, &x, &y)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		tm->x = x;
		tm->y = y;
		return True;
	}
	printf("ERROR @ pyGet : VolumetricMap_handle out of range\n");
	return False;
}


PyObject* VolumetricMap::pyGetScroll(PyObject* self, PyObject* args) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		PyObject* list = PyList_New(2);
		if (!list) throw("Unable to allocate memory for Python list");
		Py_INCREF(list);
		PyList_SET_ITEM(list, 0, PyFloat_FromDouble((double)tm->scroll.x));
		PyList_SET_ITEM(list, 1, PyFloat_FromDouble((double)tm->scroll.y));
		Py_DECREF(list);
		return list;
	}
	printf("ERROR @ pyGet : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyGetData(PyObject* self, PyObject* args) {
	int handle;
	std::string data = "";
	int layer = 0;
	if (!PyArg_ParseTuple(args, "ii|i", &handle, &data, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
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
	printf("ERROR @ pyGetData : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyLoadTileset(PyObject* self, PyObject* args) {
	int handle;
	char *name;
	if (!PyArg_ParseTuple(args, "is", &handle, &name)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		return PyLong_FromLong(tm->loadTileset(name));
	}
	printf("ERROR @ pyRedraw : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyRedraw(PyObject* self, PyObject* args) {
	int handle;
	int layer = -1;
	if (!PyArg_ParseTuple(args, "i|i", &handle, &layer)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
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
	printf("ERROR @ pyRedraw : VolumetricMap_handle out of range\n");
	return False;
}

PyObject* VolumetricMap::pyNeedsRedraw(PyObject* self, PyObject* args) {
	int handle;
	if (!PyArg_ParseTuple(args, "i", &handle)) return NULL;
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {
		VolumetricMap* tm = &Vpu::VolumetricMaps.at(handle);
		for (int i = 0; i < tm->layers.size(); i++) {
			if (tm->layers[i].redraw) return True;
		}
		return False;
	}
	printf("ERROR @ pyRedraw : VolumetricMap_handle out of range\n");
	return False;
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

VolumetricMap *VolumetricMap::find(int handle){
	if (Vpu::VolumetricMaps.find(handle) != Vpu::VolumetricMaps.end()) {		
		return &Vpu::VolumetricMaps.at(handle);
	}
	return nullptr;
}
*/

