#ifndef COMMON
#define COMMON


typedef const char *(*fp_get_key)(void *);

struct node
{
    void *item;
    struct node *next;
    struct node *prev;
};

typedef struct
{
    size_t size;
    struct node *data;
    fp_get_key get_key;
} hashtable;

struct stack_node
{
    void *item;
    struct stack_node *next;
};

typedef struct
{
    size_t size;
    struct stack_node *data;
} stack;


hashtable *hashtable_new(size_t size, fp_get_key fp);
void      *hashtable_find(hashtable *ht, const char *key);
int        hashtable_insert(hashtable *ht, const char *key, void *item);
void       hashtable_remove(hashtable *ht, const char *key);

stack *stack_new();
void   stack_del(stack *stk);
int    stack_push(stack *stk, void *item);
void  *stack_pop(stack *stk);

#endif /* COMMON */
