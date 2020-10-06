#include <stdio.h>
#include "sccu.h"

int main (void)
{
    char str[100] = "123 abc";
    printf ("origin  str = [%s]\n", str);
    SCCU_str_reverse (str);
    printf ("reverse str = [%s]\n", str);
    return 0;
}

/* Result:

$ ./ex03_str_reverse
origin  str = [123 abc]
reverse str = [cba 321]

*/
