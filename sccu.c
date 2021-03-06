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
#include <sys/wait.h>
#include <sys/stat.h>
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


int SCCU_getch (void)
{
    int ch;
#ifdef WIN32
    ch = _getch();
#else
    struct termios oldt, newt;
    tcgetattr (STDIN_FILENO, &oldt);
    memcpy (&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr (STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
#endif
    return ch;
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
    static char timeStr[256] = {0};
    static time_t oldt = 0;
    time_t timep;
    struct tm tms;
    str[0] = 0;
    timep = time (NULL);
    if (oldt != timep)
    {
        timeStr[0] = 0;

#ifdef __linux__
        localtime_r (&timep, &tms);
#else   // windows
        localtime_s (&tms, &timep);
#endif

#if 1   // method-1
        strftime (timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tms);
#else   // method-2
        sprintf (timeStr, "%d-%02d-%02d %02d:%02d:%02d",
            1900 + tms.tm_year,
            1 + tms.tm_mon,
            tms.tm_mday,
            tms.tm_hour,
            tms.tm_min,
            tms.tm_sec);
#endif

        oldt = timep;
    }
    strcpy (str, timeStr);
}


void SCCU_gettimeofday (struct timeval *tp)
{
#ifdef __linux__
    gettimeofday (tp, NULL);
#else
    uint64_t  intervals;
    FILETIME  ft;

    GetSystemTimeAsFileTime (&ft);

    intervals = ((uint64_t) ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    intervals -= 116444736000000000;

    tp->tv_sec = (long) (intervals / 10000000);
    tp->tv_usec = (long) ((intervals % 10000000) / 10);
#endif
}


long SCCU_getSysTimeFromEPOCH (void)
{
    struct timeval tp;
    SCCU_gettimeofday (&tp);
    return tp.tv_sec;
}


long SCCU_uptime_get (void)
{
#ifdef __linux__
    struct sysinfo s_info;
    int error = sysinfo (&s_info);
    if (error != 0)
    {
        //printf ("error code = %d\n", error);
        return 0;
    }
    else
    {
        //printf ("uptime is %ld\n", s_info.uptime);
        return s_info.uptime;
    }
#else
    // todo
    return 0;
#endif
}


int SCCU_str_count_letter (char *str, char s)
{
    int cnt=0, i=0 , len=0;
    len = (int) strlen (str);
    for (i=0;i<len;i++)
    {
        if (str[i] == s)
            cnt++;
    }
    return cnt;
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


// ref to https://www.linux.org/threads/how-do-i-reverse-a-string-in-c.12069/
void SCCU_str_reverse (char *str)
{
    char temp = '\0';
    size_t i = 0;
    size_t j = strlen(str) - 1; /* Point to the last char in the string. */

    while(i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}


void SCCU_str_random (char *tmp_s, int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand( (unsigned) time(NULL) * getpid());

    for (int i = 0; i < len; ++i)
    {
        tmp_s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    tmp_s[len] = 0;
}


void SCCU_str_combine_path_file (char* destination, char* path, char* filename)
{
    char separator=FILE_SEPARATOR;
    if (destination == NULL || path == NULL || filename == NULL)
        return;
    if (strlen (path) + strlen (filename) + 1 > MAX_PATH)
        return;
    sprintf (destination, "%s%c%s", path, separator, filename);
}

void SCCU_str_array_null (char **arr, int maxArrayElement)
{
    int i=0;
    for (i=0;i<maxArrayElement;i++)
        *(arr+i) = NULL;
}

void SCCU_str_array_showeach (char **arr, int maxArrayElement)
{
    int i=0;
    for (i=0;i<maxArrayElement;i++)
    {
        printf("[%d] %s\n", i, *(arr+i));
    }
}


void SCCU_str_split_to_array (char **arr, char *str, const char *delimiter, int maxArrayElement)
{
    char *pch;
    int cnt=0;

    pch = strtok(str, delimiter);
    while(pch != NULL)
    {
        *arr++ = pch;
        cnt++;
        if (cnt == maxArrayElement)
            break;
        pch = strtok(NULL, delimiter);
    }
}


void SCCU_str_showhex (char *str)
{
    int i=0;
    for (i=0;i<strlen(str);i++)
    {
        fprintf (stderr, "[%d] %c  ascii=%02x\n", i, str[i], str[i]);
    }
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


int SCCU_is_file_exist (char const *filepath)
{
    if(NULL == filepath)
        return 0;
#ifdef WIN32
    return _access(filepath, 0) == 0 ? 1 : 0;
#else
    return access(filepath, F_OK) == 0 ? 1 : 0;
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


#ifdef __linux__
// return 1, running
// return 0, not running
int SCCU_is_process_running (pid_t pid)
{
    int result=0;
    if (pid <= 0)
        return 0;
    //return (kill(pid,0) == 0 || errno != ESRCH);
    result = kill(pid, 0);
    if (result == 0)
        result = 1;
    else
    {
        perror("kill()");
        result = 0;
    }
    return result;
}
#endif


#ifdef __linux__
void SCCU_process_kill(pid_t pid)
{
    if (pid == -1)
        return;
    else
    {
        int status;
        kill(pid, SIGABRT);
        wait(&status); /* Wait for the child to terminate. */
    }
}
#endif


#ifdef __linux__
int SCCU_get_mac_addr_WithoutColon (char *m_szStrBuf, char *m_szIfName, int len)
{
    int m_Idx = 0;
    int m_iLen = 0;
    int fd;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    memset (m_szStrBuf, 0, len);
    if (len < 13)
        return 0;

    fd = socket (AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, m_szIfName, IFNAMSIZ-1);

    if(ioctl (fd, SIOCGIFHWADDR, &ifr) < 0)
    {
        m_szStrBuf[0] = 0;
        return 0;
    }

    close (fd);

    sprintf (m_szStrBuf, "%.2x%.2x%.2x%.2x%.2x%.2x",
        (unsigned char)ifr.ifr_hwaddr.sa_data[0],
        (unsigned char)ifr.ifr_hwaddr.sa_data[1],
        (unsigned char)ifr.ifr_hwaddr.sa_data[2],
        (unsigned char)ifr.ifr_hwaddr.sa_data[3],
        (unsigned char)ifr.ifr_hwaddr.sa_data[4],
        (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    m_iLen = strlen (m_szStrBuf);
    for(m_Idx = 0; m_Idx < m_iLen; m_Idx++)
        m_szStrBuf[m_Idx] = toupper (m_szStrBuf[m_Idx]);

    return m_iLen;
}
#endif


#ifdef __linux__
int SCCU_get_mac_addr_WithColon (char *m_szStrBuf, char *m_szIfName, int len)
{
    int m_Idx = 0;
    int m_iLen = 0;
    int fd;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    memset (m_szStrBuf, 0, len);
    if (len < 18)
        return 0;

    fd = socket (AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy (ifr.ifr_name, m_szIfName, IFNAMSIZ-1);

    if(ioctl (fd, SIOCGIFHWADDR, &ifr) < 0)
    {
        m_szStrBuf[0] = 0;
        return 0;
    }

    close (fd);

    sprintf (m_szStrBuf, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
        (unsigned char)ifr.ifr_hwaddr.sa_data[0],
        (unsigned char)ifr.ifr_hwaddr.sa_data[1],
        (unsigned char)ifr.ifr_hwaddr.sa_data[2],
        (unsigned char)ifr.ifr_hwaddr.sa_data[3],
        (unsigned char)ifr.ifr_hwaddr.sa_data[4],
        (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    m_iLen = strlen (m_szStrBuf);
    for(m_Idx = 0; m_Idx < m_iLen; m_Idx++)
        m_szStrBuf[m_Idx] = toupper (m_szStrBuf[m_Idx]);

    return m_iLen;
}
#endif


void SCCU_get_module_path (char *fpath)
{
    memset (fpath, 0, MAX_PATH);
#ifdef _WIN32
    GetModuleFileName (NULL, fpath, MAX_PATH);
    (strrchr(fpath, '\\'))[0] = 0;
#else
    int idx = 0;
    char * lastSlash = NULL;
    char tempPath[MAX_PATH] = {0};

    readlink("/proc/self/exe", tempPath, sizeof(tempPath));

    if (access(tempPath, F_OK) == 0)
    {
        lastSlash = strrchr(tempPath, FILE_SEPARATOR);
        if(NULL != lastSlash)
        {
            idx = lastSlash - tempPath + 1;
            memcpy(fpath, tempPath, idx);
            fpath[idx] = '\0';
        }
        (strrchr(fpath, FILE_SEPARATOR))[0] = 0;
    }
#endif
}


void SCCU_hostname_get (char *name)
{
#ifdef __linux__
    memset (name, 0 , SCCU_HOST_NAME_MAX_LEN + 1);
    gethostname (name, SCCU_HOST_NAME_MAX_LEN);
#elif _WIN32
    unsigned long Size = SCCU_HOST_NAME_MAX_LEN;
    memset (name, 0 , SCCU_HOST_NAME_MAX_LEN + 1);
    GetComputerName(name, &Size);
#endif
}


#ifdef __linux__
int SCCU_process_launch (pid_t *child_pid, char *processPath, char *processName, char *parameter)
{
    #define MAX_ARRAY_ELEMENT 20

    pid_t pid;
    char *arr[MAX_ARRAY_ELEMENT];
    const char *delimiter = " ";

    pid=fork();
    if(pid>0)
    {
        //fprintf(stderr, "[%d] I'm Parent\n", getpid());   // for debug
    }
    else if(pid==0)
    {
        char fullpath [MAX_PATH];

        //fprintf(stderr, "[%d] I'm Child\n", getpid());    // for debug
        SCCU_str_combine_path_file (fullpath, processPath, processName);
        SCCU_str_array_null (arr, MAX_ARRAY_ELEMENT);
        *arr = processName;
        SCCU_str_split_to_array (arr+1, parameter, delimiter, MAX_ARRAY_ELEMENT);

        //fprintf(stderr, "[%d] fullpath is %s\n", getpid(), fullpath);    // for debug

        //SCCU_str_array_showeach (arr, MAX_ARRAY_ELEMENT);     // for debug

        if (execv(fullpath, arr)<0)
        {
            perror("child execv()");
            exit (-1);
        }
        fprintf(stderr, "[%d] after execv\n", getpid());
    }
    else
    {
        perror("fork()");
        return -1;
    }

    //fprintf(stderr, "[%d] after fork, my child pid is %d\n", getpid(), pid);  // for debug

    if(pid>0)
    {
        *child_pid = pid;
    }

    return 1;
}
#endif


int SCCU_write_buffer_to_file (char *filepath, char *buffer)
{
    FILE *pFile;

    pFile = fopen(filepath, "w");
    if (pFile==NULL)
        return -1;

    fprintf(pFile, "%s\n", buffer);

    if (pFile)
        fclose (pFile);

    return 1;
}


char* SCCU_read_file_to_buffer (char *fn)
{
    char * buffer = 0;
    long length;
    FILE * f = fopen (fn, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, f);
            //buffer[length] = 0;
        }
        else
        {
            return NULL;
        }
        fclose (f);
    }
    else
    {
        return NULL;
    }
    return buffer;
}

void SCCU_remove_file (char *filepath)
{
#ifdef __linux__
    if(access(filepath, F_OK) == 0)
        remove(filepath);
#else
    if(filepath && strlen(filepath))
    {
        remove(filepath);
    }
#endif
}

int SCCU_folder_create (char *path)
{
#ifdef __linux__
    struct stat st = {0};
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

    if (stat(path, &st) == -1) {
        mkdir(path, mode);
}
#else
    CreateDirectory(path,NULL);
#endif
    return 0;
}


#ifdef __linux__
int SCCU_file_mode_as_public (char *filepath)
{
    int ret;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    ret = chmod (filepath, mode);
    return ret;
}
#endif

#ifdef __linux__
int SCCU_folder_mode_as_public (char *path)
{
    int ret;
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    ret = chmod (path, mode);
    return ret;
}
#endif

#define   SERVICE_FILE_TEMPLATE  "[Unit]\nDescription=%s\nAfter=network.target systemd-sysctl.service systemd-modules-load.service\n\n[Service]\nUser=root\nWorkingDirectory=%s\nRestart=no\nType=simple\nExecStart=%s %s\nRemainAfterExit=true\nStandardOutput=null\n\n[Install]\nWantedBy=multi-user.target\n"

/* service file sample
[Unit]
Description=this is test
After=network.target systemd-sysctl.service systemd-modules-load.service

[Service]
User=root
WorkingDirectory=/usr/local/sammtcbn/test
Restart=no
Type=simple
ExecStart=/usr/local/sammtcbn/test/node index.js
RemainAfterExit=true
StandardOutput=null

[Install]
WantedBy=multi-user.target
*/
#ifdef __linux__
int SCCU_service_file_generate (char *servicename, char *desc, char *workingdirectory, char *exepath, char *param)
{
    int ret=0;
    char dest[MAX_PATH];    // ex: /etc/systemd/system/demo.service
    char buffer[1024];

   SCCU_str_combine_path_file (dest, "/etc/systemd/system", servicename);
   strcat (dest, ".service");
   sprintf (buffer, SERVICE_FILE_TEMPLATE, servicename, workingdirectory, exepath, param);

   //fprintf (stderr, "file is %s\n", dest);
   //fprintf (stderr, "content=\n%s\n", buffer);

   if (SCCU_write_buffer_to_file (dest, buffer) == 1)
   {
        SCCU_file_mode_as_public (dest);
        ret = 0;
        //fprintf (stderr, "write %s ok\n", dest);
   }
   else
   {
        ret = -1;
        //fprintf (stderr, "write %s fail\n", dest);
   }
   return ret;
}
#endif

#ifdef __linux__
int SCCU_service_file_remove (char *servicename)
{
    int ret=0;
    char dest[MAX_PATH];    // ex: /etc/systemd/system/demo.service
    SCCU_str_combine_path_file (dest, "/etc/systemd/system", servicename);
    strcat (dest, ".service");
    SCCU_remove_file (dest);
    return ret;
}
#endif

// refer to http://biosengineer.blogspot.com/2007/12/vc-windows-registry.html
#ifdef _WINDOWS
void SCCU_SetRegValueBy_REG_DWORD (LPCSTR szKeyPath,LPCSTR szKeyName,DWORD *dwData)
{
   HKEY hk;
   if (RegCreateKey(HKEY_LOCAL_MACHINE,szKeyPath, &hk))
      OutputDebugString("error!");
      fprintf (stderr, "fail to create key\n");
   if (RegSetValueEx(hk, szKeyName, 0, REG_DWORD, (LPBYTE) dwData, sizeof(DWORD)))
      fprintf (stderr, "fail to create key\n");
   RegCloseKey(hk);
}
#endif

// refer to http://biosengineer.blogspot.com/2007/12/vc-windows-registry.html
#ifdef _WINDOWS
void SCCU_SetRegValueBy_REG_SZ (LPCSTR szKeyPath,LPCSTR szKeyName,LPCSTR keyValue)
{
   HKEY hk;
   if (RegCreateKey(HKEY_LOCAL_MACHINE,szKeyPath, &hk))
      fprintf (stderr, "fail to create key\n");
   if (RegSetValueEx(hk, szKeyName,	0,	REG_SZ, (BYTE*)(LPCSTR) keyValue, strlen(keyValue)))
      fprintf (stderr, "fail to create key\n");
   RegCloseKey(hk);
}
#endif

// refer to http://biosengineer.blogspot.com/2007/12/vc-windows-registry.html
#ifdef _WINDOWS
void SCCU_SetRegValueBy_REG_MULTI_SZ (LPCSTR szKeyPath,LPCSTR szKeyName,LPCSTR keyValue)
{
   HKEY hk;
   if (RegCreateKey(HKEY_LOCAL_MACHINE,szKeyPath, &hk))
      fprintf (stderr, "fail to create key\n");
   if (RegSetValueEx(hk, szKeyName,	0,	REG_MULTI_SZ, (BYTE*)(LPCSTR) keyValue, strlen(keyValue)))
      fprintf (stderr, "fail to create key\n");
   RegCloseKey(hk);
}
#endif


//refer to https://docs.microsoft.com/en-us/windows/win32/sysinfo/deleting-a-key-with-subkeys
//*************************************************************
//
//  RegDelnodeRecurse()
//
//  Purpose:    Deletes a registry key and all it's subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************
#ifdef _WINDOWS
BOOL SCCU_RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
{
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    // First, see if we can delete the key without having
    // to recurse.

    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS)
        return TRUE;

    lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            printf("Key not found.\n");
            return TRUE;
        }
        else {
            printf("Error opening key.\n");
            return FALSE;
        }
    }

    // Check for an ending slash and add one if it is missing.

    lpEnd = lpSubKey + lstrlen(lpSubKey);

    if (*(lpEnd - 1) != TEXT('\\'))
    {
        *lpEnd =  TEXT('\\');
        lpEnd++;
        *lpEnd =  TEXT('\0');
    }

    // Enumerate the keys

    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS)
    {
        do {

            StringCchCopy (lpEnd, MAX_PATH*2, szName);

            if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
                break;
            }

            dwSize = MAX_PATH;

            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);

        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey (hKey);

    // Try again to delete the key.

    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}
#endif


//refer to https://docs.microsoft.com/en-us/windows/win32/sysinfo/deleting-a-key-with-subkeys
//*************************************************************
//
//  RegDelnode()
//
//  Purpose:    Deletes a registry key and all it's subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************
#ifdef _WINDOWS
BOOL SCCU_RegDelnode (HKEY hKeyRoot, LPTSTR lpSubKey)
{
    TCHAR szDelKey[2 * MAX_PATH];

    StringCchCopy (szDelKey, MAX_PATH*2, lpSubKey);
    return RegDelnodeRecurse(hKeyRoot, szDelKey);

}
#endif
