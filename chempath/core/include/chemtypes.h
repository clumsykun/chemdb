#ifndef CHEMTYPES
#define CHEMTYPES

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"
#include "substance.h"


typedef struct
{
    PyObject_HEAD
    element *elem;
} Element;

// typedef struct
// {
//     PyObject_HEAD
//     substance *data;
// } Substance;

typedef struct
{
    PyObject_HEAD
    db_substance *data;
} DBSubstance;


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

PyObject *DBSubstance_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int       DBSubstance_init(DBSubstance *self, PyObject *args);
void      DBSubstance_dealloc(DBSubstance *self);
PyObject *DBSubstance_size(DBSubstance *self);
PyObject *DBSubstance_add_substance(DBSubstance *self, PyObject *args, PyObject *kwds);


#endif /* CHEMTYPES */
