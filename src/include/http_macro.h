#ifndef HTTP_MACRO_H
#define HTTP_MACRO_H
// 一些宏定义

// 数字相关
#define HTTP_MAX_UINT32_VALUE (uint32_t)0xffffffff // 最大的32位无符号数：十进制是‭4294967295‬
#define HTTP_INT64_LEN (sizeof("-9223372036854775808") - 1)

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
} COMM_PKG_HEADER_T, *COMM_PKG_HEADER_PTR;
#pragma pack()

#endif