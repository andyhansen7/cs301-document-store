#ifndef _TUPLE_UTILS_
#define _TUPLE_UTILS_

#define TUPLE_ARRAY_SIZE 26
#define MAX_TUPLE_LENGTH 1000

#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef struct field_struct
{
    int value;
    int valid;
    int printable;
} Field;

// Tuple type
typedef struct tuple_struct
{
    Field data[TUPLE_ARRAY_SIZE];
    char* _printOrder;
    char _sortLabel;

    struct tuple_struct* _next;
    struct tuple_struct* _prev;
} Tuple;

// Create and return a new tuple
Tuple* getNewTuple(int id)
{
    Tuple* newTuple = (Tuple*)malloc(sizeof(Tuple));
    newTuple->_printOrder = malloc(sizeof(char) * (TUPLE_ARRAY_SIZE + 1));
    strcpy(newTuple->_printOrder, "A");

    Field keyField;
    keyField.value = id;
    keyField.valid = 1;
    keyField.printable = 1;
    newTuple->data[0] = keyField;
    newTuple->_sortLabel = 'A';

    for(int i = 1; i < TUPLE_ARRAY_SIZE; i++) 
    {
        newTuple->data[i] = (Field){.value = 0, .valid = 0};
    }
    newTuple->_next = NULL;
    newTuple->_prev = NULL;

    return newTuple;
}

void applyProjectionToTuple(Tuple* tuple, const char* projection)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    for(int i = 0; i < TUPLE_ARRAY_SIZE; i++)
    {
        char label = i + 65;
        if(strchr(projection, label) != NULL)
        {
            tuple->data[i].printable = 1;
        }
        else
        {
            tuple->data[i].printable = 0;
        }
    }     
}

void removeProjectionFromTuple(Tuple* tuple)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    for(int i = 0; i < TUPLE_ARRAY_SIZE; i++)
    {
        tuple->data[i].printable = 1;
    }     
}

void printTuplePretty(Tuple* tuple)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    fprintf(stdout, "{\n");

    for(int i = 0; i < TUPLE_ARRAY_SIZE; i++)
    {
        char label = i + 65;
        if(tuple->data[i].valid == 1 && tuple->data[i].printable == 1) 
        {
            fprintf(stdout, "\t%c: %d\n", label, tuple->data[i].value);
        }
    }

    fprintf(stdout, "}\n");
}

void printTupleOrdered(Tuple* tuple)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    int fieldsPrinted = 0;

    for(int i = 0; i < (strlen(tuple->_printOrder)); i++)
    {
        char label = toupper(tuple->_printOrder[i]);
        char index = label - 65;

        if(tuple->data[index].valid == 1 && tuple->data[index].printable == 1)
        {
            fprintf(stdout, "%c: %d ", label, tuple->data[index].value);
            fieldsPrinted++;
        }
    }

    if(fieldsPrinted != 0)
    {
        fprintf(stdout, "\n");
    }
}

char* serializeTuple(Tuple* tuple)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    char* ret = malloc(sizeof(char) * MAX_TUPLE_LENGTH);
    char* temp = malloc(sizeof(char) * MAX_TUPLE_LENGTH);
    ret[0] = '\0';
    temp[0] = '\0';

    for(int i = 1; i < (strlen(tuple->_printOrder)); i++)
    {
        char label = toupper(tuple->_printOrder[i]);
        char index = label - 65;

        if(tuple->data[index].valid == 1)
        {
            sprintf(temp, "%c: %d ", label, tuple->data[index].value);
            strcat(ret, temp);
            temp[0] = '\0';
        }
    }
    char newline = '\n';
    strncat(ret, &newline, 1);

    return ret;
}


Field getDataByIndex(Tuple* tuple, int index)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    if(index < 0 || index > (TUPLE_ARRAY_SIZE - 1))
    {
        fprintf(stderr, "Invalid array access in Tuple with id %d\n", tuple->data[0].value);
        return (Field){.value = 0, .valid = 0};
    }
    else 
    {
        Field ret = tuple->data[index];

        if(ret.valid == 1) return ret;
        else 
        {
            // fprintf(stderr, "Accessed non-valid field in Tuple with id %d\n", tuple->data[0].value);
            return (Field){.value = 0, .valid = 0};
        }
    }
}

Field getDataByLabel(Tuple* tuple, char index)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    char converted = toupper(index);
    converted -= 65;

    if(converted < 0 || converted > (TUPLE_ARRAY_SIZE - 1))
    {
        fprintf(stderr, "Invalid char index access in Tuple with id %d\n", tuple->data[0].value);
        return (Field){.value = 0, .valid = 0};
    }
    else
    {
        return getDataByIndex(tuple, converted);
    }
}

int getID(Tuple* tuple)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);

    return tuple->data[0].value;
}

int updateTupleField(Tuple* tuple, char index, int value)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);
    
    if(index == 'A' || index == 'a')
    {
        fprintf(stderr, "Cannot update _id field! Tuple %d\n", tuple->data[0].value);
        return -1;
    }

    char label = (char)(toupper(index));
    char converted = (char)(label - 65);

    if(converted < 1 || converted > (TUPLE_ARRAY_SIZE - 1))
    {
        fprintf(stderr, "Invalid array access in update Tuple with id %d\n", tuple->data[0].value);
        return -1;
    }
    else 
    {
        tuple->data[converted].value = value;
        tuple->data[converted].valid = 1;
        tuple->data[converted].printable = 1;

        strncat(tuple->_printOrder, &label, 1);

        return 0;
    }
}

int deleteTupleField(Tuple* tuple, char index)
{
    assert(tuple != NULL);
    assert(tuple->data[0].valid == 1);
    
    if(index == 'A' || index == 'a')
    {
        fprintf(stderr, "Cannot delete _id field! Tuple %d\n", tuple->data[0].value);
        return -1;
    }

    char converted = toupper(index);
    converted -= 65;

    if(converted < 1 || converted > (TUPLE_ARRAY_SIZE - 1))
    {
        fprintf(stderr, "Invalid array access in delete Tuple with id %d\n", tuple->data[0].value);
        return -1;
    }
    else 
    {
        tuple->data[converted].valid = 0;
        return 0;
    }
}

int hasField(Tuple* tuple, char index)
{
    return (getDataByLabel(tuple, index).valid == 1);
}

Tuple* copyTuple(Tuple* original)
{
    assert(original != NULL);

    Tuple* result = getNewTuple(getID(original));
    result->_printOrder = original->_printOrder;
    result->_sortLabel = original->_sortLabel;

    for(int i = 1; i < TUPLE_ARRAY_SIZE; i++)
    {
        result->data[i] = original->data[i];
    }

    return result;
}

#endif