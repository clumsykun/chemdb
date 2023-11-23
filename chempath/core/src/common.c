#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "common.h"


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
hashtable_new(size_t size, fp_get_key fp)
{
    hashtable *ht = malloc(sizeof(hashtable));
    struct node *data = malloc(size * sizeof(struct node));

    if (!(ht && data)) {
        free(ht);
        free(data);
        return NULL;
    }

    ht->data = data;
    ht->size = size;    
    ht->get_key = fp;
    return ht;
}

/* NULL on failure. */
static struct node *
hashtable_find_node(hashtable *ht, const char *key)
{
    struct node *nd = ht->data + __hash((const unsigned char *)key) % ht->size;

    while (1) {

        if (!nd)
            return NULL;
    
        if (!strcmp(ht->get_key(nd->item), key))
            return nd;

        nd = nd->next;
    }
}

/* NULL on failure. */
void *
hashtable_find(hashtable *ht, const char *key)
{
    struct node *nd = hashtable_find_node(ht, key);
    return (nd ? nd->item : NULL);
}

/**
 * Create & insert node into hashtable without check if existed or not,
 * return -1 on failure.
 */
int
hashtable_insert(hashtable *ht, const char *key, void *item)
{
    struct node *root = ht->data + __hash((const unsigned char *)key) % ht->size;

    /* hashtable root empty */
    if (!root) {
        root->prev = NULL;
        root->next = NULL;    
        root->item = item;
        return 0;
    }

    /* create leaf node */
    struct node *nd = malloc(sizeof(struct node));

    if (nd == NULL)
        return -1;

    nd->item = item;

    /* insert */
    if (root->next) {
        struct node *next = root->next;
        nd->prev = root;
        nd->next = next;
        next->prev = nd;
        root->next = nd;
    }
    /* append */
    else {
        nd->prev = root;
        nd->next = NULL;
        root->next = nd;
    }

    return 0;
}

static void
hashtable_del_node(struct node *nd)
{
    if (!nd)
        return;

    struct node *prev = nd->prev;
    struct node *next = nd->next;

    /* case root */
    if (!prev) {

        if (!next) {
            memset(nd, 0, sizeof(struct node));
            return;
        }

        nd->item = next->item;
        nd->next = next->next;

        if (next->next)
            next->next->prev = nd;

        free(next);
        return;
    }

    /* case leaf */
    else {
        /* tail */
        if (!next) {
            prev->next = NULL;
            free(nd);
            return;
        }

        prev->next = next;
        next->prev = prev;
        free(nd);
        return;
    }
}

void
hashtable_remove(hashtable *ht, const char *key)
{
    hashtable_del_node(hashtable_find_node(ht, key));
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
    stk->data = NULL;
}

void
stack_del(stack *stk)
{
    struct stack_node *tmp, *nd = stk->data;

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
    struct stack_node *nd = malloc(sizeof(struct stack_node));

    if (!nd)
        return -1;

    if (!stk->data)
        nd->next = NULL;
    else
        nd->next = stk->data->next;

    nd->item = item;
    stk->data = nd;
    stk->size++;
    return 0;
}

/* Return last input item or NULL. */
void *
stack_pop(stack *stk)
{
    struct stack_node *nd = stk->data;

    if (!nd)
        return NULL;

    void *item = nd->item;
    stk->data = nd->next;
    stk->size--;

    free(nd);
    return item;
}
