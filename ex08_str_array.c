#include <stdio.h>
#include "sccu.h"

int main (void)
{
    #define MAX_ARRAY_ELEMENT 5

    char *arr1[MAX_ARRAY_ELEMENT] = {"abc", "12", "34567", "defgh"};
    char *arr2[]                  = {"abc", "12", "34567", "defgh"};
    int num=0;

// for arr1
    num = sizeof(arr1)/sizeof(arr1[0]);
    printf ("arr1 num = %d\n", num);
    SCCU_str_array_showeach (arr1, num);
    SCCU_str_array_null     (arr1, num);
    SCCU_str_array_showeach (arr1, num);

    printf ("---\n");

// for arr2
    num = sizeof(arr2)/sizeof(arr2[0]);
    printf ("arr2 num = %d\n", num);
    SCCU_str_array_showeach (arr2, num);
    SCCU_str_array_null     (arr2, num);
    SCCU_str_array_showeach (arr2, num);

    return 0;
}

/* Result:

$ ./ex08_str_array
arr1 num = 5
[0] abc
[1] 12
[2] 34567
[3] defgh
[4] (null)
[0] (null)
[1] (null)
[2] (null)
[3] (null)
[4] (null)
---
arr2 num = 4
[0] abc
[1] 12
[2] 34567
[3] defgh
[0] (null)
[1] (null)
[2] (null)
[3] (null)

*/
