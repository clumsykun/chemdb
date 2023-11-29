#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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


int main ()
{
    const char *cas = "130339-07-0";
    const char *smiles = "FC(F)OC1=CC=C(C=C1)C(NC2=NC=NC(=C2Cl)C)CC";
    const char *name = "Diflumetorim";
    const char *chinese = "";
    const char *formula = "C<15>H<16>ClF<2>N<3>O";

    db_substance *db = db_substance_new("cas");
    substance sbt = {name, cas, smiles, formula, chinese};
    db_substance_add(db, &sbt);

    return 0;
}

