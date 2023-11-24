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

// static void
// hashtable_expand(hashtable *ht)
// {
//     size_t new_size = ht->size * 2;
//     void **new_buckets = calloc(new_size, sizeof(void *));

//     for (size_t i = 0; i < ht->size; i++) {
        
//     }
    




// }

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
