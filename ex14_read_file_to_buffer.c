#include <stdio.h>
#include <stdlib.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *buffer = NULL;
    char *fn = NULL;
    if (argc < 2)
    {
        printf ("please assign a file\n");
        return;
    }
    fn=argv[1];
    buffer = SCCU_read_file_to_buffer (fn);
    if (buffer != NULL)
    {
        printf ("%s", buffer);
        free (buffer);
    }
}

/* Result:

$ ./ex14_read_file_to_buffer /etc/hostname
sampc

*/