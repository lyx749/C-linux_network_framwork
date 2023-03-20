#include <iostream>
#include "http_func.h"
#include "http_c_socket.h"
// char** g_os_argv;         // 原始命令行参数数组
// int g_os_argc;            // 启动参数个数
// size_t g_argvneedmem = 0; // 启动参数内存大小
// size_t g_envneedmem = 0;  // 相关环境变量总大小
// char* gp_envmem = NULL;   // 环境变量内存新位置

// //static void free_resource(); // 内存释放

// int main(int argc, char* const* argv) {

//     //g_stopEvent = 0;

//     // hps_pid = getpid();
//     // hps_parent = getppid();

//     g_argvneedmem = 0;
//     for (int i = 0; i < argc; ++i) {
//         g_argvneedmem += strlen(argv[i] + 1);
//     }
//     for (int i = 0; environ[i]; ++i) {
//         g_envneedmem += strlen(environ[i]) + 1;
//     }
//     g_os_argc = argc;
//     g_os_argv = (char**)argv;
//     http_init_setproctitle(); // 移动环境变量
//     http_setproctitle("nihao");
//     char buff[1024]{};
//     char *log = NULL;
//     log = http_slprintf(buff, buff + 1024, "%d, %s\n", 10, "hello");
//     printf("%s", log);
// }

// // void free_resource() {
// //     // 移动环境变量所分配的内存需要释放
// //     if (gp_envmem) {
// //         delete[] gp_envmem;
// //         gp_envmem = NULL;
// //     }

// //     // 关闭日志文件
// //     if (hps_log.fd != STDERR_FILENO && hps_log.fd != -1) {
// //         close(hps_log.fd);
// //         hps_log.fd = -1; // 标记，防止重复close
// //     }
// //     return;
// // }

int main()
{
    CSocket a{};
    a.httpEpollInit();
    a.openListeningSockets();

    while (1)
    {
        a.httpEpollProcessEvents(-1);
    }
}
