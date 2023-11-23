#include <stdlib.h>
#include <string.h>
#include "substance.h"
#include "common.h"

#define DB_INIT_SIZE 256


/**
 * vacant:             (root) --> (node) --> ... --> (tail) --> NULL
 * hashtable: NULL <-- (root) <=> (node) <=> ... <=> (tail) --> NULL
 */

typedef struct
{
    substance *data;
    hashtable *data_ht;
    stack     *vacant;
    size_t     size;
    size_t     used;
    size_t     pos;
    fp_hash_key get_key;  /* Choose one char * property of substance as hashtable key. */
} db_substance;

static const char *
substance_cas(void *sbt)
{
    return ((substance *)sbt)->cas;
}

/* NULL on failure */
db_substance *
db_substance_new()
{
    db_substance *db = malloc(sizeof(db_substance));
    substance *data = malloc(DB_INIT_SIZE * sizeof(substance));
    hashtable *data_ht = hashtable_new(db->size, substance_cas);
    stack *stk = stack_new();

    if (!(db && data && data_ht && stk)) {
        free(db);
        free(data);
        free(data_ht);
        free(stk);
        return NULL;
    }

    db->data = data;
    db->data_ht = data_ht;
    db->vacant = stk;
    db->size = DB_INIT_SIZE;
    db->used = 0;
    db->pos = 0;
    return db;
}

// static int
// db_substance_expand(db_substance *db)
// {
//     size_t new_size = db->size*2;
//     substance *expanded = malloc(new_size*sizeof(substance));

//     if (expanded == NULL)
//         return -1;

//     memcpy(expanded, db->data, db->size*sizeof(substance));
//     free(db->data);

//     db->data = expanded;
//     db->size = new_size;
//     return 0;
// }

/* Copy & insert sbt into db. */
int
db_substance_insert(db_substance *db, substance *sbt)
{
    substance *vacant;
    const char *key = db->get_key(sbt);

    /* existed */
    if (hashtable_find(db->data_ht, key))
        return 0;

    // /* no slots then expand */
    // if (db->used == db->size)
    //     if (db_substance_expand())
    //         return -1;

    vacant = stack_pop(db->vacant);

    /* vacant not available */
    if (!vacant)
        vacant = db->data + db->pos;

    if (hashtable_insert(db->data_ht, key, vacant))
        return -1;

    memcpy(vacant, sbt, sizeof(substance));
    db->used++;
    return 0;
}

int
db_substance_remove(db_substance *db, const char *key)
{
    substance *sbt = hashtable_find(db->data_ht, key);

    /* not found */
    if (!sbt)
        return 0;

    hashtable_remove(db->data_ht, key);
    stack_push(db->vacant, sbt);
    memset(sbt, 0, sizeof(substance));
    return 0;
}

substance *
db_substance_find(db_substance *db, const char *key)
{
    return (substance *) hashtable_find(db->data_ht, key);
}
