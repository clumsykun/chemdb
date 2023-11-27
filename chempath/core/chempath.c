#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "chemtypes.h"


/** ================================================================================================
 *  Element Definition.
 */

static PyGetSetDef Element_getset[] = {
    {"id",      (getter) Element_id,      (setter) NULL, "Id of chemical element.", NULL},
    {"name",    (getter) Element_name,    (setter) NULL, "Name of chemical element.", NULL},
    {"chinese", (getter) Element_chinese, (setter) NULL, "Chinese name of chemical element.", NULL},
    {"mass",    (getter) Element_mass,    (setter) NULL, "Mass of chemical element.", NULL},
    {"symbol",  (getter) Element_symbol,  (setter) NULL, "Symbol of chemical element.", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject type_Element = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "BaseElement",
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


/** ================================================================================================
 *  Database substance Definition.
 */

static PyGetSetDef DBSubstance_getset[] = {
    {"size", (getter) DBSubstance_size, (setter) NULL, "Size of database.", NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject type_DBSubstance = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "BaseDBSubstance",
    .tp_doc       = PyDoc_STR("Database of substance."),
    .tp_basicsize = sizeof(DBSubstance),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new       =              DBSubstance_new,
    .tp_init      = (initproc)   DBSubstance_init,
    .tp_dealloc   = (destructor) DBSubstance_dealloc,
    // .tp_str       = (reprfunc)   DBSubstance_str,
    // .tp_repr      = (reprfunc)   DBSubstance_str,
    .tp_getset    =              DBSubstance_getset,
};


/** ================================================================================================
 *  Package Definition.
 */


static PyMethodDef methods_module[] = {
    {"num_element", PyLong_num_element, METH_NOARGS, "Number of elements."},
    {NULL},
};

static PyModuleDef module__chempath = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "_chempath",
    .m_doc     = "C implementation of chempath.",
    .m_size    = -1,
    .m_methods = methods_module,
};

PyMODINIT_FUNC PyInit__chempath(void)
{
    PyObject *m;
    m = PyModule_Create(&module__chempath);

    if (PyType_Ready(&type_Element) < 0)
        return NULL;

    if (PyType_Ready(&type_DBSubstance) < 0)
        return NULL;

    Py_INCREF(&type_Element);
    Py_INCREF(&type_DBSubstance);

    if (PyModule_AddObject(m, "BaseElement", (PyObject *) &type_Element) < 0) {
        Py_DECREF(&type_Element);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "BaseDBSubstance", (PyObject *) &type_DBSubstance) < 0) {
        Py_DECREF(&type_DBSubstance);
        Py_DECREF(m);
        return NULL;
    }

    if (!m)
        return NULL;

    return m;
}
