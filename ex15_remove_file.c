#include <stdio.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *fn = NULL;
    if (argc < 2)
    {
        printf ("please assign a file\n");
        return;
    }
    fn=argv[1];
    SCCU_remove_file (fn);
}

