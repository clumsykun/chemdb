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

PyTypeObject type_Element = {
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


/** ================================================================================================
 *  Substance Definition.
 */

static PyGetSetDef Substance_getset[] = {
    {"name",    (getter) Substance_name,    (setter) NULL, "Name of chemical substance.", NULL},
    {"cas",     (getter) Substance_cas,     (setter) NULL, "CAS of chemical substance.", NULL},
    {"smiles",  (getter) Substance_smiles,  (setter) NULL, "SMILES of chemical substance.", NULL},
    {"formula", (getter) Substance_formula, (setter) NULL, "Formula of chemical substance.", NULL},
    {"chinese", (getter) Substance_chinese, (setter) NULL, "Chinese name of chemical substance.", NULL},    
    {NULL}  /* Sentinel */
};

PyTypeObject type_Substance = {
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


/** ================================================================================================
 *  Database substance Definition.
 */

static PyGetSetDef DBSubstance_getset[] = {
    {"size", (getter) DBSubstance_size, (setter) NULL, "Size of database.", NULL},
    {NULL}  /* Sentinel */
};

static PyMethodDef DBSubstance_methods[] = {
    {"add_substance", (PyCFunction) DBSubstance_add_substance,  METH_VARARGS | METH_KEYWORDS, "Add substance to database."},
    {NULL},
};

PyTypeObject type_DBSubstance = {
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

    if (PyType_Ready(&type_Substance) < 0)
        return NULL;


    Py_INCREF(&type_Element);
    Py_INCREF(&type_DBSubstance);
    Py_INCREF(&type_Substance);

    if (PyModule_AddObject(m, "_Element", (PyObject *) &type_Element) < 0) {
        Py_DECREF(&type_Element);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "_DBSubstance", (PyObject *) &type_DBSubstance) < 0) {
        Py_DECREF(&type_DBSubstance);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "_Substance", (PyObject *) &type_Substance) < 0) {
        Py_DECREF(&type_Substance);
        Py_DECREF(m);
        return NULL;
    }

    if (!m)
        return NULL;

    return m;
}
