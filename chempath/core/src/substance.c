#include <stdlib.h>
#include <string.h>

#include "dtypes.h"
#include "substance.h"

#define INIT_DB_SIZE 1024


/** ================================================================================================
 *  Substance
 */

/* Return NULL on failure */
static substance *
substance_new(const char *cas,
              const char *smiles,
              const char *name,
              const char *chinese,
              const char *formula)
{
    substance *sbt = malloc(sizeof(substance));
    char *cp_cas = malloc(strlen(cas) + 1);
    char *cp_smiles = malloc(strlen(smiles) + 1);
    char *cp_name = malloc(strlen(name) + 1);
    char *cp_chinese = malloc(strlen(chinese) + 1);
    char *cp_formula = malloc(strlen(formula) + 1);

    if (!(sbt && cp_cas && cp_smiles && cp_name && cp_chinese && cp_formula)) {
        free(sbt);
        free(cp_cas);
        free(cp_smiles);
        free(cp_name);
        free(cp_chinese);
        free(cp_formula);
        return NULL;
    }

    strcpy(cp_cas, cas);
    strcpy(cp_smiles, smiles);
    strcpy(cp_name, name);
    strcpy(cp_chinese, chinese);
    strcpy(cp_formula, formula);
    substance cp_sbt = {cp_cas, cp_smiles, cp_name, cp_chinese, cp_formula};
    memcpy(sbt, &cp_sbt, sizeof(substance));
    return sbt;
}

void
substance_dealloc(substance *sbt)
{
    free((void *)sbt->cas);
    free((void *)sbt->smiles);
    free((void *)sbt->name);
    free((void *)sbt->chinese);
    free((void *)sbt->formula);
    free(sbt);
}

static const char *
identity_cas(void *sbt)
{
    return ((substance *)sbt)->cas;
}

static const char *
identity_smiles(void *sbt)
{
    return ((substance *)sbt)->smiles;
}

static const char *
identity_name(void *sbt)
{
    return ((substance *)sbt)->name;
}

static const char *
identity_formula(void *sbt)
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
    db->fp_identity = identity_cas;
    return db;
}

/* Copy & insert sbt into db, return -1 on failure. */
int
db_substance_add(db_substance *db, substance *sbt)
{
    const char *key = db->fp_identity(sbt);

    /* Identity key must not be NULL! */
    if (!key)
        return -1;

    substance *item = hashtable_get(db->ht, key);

    /* Exists then delete first. */
    if (item)
        substance_dealloc(item);

    item = substance_new(
        sbt->cas,
        sbt->smiles,
        sbt->name,
        sbt->chinese,
        sbt->formula
    );

    if (!item)
        return -1;

    return hashtable_set(db->ht, key, item, true);
}

/* Return -1 on failure. */
int
db_substance_del(db_substance *db, const char *key)
{
    substance *item = hashtable_get(db->ht, key);

    if (item) {
        substance_dealloc(item);
        return hashtable_set(db->ht, key, NULL, 1);
    }

    return 0;
}

/* NULL on failure. */
substance *
db_substance_get(db_substance *db, const char *key)
{
    return hashtable_get(db->ht, key);
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
db_substance_identity_string(db_substance *db)
{
    if (db->fp_identity == identity_cas)
        return "cas";

    else if (db->fp_identity == identity_smiles)
        return "smiles";

    else if (db->fp_identity == identity_name)
        return "name";

    else if (db->fp_identity == identity_formula)
        return "formula";
    else
        return "error";  /* Should never get here! */
}
