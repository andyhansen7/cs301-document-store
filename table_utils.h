#ifndef _TABLE_UTILS_
#define _TABLE_UTILS_

// STL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tuple_utils.h"

/// Table class
typedef struct table_struct
{
    // Local members
    char* _tableName;
    int _numFields;

    // Linked list members
    Tuple* _head;
    Tuple* _tail;
} Table;

// Allocate and return a new table
Table* getNewTable(const char* tableName)
{
    Table* newTable = (Table*)malloc(sizeof(Table));
    newTable->_tableName = (char*)malloc(strlen(tableName + 1));

    strcpy(newTable->_tableName, tableName);
    newTable->_numFields = 0;
    newTable->_head = NULL;
    newTable->_tail = NULL;

    return newTable;
}

void printTable(Table* table)
{
    assert(table != NULL);

    Tuple* curr = table->_head;

    fprintf(stdout, "Printing table %s:\n", table->_tableName);
    while(curr != NULL)
    {
        printTuple(curr);
        curr = curr->_next;
    }
}

int addTupleToTable(Table* table, Tuple* tuple)
{
    assert(table != NULL);
    assert(tuple != NULL);

    if(table->_head == NULL)
    {
        table->_head = tuple;
        table->_numFields++;
        return 0;
    }
    else if(table->_tail == NULL)
    {
        table->_tail = tuple;
        tuple->_prev = table->_head;
        table->_head->_next = tuple;

        table->_numFields++;
        return 0;
    }
    else
    {
        Tuple* prev = table->_tail;
        tuple->_prev = prev;
        prev->_next = tuple;
        table->_tail = tuple;

        table->_numFields++;
        return 0;
    }
}


#endif