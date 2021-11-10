// STL
#include <stdio.h>
#include <unistd.h>

#include "table_utils.c"
#include "table_builder.c"

int main(void)
{
    buildTable("db.txt", "db");
    return 0;
}