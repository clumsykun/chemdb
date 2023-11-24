#ifndef COMMON
#define COMMON

#include <stddef.h>
#include <stdbool.h>

/* Use this function to get a constant key (const char *) from any hash-able item (void *). */
typedef const char *(*fp_cst_key)(void *);

struct node
{
    void *item;
    struct node *next;
};

typedef struct
{
    size_t size;
    size_t used;
    void **buckets;
    fp_cst_key cst_key;
} hashtable;

typedef struct
{
    size_t size;
    struct node *head;
} stack;


hashtable *hashtable_new(fp_cst_key cst_key);
void      *hashtable_get(hashtable *ht, const char *key);
int        hashtable_set(hashtable *ht, const char *key, void *item, bool is_replace);

stack *stack_new();
void   stack_del(stack *stk);
int    stack_push(stack *stk, void *item);
void  *stack_pop(stack *stk);

#endif /* COMMON */
