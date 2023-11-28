#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"
#include "element.h"
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
    return PyUnicode_FromString(self->elem->name);;
}

PyObject *
Element_chinese(Element *self, void *closure)
{
    return PyUnicode_FromString(self->elem->chinese);
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
    return PyUnicode_FromString(self->elem->symbol);
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
Substance_init(Substance *self, PyObject *args)
{
    DBSubstance *db;
    const char *key;

    if (!PyArg_ParseTuple(args, "Os", &db, &key))
        return -1;

    if (strcmp(Py_TYPE(db)->tp_name, "DBSubstance")) {
        PyErr_Format(PyExc_TypeError, "Except Instance of DBSubstance but get Instance of %s!", Py_TYPE(db)->tp_name);
        return -1;
    }

    substance *sbt = db_substance_get(db->data, key);

    if (!sbt) {
        PyErr_Format(PyExc_ValueError, "Invalid Substance key!");
        return -1;
    }

    self->data = sbt;
    return 0;
}

void
Substance_dealloc(Substance *self)
{
    self->data = NULL;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject *
Substance_str(Substance *self)
{
    return PyUnicode_FromFormat("<Substance: (%s) %s>", self->data->cas, self->data->name);
}

PyObject *Substance_cas(Substance *self)
{
    if (self->data->name)
        return PyUnicode_FromString(self->data->cas);
    else
        Py_RETURN_NONE;
}

PyObject *Substance_smiles(Substance *self)
{
    if (self->data->name)
        return PyUnicode_FromString(self->data->smiles);
    else
        Py_RETURN_NONE;
}

PyObject *Substance_name(Substance *self)
{
    if (self->data->name)
        return PyUnicode_FromString(self->data->name);
    else
        Py_RETURN_NONE;
}

PyObject *Substance_chinese(Substance *self)
{
    if (self->data->chinese)
        return PyUnicode_FromString(self->data->chinese);
    else
        Py_RETURN_NONE;
}

PyObject *Substance_formula(Substance *self)
{
    if (self->data->name)
        return PyUnicode_FromString(self->data->formula);
    else
        Py_RETURN_NONE;
}

/* =================================================================================================
 * Database of substance Definition
 */

PyObject *
DBSubstance_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DBSubstance *self;
    self = (DBSubstance *) type->tp_alloc(type, 0);

    if (self)
        return (PyObject *) self;
    else
        return NULL;
}

int
DBSubstance_init(DBSubstance *self, PyObject *args)
{
    self->data = db_substance_new();

    if (!self->data) {
        PyErr_Format(PyExc_ValueError, "Substance database initialization failed!");
        return -1;
    }

    return 0;
}

void
DBSubstance_dealloc(DBSubstance *self)
{
    db_substance_dealloc(self->data);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject *
DBSubstance_str(DBSubstance *self)
{
    return PyUnicode_FromFormat("<DBSubstance, identity by %s>", db_substance_identity_string(self->data));
}

PyObject *
DBSubstance_size(DBSubstance *self)
{
    PyObject *o = PyLong_FromSize_t(self->data->ht->used);
    return (PyObject *) o;
}

PyObject *
DBSubstance_add_substance(DBSubstance *self, PyObject *args, PyObject *kwds)
{
    const char *name;
    const char *smiles = NULL;
    const char *cas = NULL;
    const char *formula = NULL;
    const char *chinese = NULL;

    static char *kwlist[] = {"name", "cas", "smiles", "formula", "chinese", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|zzzz", kwlist, 
                                     &name, &cas, &smiles, &formula, &chinese))
    {
        return NULL;
    }

    if (cas)
        if (strlen(cas) > MAX_CAS_LEN)
            return NULL;

    substance sbt = {cas, smiles, name, chinese, formula};

    if (!self->data->fp_identity(&sbt)) {
        PyErr_Format(
            PyExc_ValueError,
            "Database use '%s' as identity key, which must not be None!",
            self->data->identity_name);
        return NULL;
    }

    if (db_substance_add(self->data, &sbt) < 0) {
        PyErr_Format(PyExc_ValueError, "Insert substance to database failed!");
        return NULL;
    }

    Py_RETURN_NONE;
}
