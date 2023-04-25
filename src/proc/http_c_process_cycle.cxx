#include "http_func.h"
#include <sys/signal.h>
#include <signal.h>
#include "http_log.h"
#include <errno.h>
#include <string.h>
#include "httpServerConfig.h"

static void http_start_worker_processes(int threadNums);
static int http_spwn_process(int id, const char *procName);
static void http_worker_process_cycle(int id, const char *procName);
static void http_worker_process_init(int id);
static void http_process_events_and_timers();

static u_char masterProcess[] = "master process";

void http_master_process_cycle()
{
    sigset_t set;

    sigemptyset(&set);

    // 设置屏蔽信号集
    sigaddset(&set, SIGCHLD);  // 子进程状态改变
    sigaddset(&set, SIGALRM);  // 定时器超时
    sigaddset(&set, SIGIO);    // 异步I/O
    sigaddset(&set, SIGINT);   // 终端中断符
    sigaddset(&set, SIGHUP);   // 连接断开
    sigaddset(&set, SIGUSR1);  // 用户定义信号
    sigaddset(&set, SIGUSR2);  // 用户定义信号
    sigaddset(&set, SIGWINCH); // 终端窗口大小改变
    sigaddset(&set, SIGTERM);  // 终止
    sigaddset(&set, SIGQUIT);  // 终端退出符

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    {
        myLog::getInterface()->getLogger()->critical("http_master_process_cycle() sigpromask error : {}", strerror(errno));
    }

    size_t size;

    size = sizeof(masterProcess);
    if (size < 1000) // 标题过长不设置
    {
        char title[1000] = {0};
        strcpy(title, (const char *)masterProcess);
        strcat(title, " ");
        for (int i = 0; i < g_os_argc; i++)
        {
            strcat(title, g_os_argv[i]);
        }
        http_setproctitle(title);
        myLog::getInterface()->getLogger()->info("{} [master process] running!", masterProcess);
    }

    http_start_worker_processes(WorkerProcesses);

    sigemptyset(&set);

    while (1)
    {
        sigsuspend(&set);
        sleep(1);
    }
}

static void http_start_worker_processes(int threadNums)
{
    for (int i = 0; i < threadNums; ++i)
    {
        http_spwn_process(i, "worker process");
    }
}

static int http_spwn_process(int id, const char *procName)
{
    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        myLog::getInterface()->getLogger()->critical("http_spwn_process() fork Produce child processes num = {}  procname = {} error:{}", strerror(errno));
        return -1;
    case 0:
        parentPid = currentPid;
        currentPid = getpid();
        http_worker_process_cycle(id, procName);
        break;
    default:
        break;
    }
    return pid;
}

static void http_worker_process_cycle(int id, const char *procName)
{
    http_process = HTTP_PROCESS_WORKER; // 设置进程类型，是worker子进程

    http_worker_process_init(id);
    http_setproctitle(procName);
    myLog::getInterface()->getLogger()->info("[worker proc] running procname = {} num = {}", procName, id);

    for (;;)
    {
        http_process_events_and_timers();
    }

    g_threadpool.destroyAllThreads();
    g_socket.shutdownSubproc();
    return;
}

static void http_worker_process_init(int id)
{
    sigset_t set;
    sigisemptyset(&set);

    if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
    {
        myLog::getInterface()->getLogger()->error("http_worker_process_init sigprocmask error : {}", strerror(errno));
        return;
    }
    if (!g_socket.Initialize())
    {
        exit(-2);
    }
    if (!g_threadpool.createAllThreads(ProcMsgRecvWorkThreadCount))
    {
        myLog::getInterface()->getLogger()->critical("http_worker_process_init createAllThreads failed");
        exit(-2);
    }
    sleep(1);

    if (!g_socket.InitializeSubproc())
    {
        myLog::getInterface()->getLogger()->critical("http_worker_process_init InitializeSubproc failed");
        exit(-2);
    }

    g_socket.httpEpollInit();
    g_socket.addThreadFuncToVector();
}

static void http_process_events_and_timers()
{
    g_socket.httpEpollProcessEvents(-1);
}
