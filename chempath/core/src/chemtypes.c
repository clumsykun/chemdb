#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "chemtypes.h"


/* =================================================================================================
 * Element Definition
 */

PyObject *
Element_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Element *self;
    self = (Element *) type->tp_alloc(type, 0);

    if (self)
        return (PyObject *) self;
    else
        return NULL;
}

int
Element_init(Element *self, PyObject *args)
{
    int id;

    if (!PyArg_ParseTuple(args, "i", &id))
        return -1;

    self->elem = element_locate_by_id((size_t)id);

    if (!self->elem) {
        PyErr_Format(PyExc_ValueError, "Illegal atomic number: %d!", id);
        return -1;
    }

    return 0;
}

void
Element_dealloc(Element *self)
{
    self->elem = NULL;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject *
Element_id(Element *self, void *closure)
{
    PyObject *o = PyLong_FromSize_t(self->elem->id);
    return (PyObject *) o;
}

PyObject *
Element_name(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->name);
    return (PyObject *) o;
}

PyObject *
Element_chinese(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->chinese);
    return (PyObject *) o;
}

PyObject *
Element_mass(Element *self, void *closure)
{
    PyObject *o = PyFloat_FromDouble(self->elem->mass);
    return (PyObject *) o;
}

PyObject *
Element_symbol(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->symbol);
    return (PyObject *) o;
}

PyObject *
Element_str(Element *self)
{
    return PyUnicode_FromFormat("<Element: (%d) %s>", self->elem->id, self->elem->name);
}

PyObject *
PyLong_num_element()
{
    return (PyObject *) PyLong_FromLong(NUM_ELEMENTS);
}


/* =================================================================================================
 * Substance Definition
 */

PyObject *
Substance_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Substance *self;
    self = (Substance *) type->tp_alloc(type, 0);

    if (self)
        return (PyObject *) self;
    else
        return NULL;
}

int
Substance_init(Substance *self, PyObject *args, PyObject *kwds)
{
    int id;

    if (!PyArg_ParseTuple(args, "i", &id))
        return -1;

    return 0;
}

void
Substance_dealloc(Substance *self)
{
    self->data = NULL;
    Py_TYPE(self)->tp_free((PyObject *) self);
}
