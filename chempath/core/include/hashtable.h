#ifndef HASHTABLE
#define HASHTABLE

#include <stddef.h>
#include <stdbool.h>

/**
 * Buckets stores: val1, key1, (NULL),..., val2, key2, ...
 */
typedef struct
{
    size_t size;
    size_t used;
    void **buckets;
} hashtable;


hashtable *hashtable_new();
void      *hashtable_get(hashtable *ht, const char *key);
int        hashtable_set(hashtable *ht, const char *key, void *item, bool is_replace);
void       hashtable_dealloc(hashtable *ht);

#endif /* HASHTABLE */
