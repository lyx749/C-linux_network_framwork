#include "http_func.h"
#include "http_macro.h"
#include "httpServerConfig.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "http_log.h"
int http_daemon()
{
    switch (fork())
    {
    case -1:
        myLog::getInterface()->getLogger()->critical("http_daemon() fork error : {}", strerror(errno));
        return -1;
    case 0:
        break;

    default:
        return 1; // 父进程
    }

    parentPid = currentPid; // 将子进程的pid设置为父进程的pid
    currentPid = getpid();

    if (setsid() == -1)
    {
        myLog::getInterface()->getLogger()->critical("http_daemon() setsid error : {}", strerror(errno));
        return -1;
    }
    
    umask(0); // 不让文件权限掩码来限制文件读取权限

    int nullFd = open("/dev/null", O_RDWR);
    if(nullFd == -1)
    {
        myLog::getInterface()->getLogger()->error("http_daemon() open error : {}", strerror(errno));
        return -1;
    }

    if(dup2(nullFd, STDIN_FILENO) == -1 || dup2(nullFd, STDERR_FILENO) == -1)
    {
        myLog::getInterface()->getLogger()->error("http_daemon() dup2 error : {}", strerror(errno));
        return -1;
    }
    return 0;
}