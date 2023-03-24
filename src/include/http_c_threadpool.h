#ifndef HTTP_C_THREADPOOL_H
#define HTTP_C_THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <list>
#include <future>
#include <chrono>
class WorkerThreadPool
{
public:
    WorkerThreadPool();
    ~WorkerThreadPool();

public:
    bool createAllThreads(int threadNumber);
    void destroyAllThreads();

    void inMsgRecvQueueAndSignal(char *buf); // 收到一个完整消息之后，入消息队列，并触发线程池中线程来处理消息
    int callAHandlerThread();               // 唤醒一个线程来干活
    int getRecvMsgQueueCount() { return this->messageRecvQueueCount; }
    //void expansionAndShrinkThreadNumber(void *threadData, bool &ifRunning, bool &ifWorking);

private:
    void ThreadFunc(void *threadData, int i);
    void clearMsgRecvQueue();

private:
    std::mutex pthreadMutex;                  // 线程同步互斥量
    std::condition_variable pthreadCondition; // 线程同步条件变量
    static bool ifThreadExit;          // 线程退出标志，false不退出，true退出

    int createThreadNumber;

    std::atomic<int> runningThreadNumbers;
    time_t lastEmgTime; // 上次发生线程不够用的时间

    std::vector<std::thread> threadVector;

    std::list<char *> messageRecvQueue;
    int messageRecvQueueCount;
    std::vector<bool> ifRunning;
};

#endif