#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "common.h"


#define INIT_HASH_SIZE 2
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
hashtable_new(fp_repr fp)
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
    ht->repr = fp;
    return ht;
}

struct node **
hashtable_get_bucket(hashtable *ht, const char *key)
{
    return ht->buckets + cal_bucket_idx(key, ht->size - 1);
}

static void *
bucket_get(struct node **bucket, fp_repr repr, const char *key)
{
    struct node *nd = *bucket;
    while (1) {

        if (!nd)
            return NULL;

        if (!strcmp(repr(nd->item), key))
            return nd->item;

        nd = nd->next;
    }    
}

static int
bucket_set(struct node **bucket, fp_repr repr, const char *key, void *item)
{
    struct node **pp = bucket, *nd = *bucket;
    while (1) {

        if (!nd) {
            struct node *new = malloc(sizeof(struct node));

            if (!new)
                return -1;

            new->item = item;
            new->next = NULL;
            *pp = new;

            return 0;
        }

        if (!strcmp(repr(nd->item), key))
            return 0;

        pp = &nd->next;
        nd = nd->next;
    }
}

static void
bucket_remove(struct node **bucket, fp_repr repr, const char *key)
{
    struct node **next, **pp = bucket;

    while (*pp) {
        if (!strcmp(repr((*pp)->item), key)) {
            next = &((*pp)->next);
            free(*pp);
            *pp = *next;
        }

        pp = &((*pp)->next);
    }
}

/* NULL on failure. */
void *
hashtable_get(hashtable *ht, const char *key)
{
    return bucket_get(hashtable_get_bucket(ht, key), ht->repr, key);
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
        bucket_remove(bucket, ht->repr, key);
        ht->used--;
        return 0;
    }

    if (bucket_set(bucket, ht->repr, key, item) < 0)
        return -1;

    ht->used++;
    return 0;
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
