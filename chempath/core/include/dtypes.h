#ifndef DTYPES
#define DTYPES
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define NUM_ELEMENT 118


typedef struct {
    size_t id;
    char name[100];
    char chinese[100];
    char symbol[2];
    double mass;
} element;


typedef struct {
    PyObject_HEAD
    element *elem;
} Element;


PyObject *PyLong_num_element();
PyObject *Element_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int       Element_init(Element *self, PyObject *args);
void      Element_dealloc(Element *self);
PyObject *Element_id(Element *self, void *closure);
PyObject *Element_name(Element *self, void *closure);
PyObject *Element_chinese(Element *self, void *closure);
PyObject *Element_mass(Element *self, void *closure);
PyObject *Element_symbol(Element *self, void *closure);
PyObject *Element_str(Element *self);


#endif /* DTYPES */
