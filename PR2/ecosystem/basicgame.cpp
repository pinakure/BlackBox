#include "basicgame.hpp"

PyMethodDef BasicGame::methods[] = {
	{ 
        "create", 
        BasicGame::pyCreate, 
        METH_VARARGS, 
        "create(width,height) : "
        ""
    },{ 
        "destroy", 
        BasicGame::pyDestroy, 
        METH_VARARGS, 
        "destroy(game_pointer) : "
        ""
    },{ 
        "draw", 
        BasicGame::pyDraw, 
        METH_VARARGS, 
        "draw(game_pointer) : "
        ""
    },{ 
        "update", 
        BasicGame::pyUpdate, 
        METH_VARARGS, 
        "update(game_pointer, delta) : "
        ""
    },
    {NULL, NULL, 0, NULL}
};


BasicGame::BasicGame(int width, int height) {
    this->width = width;
    this->height = height;
    printf("BasicGame::Initialize");
}
BasicGame::~BasicGame() {
    printf("BasicGame::Deinitialize");
}

bool BasicGame::update(float delta) {
    printf("BasicGame::Update");
    return true;
}

void BasicGame::draw() {
    printf("BasicGame::Draw");
}

PyObject* BasicGame::pyCreate(PyObject* self, PyObject* args) {
	int width  = 8;
	int height = 8;
	
    if (!PyArg_ParseTuple(args, "ii", &width, &height)) return NULL;
    BasicGame *game = new BasicGame(width, height);
	return PyLong_FromLong((long)game);
}

PyObject* BasicGame::pyDestroy(PyObject* self, PyObject* args) {
    BasicGame   *game   = nullptr;
    if (!PyArg_ParseTuple(args, "i", &game))return NULL;
    delete(game);
    return PyLong_FromLong(1);
}

PyObject* BasicGame::pyUpdate(PyObject* self, PyObject* args) {
    BasicGame   *game   = nullptr;
    float       delta   = 0;
    if (!PyArg_ParseTuple(args, "if", &game, &delta)) return NULL;
    return PyLong_FromLong(game->update(delta));
}

PyObject* BasicGame::pyDraw(PyObject* self, PyObject* args) {
    BasicGame *game = nullptr;
    if (!PyArg_ParseTuple(args, "i", &game)) return NULL;
    game->draw();
    return PyLong_FromLong(1);
}
