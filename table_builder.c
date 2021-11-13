#ifndef _TABLE_BUILDER_
#define _TABLE_BUILDER_

#include "table_utils.h"
#include "id_builder.h"

#define MAX_TUPLE_LENGTH 1000

Table* buildTable(char* fileName, char* tableName)
{
    // Open file
    FILE* datafile = fopen(fileName, "r");
    char line[1000];
    Table* table = getNewTable(tableName);
    char delim[] = " ";
    IDGen* generator = getIDGen();

    while(fgets(line, sizeof(line), datafile)) 
    {
        // fprintf(stderr, "Line read in: %s\n", line);
        
        size_t eol = strcspn(line, "\n");
        line[eol] = '\0';

        // Line size constraint enforcement
        if(eol >= MAX_TUPLE_LENGTH)
        {
            fprintf(stderr, "Read line too long: %.50s\n", line);
            break;
        }

        else if(strlen(line) < 2)
        {
//            fprintf(stderr, "Exiting after reading all lines!\n");
            break;
        }

        // Populate table
        int newID = getNewID(generator);
        Tuple* newTuple = getNewTuple(newID);

        char *ptr_key = strtok(line, delim);
        char *ptr_val = strtok(NULL, delim);

        while(ptr_key != NULL && ptr_val != NULL)
        {
            // Create entry
            char key = ptr_key[0];
            int val = atoi(ptr_val);

            // Add field to tuple
            updateTupleField(newTuple, key, val);

            // Get next values
            ptr_key = strtok(NULL, delim);
            ptr_val = strtok(NULL, delim);
        }

        // Add new tuple to table
        addTupleToTable(table, newTuple);
    }

    fclose(datafile);

    return table;
}

#endif