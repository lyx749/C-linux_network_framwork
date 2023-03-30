#include "http_c_socket.h"
#include "httpServerConfig.h"
#include "http_c_memory.h"
#include "http_global.h"
#include <sys/time.h>
#include "http_log.h"
CSocket::CSocket()
{
    // config
    workerConnections = worker_connections;
    CSlistenPortCount = ListenPortCount;
    recycleConnectionWaitTime = Sock_RecyConnectionWaitTime; // 60秒回收回收队列里的连接
    ifOpenTimeCount = Sock_WaitTimeEnable;
    ifOpenTimeoutKick = Sock_TimeOutKick;
    waitTime = Sock_MaxWaitTime;
    floodAkEnable = Sock_FloodAttackKickEnable;
    floodTimeInterval = Sock_FloodTimeInterval;
    floodKickCount = Sock_FloodKickCounter;
    // epoll
    epollHandlefd = -1;

    PKG_HEADER_LEN = sizeof(COMM_PKG_HEADER_T);
    MSG_HEADER_LEN = sizeof(STRUCT_MSG_HEADER_T);

    onlineUserCount = 0;

    // 队列相关
    messageSendQueueCount = 0;

    recycleConnection_n = 0;
    allConnectionsInPool_n = 0;
    freeConnectionsInFreePool_n = 0;

    timeMapQueueCount = 0;
    timeMapQueueHeaderValue = 0;
}

CSocket::~CSocket()
{
    return;
}

int CSocket::httpEpollInit()
{
    myLog *logPtr = myLog::getInterface();
    this->epollHandlefd = epoll_create(workerConnections);
    if (this->epollHandlefd == -1)
    {
       logPtr->getLogger()->critical("CSocket::httpEpollInit()'s epoll_create error : {}", strerror(errno));
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
            logPtr->getLogger()->critical("CSocket::httpEpollInit's httpEpollOperEvent error");
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
        myLog::getInterface()->getLogger()->error("CSocket::httpEpollOperEvent()中epoll_ctl error : {}", strerror(errno));
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
        myLog::getInterface()->getLogger()->critical("CSocket::httpEpollProcessEvents's epoll_wait() error : {}", strerror(errno));
        return -1;
    }

    if (eventsCount == 0)
    {
        if (timer != -1)
        {
            // 属于时间到了正常返回
            return 0;
        }
        myLog::getInterface()->getLogger()->critical("CSocket::httpEpollProcessEvents's epoll_wait() don't set timeout, but don't baak any events");
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
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's socket error : %s", strerror(errno));
            return false;
        }

        int opt;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        {
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's setsockopt error : %s", strerror(errno));
            return false;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        {
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's setsockopt error : {}", errno);
            return false;
        }

        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        {
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's fcntl error : {}", strerror(errno));
            return false;
        }
        serverAddr.sin_port = htons(ListenPort);
        if (bind(sockfd, (struct sockaddr *)(&serverAddr), sizeof(serverAddr)) == -1)
        {
            close(sockfd);
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's bind error : {}", strerror(errno));
            return false;
        }
        if (listen(sockfd, HTTP_LISTEN_BACKLOG) == -1)
        {
            close(sockfd);
            myLog::getInterface()->getLogger()->critical("CSocket::openListeningSockets's listen error : {}", strerror(errno));
            return false;
        }

        http_listening_ptr listenSocketItem = new http_listening_t;
        memset(listenSocketItem, 0, sizeof(http_listening_t));
        listenSocketItem->listenfd = sockfd;
        listenSocketItem->port = ListenPort;
        myLog::getInterface()->getLogger()->info("Successfully listened on port {}", ListenPort);
        listenSocketList.push_back(listenSocketItem);
    }
    if (listenSocketList.size() <= 0)
    {
        myLog::getInterface()->getLogger()->error("no port in listening");
        return false;
    }
    return true;
}

void CSocket::closeListeningSockets()
{
    for (int i = 0; i < CSlistenPortCount; ++i)
    {
        close(listenSocketList[i]->listenfd);
        myLog::getInterface()->getLogger()->info("close port's id is {}", listenSocketList[i]->listenfd);
    }
    return;
}

bool CSocket::Initialize()
{
    if (openListeningSockets() == false)
    {
        return false;
    }
    return true;
}

void CSocket::threadRecvProcFunc(char *PMsgBuff)
{
    return;
}

bool CSocket::InitializeSubproc()
{
    // 初始化发消息相关信号量，信号量用于进程/线程 之间的同步，虽然 互斥量[pthread_mutex_lock]和 条件变量[pthread_cond_wait]都是线程之间的同步手段，但
    // 这里用信号量实现 则 更容易理解，更容易简化问题，使用书写的代码短小且清晰；
    // 第二个参数=0，表示信号量在线程之间共享，确实如此 ，如果非0，表示在进程之间共享
    // 第三个参数=0，表示信号量的初始值，为0时，调用sem_wait()就会卡在那里卡着
    this->serverProcThreadPool.push_back(std::thread(&CSocket::ServerRecycleConnectionThread, this, (void *)this)); // 创建回收线程
    if (sem_init(&this->sendMsgQueueSem_t, 0, 0) == -1)
    {
        myLog::getInterface()->getLogger()->error("CSocket::InitializeSubproc() sem_init error : {}", strerror(errno));
        return false;
    }

    this->serverProcThreadPool.push_back(std::thread(&CSocket::ServerSendPackageThread, this, (void *)this));          // 创建发送线程
    this->serverProcThreadPool.push_back(std::thread(&CSocket::ServerTimerMapQueueMonitorThread, this, (void *)this)); // 创建监控时间进程
    return true;
}

void CSocket::shutdownSubproc()
{
    //(1)把干活的线程停止掉，注意 系统应该尝试通过设置 g_stopEvent = 1来 开始让整个项目停止
    //(2)用到信号量的，可能还需要调用一下sem_post
    if (sem_post(&this->sendMsgQueueSem_t) == -1)
        myLog::getInterface()->getLogger()->error("CSocket::shutdownSubproc() sem_post error : {}", strerror(errno));

    for (auto pos = this->serverProcThreadPool.begin(); pos != this->serverProcThreadPool.end(); ++pos)
    {
        if ((*pos).joinable())
            (*pos).join();
    }

    clearMessageQueue();
    ClearConnectPool();

    sem_destroy(&this->sendMsgQueueSem_t);
}

void CSocket::zdCloseSocketProc(http_connection_ptr pConn)
{
    if (this->ifOpenTimeCount == 1)
        deleteFromTimerMapQueue(pConn); // 关闭这个连接，将这个连接从时间监听队列里去除，感觉没问题
    if (pConn->fd != -1)
    {
        close(pConn->fd);
        pConn->fd = -1;
    }

    if (pConn->iThrowSendCount > 0) // 归0
        --pConn->iThrowSendCount;

    pushAConnectionToRecyclePool(pConn);
}

void CSocket::clearMessageQueue()
{
    char *tempPoint;
    CMemory *memoryPtr = CMemory::GetInstance();
    while (!this->messageSendQueue.empty())
    {
        tempPoint = messageSendQueue.front();
        messageSendQueue.pop_front();
        memoryPtr->FreeMemory(tempPoint);
    }
}

void CSocket::msgSend(char *pSendBuff)
{
    myLog *lopPtr = myLog::getInterface();
    CMemory *memoryPtr = CMemory::GetInstance();
    std::lock_guard<std::mutex> lk(sendMsgQueueMutex);

    // 发送消息队列过大也可能给服务器带来风险
    if (messageSendQueueCount > 50000)
    {
        // 发送队列过大，比如客户端恶意不接受数据，就会导致这个队列越来越大
        // 那么可以考虑为了服务器安全，干掉一些数据的发送，虽然有可能导致客户端出现问题，但总比服务器不稳定要好很多
        lopPtr->getLogger()->error("CSocket::msgSend() messageSendQueueCount is to big");
        discardPackageCount++;
        memoryPtr->FreeMemory(pSendBuff);
        return;
    }

    STRUCT_MSG_HEADER_PTR pMsgHeader = (STRUCT_MSG_HEADER_PTR)pSendBuff;
    http_connection_ptr pConn = pMsgHeader->pConn;

    if (pConn->inSendQueueCount > 400)
    {
        // 该用户收消息太慢(或者干脆不接收消息)，累积的该用户的发送队列中有的数据条目数过大，认为是恶意用户，直接切断
        lopPtr->getLogger()->warn("CSocekt::msgSend() found that a user IP = %s port = %d has a backlog of packets to be sent, cutting off the connection to him!", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        discardPackageCount++;
        memoryPtr->FreeMemory(pSendBuff);
        zdCloseSocketProc(pConn);
        return;
    }

    ++pConn->inSendQueueCount;
    messageSendQueue.push_back(pSendBuff);
    ++messageSendQueueCount;

    if (sem_post(&this->sendMsgQueueSem_t) == -1)
        lopPtr->getLogger()->error("CSocekt::msgSend() sem_post error : {}", strerror(errno));
}

void CSocket::ServerSendPackageThread(void *threadData)
{
    CSocket *thisPtr = static_cast<CSocket *>(threadData);
    char *pMsgBuff;
    STRUCT_MSG_HEADER_PTR pMsgHeader;
    COMM_PKG_HEADER_PTR pPkgHeader;
    http_connection_ptr pConn;

    CMemory *memoryPtr = CMemory::GetInstance();
    while (!g_stopEvent)
    {
        /*
        如果被某个信号中断，sem_wait也可能会过早的返回，错误为EINTR
        整个程序退出之前，也要sem_post()一下，确保如果本线程卡在sem_wait()，也能走下去从而让本线程成功返回
        */
        if (sem_wait(&thisPtr->sendMsgQueueSem_t) == -1)
        {
            if (errno != EINTR)
                myLog::getInterface()->getLogger()->error("CSocket::ServerSendPackageThread() sem_wait error : {}", strerror(errno));
        }
        if (g_stopEvent)
            break;

        // printf("1\n");
        if (thisPtr->messageSendQueueCount > 0)
        {
            std::unique_lock<std::mutex> ulk(thisPtr->sendMsgQueueMutex);
        again:
            // printf("2\n");
            for (auto pos = messageSendQueue.begin(); pos != messageSendQueue.end() && !g_stopEvent; ++pos)
            {
                pMsgBuff = (*pos);
                pMsgHeader = (STRUCT_MSG_HEADER_PTR)pMsgBuff;
                pPkgHeader = (COMM_PKG_HEADER_PTR)(pMsgBuff + thisPtr->MSG_HEADER_LEN);
                pConn = pMsgHeader->pConn;

                // 包过期，因为如果这个连接被回收，比如在freeConnectionToPool pushAConnectionToRecyclePool closeConnection中都会自增iCurrsequence
                if (pConn->inCurrsequence != pMsgHeader->inCurrsequence)
                {
                    thisPtr->messageSendQueue.erase(pos);
                    --thisPtr->messageSendQueueCount;
                    memoryPtr->FreeMemory(pMsgBuff);
                    goto again;
                }

                if (pConn->iThrowSendCount > 0)
                    // 发送缓冲区已满，目前该事件在红黑树上进行监听，要靠系统驱动发送消息，所以这里不发送
                    continue;

                --pConn->inSendQueueCount; // 该连接在发送消息队列有的数据条目数减一

                pConn->sendPackageMemPtr = pMsgBuff;
                thisPtr->messageSendQueue.erase(pos);
                --thisPtr->messageSendQueueCount;
                pConn->sendPackageSendBuf = (char *)pPkgHeader; // 要发送数据的缓冲区指针，因为发送的数据不一定全能发送出去，该变量用于标记发送到哪了
                pConn->needSendLen = ntohs(pPkgHeader->pkgLen);
                // printf("needSendLen =  %d\n", ntohs(pPkgHeader->pkgLen));
                // printf("3\n");
                //  我采用 epoll水平触发的策略，能走到这里的，都应该是还没有投递 写事件 到epoll中
                //  epoll水平触发发送数据的改进方案：
                //  开始不把socket写事件通知加入到epoll,当我需要写数据的时候，直接调用write/send发送数据；
                //  如果返回了EAGIN【发送缓冲区满了，需要等待可写事件才能继续往缓冲区里写数据】，此时，我再把写事件通知加入到epoll，
                //  此时，就变成了在epoll驱动下写数据，全部数据发送完毕后，再把写事件通知从epoll中干掉；
                //  优点：数据不多的时候，可以避免epoll的写事件的增加/删除，提高了程序的执行效率；

                ssize_t sendSize = thisPtr->sendProc(pConn, pConn->sendPackageSendBuf, pConn->needSendLen);
                // printf("4\n");
                if (sendSize > 0)
                {
                    // printf("5\n");
                    if (sendSize == pConn->needSendLen)
                    {
                        memoryPtr->FreeMemory(pConn->sendPackageMemPtr);
                        pConn->sendPackageMemPtr = NULL;
                    }
                    else // 没有全部发送完毕(EAGAIN)，数据只发出去了一部分，缓冲区已满
                    {
                        pConn->sendPackageSendBuf += sendSize;
                        pConn->needSendLen -= sendSize;
                        ++pConn->iThrowSendCount; // 标记缓冲区已满

                        if (thisPtr->httpEpollOperEvent(pConn->fd, EPOLL_CTL_MOD, EPOLLOUT, 0, pConn) == -1)
                            myLog::getInterface()->getLogger()->error("CSocket::ServerSendPackageThread httpEpollOperEvent error");
                    }
                    goto again;
                }
                else if (sendSize == 0)
                {
                    // 发送0个字节，首先因为我发送的内容不是0个字节的；
                    // 然后如果发送 缓冲区满则返回的应该是-1，而错误码应该是EAGAIN，所以我综合认为，这种情况我就把这个发送的包丢弃了【按对端关闭了socket处理】
                    // 然后这个包干掉，不发送了
                    // printf("6\n");
                    memoryPtr->FreeMemory(pConn->sendPackageMemPtr);
                    pConn->sendPackageMemPtr = NULL;
                    goto again;
                }
                else if (sendSize == -1)
                {
                    // 发送缓冲区已经满了【一个字节都没发出去，说明发送 缓冲区当前正好是满的】
                    // printf("7\n");
                    ++pConn->iThrowSendCount;
                    if (thisPtr->httpEpollOperEvent(pConn->fd, EPOLL_CTL_MOD, EPOLLOUT, 0, pConn) == -1)
                        myLog::getInterface()->getLogger()->error("CSocket::ServerSendPackageThread httpEpollOperEvent error");

                    goto again;
                }
                else // 一般走到这说明返回值为-2，一般就认为是对端断开了，等待recv()来做断开socket以及回收资源
                {
                    // printf("8\n");
                    memoryPtr->FreeMemory(pConn->sendPackageMemPtr);
                    pConn->sendPackageMemPtr = NULL;
                    goto again;
                }

                goto again;
            }
            ulk.unlock();
        }
    }
}

bool CSocket::testFlood(http_connection_ptr pConn)
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL); // 获取当前系统时间
    uint64_t currentTimeMill = (currentTime.tv_sec * 1000 + currentTime.tv_usec);
    bool ifKick = false;
    if ((currentTimeMill - pConn->floodLateKickTime) <= this->floodTimeInterval)
    {
        pConn->floodAttackCount++;
        pConn->floodLateKickTime = currentTimeMill;
    }
    else
    {
        // 这里其实是有问题的，如果我直接归0，就会给危险分子有可趁之机，每攻击9次停102ms再发，在网络高峰期也会导致服务器崩溃
        pConn->floodAttackCount = 0;
        pConn->floodLateKickTime = currentTimeMill;
    }

    if (pConn->floodAttackCount >= this->floodKickCount)
        ifKick = true;
    // printf("pConn->floodAttackCount = %d, this->floodKickCount = %d\n", pConn->floodAttackCount, this->floodKickCount);
    return ifKick;
}
