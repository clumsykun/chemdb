#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "common.h"


#define INIT_HASH_SIZE 1024
#define cal_bucket_idx(key, mask) (__hash((const unsigned char *)(key)) & (size_t)(mask))

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

/* NULL on failure. */
hashtable *
hashtable_new(fp_cst_key cst_key)
{
    hashtable *ht = malloc(sizeof(hashtable));
    void **buckets = calloc(INIT_HASH_SIZE, sizeof(void *));

    if (!(ht && buckets)) {
        free(ht);
        free(buckets);
        return NULL;
    }

    ht->buckets = buckets;
    ht->size = INIT_HASH_SIZE;
    ht->cst_key = cst_key;
    ht->used = 0;
    return ht;
}

static void **
quadratic_probe(hashtable *ht, const char *key)
{
    size_t hash = __hash((const unsigned char *)(key));
    size_t mask = ht->size - 1;
    void **bucket;

    for (size_t i = 0; i < ht->size; i++) {
        bucket = ht->buckets + ((hash + i * i) & mask);

        /* This key is not in the table, return the empty bucket. */
        if (!(*bucket))
            return bucket;

        /* Matched, return the bucket */
        if (!strcmp(ht->cst_key(*bucket), key))
            return bucket;
    }
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

    void **bucket = quadratic_probe(ht, key);

    if (item) {

        /* Empty bucket. */
        if (!(*bucket)) {
            *bucket = item;
            ht->used++;
        }

        /* Replace. */
        if ((*bucket) && is_replace)
            *bucket = item;
    }

    else {

        /* Clear the bucket */
        if (bucket) {
            *bucket = NULL;
            ht->used--;
        }
    }
}

static void
hashtable_del(hashtable *ht)
{
    struct node *nd, *tmp;
    struct node **bucket = ht->buckets;

    for (size_t i = 0; i < ht->size; i++, bucket++) {
        nd = *bucket;

        while (nd) {
            tmp = nd;
            nd = nd->next;
            free(tmp);
        }
    }
}


/** ================================================================================================
 * Stack, last in first out
 */

/* NULL on failure. */
stack *
stack_new()
{
    stack *stk = malloc(sizeof(stack));

    if (!stk)
        return NULL;

    stk->size = 0;
    stk->head = NULL;
}

void
stack_del(stack *stk)
{
    struct node *tmp, *nd = stk->head;

    while (nd) {
        tmp = nd;
        nd = nd->next;
        free(tmp);
    }

    free(stk);
}

int
stack_push(stack *stk, void *item)
{
    struct node *tmp, *nd = malloc(sizeof(struct node));

    if (!nd)
        return -1;

    nd->next = stk->head;
    nd->item = item;
    stk->head = nd;
    stk->size++;
    return 0;
}

/* Return last input item or NULL. */
void *
stack_pop(stack *stk)
{
    struct node *nd = stk->head;

    if (!nd)
        return NULL;

    void *item = nd->item;
    stk->head = nd->next;
    stk->size--;

    free(nd);
    return item;
}
