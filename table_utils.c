// STL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "entry_utils.c"

/// Table class
typedef struct table_struct
{
    // Local members
    char* _tableName;
    int _numEntries;

    // Linked list members
    Entry* _head;
    Entry* _tail;

    // Dot operator overloads
    void (*addEntry)(struct table_struct* table, int id);
    int (*removeEntry)(struct table_struct* table, int id);

} Table;

// Add an entry to a table, will allocate the entry automatically
void add_entry(Table* table, int id)
{
    Entry* last = table->_tail;

    last->_next = getEntry(id, last, NULL);

    table->_tail = last->_next;

    table->_numEntries++;
}

// Remove an entry from a table, returns the number of entries removed that matched
int remove_entry(Table* table, int id)
{
    if(table->_numEntries == 0) return 0;

    if(table->_numEntries == 1)
    {
        free(table->_head);
        table->_head = NULL;
        table->_tail = NULL;
        table->_numEntries = 0;
        return 1;
    }
    else
    {
        Entry* curr = table->_head;
        int ret = 0;

        if(curr == NULL) return 0;

        while(1)
        {
            // Remove if field name matches
            if(curr->_id == id)
            {
                // Move pointers
                if(curr->_prev != NULL) curr->_prev->_next = curr->_next;
                if(curr->_next != NULL) curr->_next->_prev = curr->_prev;

                // Deallocate
                free(curr);

                // Set return val
                ret++;
            }

            // Update pointer
            curr = curr->_next;
            if(curr == NULL) break;
        }
    }

    return 0;
}

// Allocate and return a new table
Table* getTable(char* tableName)
{
    Table* newTable = malloc(sizeof(Table));

    strcpy(newTable->_tableName, tableName);
    newTable->_numEntries = 0;
    newTable->_head = NULL;
    newTable->_tail = NULL;

    newTable->addEntry = add_entry;
    newTable->removeEntry = remove_entry;

    return newTable;
}