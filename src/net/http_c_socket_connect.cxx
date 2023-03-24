#include "http_c_socket.h"
#include "../../build/httpServerConfig.h"
#include "http_c_memory.h"
#include "http_global.h"
void http_connection_s::getOneToUse()
{
    ++inCurrsequence;
    fd = -1;
    currentStat = _PKG_HD_INIT;              // 收包状态处于初始状态，准备接收数据包头的状态位
    recvBufHeadPtr = dataHeadInfo;           // 收报我要先收到这里来，因为我要先收取包头，所以收数的buff直接就是dataHeadInfo
    needRecvLen = sizeof(COMM_PKG_HEADER_T); // 这里要指定收取数据的长度，这里先要求收包头这么长的数据

    recvMemoryPtr = NULL; // 没有new内存，自然指向的内存地址先给NULL
    iThrowSendCount = 0;  // 原子的
    sendPackageMemPtr = NULL;
    events = 0;
    lastPingTime = time(NULL); // 上次ping的时间
}

void http_connection_s::putOneToFree()
{
    ++inCurrsequence;
    if (recvMemoryPtr)
    {
        CMemory::GetInstance()->FreeMemory(recvMemoryPtr);
        recvMemoryPtr = NULL;
    }

    if (sendPackageMemPtr)
    {
        CMemory::GetInstance()->FreeMemory(sendPackageMemPtr);
    }

    iThrowSendCount = 0;
}

void CSocket::InitConnectPool()
{
    http_connection_ptr connectPtr;
    CMemory *P_memory = CMemory::GetInstance();

    int http_connection_t_len = sizeof(http_connection_t);

    for (int i = 0; i < worker_connections; ++i)
    {
        connectPtr = (http_connection_ptr)P_memory->AllocMemory(http_connection_t_len, true);
        connectPtr = new (connectPtr) http_connection_s(); // 放置new函数，意思是初始化，但是不分配内存
        connectPtr->getOneToUse();
        connectionPool.push_back(connectPtr);     // 所有连接【不管是否空闲】都放在这个list
        freeConnectionPool.push_back(connectPtr); // 空闲连接都会放在这个list
    }
    allConnectionsInPool_n = freeConnectionsInFreePool_n = connectionPool.size();
}

void CSocket::CleaConnectPool()
{
    CMemory *P_memory = CMemory::GetInstance();
    while (!connectionPool.empty())
    {
        http_connection_ptr tempPtr = connectionPool.front();
        connectionPool.pop_front();
        P_memory->FreeMemory(tempPtr);
    }
}

http_connection_ptr CSocket::getAConnectionFromPool(int isock)
{
    std::lock_guard<std::mutex> lk(getConnectionMutex);
    // 如果没有空闲了说明要扩充空闲连接池了，一次性扩充1个
    if (freeConnectionPool.empty())
    {
        CMemory *P_memory = CMemory::GetInstance();

        http_connection_ptr connectionPtr = (http_connection_ptr)P_memory->AllocMemory(sizeof(http_connection_t), true);
        connectionPtr = new (connectionPtr) http_connection_t();
        connectionPtr->getOneToUse();
        freeConnectionPool.push_back(connectionPtr);
        connectionPool.push_back(connectionPtr);
        ++allConnectionsInPool_n;
        ++freeConnectionsInFreePool_n;
    }
    http_connection_ptr tempPtr = freeConnectionPool.front();
    freeConnectionPool.pop_front();
    tempPtr->getOneToUse();
    --freeConnectionsInFreePool_n;
    tempPtr->fd = isock;
    return tempPtr;
}

// 归还参数pConn所代表的连接到到连接池中
void CSocket::freeConnectionToPool(http_connection_ptr pConn)
{
    std::lock_guard<std::mutex> lk(getConnectionMutex);
    pConn->putOneToFree();

    freeConnectionPool.push_back(pConn);

    ++freeConnectionsInFreePool_n;
}

void CSocket::pushAConnectionToRecyclePool(http_connection_ptr pConn)
{
    bool ifFind = false;
    std::lock_guard<std::mutex> lk(recycleConnectionMutex);
    for (auto pos = recycleConnectionPool.begin(); pos != recycleConnectionPool.end(); ++pos)
    {
        if (*pos == pConn)
        {
            ifFind = true;
            break;
        }
    }

    if (ifFind) // 保证这个连接只能加入一次回收池，避免被回收多次
        return;

    pConn->inRecyleTime = time(NULL);
    ++pConn->inCurrsequence;
    recycleConnectionPool.push_back(pConn);
    ++recycleConnection_n;
    --onlineUserCount;
    return;
}

void CSocket::closeConnection(http_connection_ptr pConn)
{
    freeConnectionToPool(pConn);
    if (pConn->fd != -1)
    {
        close(pConn->fd);
        pConn->fd = -1;
    }
    return;
}

void CSocket::ServerRecycleConnectionThread(void *threadData)
{
    CSocket *thisPtr = (CSocket *)threadData;
    
}