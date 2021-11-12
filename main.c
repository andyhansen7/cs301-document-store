// STL
#include <stdio.h>
#include <unistd.h>

#include "table_utils.h"
#include "table_builder.c"
#include "filter_utils.h"

int main(void)
{
    Table* t = buildTable("db.txt", "db");
    printTable(t);
    
    Table* f = filterTable(t, eq, 'B', 555, "f1");
    printTable(f);
    return 0;
}