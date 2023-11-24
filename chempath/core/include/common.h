#ifndef COMMON
#define COMMON

#include <stddef.h>

typedef const char *(*fp_repr)(void *);

struct node
{
    void *item;
    struct node *next;
};

typedef struct
{
    size_t size;
    size_t used;
    struct node **buckets;
    fp_repr repr;
} hashtable;

typedef struct
{
    size_t size;
    struct node *head;
} stack;


hashtable *hashtable_new(fp_repr fp);
void      *hashtable_get(hashtable *ht, const char *key);
int        hashtable_set(hashtable *ht, const char *key, void *item);

stack *stack_new();
void   stack_del(stack *stk);
int    stack_push(stack *stk, void *item);
void  *stack_pop(stack *stk);

#endif /* COMMON */
