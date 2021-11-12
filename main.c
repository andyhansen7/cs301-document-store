// STL
#include <stdio.h>
#include <unistd.h>

#include "table_utils.h"
#include "table_builder.c"

int main(void)
{
    Table* t = buildTable("db.txt", "db");
    printTable(t);
    return 0;
}