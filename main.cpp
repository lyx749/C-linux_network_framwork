#include <iostream>
#include "http_c_socket.h"
#include "http_global.h"
#include "httpServerConfig.h"
#include "http_log.h"
#include "http_func.h"
#include "http_c_memory.h"
#include "http_c_crc32.h"

char **g_os_argv;         // 原始命令行参数数组
int g_os_argc;            // 启动参数个数
size_t g_argvneedmem = 0; // 启动参数内存大小
size_t g_envneedmem = 0;  // 相关环境变量总大小
char *gp_envmem = NULL;   // 环境变量内存新位置
int g_daemonized = 0;

CLogicSocket g_socket;
WorkerThreadPool g_threadpool;

pid_t currentPid;
pid_t parentPid;
int http_process;
bool g_stopEvent;

sig_atomic_t http_reap;

static void free_resource(); // 内存释放

int main(int argc, char *const *argv)
{
    int exitCode = 0;
    g_stopEvent = false;

    currentPid = getpid();
    parentPid = getppid();
    for (int i = 0; i < argc; ++i)
    {
        g_argvneedmem += strlen(argv[i]) + 1; //+1留给\0的空间
    }

    for (int i = 0; environ[i]; ++i)
    {
        g_envneedmem += strlen(environ[i]) + 1;
    }

    g_os_argc = argc;
    g_os_argv = (char **)argv;

    http_process = HTTP_PROCESS_MASTER;
    http_reap = 0; // 标记子进程没有发生变化

    http_init_setproctitle();

    if (Daemon)
    {
        int flag = http_daemon();
        if (flag == -1)
        {
            exitCode = 1;
            goto lblexit;
        }

        if (flag == 1)
        {
            free_resource();
            exitCode = 0;
            return exitCode;
        }
        CMemory::GetInstance();
        CCRC32::GetInstance();
        myLog::getInterface();

        if (!g_socket.Initialize())
        {
            exitCode = 1;
            goto lblexit;
        }
        g_daemonized = 1; // 守护进程标记
    }

    http_master_process_cycle();
lblexit:
    myLog::getInterface()->getLogger()->info("The program exits bye bye");
    free_resource();
}

void free_resource()
{
    // 移动环境变量所分配的内存需要释放
    if (gp_envmem)
    {
        delete[] gp_envmem;
        gp_envmem = NULL;
    }
    return;
}
