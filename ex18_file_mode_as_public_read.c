#include <stdio.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *fn = NULL;
    int ret=0;
    if (argc < 2)
    {
        printf ("please assign a file\n");
        return;
    }
    fn=argv[1];
    ret = SCCU_file_mode_as_public_read (fn);
    if (ret == 0)
        printf ("successful\n");
    else
        printf ("error\n");
}

