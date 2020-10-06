#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sccu.h"

int main (int argc, char *argv[])
{
   int ret = 0;
   pid_t pid;

   if (argc < 2)
   {
       printf ("please assign pid\n");
       return -1;
   }

   pid = atoi (argv[1]);

   ret = SCCU_is_process_running (pid);
   if (ret == 1)
       printf("PID=%d is running\n", pid);
   else
   {
       printf("PID=%d is not running\n", pid);
   }
   return 0; 
}
