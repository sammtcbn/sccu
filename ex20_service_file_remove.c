#include <stdio.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *service_name = NULL;

    int ret=0;
    if (argc < 2)
    {
        printf ("please assign service name\n");
        return;
    }
    service_name=argv[1];

    ret =  SCCU_service_file_remove (service_name);
    if (ret == 0)
        printf ("successful\n");
    else
        printf ("error\n");
}

