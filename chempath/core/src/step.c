#include <stdlib.h>
#include "step.h"


// static step *
// step_new()
// {

// }

static void
step_dealloc(step *st)
{

}

/* NULL on failure */
db_step *
db_step_new(db_substance *db)
{
    db_step *dbs = malloc(sizeof(db_step));
    hashtable *ht = hashtable_new();

    if (!(db && dbs && ht)) {
        hashtable_dealloc(ht);
        return NULL;
    }

    dbs->db = db;
    dbs->ht = ht;
    return dbs;
}

static void
db_step_clear(db_step *dbs)
{
    if (!dbs->ht)
        return;

    for (size_t i = 0; i < dbs->ht->size; i++) {

        void **bucket = dbs->ht->buckets + (i * 2);

        if (bucket[0]) {
            step_dealloc(bucket[0]);
            bucket[0] = NULL;
            bucket[1] = NULL;
        }
    }
}

void
db_step_dealloc(db_step *dbs)
{
    if (!dbs)
        return;

    db_step_clear(dbs);
    hashtable_dealloc(dbs->ht);
    free(dbs);
}

// int
// db_step_add()
// {

// }