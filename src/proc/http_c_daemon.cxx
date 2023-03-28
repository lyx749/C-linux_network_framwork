#include "http_func.h"
#include "http_macro.h"
#include "../../build/httpServerConfig.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int http_daemon()
{
    switch (fork())
    {
    case -1:
        perror("http_daemon() fork error");
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
        perror("http_daemon() setsid() == -1 error");
        return -1;
    }

    umask(0); // 不让文件权限掩码来限制文件读取权限

    // int NullFd = open("dev/null", O_RDWR);  //文件黑洞
    int errorLogFd = open(ErrorLog, O_TRUNC | O_CREAT | O_RDWR, 0777);
    int LogFd = open(Log, O_TRUNC | O_CREAT | O_RDWR, 0777);
    if (errorLogFd == -1 || LogFd == -1)
    {
        perror("http_daemon() open error NullFd == -1 || errorLogFd == -1 || LogFd == -1");
        return -1;
    }

    if (dup2(errorLogFd, STDERR_FILENO) == -1)
    {
        close(errorLogFd);
        close(LogFd);
        perror("http_daemon() dup2(errorLogFd, STDERR_FILENO) == -1");
        return -1;
    }

    if (dup2(LogFd, STDOUT_FILENO) == -1)
    {
        close(errorLogFd);
        close(LogFd);
        perror("dup2(LogFd, STDOUT_FILENO) == -1");
        return -1;
    }
    return 0;
}