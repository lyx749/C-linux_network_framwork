#ifndef HTTP_C_SOCKET_H
#define HTTP_C_SOCKET_H
#include <mutex>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <string.h>
#include <thread>
#include <semaphore.h>
#include <errno.h>
#include <vector>
#include <map>
#include <atomic>
#include <unistd.h>
#include <fcntl.h>
#include "http_macro.h"
#define HTTP_LISTEN_BACKLOG 511 // 已完成连接队列最大值
#define HTTP_MAX_EVENTS 512     // epoll_wait 一次最多接收事件数量

class CSocket;
typedef struct http_listening_s http_listening_t, *http_listening_ptr;
typedef struct http_connection_s http_connection_t, *http_connection_ptr;

typedef void (CSocket::*http_event_handler_ptr)(http_connection_ptr); // 定义一个处理函数的函数指针类型

struct http_listening_s
{
    int port;
    int listenfd;
    http_connection_ptr connection; // connected pool next connection
};

struct http_connection_s
{
    http_connection_s()
    {
        inCurrsequence = 0;
    }
    void getOneToUse();
    void putOneToFree();

    int fd;
    http_listening_ptr listening; // 对应的监听端口信息

    uint64_t inCurrsequence; // 每次分配出去加1，一定程度上检测错包，废包
    struct sockaddr_in clienAddr;

    http_event_handler_ptr readHandler;
    http_event_handler_ptr writeHandler;

    uint32_t events;

    // Accept package message
    unsigned char currentStat;         // 当前的收包状态
    char dataHeadInfo[_DATA_BUFSIZE_]; // 用来保存收到的数据包头部信息
    char *recvBufHeadPtr;              // 接收数据的缓冲区的头指针，对收到不全的包非常有用
    unsigned int needRecvLen;
    char *recvMemoryPtr; // new出来的用于收包的内存首地址，释放用的

    std::mutex logicProcMutex; // 逻辑处理互斥量

    // send package message

    std::atomic<int> iThrowSendCount; // 发送消息，如果发送缓冲区满了，则需要通过epoll事件来驱动消息的继续发送，所以如果发送缓冲区满，则用这个变量标记
    char *sendPackageMemPtr;          // 发送完成后释放用，消息头+包头+包体
    char *sendPackageSendBuf;         // 发送数据的缓冲区的头指针，包头+包体
    unsigned int needSendLen;

    time_t inRecyleTime; // 判断回收时间
    time_t lastPingTime; // 心跳包检测
};

typedef struct _STRUCT_MSG_HEADER
{
    http_connection_ptr pConn; // 记录对应的链接，注意这是一个指针
    uint64_t inCurrsequence;   // 收到数据包时记录对应连接的序号，将来能用于比较连接是否已经作废
} STRUCT_MSG_HEADER_T, *STRUCT_MSG_HEADER_PTR;

class CSocket
{
public:
    CSocket();
    virtual ~CSocket();
    virtual bool Initialize();
    virtual void threadRecvProcFunc(char *PMsgBuff);

public:
    int httpEpollInit();
    int httpEpollProcessEvents(int timer);
    int httpEpollOperEvent(int fd, uint32_t eventType, uint32_t flag, int bcaction, http_connection_ptr pConn);

public:
    // connected pool
    void InitConnectPool();
    void CleaConnectPool();
    http_connection_ptr getAConnectionFromPool(int isock);
    void freeConnectionToPool(http_connection_ptr pConn);
    void pushAConnectionToConnetPool(http_connection_ptr pConn);

    // init socket
    bool openListeningSockets();
    void closeListeningSockets();

    // handler fucntion(callback fucntion)
    void testHandle(http_connection_ptr pConn);
    void httpEventAccept(http_connection_ptr oldc);
    void closeConnection(http_connection_ptr pConn);
    void readRequestHandler(http_connection_ptr pConn);
    void writeRequestHandler(http_connection_ptr pConn);

    ssize_t recvProc(http_connection_ptr pConn, char *buff, ssize_t buflen);
    void httpWaitRequestHandlerProcHeader(http_connection_ptr pConn, bool &isFlood);
    void httpWaitRequestHandlerProcBody(http_connection_ptr pConn, bool &isFlood);

private:
    int epollHandlefd;
    int workerConnections; // epoll连接的最大项数
    int CSlistenPortCount; // 所监听的端口数量

    std::list<http_connection_ptr> connectionPool;
    std::list<http_connection_ptr> freeConnectionPool;
    std::atomic<int> allConnectionsInPool_n;              // 连接池的总连接数
    std::atomic<int> freeConnectionsInFreePool_n;         // 连接池空闲连接数
    std::mutex getConnectionMutex;                        // 连接相关互斥量
    std::list<http_connection_ptr> recycleConnectionPool; // 回收池，将要回收的连接放到这里
    std::atomic<int> recycleConnection_n;                 // 回收池连接数量
    std::mutex recycleConnectionMutex;                    // 回收队列的互斥量
    /*
    为什么要延迟回收，保证这个连接的业务逻辑执行完再分配给新的连接
    */
    int recycleConnectionWaitTime; // 延迟回收时间

    std::vector<http_listening_ptr> listenSocketList; // 监听套接字队列
    struct epoll_event readyEvents[HTTP_MAX_EVENTS];  // 用于接收准备就绪的事件数组

private:
    std::atomic<int> onlineUserCount; // 目前服务器在线人数
    

    //网络安全相关
    int floodAkEnable;              // Flood攻击检测是否开启,1：开启   0：不开启
    unsigned int floodTimeInterval; // 表示每次收到数据包的时间间隔是100(毫秒)
    int floodKickCount;             // 累积多少次踢出此人

protected:
    int PKG_HEADER_LEN; // 数据包包头长度
    int MSG_HEADER_LEN; // 消息头长度
};

#endif