#ifndef _TABLE_BUILDER_
#define _TABLE_BUILDER_

#include "table_utils.c"

#define MAX_TUPLE_LENGTH 1000

Table* buildTable(const char* fileName, const char* tableName)
{
    // Open file
    FILE* datafile = fopen(fileName, "r");
    char line[1000];
    Table* table = getTable(tableName);
    char delim[] = " ";

    fprintf(stderr, "Opened file, now reading...");

    while(fgets(line, sizeof(line), datafile)) 
    {
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
            fprintf(stderr, "Exiting after completing all requests!\n");

            break;
        }

        // Populate table
        char *ptr = strtok(line, delim);

        while(ptr != NULL)
        {
            fprintf(stderr, "'%s'\n", ptr);
            ptr = strtok(NULL, delim);
        }
    }

    return NULL;
}

#endif