#include <stdlib.h>
#include <string.h>
#include "dtypes.h"
#include "common.h"


#define INIT_STK_SIZE 4


/** ================================================================================================
 * Stack, last in first out
 */

/* NULL on failure. */
stack *
stack_new()
{
    stack *stk = malloc(sizeof(stack));
    void **head = calloc(INIT_STK_SIZE, sizeof(void *));

    if (!(head && stk))
        return NULL;

    stk->size = INIT_STK_SIZE;
    stk->used = 0;
    stk->head = head;
    return stk;
}

void
stack_dealloc(stack *stk)
{
    free(stk->head);
    free(stk);
}

static int
stack_expand(stack *stk)
{

    size_t new_size = stk->size + INIT_STK_SIZE;
    void **old_head = stk->head;
    void **new_head = calloc(new_size, sizeof(void *));

    if (!new_head)
        return -1;

    stk->head = new_head;
    memcpy(stk->head, old_head, stk->size * sizeof(void *));
    stk->size = new_size;
    free(old_head);
    return 0;
}

/* Return -1 on failure. */
int
stack_push(stack *stk, void *item)
{
    if (stk->used == stk->size)
        if (stack_expand(stk) < 0)
            return -1;

    memcpy(stk->head+1, stk->head, stk->used * sizeof(void *));
    stk->head[0] = item;
    stk->used++;
    return 0;
}

/* Return last input item or NULL. */
void *
stack_pop(stack *stk)
{
    void *item = stk->head[0];

    if (!item)
        return NULL;

    memcpy(stk->head, stk->head+1, (stk->used - 1) * sizeof(void *));
    stk->head[stk->used - 1] = 0;
    stk->used--;
    return item;
}
