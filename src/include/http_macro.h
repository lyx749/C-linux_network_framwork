#ifndef HTTP_MACRO_H
#define HTTP_MACRO_H
// 一些宏定义

#define HTTP_MAX_ERROR_STR 2048 // 显示的错误信息最大数组长度

// 一般 memcpy 返回的是指向目标 dst 的指针，而 HTTP_memcpy 返回的是目标拷贝数据后的终点位置，方便多次复制数据
#define http_memcpy(dst, src, n) (((char *)memcpy(dst, src, n)) + (n))
#define http_min(val1, val2) ((val1 < val2) ? (val1) : (val2))

// 数字相关
#define HTTP_MAX_UINT32_VALUE (uint32_t)0xffffffff // 最大的32位无符号数：十进制是‭4294967295‬
#define HTTP_INT64_LEN (sizeof("-9223372036854775808") - 1)

// 日志级别
#define HTTP_LOG_STDERR 0 // 控制台错误【stderr】日志不仅写文件，且尝试向标准输出输出错误信息
#define HTTP_LOG_EMERG 1  // 紧急 【emerg】
#define HTTP_LOG_ALERT 2  // 警戒 【alert】
#define HTTP_LOG_CRIT 3   // 严重 【crit】
#define HTTP_LOG_ERR 4    // 错误 【error】
#define HTTP_LOG_WARN 5   // 警告 【warn】
#define HTTP_LOG_NOTICE 6 // 注意 【notice】
#define HTTP_LOG_INFO 7   // 信息 【info】
#define HTTP_LOG_DEBUG 8  // 调试 【debug】

// 缺省的日志存放路径和文件名
#define HTTP_ERROR_LOG_PATH "error.log"

// 当前进程类型
#define HTTP_PROCESS_MASTER 0 // master 进程，管理进程
#define HTTP_PROCESS_WORKER 1 // worker 进程，工作进程



#define _PKG_MAX_LENGTH 30000 //包头加包体的最大长度

//收包状态
#define _PKG_HD_INIT 0  //初始状态。准备接收数据包头
#define _PKG_HD_RECVING 1 //接收包头中，包头信息不完整，继续接受中
#define _PKG_BD_INIT 2 //包头信息接收完，准备接收包体
#define _PKG_BD_RECVING 3 //接收包体中，包体不完整，继续接收中，处理后直接返回到_PKG_HD_INIT_状态
#define _DATA_BUFSIZE_ 20 //存放包头数组大小

#pragma pack(1) //内存对齐1个字节
typedef struct _COMM_PKG_HEADER
{
    unsigned short pkgLen; // “包头加包体”的总长度
    unsigned short msgCode; //消息类型
    int crc32;              // CRC32位校验码
} COMM_PKG_HEADER_T, COMM_PKG_HEADER_PTR;
#pragma pack()
#endif