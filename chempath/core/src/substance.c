#include <stdlib.h>
#include <string.h>

#include "dtypes.h"
#include "substance.h"

#define INIT_DB_SIZE 1024
#define IDENTITY_NAME_NAME "name"
#define IDENTITY_NAME_CAS "cas"
#define IDENTITY_NAME_SMILES "smiles"
#define IDENTITY_NAME_FORMULA "formula"


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
    substance cp_sbt = {cp_name, cp_cas, cp_smiles, cp_formula, cp_chinese};
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
db_substance_new(const char *identity_name)
{
    db_substance *db = malloc(sizeof(db_substance));
    hashtable *ht    = hashtable_new();

    if (!(db && ht))
        goto db_substance_new_failed;

    if (!strcmp(identity_name, IDENTITY_NAME_NAME)) {
        db->fp_identity = substance_name;
        db->identity_name = IDENTITY_NAME_NAME;
    }

    else if (!strcmp(identity_name, IDENTITY_NAME_CAS)) {
        db->fp_identity = substance_cas;
        db->identity_name = IDENTITY_NAME_CAS;
    }

    else if (!strcmp(identity_name, IDENTITY_NAME_SMILES)) {
        db->fp_identity = substance_smiles;
        db->identity_name = IDENTITY_NAME_SMILES;
    }

    else if (!strcmp(identity_name, IDENTITY_NAME_FORMULA)) {
        db->fp_identity = substance_formula;
        db->identity_name = IDENTITY_NAME_FORMULA;
    }

    else
        goto db_substance_new_failed;

    db->ht = ht;
    return db;

db_substance_new_failed:
    free(db);
    hashtable_dealloc(ht);
    return NULL;
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

/* Clear all key-item pairs in hashtable. */
static void
db_substance_clear(db_substance *db)
{
    if (!db->ht)
        return;

    for (size_t i = 0; i < db->ht->size; i++) {

        void **bucket = db->ht->buckets + (i * 2);

        if (bucket[0]) {
            substance_dealloc(bucket[0]);
            bucket[0] = NULL;
            bucket[1] = NULL;
        }
    }
}

void
db_substance_dealloc(db_substance *db)
{
    if (!db)
        return;

    db_substance_clear(db);
    hashtable_dealloc(db->ht);
    free(db);
}
