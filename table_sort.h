#ifndef _TABLE_SORT_
#define _TABLE_SORT_

#include "table_utils.h"

Tuple* sortMerge(Tuple* a, Tuple* b, char label)
{
    Tuple* result = NULL;

    // Base cases
    if(a == NULL) return (b);
    else if(b == NULL) return (a);

    // Recurse on either a or b
    char converted = toupper(label);
    converted -= 65;

    // b is larger
    if(a->data[converted].value <= b->data[converted].value)
    {
        result = a;
        result->_next = sortMerge(a->_next, b, label);
    }
    // a is larger
    else
    {
        result = b;
        result->_next = sortMerge(a, b->_next, label);
    }
    return (result);
}

void splitFrontBack(Tuple* source, Tuple** front, Tuple** back)
{
    Tuple* fast = source->_next;
    Tuple* slow = source;

    // Advance 'fast' two nodes, and advance 'slow' one node
    while (fast != NULL)
    {
        fast = fast->_next;
        if(fast != NULL)
        {
            slow = slow->_next;
            fast = fast->_next;
        }
    }

    *front = source;
    *back = slow->_next;
    slow->_next = NULL;
}

void sortTuples(Tuple** headTuple, char label)
{
    Tuple* head = *headTuple;
    Tuple* a;
    Tuple* b;

    // Base case - 0/1 tuples
    if((head == NULL) || (head->_next == NULL)) return;

    // Split head into a and b sublists
    splitFrontBack(head, &a, &b);

    // Recursively sort sublists
    sortTuples(&a, label);
    sortTuples(&b, label);

    // Merge the two sorted lists together
    *headTuple = sortMerge(a, b, label);
}

void reverseList(Tuple** head)
{
    Tuple* current = *head;
    Tuple* prev = NULL;
    Tuple* next = NULL;

    while(current != NULL)
    {
        next = current->_next;
        current->_next = prev;
        prev = current;
        current = next;
    }

    *head = prev;
}

Table* sortTable(Table* table, char label, int order)
{
    assert(table != NULL);
    assert(order != 0);

    // Return if less than 2 nodes
    if(table->_numFields < 2) return table;

    // Run sort using helper functions
    sortTuples(&(table->_head), label);

    // Reverse linked list if in descending order
    if(order == -1)
    {
        reverseList(&(table->_head));
    }

    // Mergesort only sorts one direction, set prev pointers because why not
    Tuple* first = table->_head;
    Tuple* second = first->_next;
    while(second != NULL)
    {
        second->_prev = first;
        first = second;
        second = second->_next;
    }

//    fprintf(stdout, "Running sort on table %s with label %c\n", table->_tableName, label);
//    Tuple* curr = table->_head;
//    while(curr != NULL)
//    {
//        char converted = toupper(label);
//        converted -= 65;
//        fprintf(stdout, "%d\n", curr->data[converted].value);
//        curr = curr->_next;
//    }

    return table;
}

#endif //_TABLE_SORT_
