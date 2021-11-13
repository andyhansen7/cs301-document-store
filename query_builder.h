#ifndef _QUERY_BUILDER_
#define _QUERY_BUILDER_

#include "filter_utils.h"
#include "table_utils.h"
#include "table_sort.h"

#define MAX_QUERY_LINES 100
#define MAX_QUERY_LINE_LENGTH 50
#define MAX_TABLE_NAME_LENGTH 1000

typedef struct query_builder_struct
{
    Table* _table;
    int query_count;
} QueryBuilder;

QueryBuilder* getNewQueryBuilder(Table* table)
{
    QueryBuilder* newqb = (QueryBuilder*)malloc(sizeof(QueryBuilder));
    newqb->query_count = 0;
    newqb->_table = table;

    return newqb;
}

Table* getTableFromQueryString(QueryBuilder* qb, char* queryString)
{
    char* all = displayAllFields();
    char** queryLines = malloc(sizeof(char*) * MAX_QUERY_LINES);
    for(int i = 0; i < MAX_QUERY_LINES; i++) queryLines[i] = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);

    int lineCount = 0;
    char delim[] = "\n";

    // Seperate sections of query out
    char *ptr_key = strtok(queryString, delim);
    while(ptr_key != NULL)
    {
        // Create entry
        strcpy(queryLines[lineCount], ptr_key);

        // Increment counter
        lineCount++;

        // Get next values
        ptr_key = strtok(NULL, delim);
    }

    // Seperate first line
    char* queryText = malloc(sizeof(char) * MAX_QUERY_LINE_LENGTH);
    int securityLevel = -1;
    sscanf(queryLines[0], "%s %d", queryText, &securityLevel);

    // Create unique table names as we progress
    char* newTableName = malloc(sizeof(char) * MAX_TABLE_NAME_LENGTH);
    strcat(newTableName, "temp_");
    strcat(newTableName, qb->_table->_tableName);

    // Query line map:
    // 0: Operation ([space] security_level OPTIONAL)
    // 1..N-1: Conditions
    // N: projection
    if(strstr(queryText, "FIND"))
    {
        // Run find operation
        Table* result = filterTable(qb->_table, matchall, 'A', -1, newTableName);

        // Security enforcement
        if(securityLevel != -1)
        {
            strcat(newTableName, "_sec");
            result = filterTable(result, lteq, 'Y', securityLevel, newTableName);
        }

        // Run find conditions, one at a time
        char label = ' ';
        int value = -1;
        char* operator = malloc(sizeof(char) * 5);

        for(int i = 1; i < (lineCount - 1); i++)
        {
            sscanf(queryLines[i], "%c %s %d", &label, operator, &value);

            // Determine operation
            if(strstr(operator, "<="))
            {
                strcat(newTableName, "_lteq");
                result = filterTable(result, lteq, label, value, newTableName);
            }
            else if(strstr(operator, "<"))
            {
                strcat(newTableName, "_lt");
                result = filterTable(result, lt, label, value, newTableName);
            }
            else if(strstr(operator, ">="))
            {
                strcat(newTableName, "_gteq");
                result = filterTable(result, gteq, label, value, newTableName);
            }
            else if(strstr(operator, ">"))
            {
                strcat(newTableName, "_gt");
                result = filterTable(result, gt, label, value, newTableName);
            }
            else if(strstr(operator, "!="))
            {
                strcat(newTableName, "_neq");
                result = filterTable(result, neq, label, value, newTableName);
            }
            else if(strstr(operator, "="))
            {
                strcat(newTableName, "_eq");
                result = filterTable(result, eq, label, value, newTableName);
            }
            else 
            {
                fprintf(stderr, "Invalid operator: %s\n", operator);
                return NULL;
            }
        }

        // Create projection
        if(strlen(queryLines[lineCount - 1]) > 2)
        {
            strcat(newTableName, "_proj");
            applyProjectionToTable(result, queryLines[lineCount - 1]);
            return result;
        }
        else
        {
            return result;
        }
        
    }
    else if(strstr(queryText, "SORT"))
    {
        if(lineCount != 3)
        {
            fprintf(stdout, "Sort given the wrong number of parameters, 3 needed, %d provided\n", lineCount);
            return NULL;
        }

        // Run find operation
        Table* result = filterTable(qb->_table, matchall, 'A', -1, newTableName);

        // Security enforcement
        if(securityLevel != -1)
        {
            strcat(newTableName, "_sec");
            result = filterTable(result, lteq, 'Y', securityLevel, newTableName);
        }

        // Load sort range
        char label = ' ';
        int value = 0;
        sscanf(queryLines[1], "%c: %d", &label, &value);
        if(value != 1 && value != -1)
        {
            fprintf(stdout, "Sort given invalid sort direction: %d\n", value);
            return NULL;
        }

        // Run sort
        strcat(newTableName, "_valid");
        result = filterTable(result, fieldexists, label, value, newTableName);

        strcat(newTableName, "_sorted");
        result = sortTable(result, label, value);

        // Create projection
        if(strlen(queryLines[lineCount - 1]) > 2)
        {
            strcat(newTableName, "_proj");
            applyProjectionToTable(result, queryLines[lineCount - 1]);
            return result;
        }
        else
        {
            return result;
        }
    }
    else if(strstr(queryText, "COUNT"))
    {
        return NULL;
    }
    else
    {
        fprintf(stdout, "Invalid operation: %s\n", queryLines[0]);
        return NULL;
    }
}


#endif