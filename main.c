// STL
#include <stdio.h>
#include <unistd.h>

#include "table_utils.h"
#include "table_builder.c"
#include "filter_utils.h"
#include "query_builder.h"

int main(void)
{
    char* all = displayAllFields();
    char* test = malloc(sizeof(char) * 100);
    strcpy(test, "FIND 9\nB < 300\nA B C D E Y;");

    Table* t = buildTable("db.txt", "db");
    // printTable(t, all);

    QueryBuilder* q = getNewQueryBuilder(t);
    Table* r = getTableFromQueryString(q, test);
    
    return 0;
}