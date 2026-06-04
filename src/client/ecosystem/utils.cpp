#include "utils.hpp"
#include "console.hpp"

std::vector<std::string> explode(std::string const& s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

// PyObject -> Vector
#include "vpu.hpp"
std::vector<Pixel> listToPalette(PyObject* incoming) {
	std::vector<Pixel> data;
	if (PyTuple_Check(incoming)) {
		for (Py_ssize_t i = 0; i < PyTuple_Size(incoming); i++) {
			PyObject* value = PyTuple_GetItem(incoming, i);
			PyObject* r = PyTuple_GetItem(value, 0);
			PyObject* g = PyTuple_GetItem(value, 1);
			PyObject* b = PyTuple_GetItem(value, 2);
			data.push_back({
				(int)PyLong_AsLong(r),
				(int)PyLong_AsLong(g),
				(int)PyLong_AsLong(b)
			});
		}
	} else if (PyList_Check(incoming)) {
		for (Py_ssize_t i = 0; i < PyList_Size(incoming); i++) {
			PyObject* value = PyList_GetItem(incoming, i);
			PyObject* r = PyList_GetItem(value, 0);
			PyObject* g = PyList_GetItem(value, 1);
			PyObject* b = PyList_GetItem(value, 2);
			data.push_back({
				(int)PyLong_AsLong(r),
				(int)PyLong_AsLong(g),
				(int)PyLong_AsLong(b)
			});
		}
	} else {
		throw ("Passed PyObject pointer was not a list or tuple!");
	}
	return data;
}