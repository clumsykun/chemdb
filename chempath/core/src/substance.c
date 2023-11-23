#include <stdlib.h>
#include <string.h>

#include "dtypes.h"
#include "common.h"
#include "substance.h"

#define INIT_DB_SIZE 1024


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
    substance **head = malloc(INIT_DB_SIZE * sizeof(substance *));
    hashtable *ht    = hashtable_new(substance_cas);
    stack *stk       = stack_new();

    if (!(db && head && ht && stk)) {
        free(db);
        free(head);
        free(ht);
        free(stk);
        return NULL;
    }

    db->head = head;
    db->ht = ht;
    db->vacant = stk;
    db->size = INIT_DB_SIZE;
    db->used = 0;
    db->pos = 0;
    return db;
}

static int
db_substance_expand(db_substance *db)
{
    size_t new_size = db->size * 2;
    substance *expanded = malloc(new_size * sizeof(substance *));

    if (expanded == NULL)
        return -1;

    memcpy(expanded, db->head, db->size * sizeof(substance *));
    free(db->head);

    db->head = expanded;
    db->size = new_size;
    return 0;
}

/* Copy & insert sbt into db. */
int
db_substance_set(db_substance *db, substance *sbt)
{
    substance *vacant;
    const char *key = db->get_key(sbt);

    /* existed */
    if (hashtable_get(db->ht, key))
        return 0;

    /* no slots then expand */
    if (db->used == db->size)
        if (db_substance_expand(db))
            return -1;

    vacant = stack_pop(db->vacant);

    /* vacant not available */
    if (!vacant)
        vacant = db->head + db->pos++;

    if (hashtable_set(db->ht, key, vacant))
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
