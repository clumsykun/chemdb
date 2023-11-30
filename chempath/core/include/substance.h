#ifndef SUBSTANCE
#define SUBSTANCE

#include "dtypes.h"
#include "hashtable.h"

typedef const char *(*fp_get_identity)(void *);


typedef struct
{
    hashtable  *ht;
    const char *identity_name;
    fp_get_identity get_identity;  /* Choose identity as hashtable key. */
} db_substance;

db_substance *db_substance_new(const char *identity_name);
void          db_substance_dealloc(db_substance *db);
int           db_substance_add(db_substance *db, substance *sbt);
int           db_substance_del(db_substance *db, const char *key);
substance    *db_substance_get(db_substance *db, const char *key);

#endif /* SUBSTANCE */
