#include <stdio.h>
#include "sccu.h"

char gModulePath [MAX_PATH];

int main (void)
{
    SCCU_get_module_path (gModulePath);
    printf ("module path = [%s]\n", gModulePath);
    return 0;
}

/* Result:

$ ./ex04_get_module_path
module path = [/home/sam/sccu]

*/
