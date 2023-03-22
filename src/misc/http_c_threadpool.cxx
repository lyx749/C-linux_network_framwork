#include "http_c_threadpool.h"
#include "http_c_socket.h"
bool threadPool::ifThreadExit = false;

threadPool::threadPool()
{
    runningThreadNumbers = 0;
    lastEmgTime = 0;
    messageRecvQueueCount = 0;
}

threadPool::~threadPool()
{
    clearMsgRecvQueue();
}

void threadPool::clearMsgRecvQueue()
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

bool threadPool::createAllThreads(int threadNum)
{
    createThreadNumber = threadNum;

    try
    {
        for (int i = 0; i < threadNum; ++i)
        {
            std::atomic<bool> temp = false;
            ifRunning.push_back(temp);
            threadVector.push_back(std::thread(&threadPool::ThreadFunc, this, this, temp));
        }
        std::atomic<bool> temp = false;
        ifRunning.push_back(temp);
        threadVector.push_back(std::thread(&threadPool::expansionAndShrinkThreadNumber, this, this, temp));
    }
    catch (...)
    {
        printf("create thread abnomal\n");
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
}

void threadPool::ThreadFunc(void *threadData, std::atomic<bool> &ifRunning)
{
    threadPool *tPtr = (threadPool *)threadData;
    CMemory *memoryPtr = CMemory::GetInstance();
    while (true)
    {
        std::unique_lock<std::mutex> ulk(tPtr->pthreadMutex);
        tPtr->pthreadCondition.wait(ulk, [&]()
                                    { if (ifRunning == false) ifRunning = true;
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

        /*
         */
    }
}

void threadPool::destroyAllThreads()
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

void threadPool::inMsgRecvQueueAndSignal(char *buf)
{
    std::unique_lock<std::mutex> ulk(this->pthreadMutex);

    this->messageRecvQueue.push_back(buf);

    ulk.unlock();
again:
    if(this->callAHandlerThread() == -1)
    {
        printf("wait for 5s\n");
        std::this_thread::sleep_for(std::chrono::minutes(5));
        goto again;
    }
}

int threadPool::callAHandlerThread()
{
    if(this->createThreadNumber == this->runningThreadNumbers)
    {
        printf("threadNums is not enough\n");
        return - 1;
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