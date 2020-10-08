#include <stdio.h>
#include "sccu.h"

char gModulePath[MAX_PATH];

void write_func (char *f, char *b)
{
   if (SCCU_write_buffer_to_file (f, b) == 1)
      fprintf (stderr, "write %s ok\n", f);
   else
      fprintf (stderr, "write %s fail\n", f);
}

void main (void)
{
   int ret;
   char *buffer = "12345\n789abcdefg";
   char *fn = "ex13_write_buffer_to_file.txt";
   char dest[MAX_PATH];

   SCCU_get_module_path (gModulePath);
   SCCU_str_combine_path_file (dest, gModulePath, fn);
   write_func (dest, buffer);

   SCCU_str_combine_path_file (dest, "/root", fn);
   write_func (dest, buffer);
}

/* Result:

$ ./ex13_write_buffer_to_file
write /home/sam/sccu/ex13_write_buffer_to_file.txt ok
write /root/ex13_write_buffer_to_file.txt fail

*/