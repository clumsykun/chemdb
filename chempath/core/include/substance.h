#ifndef SUBSTANCE
#define SUBSTANCE

#include "dtypes.h"
#include "hashtable.h"

#define MAX_CAS_LEN 12


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
const char   *db_substance_key_string(db_substance *db);

substance *substance_new(const char *cas, const char *smiles,
                         const char *name, const char *chinese,
                         const char *formula);
void       substance_dealloc(substance *sbt);


#endif /* SUBSTANCE */
