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

/*进程相关函数*/
int http_init_signals();
void http_master_process_cycle();
int http_daemon();

#endif