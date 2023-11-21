#include <stddef.h>
#include "common.h"


/* djb2 hash */
size_t
hash(const unsigned char *str)
{
    size_t hash = 5381;
    unsigned char c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
