/*
    https://github.com/sammtcbn/sccu
    Written by sammtcbn 2018.8.28
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <errno.h>
#endif

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#endif

#include "sccu.h"


void SCCU_logInit (char *ident)
{
#ifdef __linux__
    openlog (ident, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
#endif
}


void SCCU_logRelease (void)
{
#ifdef __linux__
    closelog ();
#endif
}


void SCCU_log (int level, const char *fmt, ...)
{
    va_list ap;
    char str[512];
#ifdef __linux__
    int syslog_priority = LOG_INFO;
#endif

    va_start(ap, fmt);
    vsprintf (str, fmt, ap);

#ifdef __linux__
    if (level == SCCU_LEVEL_INFO)
        syslog_priority = LOG_INFO;
    else if (level == SCCU_LEVEL_WARN)
        syslog_priority = LOG_WARNING;
    else if (level == SCCU_LEVEL_ERR)
        syslog_priority = LOG_ERR;
    syslog (syslog_priority, "%s", str);
#endif

    va_end(ap);
}


void SCCU_pause (void)
{
#ifdef WIN32
    system("pause");
#else
    fgetc(stdin);
#endif
}


void SCCU_sleep (int sec)
{
#ifdef _WIN32
    Sleep(sec*1000);
#else
    //usleep(sec*1000000);
    sleep (sec);
#endif
}


void SCCU_msleep (int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}


void SCCU_localtime_get (char *str)
{
    //char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    time_t timep;
    struct tm *p;
    time (&timep);
    p = localtime (&timep);
    //sprintf (str, "%d-%02d-%02d %s %02d:%02d:%02d",
    sprintf (str, "%d-%02d-%02d %02d:%02d:%02d",
        1900 + p->tm_year,
        1 + p->tm_mon,
        p->tm_mday,
        //wday[p->tm_wday],
        p->tm_hour,
        p->tm_min,
        p->tm_sec);
}


void SCCU_str_upper (char *str)
{
	int i = 0, l = 0;
	l = strlen(str);
	for (i = 0; i < l; i++)
		str[i] = toupper (str[i]);
}


void SCCU_str_trim (char *str)
{
    int i,l=strlen(str);
    for(i=0;i<l;i++)
    {
        if (str[i] == 0x0A || str[i] == 0x0D)
            str[i] = 0x0;
    }
}


/*
    Carriage Return (CR)  \r
    Line Feed (LF)        \n
*/
void SCCU_str_append_CRLF (char *str)
{
    strcat (str, "\r\n");
}


// return 0  : correct format
// return -1 : wrong format
int SCCU_str_check_field_num (char *str, char delim, int fieldcnt, int start_location)
{
    int ptr = 0;
    int delimiter_cnt = 0;
    int len = strlen (str);
    int i=0;
    int max_delimiter_cnt = fieldcnt-1;
    int field_size = 0;

    if (start_location >= len)
        return -1;

    ptr += start_location;

    while (ptr < len)
    {
        if (str[ptr] == delim)
        {
            if (field_size == 0)
                return -1;

            delimiter_cnt ++;
            field_size = 0;
        }
        else
        {
            field_size++;
        }
        ptr++;
    }

    if (field_size == 0)
        return -1;

    if (delimiter_cnt != max_delimiter_cnt)
        return -1;

    return 0;
}


int SCCU_str_dilimiter_location (char *str, char delim, int num)
{
    int ptr = 0;
    int delimiter_cnt = 0;
    int len = 0;
    len = strlen (str);
    if (len <= 0)
        return -1;
    while (ptr < len)
    {
        if (str[ptr] == delim)
        {
            if (delimiter_cnt == num)
                return ptr;
            delimiter_cnt ++;
        }
        ptr++;
    }
    return -1;
}


int SCCU_int_setmask (int val, int bitNum, int bitwise)
{
	//fprintf (stderr, "val=%x, bitNum=%d, bitwise=%d\n", val, bitNum, bitwise);
	if (bitwise == 1)
		val |=  (0x1 << bitNum);
	else if (bitwise == 0)
		val &=  ~(0x1 << bitNum);
	//fprintf (stderr, "val set to %x\n", val);
	return val;
}


int SCCU_int_getbit (int val, int bitNum)
{
    return (val >> bitNum) & 0x1;
}


void SCCU_show_allbit (int val, int num)
{
    int i=0;
    for (i=0;i<num;i++)
        printf ("bit%d = %d\n", i, SCCU_int_getbit (val, i));
}


char* SCCU_getCurrentDir (char *path)
{
#ifdef WIN32
    _getcwd (path, FILENAME_MAX);
#else
    getcwd (path, FILENAME_MAX);
#endif
    return path;
}


void SCCU_daemonlize (void)
{
#ifdef __linux__
    pid_t pid;

	pid = fork ();
	if (pid != 0) {
		exit (0);
	}
#endif
    return;
}


void SCCU_dump_sys_def_limit (void)
{
#ifdef WIN32
    fprintf (stderr, "MAX_PATH = %d\n", MAX_PATH);
#endif

#ifdef __linux__
#endif
}


#ifdef __linux__
int SCCU_pidfile_create (char *path)
{
    struct flock lck;
    char tmp[64] = { 0 };
    pid_t pid;
    int fd, ret = 0;
    int len;
    ssize_t nwritten;

    pid = getpid();

    fd = open (path, O_CREAT|O_WRONLY|O_NONBLOCK, 0644);
    if (fd == -1) {
        return errno;
    }

    lck = (struct flock) {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
    };

    do {
        ret = fcntl(fd, F_SETLK, &lck);
    } while ((ret == -1) && (errno == EINTR));

    if (ret != 0) {
        ret = errno;
        close(fd);
        return ret;
    }

    do {
        ret = ftruncate(fd, 0);
    } while ((ret == -1) && (errno == EINTR));

    if (ret == -1) {
        ret = EIO;
        goto fail;
    }

    len = snprintf(tmp, sizeof(tmp), "%u\n", pid);
    if (len < 0) {
        ret = errno;
        goto fail;
    }
    if (len >= sizeof(tmp)) {
        ret = ENOSPC;
        goto fail;
    }

    do {
        nwritten = write(fd, tmp, len);
    } while ((nwritten == -1) && (errno == EINTR));

    if ((nwritten == -1) || (nwritten != len)) {
        ret = EIO;
        goto fail;
    }

    return 0;

fail:
    close(fd);
    return ret;
}
#endif


#ifdef __linux__
void SCCU_pidfile_remove (char *path)
{
    int ret;
    ret = unlink (path);
}
#endif


int SCCU_service_lock (char *lockfile)
{
#ifdef __linux__
    if (SCCU_pidfile_create (lockfile) != 0)
    {
        fprintf (stderr, "Can't create pid file\n");
        return -1;
    }
#endif
    return 0;
}


void SCCU_service_unlock (char *lockfile)
{
#ifdef __linux__
    SCCU_pidfile_remove (lockfile);
#endif
    return;
}
