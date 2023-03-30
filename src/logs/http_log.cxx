#include "http_func.h"
#include "../../build/httpServerConfig.h"
#include "http_global.h"
#include "http_macro.h"
#include <sys/time.h>
#include <time.h>
#include "http_c_memory.h"
#include <stdio.h>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
char *getNowTime()
{
    char *timeBuff = (char *)CMemory::GetInstance()->AllocMemory(26, true);
    struct timeval tv;
    struct tm correctTime;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &correctTime);
    strcpy(timeBuff, asctime(&correctTime));
    timeBuff[24] = '\0';
    return timeBuff;
}

void httpErrorLog(char *fmt, ...)
{
    CMemory *memoryPtr = CMemory::GetInstance();
    char *fmtBuff = (char *)memoryPtr->AllocMemory(HTTP_MAX_ERROR_STR, true);
    char *last = fmtBuff + HTTP_MAX_ERROR_STR;
    va_list argus;
    va_start(argus, fmt);
    http_vslprintf(fmtBuff, last, fmt, argus);
    char *timeBuff = getNowTime(); // 24 + 7 + 3    time : [error]
    fprintf(stderr, "%s : [error] %s\n", timeBuff, fmtBuff);
    va_end(argus);
    memoryPtr->FreeMemory(timeBuff);
    memoryPtr->FreeMemory(fmtBuff);
}

void httpCommonLog(char *fmt, ...)
{
    CMemory *memoryPtr = CMemory::GetInstance();
    char *fmtBuff = (char *)memoryPtr->AllocMemory(HTTP_MAX_ERROR_STR, true);
    char *last = fmtBuff + HTTP_MAX_ERROR_STR;
    va_list argus;
    va_start(argus, fmt);
    http_vslprintf(fmtBuff, last, fmt, argus);
    char *timeBuff = getNowTime(); // 24 + 7 + 3    time : [error]
    fprintf(stderr, "%s : [log] %s\n", timeBuff, fmtBuff);
    va_end(argus);
    memoryPtr->FreeMemory(timeBuff);
    memoryPtr->FreeMemory(fmtBuff);
}

void test()
{
    spdlog::info("i love c++");
}