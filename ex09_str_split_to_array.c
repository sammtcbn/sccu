#include <stdio.h>
#include "sccu.h"

int main (void)
{
    #define MAX_ARRAY_ELEMENT 5
    char str[] = "10 20 30";
    char *arr[MAX_ARRAY_ELEMENT];
    const char *delimiter = " ";

    printf ("str = [%s]\n", str);

    SCCU_str_array_null     (arr, MAX_ARRAY_ELEMENT);
    SCCU_str_array_showeach (arr, MAX_ARRAY_ELEMENT);
    SCCU_str_split_to_array (arr, str, delimiter, MAX_ARRAY_ELEMENT);
    SCCU_str_array_showeach (arr, MAX_ARRAY_ELEMENT);

	printf ("str = [%s]\n", str);

    return 0;
}

/* Result:

$ ./ex09_str_split_to_array
str = [10 20 30]
[0] (null)
[1] (null)
[2] (null)
[3] (null)
[4] (null)
[0] 10
[1] 20
[2] 30
[3] (null)
[4] (null)
str = [10]

*/
