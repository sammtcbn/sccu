/*
    https://github.com/sammtcbn/sccu
    Written by sammtcbn 2018.8.28
*/
#ifndef __SCCU_H__
#define __SCCU_H__

#define SCCU_LEVEL_INFO 1
#define SCCU_LEVEL_WARN 2
#define SCCU_LEVEL_ERR  3

#define SCCU_LogInfo(x, ...)  SCCU_log(SCCU_LEVEL_INFO, x, ##__VA_ARGS__);
#define SCCU_LogWarn(x, ...)  SCCU_log(SCCU_LEVEL_WARN, x, ##__VA_ARGS__);
#define SCCU_LogError(x, ...) SCCU_log(SCCU_LEVEL_ERR,  x, ##__VA_ARGS__);

void SCCU_logInit (char *ident);
void SCCU_logRelease (void);
void SCCU_log (int level, const char *fmt, ...);
void SCCU_pause (void);
void SCCU_sleep (int sec);
void SCCU_msleep (int ms);
void SCCU_localtime_get (char *str);
void SCCU_str_upper (char *str);
void SCCU_str_trim (char *str);
void SCCU_str_append_CRLF (char *str);
int SCCU_str_check_field_num (char *str, char delim, int fieldcnt, int start_location);
int SCCU_str_dilimiter_location (char *str, char delim, int num);
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
long SCCU_get_uptime (void);

#ifdef __linux__
int SCCU_check_process_exists_by_pid (pid_t pid);
int SCCU_get_mac_addr_WithoutColon (char *m_szStrBuf, char *m_szIfName, int len);
int SCCU_get_mac_addr_WithColon (char *m_szStrBuf, char *m_szIfName, int len);
#endif

#endif // __SCCU_H__
