#include <stdlib.h>
#include <string.h>

#include "dtypes.h"
#include "substance.h"

#define INIT_DB_SIZE 1024


/** ================================================================================================
 *  Substance
 */

static const char *
substance_cas(void *sbt)
{
    return ((substance *)sbt)->cas;
}

static const char *
substance_smiles(void *sbt)
{
    return ((substance *)sbt)->smiles;
}

static const char *
substance_name(void *sbt)
{
    return ((substance *)sbt)->name;
}

static const char *
substance_formula(void *sbt)
{
    return ((substance *)sbt)->formula;
}


/** ================================================================================================
 *  Database of substance
 */

/* NULL on failure */
db_substance *
db_substance_new()
{
    db_substance *db = malloc(sizeof(db_substance));
    hashtable *ht    = hashtable_new();

    if (!(db && ht)) {
        free(db);
        free(ht);
        return NULL;
    }

    db->ht = ht;
    db->fp_cst_key = substance_cas;
    return db;
}

/* Copy & insert sbt into db, return -1 on failure. */
int
db_substance_add(db_substance *db, substance *sbt)
{
    const char *key = db->fp_cst_key(sbt);
    substance *item = hashtable_get(db->ht, key);

    /* Exists then replace. */
    if (item) {
        memcpy(item, sbt, sizeof(substance));
        return 0;
    }

    /* Create. */
    item = malloc(sizeof(substance));

    if (!(item && key))
        return -1;

    memcpy(item, sbt, sizeof(substance));
    hashtable_set(db->ht, key, item, true);
    return 0;
}

/* Return -1 on failure. */
int
db_substance_del(db_substance *db, const char *key)
{
    return hashtable_set(db->ht, key, NULL, 1);
}

/* NULL on failure. */
substance *
db_substance_get(db_substance *db, const char *key)
{
    return (substance *) hashtable_get(db->ht, key);
}

void
db_substance_dealloc(db_substance *db)
{
    for (size_t idx = 0; idx < db->ht->size * 2; idx += 2)
        free(db->ht->buckets[idx]);

    hashtable_dealloc(db->ht);
    free(db);
}

const char *
db_substance_key_string(db_substance *db)
{
    if (db->fp_cst_key == substance_cas)
        return "cas";

    else if (db->fp_cst_key == substance_smiles)
        return "smiles";

    else if (db->fp_cst_key == substance_name)
        return "name";

    else if (db->fp_cst_key == substance_formula)
        return "formula";
    else
        return "error";  /* Should never get here! */
}
