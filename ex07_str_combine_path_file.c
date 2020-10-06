#include <stdio.h>
#include <string.h>
#include "sccu.h"


int main (void)
{
    char fullpath[MAX_PATH];
#ifdef _WINDOWS
    SCCU_str_combine_path_file (fullpath, "c:\\windows\\aaa\\bbb", "fileC");
#else
    SCCU_str_combine_path_file (fullpath, "/home/sam", "fileC");
#endif
    printf ("%s\n", fullpath);
    return 0;
}

/* Result:

$ ./ex07_str_combine_path_file
/home/sam/fileC

*/
