#include "http_func.h"


void http_init_setproctitle()
{
    gp_envmem = new char[g_envneedmem];
    memset(gp_envmem, 0, g_envneedmem * sizeof(char));
    char *ptmp = gp_envmem;
    for (int i = 0; environ[i]; ++i)
    {
        size_t size = strlen(environ[i]) + 1;
        strcpy(ptmp, environ[i]);
        environ[i] = ptmp;
        ptmp += size;
    }
}

void http_setproctitle(const char *title)
{
    size_t new_tile_len = strlen(title);
    size_t total_len = g_argvneedmem + g_envneedmem;
    if(total_len <= new_tile_len)
        return; //new title is too long

    g_os_argv[1] = NULL; //后续命令行参数清空，防止argv误用
    char *ptmp = g_os_argv[0];
    strcpy(ptmp, title);
    ptmp += new_tile_len;
    size_t invalid = total_len - new_tile_len;
    memset(ptmp, 0, invalid); //清空进程名后的内存，否则有可能造成残留的内存误显示
}