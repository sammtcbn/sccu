#include <stdio.h>
#include "sccu.h"

int main (void)
{
    int c;
    printf ("Press q to quit\n");
    while (1)
    {
        c = SCCU_getch ();
        printf ("%c", c);
        if (c == 'q')
            break;
    }
    return 0;
}
