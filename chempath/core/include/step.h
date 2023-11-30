#ifndef STEP_H
#define STEP_H

#include "dtypes.h"
#include "hashtable.h"
#include "substance.h"

typedef struct
{
    hashtable    *ht;
    db_substance *db;
} db_step;


db_step *db_step_new(db_substance *db);
void     db_step_dealloc(db_step *dbs);


#endif /* STEP_H */
