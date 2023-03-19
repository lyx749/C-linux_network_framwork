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
#include <vector>
#include <map>
#include <atomic>
#include <unistd.h>
#include <fcntl.h>
#include "http_macro.h"
#include "../../build/httpServerConfig.h"
#include <http_c_memory.h>

#define HTTP_LISTEN_BACKLOG 511  //已完成连接队列最大值
#define HPS_MAX_EVENTS 512      //epoll_wait 一次最多接收事件数量
class CSocket;
typedef struct http_listening_s http_listening_t, *http_listening_ptr;
typedef struct http_connection_s http_connection_t, *http_connection_ptr;

typedef void (CSocket::*http_event_handler_ptr)(http_connection_ptr); //定义一个处理函数的函数指针类型

struct http_listening_s
{
    int port;
    int listenfd;
    http_connection_ptr connection; //connected pool next connection
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
    http_listening_ptr listening;   //对应的监听端口信息

    uint64_t inCurrsequence;        //每次分配出去加1，一定程度上检测错包，废包
    struct sockaddr_in clienAddr;

    http_event_handler_ptr readHandler;
    http_event_handler_ptr writeHandler;

    uint32_t events;

    //Accept package message
    unsigned char currentStat;      //当前的收包状态
    char dataHeadInfo[_DATA_BUFSIZE_];  //用来保存收到的数据包头部信息
    char *recvBufHeadPtr;           //接收数据的缓冲区的头指针，对收到不全的包非常有用
    unsigned int needRecvLen;   
    char *recvMemoryPtr;       //new出来的用于收包的内存首地址，释放用的

    std::mutex logicProcMutex;         //逻辑处理互斥量

    //send package message

    std::atomic<int> iThrowSendCount;      //发送消息，如果发送缓冲区满了，则需要通过epoll事件来驱动消息的继续发送，所以如果发送缓冲区满，则用这个变量标记
    char *sendPackageMemPtr;    //发送完成后释放用，消息头+包头+包体
    char *sendPackageSendBuf;    //发送数据的缓冲区的头指针，包头+包体
    unsigned int needSendLen;

    time_t inRecyleTime;  //判断回收时间
    time_t lastPingTime;   //心跳包检测
};

class CSocket
{
public:
    CSocket();
    virtual ~CSocket();
    int httpEpollInit();
    int httpEpollOperEvent(int fd, uint32_t eventType, uint32_t flag, int bcaction, http_connection_ptr pConn);

public:
    //connected pool
    void InitConnectPool();
    void CleaConnectPool();
    http_connection_ptr getAConnectionFromPool(int isock);
    void freeConnectionToPool(http_connection_ptr pConn);

    //init socket
    bool openListeningSockets();
    void closeListeningSockets();


    //handler fucntion(callback fucntion)
    void httpEventAccept(http_connection_ptr oldc);
    void closeConnection(http_connection_ptr pConn);
    void readRequestHandler(http_connection_ptr pConn);
    void writeRequestHandler(http_connection_ptr pConn);

private:
    int epollHandlefd;
    int workerConnections;  //epoll连接的最大项数
    int CSlistenPortCount;    //所监听的端口数量

    std::list<http_connection_ptr> connectionPool;
    std::list<http_connection_ptr> freeConnectionPool;
    std::atomic<int> allConnectionsInPool_n;          //连接池的总连接数
    std::atomic<int> freeConnectionsInFreePool_n;     //连接池空闲连接数
    std::mutex getConnectionMutex;      //连接相关互斥量

    std::vector<http_listening_ptr> listenSocketList;     //监听套接字队列
};

#endif