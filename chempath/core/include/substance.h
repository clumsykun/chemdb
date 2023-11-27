#ifndef SUBSTANCE
#define SUBSTANCE

#include "dtypes.h"
#include "hashtable.h"

typedef struct
{
    hashtable  *ht;
    const char *(*fp_cst_key)(void *);  /* Choose one char * property of substance as hashtable key. */
} db_substance;


db_substance *db_substance_new();
void          db_substance_dealloc(db_substance *db);
int           db_substance_add(db_substance *db, substance *sbt);
int           db_substance_del(db_substance *db, const char *key);
substance    *db_substance_get(db_substance *db, const char *key);

#endif /* SUBSTANCE */
