#ifndef HTTP_FUNC_H
#define HTTP_FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "http_global.h"
#include "http_macro.h"

/*----------------------     进程改名函数---------------------------------*/
/*
  进程名及启动参数和该进程相关环境变量存放位置是相邻的，在修改进程名时，
  为了避免影戏环境变量，需要将环境变量位置移动
*/
void http_init_setproctitle();


/*
    1. 忽略所有启动参数
    2. 新进程名长度不能超过原始标题长度 + 原始环境变量
*/
void http_setproctitle(const char *title);

/*------------------------格式化输出函数--------------------------------- */
char* http_snprintf(char* buf, size_t max, const char* fmt, ...);
char* http_slprintf(char* buf, char* last, const char* fmt, ...);
char* http_vslprintf(char* buf, char* last, const char* fmt, va_list args);

/*----------------------------日志输出，写入函数---------------------------*/
void https_log_init();                                               // 日志初始化
void https_log_stderr(int err, const char* fmt, ...);                // 输出日志消息
void https_log_error_core(int level, int err, const char* fmt, ...); // 写日志文件

// 由错误码获取错误信息
char* https_log_errno(char* buf, char* last, int err);



#endif