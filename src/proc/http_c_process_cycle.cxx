#include "http_func.h"
#include <sys/signal.h>
#include <signal.h>
static void http_start_worker_processes(int threadNums);
static int http_spwn_process(int threadNums, const char *procName);
static void http_worker_process_cycle(int id, const char *procName);
static void http_worker_process_init(int id);


void http_master_process_cycle()
{
    sigset_t set;

    sigemptyset(&set);

    //设置屏蔽信号集
    sigaddset(&set, SIGCHLD);   
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGHUP);
}