#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "hashtable.h"

#define INIT_HASH_SIZE 1024


/* Hash function implement djb2. */
static size_t
__hash(const unsigned char *key)
{
    size_t hash = 5381;
    unsigned char c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static void **
quadratic_probe(hashtable *ht, const char *key)
{
    size_t hash = __hash((const unsigned char *)(key));
    size_t mask = ht->size - 1;
    void **bucket;

    for (size_t idx, i = 0; i < ht->size; i++) {
        idx = ((hash + i * i) & mask);
        bucket = ht->buckets + (idx * 2);

        /* This key is not in the table, return the empty bucket. */
        if (!bucket[0])
            return bucket;

        /* Matched, return the bucket */
        if (!strcmp(bucket[1], key))
            return bucket;
    }

    /* Should never go to here. */
    return NULL;
}

/* NULL on failure. */
hashtable *
hashtable_new()
{
    hashtable *ht = malloc(sizeof(hashtable));
    void **buckets = calloc(INIT_HASH_SIZE * 2, sizeof(void *));

    if (!(ht && buckets)) {
        free(ht);
        free(buckets);
        return NULL;
    }

    ht->buckets = buckets;
    ht->size = INIT_HASH_SIZE;
    ht->used = 0;
    return ht;
}

static int
hashtable_expand(hashtable *ht)
{
    size_t old_size = ht->size;
    size_t new_size = ht->size * 2;
    void **old_buckets = ht->buckets;
    void **new_buckets = calloc(new_size * 2, sizeof(void *));

    if (!new_buckets)
        return -1;

    ht->buckets = new_buckets;
    ht->size = new_size;
    ht->used = 0;

    for (size_t i = 0; i < old_size; i++)
        if (old_buckets[2 * i])
            hashtable_set(ht, old_buckets[2 * i + 1], old_buckets[2 * i], 1);

    free(old_buckets);
    return 0;
}

/* Return NULL on failure. */
void *
hashtable_get(hashtable *ht, const char *key)
{
    if (!key)
        return NULL;

    return *quadratic_probe(ht, key);
}

/* Return -1 on failure. */
int
hashtable_set(hashtable *ht, const char *key, void *item, bool is_replace)
{
    if (!key)
        return -1;

    void **bucket;

    /* Delete */
    if (!item) {
        bucket = quadratic_probe(ht, key);

        /* Clear the bucket */
        if (bucket[0]) {
            bucket[0] = NULL;
            bucket[1] = NULL;
            ht->used--;
        }
    }

    /* Expand if necessary. */
    if (ht->used * 2 >= ht->size)
        if (hashtable_expand(ht) < 0)
            return -1;

    bucket = quadratic_probe(ht, key);

    /* Insert */
    if (!bucket[0]) {
        bucket[0] = item;
        bucket[1] = (void *)key;
        ht->used++;
    }

    /* Replace */
    if (bucket[0] && is_replace)
        bucket[0] = item;

    return 0;
}

void
hashtable_dealloc(hashtable *ht)
{
    free(ht->buckets);
    free(ht);
}
