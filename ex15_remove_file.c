#include <stdio.h>
#include "sccu.h"

void SCCU_remove_file (char *filepath)
{
#ifdef __linux__
    if(access(filepath, F_OK) == 0)
        remove(filepath);
#else
    if(filepath && strlen(filepath))
    {
        remove(filepath);
    }
#endif
}

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

