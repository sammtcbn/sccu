#include <stdio.h>
#include "sccu.h"

void main (int argc, char *argv[])
{
    char *service_name = NULL;
    char *service_desc = "this is test";
    char *working_path = "/usr/local/sammtcbn/test";
    char *executable_path = "/usr/local/sammtcbn/test/node";
    char *executable_param = "index.js";
    int ret=0;
    if (argc < 2)
    {
        printf ("please assign service name\n");
        return;
    }
    service_name=argv[1];

    ret =  SCCU_service_file_generate (service_name, service_desc, working_path, executable_path, executable_param);
    if (ret == 0)
        printf ("successful\n");
    else
        printf ("error\n");
}

