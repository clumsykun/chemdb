#include <stdlib.h>
#include <string.h>
#include "substance.h"
#include "common.h"

#define DB_INIT_SIZE 256
#define DB_HASH_RATIO 4


/**
 * vacant:             (root) --> (node) --> ... --> (tail) --> NULL
 * hashtable: NULL <-- (root) <=> (node) <=> ... <=> (tail) --> NULL
 */
typedef struct
{
    substance  *data;
    size_t      size;
    size_t      used;
    size_t      pos;
    struct pnode *vacant;
    struct pnode *hashtable;
} db_substance;

substance  __DATA[DB_INIT_SIZE];
struct ids __HASH_TABLE[DB_INIT_SIZE/DB_HASH_RATIO] = {0};
db_substance DB_SUBSTANCE = {__DATA, DB_INIT_SIZE, 0, 0, NULL, __HASH_TABLE};


inline static const char *
__substance_key(substance *b)
{
    return (const char *) b->cas;
}

inline static size_t
__hash_value(const char *key)
{
    return hash((const unsigned char *)key) % (DB_SUBSTANCE.size/DB_HASH_RATIO);
}

/* NULL on failure. */
static struct pnode *
hashtable_find(const char *key)
{
    struct pnode *node = DB_SUBSTANCE.hashtable + __hash_value(key);

    /* root empty */
    if (!node)
        return NULL;

    while (node) {        
        if (!strcmp(__substance_key((substance *)node->p), key))
            return node;

        node = node->next;
    }

    return NULL;
}

/**
 * Create & insert node into hashtable without check if existed or not,
 * return hashtable node address, NULL on failure.
 */
static int
hashtable_insert(const char *key, substance *item)
{
    struct pnode *hashtable = DB_SUBSTANCE.hashtable;
    struct pnode *root = hashtable + __hash_value(key);
    struct pnode *node = malloc(sizeof(struct pnode));

    if (node == NULL)
        return NULL;

    node->p = (void *)item;

    /* hashtable slot empty */
    if (!root) {
        node->prev = NULL;
        node->next = NULL;    
        *root = *node;
        return 0;
    }

    /* insert */
    if (root->next) {
        struct pnode *next = root->next;
        node->prev = root;
        node->next = next;
        next->prev = node;
        root->next = node;
    }
    /* append */
    else {
        node->prev = root;
        node->next = NULL;
        root->next = node;
    }

    return 0;
}

static void
hashtable_del(struct pnode *node)
{
    // /* case root */
    // if (!node->prev) {
    //     memset(node, 0, sizeof(struct pnode));
    //     return;
    // }

    // /* case leaf */
    // else {
    //     if (node->next)
    //         node->next->prev = node->prev;

    //     node->prev->next = node->next;
    //     free(node);
    //     return;
    // }
}

static int
db_substance_expand()
{
    db_substance *db = &DB_SUBSTANCE;
    size_t new_size = db->size*2;
    substance *expanded = malloc(new_size*sizeof(substance));

    if (expanded == NULL)
        return -1;

    memcpy(expanded, db->data, db->size*sizeof(substance));
    free(db->data);

    db->data = expanded;
    db->size = new_size;
    return 0;
}

int
db_substance_add(substance *target)
{
    db_substance *db = &DB_SUBSTANCE;
    struct ids *p = db->vacant;

    /* existed */
    if (hashtable_find(target))
        return 0;

    /* no slots then expand */
    if (db->used == db->size)
        if (db_substance_expand())
            return -1;

    /* vacant available */
    if (p) {
        if (hashtable_insert(target, p->id))
            return -1;

        memcpy(db->data + p->id, target, sizeof(substance));
        db->used++;
        db->vacant = db->vacant->next;
        free(p);
    }
    /* no vacant */
    else {
        if (hashtable_insert(target, db->data + db->pos))
            return -1;

        memcpy(db->data + db->pos++, target, sizeof(substance));
        db->used++;
    }

    return 0;
}

int
db_substance_del(substance *target)
{
    db_substance *db = &DB_SUBSTANCE;
    struct ids *vacant, *node = hashtable_find(target);

    /* not found */
    if (!node)
        return 0;

    vacant = malloc(sizeof(struct ids));

    if (!vacant)
        return -1;

    memset(db->data + node->id, 0, sizeof(substance));    
    vacant->id = node->id;
    vacant->next = db->vacant;
    vacant->prev = NULL;
    db->vacant = vacant;
    hashtable_del(node);
    return 0;
}
