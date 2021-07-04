#ifndef __BASICGAME_HPP
#define __BASICGAME_HPP

#include "Script.hpp"

class BasicGame {
private: 
	int width  = 0;
	int height = 0;
	
	
public:
	BasicGame(int width, int height);
	~BasicGame();
	bool update					(float delta);
	void draw					();
	static PyObject *pyCreate	(PyObject* self, PyObject* args);
	static PyObject *pyDestroy  (PyObject* self, PyObject* args);
	static PyObject *pyDraw		(PyObject* self, PyObject* args);
	static PyObject *pyUpdate	(PyObject* self, PyObject* args);
	static PyMethodDef			methods[];
};

#endif
