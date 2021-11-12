// STL
#include <stdio.h>
#include <unistd.h>

#include "table_utils.h"
#include "table_builder.c"
#include "filter_utils.h"

int main(void)
{
    char* all = displayAllFields();

    Table* t = buildTable("db.txt", "db");
    printTable(t, all);
    
    Table* f = filterTable(t, eq, 'B', 555, "f1");
    printTable(f, all);

    Table* g = filterTable(t, lteq, 'Y', 2, "f2");
    printTable(g, all);
    return 0;
}