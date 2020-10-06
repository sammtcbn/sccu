#include <stdio.h>
#include "sccu.h"

int main (void)
{
    char hostnamebuf [SCCU_HOST_NAME_MAX_LEN + 1];
    SCCU_hostname_get (hostnamebuf);
    printf ("%s\n", hostnamebuf);
    return 0;
}

/* Result:

$ ./ex05_hostname_get
sampc

*/
