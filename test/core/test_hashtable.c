#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "common.h"


typedef struct
{
    const char *key;
    char *value;
} stuff;


const char *
__repr(void *item)
{
    return ((stuff *)item)->key;
}


int main ()
{
    hashtable *ht = hashtable_new(__repr);
    stuff a = {"key 9", "new value"};
    stuff arr[10] = {
        {"key 0", "value 0"},
        {"key 1", "value 1"},
        {"key 2", "value 2"},
        {"key 3", "value 3"},
        {"key 4", "value 4"},
        {"key 5", "value 5"},
        {"key 6", "value 6"},
        {"key 7", "value 7"},
        {"key 8", "value 8"},
        {"key 9", "value 9"},
    };

    for (size_t i = 0; i < 10; i++) {
        hashtable_set(ht, arr[i].key, &arr[i], 1);
    }

    for (size_t i = 0; i < 10; i++) {
        stuff *p = hashtable_get(ht, arr[i].key);
        if (p)
            printf("%s: %s\n", arr[i].key, p->value);
        else
            printf("%s is deleted!\n", arr[i].key);
    }

    hashtable_set(ht, arr[0].key, NULL, 1);
    hashtable_set(ht, arr[1].key, NULL, 1);
    hashtable_set(ht, arr[2].key, NULL, 1);
    hashtable_set(ht, arr[3].key, NULL, 1);
    hashtable_set(ht, arr[4].key, NULL, 1);
    hashtable_set(ht, arr[5].key, NULL, 1);
    hashtable_set(ht, arr[9].key, &a, 1);

    for (size_t i = 0; i < 10; i++) {
        stuff *p = hashtable_get(ht, arr[i].key);
        if (p)
            printf("%s: %s\n", arr[i].key, p->value);
        else
            printf("%s is deleted!\n", arr[i].key);
    }

    hashtable_set(ht, arr[5].key, NULL, 1);
    hashtable_set(ht, arr[6].key, NULL, 1);
    hashtable_set(ht, arr[7].key, NULL, 1);
    hashtable_set(ht, arr[8].key, NULL, 1);
    hashtable_set(ht, arr[9].key, NULL, 1);

    for (size_t i = 0; i < 10; i++) {
        stuff *p = hashtable_get(ht, arr[i].key);
        if (p)
            printf("%s: %s\n", arr[i].key, p->value);
        else
            printf("%s is deleted!\n", arr[i].key);
    }

    hashtable_set(ht, "key 4", "a new string", 1);
    printf("%s: %s\n", "key 4", hashtable_get(ht, "key 4"));

    stack *stk = stack_new();
    stack_push(stk, "string1");
    stack_push(stk, "string2");
    stack_push(stk, "string3");
    stack_push(stk, "string4");
    stack_push(stk, "string5");
    stack_push(stk, "string6");
    stack_push(stk, "string7");
    stack_push(stk, "string8");
    stack_push(stk, "string9");

    while (1) {
        void *item = stack_pop(stk);
        if (!item)
            break;

        printf("%s\n", item);
    }

    return 0;
}

