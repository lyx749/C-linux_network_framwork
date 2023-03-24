#include "http_c_socket.h"
#include "../../build/httpServerConfig.h"
#include "http_c_memory.h"
#include "http_global.h"
CSocket::CSocket()
{
    workerConnections = 1;
    epollHandlefd = -1;
    CSlistenPortCount = ListenPortCount;
    onlineUserCount = 0;
    recycleConnectionWaitTime = 60; // 60秒回收回收队列里的连接
    PKG_HEADER_LEN = sizeof(COMM_PKG_HEADER_T);
    MSG_HEADER_LEN = sizeof(STRUCT_MSG_HEADER_T);
}

CSocket::~CSocket()
{
    return;
}

int CSocket::httpEpollInit()
{
    this->epollHandlefd = epoll_create(workerConnections);
    if (this->epollHandlefd == -1)
    {
        fprintf(stderr, "CSocket::httpEpollInit()'s epoll_create error %s\n", strerror(this->epollHandlefd));
        exit(2);
    }

    InitConnectPool();

    for (auto pos = listenSocketList.begin(); pos != listenSocketList.end(); ++pos)
    {
        http_connection_ptr p_Conn = getAConnectionFromPool((*pos)->listenfd);

        p_Conn->listening = (*pos);  // 连接对象和监听对象关联，方便通过连接对象找监听对象
        (*pos)->connection = p_Conn; // 监听对象和连接对象关联，方便通过监听对象找连接对象
        p_Conn->readHandler = &CSocket::httpEventAccept;

        if (httpEpollOperEvent((*pos)->listenfd, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP, 0, p_Conn) == -1)
        {
            perror("CSocket::httpEpollInit's httpEpollOperEvent error");
            exit(2);
        }
    }
    return 1;
}

/*
fd 文件描述符，一个socket
eventType 事件类型，一般是EPOLL_CTL_ADD，EPOLL_CTL_MOD，EPOLL_CTL_DEL
flag    标志，具体含义取决于eventType
bcaction    补充动作，用于补充eventType的不足 :  0：增加 1：去掉 2：完全覆盖 ，eventType是EPOLL_CTL_MOD时这个参数就有用
pConn   一个指针【其实是一个连接】，EPOLL_CTL_ADD时增加到红黑树中去，将来epoll_wait时能取出来用
*/

int CSocket::httpEpollOperEvent(int fd, uint32_t eventType, uint32_t flag, int bcaction, http_connection_ptr pConn)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    if (eventType == EPOLL_CTL_ADD)
    {
        ev.data.fd = fd;
        ev.events = flag;
        pConn->events = flag;
    }
    else if (eventType == EPOLL_CTL_MOD)
    {
        ev.events = pConn->events;
        if (bcaction == 0)
        {
            ev.events |= flag; // 增加标记
        }
        else if (bcaction == 1)
        {
            ev.events &= ~flag; // 去掉对应标记
        }
        else
        {
            ev.events = flag; // 覆盖
        }
        pConn->events = ev.events;
    }
    else
    {
        // 删除红黑树中的节点，目前没有这个需求{内核会在socket关闭之后自动删除这个节点}，目前这个模块用于将来实现
        return 1;
    }

    ev.data.ptr = (void *)pConn;

    if (epoll_ctl(epollHandlefd, eventType, fd, &ev) == -1)
    {
        perror("CSocket::httpEpollOperEvent()中epoll_ctl error");
        return -1;
    }
    return 1;
}

// 0 正常退出， 1 成功， -1 error
int CSocket::httpEpollProcessEvents(int timer)
{
    int eventsCount = epoll_wait(epollHandlefd, readyEvents, HTTP_MAX_EVENTS, timer);
    if (eventsCount == -1)
    {
        perror("CSocket::httpEpollProcessEvents's epoll_wait() error");
        return -1;
    }

    if (eventsCount == 0)
    {
        if (timer != -1)
        {
            // 属于时间到了正常返回
            return 0;
        }
        perror("CSocket::httpEpollProcessEvents's epoll_wait() don't set timeout, but don't baak any events");
        return -1;
    }

    http_connection_ptr p_Conn;
    for (int i = 0; i < eventsCount; ++i)
    {
        p_Conn = (http_connection_ptr)readyEvents[i].data.ptr;

        if ((p_Conn->events & EPOLLIN) && (readyEvents[i].events & EPOLLIN))
        {
            (this->*(p_Conn->readHandler))(p_Conn);
            /*
                这种写法使用了成员函数指针调用的语法，其中(this->*(p_Conn->rhandler))表示解除成员函数指针的引用，将其作为一个函数调用，
                并在this指向的对象上调用该函数，然后将p_Conn作为参数传递。
            */
        }

        if ((p_Conn->events & EPOLLOUT) && (readyEvents[i].events & EPOLLOUT))
        {
            if (readyEvents[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
            {
                // 只有投递了 写事件，但对端断开时，程序流程才走到这里，投递了写事件意味着 iThrowsendCount标记肯定被+1了，这里我们减回
                --p_Conn->iThrowSendCount;
            }
            else
            {
                (this->*(p_Conn->writeHandler))(p_Conn);
            }
        }
    }
    return 1;
}

bool CSocket::openListeningSockets()
{
    int sockfd;
    struct sockaddr_in serverAddr;
    // int servPort;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    for (int i = 0; i < CSlistenPortCount; ++i)
    {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            fprintf(stderr, "CSocket::openListeningSockets's socket error\n");
            return false;
        }

        int opt;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        {
            fprintf(stderr, "CSocket::openListeningSockets's setsockopt error\n");
            return false;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        {
            fprintf(stderr, "CSocket::openListeningSockets's setsockopt error\n");
            return false;
        }

        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        {
            fprintf(stderr, "CSocket::openListeningSockets's fcntl error\n");
            return false;
        }
        serverAddr.sin_port = htons(ListenPort);
        if (bind(sockfd, (struct sockaddr *)(&serverAddr), sizeof(serverAddr)) == -1)
        {
            close(sockfd);
            perror("CSocket::openListeningSockets's bind error\n");
            return false;
        }
        if (listen(sockfd, HTTP_LISTEN_BACKLOG) == -1)
        {
            close(sockfd);
            fprintf(stderr, "CSocket::openListeningSockets's listen error\n");
            return false;
        }

        http_listening_ptr listenSocketItem = new http_listening_t;
        memset(listenSocketItem, 0, sizeof(http_listening_t));
        listenSocketItem->listenfd = sockfd;
        listenSocketItem->port = ListenPort;
        printf("监听%d端口成功!\n", ListenPort);
        listenSocketList.push_back(listenSocketItem);
    }
    if (listenSocketList.size() <= 0)
    {
        perror("no port in listening");
        return false;
    }
    return true;
}

void CSocket::closeListeningSockets()
{
    for (int i = 0; i < CSlistenPortCount; ++i)
    {
        close(listenSocketList[i]->listenfd);
        printf("close port'id is %d\n", listenSocketList[i]->listenfd);
    }
    return;
}

bool CSocket::Initialize()
{
    if(openListeningSockets() == false)
    {
        return false;
    }
    return true;
}

void CSocket::threadRecvProcFunc(char *PMsgBuff)
{
    return;
}

