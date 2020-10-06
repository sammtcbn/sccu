#include <stdio.h>
#include "sccu.h"

int main (void)
{
    char str[10];
    SCCU_str_random(str, 9);
    printf ("%s\n", str);
    return 0;
}

/* Result:

$ ./ex06_str_random
OpYEoEl52

*/
