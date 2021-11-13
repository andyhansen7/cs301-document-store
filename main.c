// STL
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "table_utils.h"
#include "table_builder.c"
#include "filter_utils.h"
#include "query_builder.h"

#define MAX_FILEPATH_LENGTH 100
#define MAX_DB_NAME_LENGTH 20

int interrupted = 0;
Table* table;
char* filepath;

void signalHandle(int code)
{
    // Kill loop
    interrupted = 1;

    // Terminal cleanup
    fprintf(stdout, "\n");

    // Dump to file
    removeProjectionsFromTable(table);
    char* serialized = serializeTable(table);
    FILE* outfile = fopen(filepath, "w");
    fputs(serialized, outfile);
    fclose(outfile);

    // Exit
    exit(code);
}

int main(void)
{
    // Signal handler
    signal(SIGINT, signalHandle);

    // Load filepath from user
    filepath = malloc(sizeof(char) * MAX_FILEPATH_LENGTH);
    char* dbname = malloc(sizeof(char) * MAX_DB_NAME_LENGTH);
    fprintf(stdout, "CS301 Document Store - Andy Hansen\n");
    fprintf(stdout, "Enter a filepath to load db or \'.\' to load default path: > ");
    fscanf(stdin, "%s", filepath);

    // If zero length, use default path
    if(strcmp(filepath, ".") == 0) strcpy(filepath, "db.txt");

    if(!access(filepath, F_OK|R_OK|W_OK ) == 0 )
    {
        fprintf(stderr, "Provided file does not exist: %s\n", filepath);
        exit(-1);
    }

    // Load db name from user
    fprintf(stdout, "Enter a db name: > ");
    fscanf(stdin, "%s", dbname);

    // Load table and query builder
    table = buildTable(filepath, dbname);
    QueryBuilder* qb = getNewQueryBuilder(table);
    char* query = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);
    query[0] = '\0';
    Table* response = NULL;

    // Main loop to get user input and run
    int done = 1;
    while(interrupted == 0)
    {
        // Print prompt if finished
        if(done == 1)
        {
            fprintf(stdout, "%s: > ", dbname);
            done = 0;
        }

        char input = (char)getchar();
        strncat(query, &input, 1);

        // Run query
        if(input == ';')
        {
            done = 1;
            response = getTableFromQueryString(qb, query);

            if(response != NULL)
            {
                if(response->_numFields != 0) printTableInOrder(response);
                else fprintf(stdout, "\n{ No results }\n");
            }
            else
            {
                fprintf(stdout, "Could not process query: %s\n", query);
            }

            // Reset query string
            free(query);
            query = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);
            query[0] = '\0';
        }

    }
    printTableInOrder(table);

    signalHandle(0);
}