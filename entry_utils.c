// STL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "field_utils.c"


/// Entry class
typedef struct entry_struct
{
    // Entry data
    int _id;
    int _numFields;
    Field* _head;
    Field* _tail;

    // Linked list members
    struct entry_struct* _prev;
    struct entry_struct* _next;

    // Dot operator overloads
    void (*addField)(struct entry_struct* entry, char fieldName, int fieldValue);
    int (*removeField)(struct entry_struct* entry, char fieldName);

} Entry;

// Add a field to an entry, will allocate the field automatically
void add_field(Entry* entry, char fieldName, int fieldValue)
{
    Field* last = entry->_tail;

    last->_next = getField(fieldName, fieldValue, last, NULL);

    entry->_tail = last->_next;

    entry->_numFields++;
}

// Remove a field from an entry, returns the number of fields removed that matched
int remove_field(Entry* entry, char fieldName)
{
    if(entry->_numFields == 0) return 0;

    if(entry->_numFields == 1)
    {
        free(entry->_head);
        entry->_head = NULL;
        entry->_tail = NULL;
        entry->_numFields = 0;
        return 1;
    }
    else
    {
        Field* curr = entry->_head;
        int ret = 0;

        if(curr == NULL) return 0;

        while(1)
        {
            // Remove if field name matches
            if(curr->_fieldName == fieldName)
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

// Allocate and return a new entry
Entry* getEntry(int id, Entry* prev, Entry* next)
{
    Entry* newEntry = malloc(sizeof(Entry));
    newEntry->_id = id;
    newEntry->_numFields = 0;
    newEntry->_head = NULL;
    newEntry->_tail = NULL;

    newEntry->_prev = prev;
    newEntry->_next = next;

    newEntry->addField = add_field;
    newEntry->removeField = remove_field;

    return newEntry;
}