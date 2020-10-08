/*
    https://github.com/sammtcbn/sccu
    Written by sammtcbn 2018.8.28
*/
#ifndef __SCCU_H__
#define __SCCU_H__

#ifdef __linux__
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <limits.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#endif

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <stdint.h>
#endif


#define SCCU_LEVEL_INFO 1
#define SCCU_LEVEL_WARN 2
#define SCCU_LEVEL_ERR  3

#define SCCU_LogInfo(x, ...)  SCCU_log(SCCU_LEVEL_INFO, x, ##__VA_ARGS__);
#define SCCU_LogWarn(x, ...)  SCCU_log(SCCU_LEVEL_WARN, x, ##__VA_ARGS__);
#define SCCU_LogError(x, ...) SCCU_log(SCCU_LEVEL_ERR,  x, ##__VA_ARGS__);

#ifdef WIN32
#define FILE_SEPARATOR  '\\'
#else
#define FILE_SEPARATOR  '/'
#endif

#ifdef __linux__
#define MAX_PATH    260
#endif

#ifdef __linux__
#define SCCU_HOST_NAME_MAX_LEN HOST_NAME_MAX
#elif _WIN32
#define SCCU_HOST_NAME_MAX_LEN MAX_COMPUTERNAME_LENGTH
#else
#define SCCU_HOST_NAME_MAX_LEN 254
#endif

void SCCU_logInit (char *ident);
void SCCU_logRelease (void);
void SCCU_log (int level, const char *fmt, ...);
int SCCU_getch (void);
void SCCU_pause (void);
void SCCU_sleep (int sec);
void SCCU_msleep (int ms);
void SCCU_localtime_get (char *str);
void SCCU_gettimeofday (struct timeval *tp);
long SCCU_getSysTimeFromEPOCH (void);
long SCCU_uptime_get (void);
int SCCU_str_count_letter (char *str, char s);
void SCCU_str_upper (char *str);
void SCCU_str_trim (char *str);
void SCCU_str_append_CRLF (char *str);
int SCCU_str_check_field_num (char *str, char delim, int fieldcnt, int start_location);
int SCCU_str_dilimiter_location (char *str, char delim, int num);
void SCCU_str_reverse (char *str);
void SCCU_str_random (char *tmp_s, int len);
void SCCU_str_combine_path_file (char* destination, char* path, char* filename);
void SCCU_str_array_null (char **arr, int maxArrayElement);
void SCCU_str_array_showeach (char **arr, int maxArrayElement);
void SCCU_str_split_to_array (char **arr, char *str, const char *delimiter, int maxArrayElement);
void SCCU_str_showhex (char *str);
int SCCU_int_setmask (int val, int bitNum, int bitwise);
int SCCU_int_getbit (int val, int bitNum);
void SCCU_show_allbit (int val, int num);
char* SCCU_getCurrentDir (char *path);
void SCCU_daemonlize (void);
void SCCU_dump_sys_def_limit (void);
int SCCU_is_file_exist (char const *filepath);
int SCCU_pidfile_create (char *path);
void SCCU_pidfile_remove (char *path);
int SCCU_service_lock (char *lockfile);
void SCCU_service_unlock (char *lockfile);

#ifdef __linux__
int SCCU_is_process_running (pid_t pid);
void SCCU_process_kill(pid_t pid);
int SCCU_get_mac_addr_WithoutColon (char *m_szStrBuf, char *m_szIfName, int len);
int SCCU_get_mac_addr_WithColon (char *m_szStrBuf, char *m_szIfName, int len);
int SCCU_process_launch (pid_t *child_pid, char *processPath, char *processName, char *parameter);
#endif

void SCCU_get_module_path (char *fpath);
void SCCU_hostname_get (char *name);
int SCCU_write_buffer_to_file (char *filepath, char *buffer);
char* SCCU_read_file_to_buffer (char *fn);
void SCCU_remove_file (char *filepath);
int SCCU_folder_create (char *path);

#endif // __SCCU_H__
