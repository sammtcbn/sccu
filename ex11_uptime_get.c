#include <stdio.h>
#include "sccu.h"

int main (void)
{
   long uptime;
   uptime = SCCU_uptime_get();
   printf ("uptime is %ld\n", uptime);
   return 0; 
}

/* Result:

$ ./ex11_uptime_get
uptime is 1136127

*/
