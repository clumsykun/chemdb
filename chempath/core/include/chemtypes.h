#ifndef CHEMTYPES
#define CHEMTYPES

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"


typedef struct
{
    PyObject_HEAD
    element *elem;
} Element;


PyObject *Element_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int       Element_init(Element *self, PyObject *args);
void      Element_dealloc(Element *self);
PyObject *Element_id(Element *self, void *closure);
PyObject *Element_name(Element *self, void *closure);
PyObject *Element_chinese(Element *self, void *closure);
PyObject *Element_mass(Element *self, void *closure);
PyObject *Element_symbol(Element *self, void *closure);
PyObject *Element_str(Element *self);
PyObject *PyLong_num_element();


#endif /* CHEMTYPES */
