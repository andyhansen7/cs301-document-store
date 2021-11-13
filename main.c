// STL
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "table_utils.h"
#include "table_builder.c"
#include "filter_utils.h"
#include "query_builder.h"

int interrupted = 0;

void signalHandle(int code)
{
    interrupted = 1;
    exit(0);
}

int main(void)
{
    signal(SIGINT, signalHandle);
    // strcpy(test, "FIND 9\nB < 300\nA B C D E Y;");

    Table* tab = buildTable("db.txt", "db");
    QueryBuilder* qb = getNewQueryBuilder(t);
    char* query = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);
    Table* response = NULL;

    while(interrupted == 0)
    {
        char input = getchar();
        strncat(query, &input, 1);

        // Run query
        if(input == ';')
        {
            response = getTableFromQueryString(qb, query);
            
            if(response != NULL) printTable(response);
            else 
            {
                fprintf(stdout, "Could not process query: %s\n", query);
            }

            free(query);
            query = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);
        }

    }
    
    exit(0);
}