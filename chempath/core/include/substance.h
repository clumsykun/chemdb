#ifndef SUBSTANCE
#define SUBSTANCE

#include "dtypes.h"
#include "common.h"

typedef struct
{
    substance **head;
    hashtable  *ht;
    stack      *vacant;
    size_t      size;
    size_t      used;
    size_t      pos;
    fp_repr get_key;  /* Choose one char * property of substance as hashtable key. */
} db_substance;


#endif /* SUBSTANCE */
