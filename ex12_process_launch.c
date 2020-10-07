#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#ifdef __linux__
#include <sys/wait.h>
#endif

#include "sccu.h"

#define PROCESSNAME_MAXLEN     256
#define PARAMETER_MAXLEN       256

#ifdef __linux__
   pid_t gProcessID;
#endif

char gProcessPath[MAX_PATH];
char gProcessName[PROCESSNAME_MAXLEN] = "node";
char gParameter[PARAMETER_MAXLEN] = "index.js";

#ifdef __linux__
void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        fprintf(stderr, "[%d] child %d terminated\n", getpid(), pid);
}
#endif

int project_enable = 1;


void handle_signal (int signo)
{
    switch (signo)
    {
        case SIGINT:
            fprintf(stderr, "[%d] received SIGINT\n", getpid());
            project_enable = 0;
            break;
        case SIGTERM:
            fprintf(stderr, "[%d] received SIGTERM\n", getpid());
            project_enable = 0;
            break;
        case SIGSTOP:
            fprintf(stderr, "[%d] received SIGSTOP\n", getpid());
            project_enable = 0;
            break;
        case SIGKILL:
            fprintf(stderr, "[%d] received SIGKILL\n", getpid());
            project_enable = 0;
            break;
    }

}


int main (void)
{
   int ret;

   SCCU_get_module_path (gProcessPath);

   signal(SIGINT,  handle_signal);
   signal(SIGTERM, handle_signal);
   signal(SIGSTOP, handle_signal);
   signal(SIGKILL, handle_signal);

#ifdef __linux__
    signal(SIGCHLD, sig_chld);
#endif

   ret = SCCU_process_launch (&gProcessID, gProcessPath, gProcessName, gParameter);
   if (ret == 1)
   {
      fprintf(stderr, "[%d] run program pid = %d\n", getpid(), gProcessID);
   }
   else
   {
      fprintf(stderr, "[%d] fail to launch process\n", getpid());
      project_enable = 0;
   }

   while (project_enable)
   {
      fprintf(stderr, "[%d] main loop\n", getpid());
      if (SCCU_is_process_running (gProcessID) == 1)
      {
         fprintf(stderr, "[%d] child process %d is still running\n", getpid(), gProcessID);
         SCCU_sleep (1);
      }
      else
      {
         fprintf(stderr, "[%d] child process is not running\n", getpid());
         project_enable = 0;
      }
   }

// in case , main loop receive signal to break loop, so here need to stop child process
   while (1)
   {
      if (SCCU_is_process_running (gProcessID) == 1)
      {
         fprintf(stderr, "[%d] to kill child process %d\n", getpid(), gProcessID);
         SCCU_process_kill (gProcessID);
         SCCU_sleep (1);
      }
      else
      {
         break;
      }
   }

   fprintf(stderr, "[%d] exit\n", getpid());

   return 0; 
}

/* Result:


*/
