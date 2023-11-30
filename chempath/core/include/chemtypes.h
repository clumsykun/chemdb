#ifndef CHEMTYPES
#define CHEMTYPES

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"
#include "substance.h"


typedef struct
{
    PyObject_HEAD
    element *data;
} Element;

typedef struct
{
    PyObject_HEAD
    substance *data;
    PyObject *db;
    PyObject *identity;
} Substance;

typedef struct
{
    PyObject_HEAD
    db_substance *data;
} DBSubstance;


PyObject *PyLong_num_element();


#endif /* CHEMTYPES */
