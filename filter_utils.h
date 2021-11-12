#ifndef _FILTER_UTILS_
#define _FILTER_UTILS_

#include <assert.h>

#include "tuple_utils.h"
#include "table_utils.h"

Table* filterTable(Table* table, int(*filterFunction)(Tuple*, char, int), char index, int filterValue, char* resultName)
{
    assert(table != NULL);

    Table* result = getNewTable(resultName);

    Tuple* curr = table->_head;
    while(curr != NULL)
    {
        if(hasField(curr, index))
        {
            if(filterFunction(curr, index, filterValue) == 1)
            {
                addTupleToTable(result, copyTuple(curr));
            }
        }

        curr = curr->_next;
    }

    return result;
}

int eq(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);

    if(f.valid == 1 && f.value == value) 
        return 1;
    else
        return 0;
}

int lt(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);
    
    if(f.valid == 1 && f.value < value) 
        return 1;
    else
        return 0;
}

int lteq(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);
    
    if(f.valid == 1 && f.value <= value) 
        return 1;
    else
        return 0;
}

int gt(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);
    
    if(f.valid == 1 && f.value > value) 
        return 1;
    else
        return 0;
}

int gteq(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);
    
    if(f.valid == 1 && f.value >= value) 
        return 1;
    else
        return 0;
}

int nnull(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);

    Field f = getDataByLabel(tuple, index);
    
    if(f.valid == 1) 
        return 1;
    else
        return 0;
}

int 

#endif