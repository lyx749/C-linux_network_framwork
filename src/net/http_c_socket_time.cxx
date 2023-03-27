#include "http_c_socket.h"
#include "http_global.h"
#include "http_c_memory.h"

void CSocket::addToTimerMapQueue(http_connection_ptr pConn)
{
    CMemory *memoryPtr = CMemory::GetInstance();
    time_t checkTime = time(NULL) + waitTime; // 检测心跳包是否发送的时间20s之后
    std::lock_guard<std::mutex> lk(this->timeMapQueueMutex);
    STRUCT_MSG_HEADER_PTR pMsgHeader = (STRUCT_MSG_HEADER_PTR)memoryPtr->AllocMemory(MSG_HEADER_LEN, false);
    pMsgHeader->pConn = pConn;
    pMsgHeader->inCurrsequence = pConn->inCurrsequence;
    timerMapQueue.insert(std::make_pair(checkTime, pMsgHeader));
    ++this->timeMapQueueCount;
    this->timeMapQueueHeaderValue = getEarliestTime();
}

time_t CSocket::getEarliestTime() // 调用者负责互斥
{
    return this->timerMapQueue.begin()->first;
}

STRUCT_MSG_HEADER_PTR CSocket::removeFirstTimer() // 调用者负责互斥
{
    if (timeMapQueueCount <= 0)
        return NULL;

    auto pos = timerMapQueue.begin();

    STRUCT_MSG_HEADER_PTR tmp = pos->second;
    timerMapQueue.erase(pos);
    --this->timeMapQueueCount;
    return tmp;
}

STRUCT_MSG_HEADER_PTR CSocket::getOverTimeTimer(time_t currentTime)
{
    STRUCT_MSG_HEADER_PTR tmp;
    if (this->timeMapQueueCount == 0)
        return NULL;

    time_t earliestTime = this->getEarliestTime();

    if (earliestTime <= currentTime)
    {
        tmp = this->removeFirstTimer();

        if (ifOpenTimeoutKick == 0) // 未开启立刻踢人
        {
            time_t newTime = currentTime + this->waitTime;
            timerMapQueue.insert(std::make_pair(newTime, tmp));
            ++this->timeMapQueueCount;
        }

        if (timeMapQueueCount > 0)
            this->timeMapQueueHeaderValue = this->getEarliestTime();
        else
            this->timeMapQueueHeaderValue = -1;

        return tmp;
    }
    return NULL;
}

void CSocket::deleteFromTimerMapQueue(http_connection_ptr pConn)
{
    std::lock_guard<std::mutex> lk(this->timeMapQueueMutex);
again:
    for (auto pos = this->timerMapQueue.begin(); pos != this->timerMapQueue.end(); ++pos)
    {
        if ((*pos).second->pConn == pConn)
        {
            --this->timeMapQueueCount;
            CMemory::GetInstance()->FreeMemory((*pos).second);
            timerMapQueue.erase(pos);
            goto again;
        }
    }
    if (this->timeMapQueueCount > 0)
        this->timeMapQueueHeaderValue = this->getEarliestTime();
    else
        this->timeMapQueueHeaderValue = -1;
}

void CSocket::ServerTimerMapQueueMonitorThread(void *threadData)
{
    CSocket *thisPtr = static_cast<CSocket *>(threadData);
    time_t absoluteTime, currentTime;

    while (!g_stopEvent)
    {
        if (thisPtr->timeMapQueueCount > 0)
        {
            absoluteTime = thisPtr->timeMapQueueHeaderValue;
            currentTime = time(NULL);
            if (absoluteTime < currentTime)
            {
                // 时间到了，开始处理
                std::list<STRUCT_MSG_HEADER_PTR> needHandlerList;
                STRUCT_MSG_HEADER_PTR result;
                std::unique_lock<std::mutex> ulk(thisPtr->timeMapQueueMutex);
                while ((result = thisPtr->getOverTimeTimer(currentTime)) != NULL)
                {
                    needHandlerList.push_back(result);
                }
                ulk.unlock();

                STRUCT_MSG_HEADER_PTR tmp;
                while (!needHandlerList.empty())
                {
                    tmp = needHandlerList.front();
                    needHandlerList.pop_front();
                    thisPtr->procPingTimeOutChecking(tmp, currentTime);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 每500ms检测一次
    }
}

void CSocket::procPingTimeOutChecking(STRUCT_MSG_HEADER_PTR pMsgHeader, time_t currentTime) // 在子进程中具体实现了
{
    CMemory::GetInstance()->FreeMemory(pMsgHeader);
}