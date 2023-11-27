#ifndef COMMON
#define COMMON

#include <stddef.h>
#include <stdbool.h>


struct node
{
    void *item;
    struct node *next;
};

typedef struct
{
    size_t size;
    size_t used;
    void **head;
} stack;


stack *stack_new();
void   stack_dealloc(stack *stk);
int    stack_push(stack *stk, void *item);
void  *stack_pop(stack *stk);

#endif /* COMMON */
