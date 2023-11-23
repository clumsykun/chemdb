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
hashtable_new(fp_hash_key fp)
{
    hashtable *ht = malloc(sizeof(hashtable));
    struct node **buckets = calloc(INIT_HASH_SIZE, sizeof(struct node *));

    if (!(ht && buckets)) {
        free(ht);
        free(buckets);
        return NULL;
    }

    ht->buckets = buckets;
    ht->size = INIT_HASH_SIZE;
    ht->used = 0;
    ht->hash_key = fp;
    return ht;
}

struct node **
hashtable_get_bucket(hashtable *ht, const char *key)
{
    return ht->buckets + cal_bucket_idx(key, ht->size - 1);
}

static void *
bucket_get(struct node **bucket, fp_hash_key fp, const char *key)
{
    struct node *nd = *bucket;
    while (1) {

        if (!nd)
            return NULL;

        if (!strcmp(fp(nd->item), key))
            return nd->item;

        nd = nd->next;
    }    
}

static int
bucket_set(struct node **bucket, fp_hash_key fp, const char *key, void *item)
{
    struct node *nd = malloc(sizeof(struct node));

    if (!nd)
        return -1;

    nd->item = item;
    nd->next = *bucket;
    *bucket = nd;
    return 0;
}

/* NULL on failure. */
void *
hashtable_get(hashtable *ht, const char *key)
{
    return bucket_get(hashtable_get_bucket(ht, key), key, ht->hash_key);
}

/**
 * Create & insert node into hashtable without check if existed or not,
 * return -1 on failure.
 */
int
hashtable_set(hashtable *ht, const char *key, void *item)
{
    struct node **bucket = hashtable_get_bucket(ht, key);

    if (!item) {
        struct node **next, **pp = bucket;

        while (*pp) {
            if (!strcmp(ht->hash_key((*pp)->item), key)) {
                *next = (*pp)->next;
                free(*pp);
                *pp = *next;
                ht->used--;
                return 0;
            }

            pp = &((*pp)->next);
        }

        return 0;
    }

    struct node *nd = *bucket;

    if (bucket_get(bucket, ht->hash_key, key))
        return 0;

    ht->used++;
    return bucket_set(bucket, ht->hash_key, key, item);
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
