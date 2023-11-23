#ifndef COMMON
#define COMMON


typedef const char *(*fp_hash_key)(void *);

struct node
{
    void *item;
    struct node *next;
};

typedef struct
{
    size_t size;
    struct node **buckets;
    fp_hash_key hash_key;
} hashtable;

typedef struct
{
    size_t size;
    struct node *head;
} stack;


hashtable *hashtable_new(fp_hash_key fp);
void      *hashtable_get(hashtable *ht, const char *key);
int        hashtable_set(hashtable *ht, const char *key, void *item);

stack *stack_new();
void   stack_del(stack *stk);
int    stack_push(stack *stk, void *item);
void  *stack_pop(stack *stk);

#endif /* COMMON */
