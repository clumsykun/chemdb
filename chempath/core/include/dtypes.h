#ifndef DTYPES
#define DTYPES

#include <stddef.h>


typedef struct
{
    size_t id;
    char name[100];
    char chinese[100];
    char symbol[2];
    double mass;
} element;

typedef struct
{
    const char *const name;
    const char *const cas;     /* CAS Registry Number */
    const char *const smiles;  /* Simplified molecular-input line-entry system */
    const char *const formula;
    const char *const chinese;
} substance;

typedef struct
{
    substance *reactants;
    size_t     reactants_num;
    substance *products;
    size_t     products_num;
} reaction;

typedef struct
{
    reaction  *reactions;
    size_t     reactions_num;
    substance *inputs;
    size_t     inputs_num;
    substance *target;
} step;

typedef struct
{
    step   *steps;
    size_t *steps_tier;
    size_t  steps_num;
} pathway;


#endif /* DTYPES */
