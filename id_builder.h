#ifndef _ID_BUILDER_
#define _ID_BUILDER_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct id_gen
{
    int current;
} IDGen;

int getNewID(IDGen* gen)
{
    int ret = gen->current;
//    gen->current++;
    gen->current--;
    return ret;
}

IDGen* getIDGen()
{
    IDGen* gen = malloc(sizeof(IDGen));
//    gen->current = 0;
    gen->current = 100;


    return gen;
}

#endif