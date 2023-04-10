#include "http_c_socket.h"
#include "http_c_memory.h"
#include "http_global.h"
#include "http_log.h"
bool WorkerThreadPool::ifThreadExit = false;
WorkerThreadPool::WorkerThreadPool()
{
    runningThreadNumbers = 0;
    lastEmgTime = 0;
    messageRecvQueueCount = 0;
}

WorkerThreadPool::~WorkerThreadPool()
{
    ifRunning = {false};
    clearMsgRecvQueue();
}

void WorkerThreadPool::clearMsgRecvQueue()
{
    char *temp;
    CMemory *memoryPtr = CMemory::GetInstance();

    while (!messageRecvQueue.empty())
    {
        temp = messageRecvQueue.front();
        messageRecvQueue.pop_front();
        memoryPtr->FreeMemory(temp);
    }
}

bool WorkerThreadPool::createAllThreads(int threadNum)
{
    createThreadNumber = threadNum;
    myLog *logPtr = myLog::getInterface();
    try
    {
        for (int i = 0; i < threadNum; ++i)
        {
            bool temp = false;
            ifRunning.push_back(temp);
            threadVector.push_back(std::thread(&WorkerThreadPool::ThreadFunc, this, (void *)this, i));
        }
        // bool temp = false;
        // ifRunning.push_back(temp);
        // threadVector.push_back(std::thread(&WorkerThreadPool::expansionAndShrinkThreadNumber, this, this, temp));
    }
    catch (...)
    {
        logPtr->getLogger()->critical("create thread abnormal");
        throw;
    }

again:
    for (auto pos = ifRunning.begin(); pos != ifRunning.end(); ++pos)
    {
        if (!*pos)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 保证所有线程全部启动
            goto again;
        }
    }

    return true;
}

void WorkerThreadPool::ThreadFunc(void *threadData, int i)
{
    WorkerThreadPool *tPtr = static_cast<WorkerThreadPool*>(threadData);
    CMemory *memoryPtr = CMemory::GetInstance();
    while (true)
    {
        std::unique_lock<std::mutex> ulk(tPtr->pthreadMutex);
        tPtr->pthreadCondition.wait(ulk, [&]()
                                    { if (tPtr->ifRunning[i] == false) tPtr->ifRunning[i] = true;
                                        return !tPtr->ifThreadExit && tPtr->messageRecvQueue.size() != 0; });

        if (ifThreadExit)
        {
            ulk.unlock();
            break;
        }
        ++tPtr->runningThreadNumbers;
        char *jobBuff = tPtr->messageRecvQueue.front();
        messageRecvQueue.pop_front();
        --tPtr->messageRecvQueueCount;
        ulk.unlock();
        g_socket.threadRecvProcFunc(jobBuff);
        memoryPtr->FreeMemory(jobBuff);
        --tPtr->runningThreadNumbers;
    }
}

void WorkerThreadPool::destroyAllThreads()
{
    if (this->ifThreadExit)
    {
        return; // 已经销毁所有线程不用重复调用
    }
    this->ifThreadExit = true;

    this->pthreadCondition.notify_all();

    for (auto pos = this->threadVector.begin(); pos != this->threadVector.end(); ++pos)
    {
        if (pos->joinable())
            pos->join();
    }
    this->clearMsgRecvQueue();
}

void WorkerThreadPool::inMsgRecvQueueAndSignal(char *buf)
{
    std::unique_lock<std::mutex> ulk(pthreadMutex);
    this->messageRecvQueue.push_back(buf);
    ++this->messageRecvQueueCount;
    ulk.unlock();
again:
    if (this->callAHandlerThread() == -1)
    {
        std::this_thread::sleep_for(std::chrono::minutes(5));
        goto again;
    }
}

int WorkerThreadPool::callAHandlerThread()
{
    myLog *logPtr = myLog::getInterface();
    if (this->createThreadNumber == this->runningThreadNumbers)
    {
        logPtr->getLogger()->warn("threadNums is not enough");
        return -1;
    }
    this->pthreadCondition.notify_one();
    return 1;
}

// void threadPool::expansionAndShrinkThreadNumber(void *threadData, std::atomic<bool> &ifRunning, std::atomic<bool> &ifWorking)
// {
//     while (true)
//     {
//         ifRunning = true;
//         if (ifThreadExit)
//         {
//             break;
//         }
//         threadPool *tPtr = (threadPool *)threadData;
//         if (tPtr->runningThreadNumbers >= tPtr->createThreadNumber * 0.9) // 正在运行的进程已经大于创建进程总数的90%的时候，就需要进行进程扩容了
//         {
//         }

//         if (tPtr)
//     }
// }