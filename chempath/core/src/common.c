#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "common.h"


#define INIT_HASH_SIZE 1024
#define INIT_STK_SIZE 4


/** ================================================================================================
 * Hash table, use djb2 hash.
 */

static size_t
__hash(const unsigned char *key)
{
    size_t hash = 5381;
    unsigned char c;

    while (c = *key++)
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
    size_t new_size = ht->size * 2;
    void **old_buckets = ht->buckets;
    void **new_buckets = calloc(new_size * 2, sizeof(void *));

    if (!new_buckets)
        return -1;

    ht->buckets = new_buckets;
    ht->size = new_size;
    ht->used = 0;

    for (size_t idx = 0; idx < ht->size * 2; idx += 2)
        if (old_buckets[idx])
            hashtable_set(ht, old_buckets[idx + 1], old_buckets[idx], 1);

    free(old_buckets);
    return 0;
}

/* NULL on failure. */
void *
hashtable_get(hashtable *ht, const char *key)
{
    return *quadratic_probe(ht, key);
}

/**
 * Create & insert node into hashtable without check if existed or not,
 * return -1 on failure.
 */
int
hashtable_set(hashtable *ht, const char *key, void *item, bool is_replace)
{
    if (!key)
        return 0;

    if (ht->used * 2 >= ht->size)
        if (hashtable_expand(ht) < 0)
            return -1;

    void **bucket = quadratic_probe(ht, key);

    if (item) {

        /* Insert empty bucket. */
        if (!bucket[0]) {
            bucket[0] = item;
            bucket[1] = (void *)key;
            ht->used++;
        }

        /* Replace. */
        if (bucket[0] && is_replace)
            bucket[0] = item;
    }

    else {

        /* Clear the bucket */
        if (bucket[0]) {
            bucket[0] = NULL;
            bucket[1] = NULL;
            ht->used--;
        }
    }
}

static void
hashtable_del(hashtable *ht)
{
    free(ht->buckets);
    free(ht);
}


/** ================================================================================================
 * Stack, last in first out
 */

/* NULL on failure. */
stack *
stack_new()
{
    stack *stk = malloc(sizeof(stack));
    void **head = calloc(INIT_STK_SIZE, sizeof(void *));

    if (!(head && stk))
        return NULL;

    stk->size = INIT_STK_SIZE;
    stk->used = 0;
    stk->head = head;
}

void
stack_del(stack *stk)
{
    free(stk->head);
    free(stk);
}

static int
stack_expand(stack *stk)
{

    size_t new_size = stk->size + INIT_STK_SIZE;
    void **old_head = stk->head;
    void **new_head = calloc(new_size, sizeof(void *));

    if (!new_head)
        return -1;

    stk->head = new_head;
    memcpy(stk->head, old_head, stk->size * sizeof(void *));
    stk->size = new_size;
    free(old_head);
    return 0;
}

int
stack_push(stack *stk, void *item)
{
    if (stk->used == stk->size)
        if (stack_expand(stk) < 0)
            return -1;

    memcpy(stk->head+1, stk->head, stk->used * sizeof(void *));
    stk->head[0] = item;
    stk->used++;
    return 0;
}

/* Return last input item or NULL. */
void *
stack_pop(stack *stk)
{
    void *item = stk->head[0];
    memcpy(stk->head, stk->head+1, (stk->used - 1) * sizeof(void *));
    stk->head[stk->used - 1] = 0;
    stk->used--;
    return item;
}
