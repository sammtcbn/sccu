#include <stdio.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *path = NULL;
    if (argc < 2)
    {
        printf ("please assign path\n");
        return;
    }
    path=argv[1];
    SCCU_folder_create (path);
}

