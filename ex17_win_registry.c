#include <stdio.h>

#ifdef _WINDOWS
#include <windows.h>
#include <strsafe.h>
#endif

#include "sccu.h"


int main (void)
{
   char szKeyPath[MAX_PATH] = "Software\\SAM_COMPANY\\SCCU";
   char *productname = "sccu";
   char *productver = "1.0.1";

#ifdef _WINDOWS
    BOOL bSuccess;

// create key and value

   SetRegValueBy_REG_SZ  (szKeyPath, "ProductName",    productname);
   SetRegValueBy_REG_SZ  (szKeyPath, "ProductVersion", productver);

// delete registry

   bSuccess = RegDelnode (HKEY_LOCAL_MACHINE, szKeyPath);
   if(bSuccess)
      printf("Success to del registry\n");
   else
      printf("Fail to del registry\n");

#endif

    return 0;
}
