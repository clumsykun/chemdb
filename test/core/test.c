#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "common.h"
#include "substance.h"


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


int
string_compare(const void *s1, const void *s2 )
{
    // const char *rec1 = *(char**)str1;
    // const char *rec2 = *(char**)str2;

    return strcmp((const char *)s1, (const char *)s2);
}

int main ()
{
    // const char *cas = "130339-07-0";
    // const char *smiles = "FC(F)OC1=CC=C(C=C1)C(NC2=NC=NC(=C2Cl)C)CC";
    // const char *name = "Diflumetorim";
    // const char *chinese = "";
    // const char *formula = "C<15>H<16>ClF<2>N<3>O";

    // db_substance *db = db_substance_new("cas");
    // substance sbt = {name, cas, smiles, formula, chinese};
    // db_substance_add(db, &sbt);

    size_t size = 5;
    const char **substances = malloc(size * sizeof(const char *));
    substances[0] = "Pyriproxyfen";
    substances[1] = "Bromoacetic acid methyl ester";
    substances[2] = "Methyl chloroacetate";
    substances[3] = "Methyl (2R)-2-(4-hydroxyphenoxy)propanoate";
    substances[4] = "2-sec-Butyl-4,6-dinitrophenyl isopropyl carbonate";

    qsort(substances, size, sizeof(const char *), string_compare);

    for (size_t i = 0; i < 5; i++) {
        printf("%s\n", substances[i]);
    }

    return 0;
}

