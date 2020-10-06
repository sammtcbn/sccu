#include <stdio.h>
#include "sccu.h"

int main (void)
{
    char timestr[200];
    SCCU_localtime_get (timestr);
    printf ("time is %s\n", timestr);
    return 0;
}
