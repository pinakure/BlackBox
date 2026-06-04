#include "genericgame.hpp"

GenericGame *the_game = nullptr;

void RawMap::draw() {
    printf("\tRawMap::Draw \t\t\t\r");
}

bool RawMap::update(double delta) {
    printf("\tRawMap::     Update %f\t\r", delta);
    return false;
}

PyMethodDef GenericGame::methods[] = {
	{ 
        "create", 
        GenericGame::pyCreate, 
        METH_VARARGS, 
        "create(width,height) : "
        ""
    },{ 
        "destroy", 
        GenericGame::pyDestroy, 
        METH_VARARGS, 
        "destroy(game_pointer) : "
        ""
    },{ 
        "draw", 
        GenericGame::pyDraw, 
        METH_VARARGS, 
        "draw(game_pointer) : "
        ""
    },{ 
        "update", 
        GenericGame::pyUpdate, 
        METH_VARARGS, 
        "update(game_pointer, delta) : "
        ""
    },
    {NULL, NULL, 0, NULL}
};
/* 
GLUE CODE ; It's Dirty but it's quick 
*/
static void draw_volumetric   (GenericGame *self) { self->volumetric_map->draw(0, 0); }
static void draw_tiled        (GenericGame *self) { self->tiled_map->draw(0, 0); }
static void draw_raw          (GenericGame *self) { self->raw_map->draw(); }
static bool update_volumetric (GenericGame *self, double delta) { return self->volumetric_map->update(delta); }
static bool update_tiled      (GenericGame *self, double delta) { return self->tiled_map->update(delta); }
static bool update_raw        (GenericGame *self, double delta) { return self->raw_map->update(delta); }
static void destroy_volumetric(GenericGame *self) { if(self->volumetric_map) delete(self->volumetric_map);  self->volumetric_map = nullptr; }
static void destroy_tiled     (GenericGame *self) { if(self->tiled_map) delete(self->tiled_map); self->tiled_map = nullptr; }
static void destroy_raw       (GenericGame *self) { if(self->raw_map) delete(self->raw_map); self->raw_map = nullptr; }
static bool create_volumetric (GenericGame *self, int width, int height) { self->volumetric_map = new VolumetricMap(width, height, 16, 16, ""); return true;  }
static bool create_tiled      (GenericGame *self, int width, int height) { self->tiled_map = new TiledMap(width, height, 4, 16, 16); return true;  }
static bool create_raw        (GenericGame *self, int width, int height) { self->raw_map = new RawMap(width, height); return true; }


GenericGame::GenericGame() {
    this->width         = 320;
    this->height        = 240;
    this->setType(MAPTYPE_RAW);
    this->map_handler.create(this, this->width, this->height);
}

void GenericGame::setType(MapType type) {
    this->map_type = type;
    switch (this->map_type) {
        case MAPTYPE_RAW: 
            map_handler.create  = create_raw;
            map_handler.destroy = destroy_raw;
            map_handler.draw    = draw_raw;
            map_handler.update  = update_raw;
            break;
        case MAPTYPE_TILED:
            map_handler.create  = create_tiled;
            map_handler.destroy = destroy_tiled;
            map_handler.draw    = draw_tiled;
            map_handler.update  = update_tiled;
            break;
        case MAPTYPE_VOLUMETRIC:
            map_handler.create  = create_volumetric;
            map_handler.destroy = destroy_volumetric;
            map_handler.draw    = draw_volumetric;
            map_handler.update  = update_volumetric;
            break;
    }
}

GenericGame::GenericGame(int width, int height, MapType type) {
    this->width         = width;
    this->height        = height;
    this->setType(type);
    this->map_handler.create(this, width, height);
    printf("GenericGame::Initialize");
}
GenericGame::~GenericGame() {
    printf("GenericGame::Deinitialize");
    this->map_handler.destroy(this);
}

bool GenericGame::update(float delta) {
    printf("GenericGame::Update");
    this->map_handler.update(this, delta);
    return true;
}

void GenericGame::draw() {
    printf("GenericGame::Draw");
    this->map_handler.draw(this);
}

PyObject* GenericGame::pyCreate(PyObject* self, PyObject* args) {
	int width  = 8;
	int height = 8;
	MapType type = MAPTYPE_RAW;
    if (!PyArg_ParseTuple(args, "ii|i", &width, &height, &type)) return NULL;
    if(the_game) delete(the_game);
    the_game = nullptr;
    the_game = new GenericGame(width, height, type);
	return PyLong_FromLong(1);
}

PyObject* GenericGame::pyDestroy(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ""))return NULL;
    if(the_game)
        delete(the_game);
    the_game = nullptr;
    return PyLong_FromLong(1);
}

PyObject* GenericGame::pyUpdate(PyObject* self, PyObject* args) {
    float delta = 0;
    if (!PyArg_ParseTuple(args, "f", &delta)) return NULL;
    if(the_game) 
        return PyLong_FromLong(the_game->update(delta));
    return NULL;
}

PyObject* GenericGame::pyDraw(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, "")) return NULL;
    if(the_game)
        the_game->draw();
    return NULL;
}
