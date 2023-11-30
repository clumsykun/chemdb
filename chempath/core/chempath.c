#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "chemtypes.h"


extern PyTypeObject Element_Type;
extern PyTypeObject Substance_type;
extern PyTypeObject DBSubstance_type;
extern PyTypeObject DBStep_type;

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

    if (PyType_Ready(&Element_Type) < 0)
        return NULL;

    if (PyType_Ready(&DBSubstance_type) < 0)
        return NULL;

    if (PyType_Ready(&Substance_type) < 0)
        return NULL;

    if (PyType_Ready(&DBStep_type) < 0)
        return NULL;

    Py_INCREF(&Element_Type);
    Py_INCREF(&DBSubstance_type);
    Py_INCREF(&Substance_type);
    Py_INCREF(&DBStep_type);

    if (PyModule_AddObject(m, "_Element", (PyObject *) &Element_Type) < 0) {
        Py_DECREF(&Element_Type);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "_DBSubstance", (PyObject *) &DBSubstance_type) < 0) {
        Py_DECREF(&DBSubstance_type);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "_Substance", (PyObject *) &Substance_type) < 0) {
        Py_DECREF(&Substance_type);
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "_DBStep", (PyObject *) &DBStep_type) < 0) {
        Py_DECREF(&DBStep_type);
        Py_DECREF(m);
        return NULL;
    }

    if (!m)
        return NULL;

    return m;
}
