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
    exit(0);
}

int main(void)
{
    // Signal handler
    signal(SIGINT, signalHandle);

    // Load filepath from user
    filepath = malloc(sizeof(char) * MAX_FILEPATH_LENGTH);
    fprintf(stdout, "CS301 Document Store - Andy Hansen\n");
    fprintf(stdout, "Enter a filepath to load db: > ");
    fscanf(stdin, "%s", filepath);

    // Load table and query builder
    table = buildTable(filepath, "db");
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
            fprintf(stdout, "Enter command: > ");
            done = 0;
        }

        char input = (char)getchar();
        strncat(query, &input, 1);

        // Run query
        if(input == ';')
        {
            done = 1;

//            fprintf(stdout, "%s\n", query);
            response = getTableFromQueryString(qb, query);

            if(response != NULL) printTable(response, 0);
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
    
    exit(0);
}