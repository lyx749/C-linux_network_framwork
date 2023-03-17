#ifndef HTTP_GLOBAL_H
#define HTTP_GLOBAL_H

// 日志
typedef struct {
    int log_level; // 日志级别
    int fd;        // 日志文件描述符
} hps_log_t;

// 外部全局量声明
extern char** g_os_argv;         // main 函数参数 argv
extern int g_os_argc;            // 启动参数个数
extern size_t g_envneedmem;      // 相关环境变量总大小
extern char* gp_envmem;          // 环境变量内存新位置
extern size_t g_argvneedmem;     // 启动参数内存大小
extern int g_daemonized;         // 是否以守护进程方式运行
//extern CLogicSocket g_socket;    // 全局socket
//extern CThreadPool g_threadpool; // 线程池
extern int g_stopEvent;

extern pid_t hps_pid;         // 当前进程 id
extern pid_t hps_parent;      // 当前进程父进程 id
extern hps_log_t hps_log;     // 日志相关信息
extern int hps_process;       // 标识进程类型
//extern sig_atomic_t hps_reap; // 标识子进程状态变化
#endif