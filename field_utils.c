#ifndef _FIELD_UTILS_
#define _FIELD_UTILS_

// STL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/// Field class
typedef struct field_struct
{
    // Field data
    char _fieldName;
    int _fieldValue;

    // Linked list members
    struct field_struct* _prev;
    struct field_struct* _next;

    // Dot operator overloads
    void (*updateFieldValue)(struct field_struct* field, int newValue);
} Field;

// Updates the value of a field
void update_field_value(Field* field, int newValue)
{
    field->_fieldValue = newValue;
}

// Returns a new field
Field* getField(char fieldName, int fieldValue, Field* prev, Field* next)
{
    Field* newField = malloc(sizeof(Field));

    newField->_fieldName = fieldName;
    newField->_fieldValue = fieldValue;
    newField->_prev = prev;
    newField->_next = next;

    newField->updateFieldValue = update_field_value;

    return newField;
}

#endif