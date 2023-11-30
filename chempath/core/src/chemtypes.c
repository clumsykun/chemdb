#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"
#include "element.h"
#include "chemtypes.h"


/* =================================================================================================
 * Element Definition
 */

static PyObject *
Element_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Element *self;
    self = (Element *) type->tp_alloc(type, 0);

    if (!self)
        return NULL;

    self->data = NULL;
    return (PyObject *) self;
}

static int
Element_init(Element *self, PyObject *args)
{
    int id;

    if (!PyArg_ParseTuple(args, "i", &id))
        return -1;

    self->data = element_locate_by_id((size_t)id);

    if (!self->data) {
        PyErr_Format(PyExc_ValueError, "Illegal atomic number: %d!", id);
        return -1;
    }

    return 0;
}

static void
Element_dealloc(Element *self)
{
    self->data = NULL;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
Element_id(Element *self, void *closure)
{
    PyObject *o = PyLong_FromSize_t(self->data->id);
    return (PyObject *) o;
}

static PyObject *
Element_name(Element *self, void *closure)
{
    return PyUnicode_FromString(self->data->name);;
}

static PyObject *
Element_chinese(Element *self, void *closure)
{
    return PyUnicode_FromString(self->data->chinese);
}

static PyObject *
Element_mass(Element *self, void *closure)
{
    PyObject *o = PyFloat_FromDouble(self->data->mass);
    return (PyObject *) o;
}

static PyObject *
Element_symbol(Element *self, void *closure)
{
    return PyUnicode_FromString(self->data->symbol);
}

static PyObject *
Element_str(Element *self)
{
    return PyUnicode_FromFormat("<Element: (%d) %s>", self->data->id, self->data->name);
}

PyObject *
PyLong_num_element()
{
    return (PyObject *) PyLong_FromLong(NUM_ELEMENTS);
}


static PyGetSetDef Element_getset[] = {
    {"id",      (getter) Element_id,      (setter) NULL, "Id of chemical element.", NULL},
    {"name",    (getter) Element_name,    (setter) NULL, "Name of chemical element.", NULL},
    {"chinese", (getter) Element_chinese, (setter) NULL, "Chinese name of chemical element.", NULL},
    {"mass",    (getter) Element_mass,    (setter) NULL, "Mass of chemical element.", NULL},
    {"symbol",  (getter) Element_symbol,  (setter) NULL, "Symbol of chemical element.", NULL},
    {NULL}  /* Sentinel */
};

PyTypeObject Element_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "_Element",
    .tp_doc       = PyDoc_STR("Chemical Element"),
    .tp_basicsize = sizeof(Element),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       =              Element_new,
    .tp_init      = (initproc)   Element_init,
    .tp_dealloc   = (destructor) Element_dealloc,
    .tp_str       = (reprfunc)   Element_str,
    .tp_repr      = (reprfunc)   Element_str,
    .tp_getset    =              Element_getset,
};

/* =================================================================================================
 * Substance Definition
 */

static PyObject *
Substance_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Substance *self;
    self = (Substance *) type->tp_alloc(type, 0);

    if (!self)
        return NULL;

    self->data = NULL;
    self->DB = Py_NewRef(Py_None);
    self->identity = Py_NewRef(Py_None);

    return (PyObject *) self;
}

static int
Substance_init(Substance *self, PyObject *args)
{
    PyObject *db;
    const char *identity;

    if (!PyArg_ParseTuple(args, "Os", &db, &identity))
        return -1;

    extern PyTypeObject DBSubstance_type;
    if (!PyObject_IsInstance(db, (PyObject *)(&DBSubstance_type))) {
        PyErr_Format(
            PyExc_TypeError,
            "except 'DBSubstance' but get '%s'!",
            Py_TYPE(db)->tp_name
        );
        return -1;
    }

    self->data = db_substance_get(
        ((DBSubstance *)db)->data,
        identity
    );

    if (!self->data) {
        PyErr_Format(
            PyExc_ValueError,
            "Invalid identity: '%s'!",
            identity
        );
        return -1;
    }

    Py_SETREF(self->DB, Py_NewRef(db));
    Py_SETREF(self->identity, PyUnicode_FromString(identity));
    return 0;
}

static void
Substance_dealloc(Substance *self)
{
    self->data = NULL;
    Py_DECREF(self->DB);
    Py_DECREF(self->identity);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
Substance_str(Substance *self)
{
    return PyUnicode_FromFormat(
        "<Substance: (%s) %s>",
        PyUnicode_AsUTF8AndSize(self->identity, NULL),
        self->data->name
    );
}

static PyObject *
Substance_cas(Substance *self)
{
    if (self->data->cas)
        return PyUnicode_FromString(self->data->cas);
    else
        Py_RETURN_NONE;
}

static PyObject *
Substance_smiles(Substance *self)
{
    if (self->data->smiles)
        return PyUnicode_FromString(self->data->smiles);
    else
        Py_RETURN_NONE;
}

static PyObject *
Substance_name(Substance *self)
{
    if (self->data->name)
        return PyUnicode_FromString(self->data->name);
    else
        Py_RETURN_NONE;
}

static PyObject *
Substance_chinese(Substance *self)
{
    if (self->data->chinese)
        return PyUnicode_FromString(self->data->chinese);
    else
        Py_RETURN_NONE;
}

static PyObject *
Substance_formula(Substance *self)
{
    if (self->data->formula)
        return PyUnicode_FromString(self->data->formula);
    else
        Py_RETURN_NONE;
}

static PyObject *
Substance_identity(Substance *self)
{
    return Py_NewRef(self->identity);
}

static PyGetSetDef Substance_getset[] = {
    {"name",     (getter) Substance_name,     (setter) NULL, "Name of chemical substance.", NULL},
    {"cas",      (getter) Substance_cas,      (setter) NULL, "CAS of chemical substance.", NULL},
    {"smiles",   (getter) Substance_smiles,   (setter) NULL, "SMILES of chemical substance.", NULL},
    {"formula",  (getter) Substance_formula,  (setter) NULL, "Formula of chemical substance.", NULL},
    {"chinese",  (getter) Substance_chinese,  (setter) NULL, "Chinese name of chemical substance.", NULL},    
    {"identity", (getter) Substance_identity, (setter) NULL, "Identity of chemical substance.", NULL},    
    {NULL}  /* Sentinel */
};

PyTypeObject Substance_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "_Substance",
    .tp_doc       = PyDoc_STR("Substance."),
    .tp_basicsize = sizeof(Substance),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       =              Substance_new,
    .tp_init      = (initproc)   Substance_init,
    .tp_dealloc   = (destructor) Substance_dealloc,
    .tp_str       = (reprfunc)   Substance_str,
    .tp_repr      = (reprfunc)   Substance_str,
    .tp_getset    =              Substance_getset,
    // .tp_methods   =              Substance_methods,
};

/* =================================================================================================
 * Database of substance Definition
 */

static PyObject *
DBSubstance_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DBSubstance *self;
    self = (DBSubstance *) type->tp_alloc(type, 0);

    if (!self)
        return NULL;

    self->data = NULL;
    return (PyObject *) self;
}

static int
DBSubstance_init(DBSubstance *self, PyObject *args)
{
    const char *identity_name;

    if (!PyArg_ParseTuple(args, "s", &identity_name))
        return -1;

    self->data = db_substance_new(identity_name);

    if (!self->data) {
        PyErr_Format(
            PyExc_ValueError,
            "invalid identity name: '%s'!",
            identity_name
        );
        return -1;
    }

    return 0;
}

static void
DBSubstance_dealloc(DBSubstance *self)
{
    db_substance_dealloc(self->data);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
DBSubstance_str(DBSubstance *self)
{
    return PyUnicode_FromFormat("<DBSubstance, identity by %s>", self->data->identity_name);
}

static PyObject *
DBSubstance_size(DBSubstance *self)
{
    PyObject *o = PyLong_FromSize_t(self->data->ht->used);
    return (PyObject *) o;
}

/**
 * A CAS Registry Number includes up to 10 digits which are separated into 3 groups by 2 hyphens. 
 */
static bool
check_cas_validation(const char *cas)
{
    if (strlen(cas) > 12)
        return false;

    size_t cnt = 0;
    char c;

    while ((c = *cas++))
        if (c == '-')
            cnt++;

    if (cnt != 2)
        return false;

    return true;
}

static PyObject *
DBSubstance_add_substance(DBSubstance *self, PyObject *args, PyObject *kwds)
{
    const char *name;
    const char *cas = NULL;
    const char *smiles = NULL;
    const char *formula = NULL;
    const char *chinese = NULL;

    static char *kwlist[] = {"name", "cas", "smiles", "formula", "chinese", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|zzzz", kwlist, 
                                     &name, &cas, &smiles, &formula, &chinese))
    {
        return NULL;
    }

    if (cas)
        if (!check_cas_validation(cas)) {
            PyErr_Format(
                PyExc_ValueError,
                "invalid CAS number: '%s'!",
                cas
            );
            return NULL;
        }

    substance sbt = {name, cas, smiles, formula, chinese};
    const char *identity = self->data->get_identity(&sbt);

    if (!identity) {
        PyErr_Format(
            PyExc_ValueError,
            "except identity not be None!",
            self->data->identity_name);
        return NULL;
    }

    if (db_substance_add(self->data, &sbt) < 0) {
        PyErr_Format(PyExc_ValueError, "insertion failed!");
        return NULL;
    }

    return PyUnicode_FromString(identity);
}

static PyGetSetDef DBSubstance_getset[] = {
    {"size", (getter) DBSubstance_size, (setter) NULL, "Size of database.", NULL},
    {NULL}  /* Sentinel */
};

static PyMethodDef DBSubstance_methods[] = {
    {"_add_substance", (PyCFunction) DBSubstance_add_substance,  METH_VARARGS | METH_KEYWORDS, "Add substance to database."},
    {NULL},
};

PyTypeObject DBSubstance_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "_DBSubstance",
    .tp_doc       = PyDoc_STR("Database of substance."),
    .tp_basicsize = sizeof(DBSubstance),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       =              DBSubstance_new,
    .tp_init      = (initproc)   DBSubstance_init,
    .tp_dealloc   = (destructor) DBSubstance_dealloc,
    .tp_str       = (reprfunc)   DBSubstance_str,
    .tp_repr      = (reprfunc)   DBSubstance_str,
    .tp_getset    =              DBSubstance_getset,
    .tp_methods   =              DBSubstance_methods,
};


/* =================================================================================================
 * Database of step Definition
 */

static PyObject *
DBStep_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DBStep *self;
    self = (DBStep *) type->tp_alloc(type, 0);

    if (!self)
        return NULL;

    self->data = NULL;
    self->DB = Py_NewRef(Py_None);
    return (PyObject *) self;
}

static int
DBStep_init(DBStep *self, PyObject *args)
{
    PyObject *db;

    if (!PyArg_ParseTuple(args, "O", &db))
        return -1;

    if (!PyObject_IsInstance(db, (PyObject *)(&DBSubstance_type))) {
        PyErr_Format(
            PyExc_TypeError,
            "except 'DBSubstance' but get '%s'!",
            Py_TYPE(db)->tp_name
        );
        return -1;
    }

    self->data = db_step_new((db_substance *)db);

    if (!self->data) {
        PyErr_Format(
            PyExc_ValueError,
            "initialization failed!"
        );
        return -1;
    }

    Py_SETREF(self->DB, Py_NewRef(db));
    return 0;
}

static void
DBStep_dealloc(DBStep *self)
{
    db_step_dealloc(self->data);
    Py_DECREF(self->DB);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
DBStep_db_substance(DBStep *self)
{
    Py_INCREF(self->DB);
    return self->DB;
}

static PyObject *
DBStep_add_step(DBStep *self, PyObject *args, PyObject *kwds)
{
    PyObject *target;
    PyObject *inputs;
    // const char *name;

    if (!PyArg_ParseTuple(args, "OO", &target, &inputs))
        return NULL;

    if (!PySequence_Check(inputs)) {
        PyErr_Format(
            PyExc_TypeError,
            "except sequence object but get '%s'!",
            Py_TYPE(inputs)
        );
        return NULL;
    }

    PyObject *o;
    for (Py_ssize_t i = 0; i < PySequence_Length(inputs); i++) {
        o = PySequence_GetItem(inputs, i);

        if (!PyObject_IsInstance(o, (PyObject *)(&Substance_type))) {
            PyErr_Format(
                PyExc_TypeError,
                "except members of 'inputs' are 'DBSubstance', but get '%s'!",
                Py_TYPE(o)->tp_name
            );
            return NULL;
        }

        PyObject_Print(o, stdout, 0);
    }

    // self->DB
    Py_RETURN_NONE;
}


static PyGetSetDef DBStep_getset[] = {
    {"db_substance", (getter) DBStep_db_substance, (setter) NULL, "Database of substance.", NULL},
    {NULL}  /* Sentinel */
};

static PyMethodDef DBStep_methods[] = {
    {"_add_step", (PyCFunction) DBStep_add_step,  METH_VARARGS, "Add step to database."},
    {NULL},
};

PyTypeObject DBStep_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "_DBStep",
    .tp_doc       = PyDoc_STR("Database of substance."),
    .tp_basicsize = sizeof(DBStep),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       =              DBStep_new,
    .tp_init      = (initproc)   DBStep_init,
    .tp_dealloc   = (destructor) DBStep_dealloc,
    // .tp_str       = (reprfunc)   DBStep_str,
    // .tp_repr      = (reprfunc)   DBStep_str,
    .tp_getset    =              DBStep_getset,
    .tp_methods   =              DBStep_methods,
};
