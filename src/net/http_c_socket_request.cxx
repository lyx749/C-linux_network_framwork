#include "http_c_socket.h"
#include "httpServerConfig.h"
#include "http_c_memory.h"
#include "http_global.h"
#include "../logs/http_log.h"
void CSocket::readRequestHandler(http_connection_ptr pConn)
{
    bool isFlood = false;
    ssize_t recvLen = recvProc(pConn, pConn->recvBufHeadPtr, pConn->needRecvLen);
    if (recvLen <= 0) // recvproc函数已经处理完了
    {
        return;
    }
    if (pConn->currentStat == _PKG_HD_INIT)
    {
        if (recvLen == pConn->needRecvLen)
        {
            httpWaitRequestHandlerProcHeader(pConn, isFlood);
        }
        else
        {
            pConn->needRecvLen -= recvLen;
            pConn->recvBufHeadPtr += recvLen;
            pConn->currentStat = _PKG_HD_RECVING;
        }
    }
    else if (pConn->currentStat == _PKG_HD_RECVING)
    {
        if (recvLen == pConn->needRecvLen)
        {
            httpWaitRequestHandlerProcHeader(pConn, isFlood);
        }
        else
        {
            pConn->needRecvLen -= recvLen;
            pConn->recvBufHeadPtr += recvLen;
        }
    }
    else if (pConn->currentStat == _PKG_BD_INIT)
    {
        if (recvLen == pConn->needRecvLen)
        {
            if (this->floodAkEnable)
                isFlood = this->testFlood(pConn);
            httpWaitRequestHandlerProcBody(pConn, isFlood);
        }
        else
        {
            pConn->currentStat = _PKG_BD_RECVING;
            pConn->recvBufHeadPtr += recvLen;
            pConn->needRecvLen -= recvLen;
        }
    }
    else if (pConn->currentStat == _PKG_BD_RECVING)
    {
        if (recvLen == pConn->needRecvLen)
        {
            httpWaitRequestHandlerProcBody(pConn, isFlood);
        }
        else
        {
            pConn->recvBufHeadPtr += recvLen;
            pConn->needRecvLen -= recvLen;
        }
    }

    // if (isFlood)
    // {
    //     httpErrorLog("Attacked by a flood, IP = %s, port = %d\n", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
    //     zdCloseSocketProc(pConn);
    // }

    return;
}

ssize_t CSocket::recvProc(http_connection_ptr pConn, char *buff, ssize_t buflen)
{
    ssize_t n = recv(pConn->fd, buff, buflen, 0);
    if (n == 0) // client close connetion
    {
        myLog::getInterface()->getLogger()->warn("client IP = %s port = %d, closed connect", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        zdCloseSocketProc(pConn);
        return -1;
    }

    if (n == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // 因为采用LT模式，这个错误一般时ET模式下出的，ET模式不断地读就会出现这个错误
        {
            myLog::getInterface()->getLogger()->error("CSocket::recvProc's recv error : {}", strerror(errno));
            return -1;
        }
        if (errno == EINTR) // 慢系统调用，阻塞
        {
            myLog::getInterface()->getLogger()->error("CSocket::recvProc's recv error : {}", strerror(errno));
            return -1;
        }
        // 以上均不属于异常，不需要将客户从连接池移出

        if (errno == ECONNRESET) // #define ECONNRESET 104 /* Connection reset by peer */
        {
            // 如果客户端没有正常关闭socket连接，却关闭了整个运行程序【真是够粗暴无理的，应该是直接给服务器发送rst包而不是4次挥手包完成连接断开】，那么会产生这个错误
            // 10054(WSAECONNRESET)--远程程序正在连接的时候关闭会产生这个错误--远程主机强迫关闭了一个现有的连接
            myLog::getInterface()->getLogger()->error("CSocket::recvProc's recv error : {}", strerror(errno));
        } // 这个属于客户端出错，下面属于服务器出错
        else
        {
            if (errno == EBADF) // #define EBADF   9 /* Bad file descriptor */
            {
                // 多线程偶尔出现这个错误，关闭了文件描述符
                myLog::getInterface()->getLogger()->error("CSocket::recvProc's recv error : {}", strerror(errno));
            }
            else
            {
                myLog::getInterface()->getLogger()->error("CSocket::recvProc's recv error : {}", strerror(errno));
            }
        }
        zdCloseSocketProc(pConn);
        return -1;
    }
    return n;
}

void CSocket::httpWaitRequestHandlerProcHeader(http_connection_ptr pConn, bool &isFlood)
{
    CMemory *CMemoryPtr = CMemory::GetInstance();
    COMM_PKG_HEADER_PTR pakagePtr = (COMM_PKG_HEADER_PTR)pConn->dataHeadInfo;
    uint16_t pakageLen = ntohs(pakagePtr->pkgLen);
    if (pakageLen < PKG_HEADER_LEN) // 数据包的总长度比包头长度还小，肯定是伪造数据包
    {
        myLog::getInterface()->getLogger()->warn("IP = {}, port = {}, pakageLen < PKG_HEADER_LEN error", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        pConn->recvBufHeadPtr = pConn->dataHeadInfo;
        pConn->currentStat = _PKG_HD_INIT;
        pConn->needRecvLen = PKG_HEADER_LEN;
        return;
    }

    if (pakageLen >= _PKG_MAX_LENGTH - 1000) // pakage too len
    {
        myLog::getInterface()->getLogger()->warn("IP = {}, port = {}, pakageLen >= _PKG_MAX_LENGTH - 1000 error", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        pConn->recvBufHeadPtr = pConn->dataHeadInfo;
        pConn->currentStat = _PKG_HD_INIT;
        pConn->needRecvLen = PKG_HEADER_LEN;
        return;
    }
    char *recvBuff = (char *)CMemoryPtr->AllocMemory(pakageLen + MSG_HEADER_LEN, false);
    pConn->recvMemoryPtr = recvBuff;
    // 填写消息体
    STRUCT_MSG_HEADER_PTR messageHeaderPtr = (STRUCT_MSG_HEADER_PTR)recvBuff;
    messageHeaderPtr->inCurrsequence = pConn->inCurrsequence;
    messageHeaderPtr->pConn = pConn;
    recvBuff += MSG_HEADER_LEN;
    // 填写包头
    memcpy(recvBuff, pConn->dataHeadInfo, PKG_HEADER_LEN);
    recvBuff += PKG_HEADER_LEN;

    if (pakageLen == PKG_HEADER_LEN) // 只有一个包头
    {
        if (this->floodAkEnable)
            isFlood = this->testFlood(pConn);
        httpWaitRequestHandlerProcBody(pConn, isFlood);
        return;
    }
    // 开始收包体
    pConn->currentStat = _PKG_BD_INIT;
    pConn->recvBufHeadPtr = recvBuff;
    pConn->needRecvLen = pakageLen - PKG_HEADER_LEN;
    return;
}

void CSocket::httpWaitRequestHandlerProcBody(http_connection_ptr pConn, bool &isFlood)
{
    if (isFlood)
    {
        myLog::getInterface()->getLogger()->warn("Attacked by a flood, IP = {}, port = {}", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        CMemory::GetInstance()->FreeMemory(pConn->recvMemoryPtr);
        zdCloseSocketProc(pConn);
        return;
    }
    g_threadpool.inMsgRecvQueueAndSignal(pConn->recvMemoryPtr);
    pConn->recvMemoryPtr = NULL;
    pConn->recvBufHeadPtr = pConn->dataHeadInfo;
    pConn->needRecvLen = PKG_HEADER_LEN;
    pConn->currentStat = _PKG_HD_INIT;
    return;
}

ssize_t CSocket::sendProc(http_connection_ptr pConn, char *sendBuff, ssize_t size)
{
    ssize_t n;
    while (true)
    {
        n = send(pConn->fd, sendBuff, size, 0);
        if (n > 0)
            return n;
        // 发送成功一些数据，但发送了多少，我这里不关心，也不需要再次send
        // 这里有两种情况
        //(1) n == size也就是想发送多少都发送成功了，这表示完全发完毕了
        //(2) n < size 没发送完毕，那肯定是发送缓冲区满了，所以也不必要重试发送，直接返回吧
        if (n == 0)
        {
            // send返回0代表对方关闭了连接
            return 0;
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // 缓冲区满了
            return -1;
        }

        if (errno == EINTR) // 慢系统调用，阻塞
        {
            continue;
        }
        else
        {
            return -2; // 一般来说这种情况属于对端关闭连接
        }
    }
}

void CSocket::writeRequestHandler(http_connection_ptr pConn)
{
    CMemory *memoryPtr = CMemory::GetInstance();
    ssize_t n = send(pConn->fd, pConn->sendPackageSendBuf, pConn->needSendLen, 0);

    if (n > 0 && n != pConn->needSendLen) // 数据没发送完并且缓冲区满，继续监听
    {
        pConn->sendPackageSendBuf += n;
        pConn->needSendLen -= n;
        return;
    }
    else if (n == -1)
    {
        // 虽然不太可能发生，因为已经通知我发送数据了，一般不会出现发送数据失败的问题，但是为了程序的健壮性还是打印一下
         myLog::getInterface()->getLogger()->error("CSocket::writeRequestHandler send error : {}", strerror(errno));
        return;
    }

    if (n > 0 && n == pConn->needSendLen) // 缓冲区可以全部发送完，说明缓冲区比较空闲，将写监听事件从红黑树上摘下
    {
        if (httpEpollOperEvent(pConn->fd, EPOLL_CTL_MOD, EPOLLOUT, 1, pConn) == -1)
        {
             myLog::getInterface()->getLogger()->error("CSocket::writeRequestHandler httpEpollOperEvent error");
            return;
        }
    }

    // 走到这的要么是数据发送完毕的，要么是对端断开了，执行收尾工作
    memoryPtr->FreeMemory(pConn->sendPackageMemPtr);
    pConn->sendPackageMemPtr = NULL;
    --pConn->iThrowSendCount;
    if (sem_post(&this->sendMsgQueueSem_t) == -1)
         myLog::getInterface()->getLogger()->error("CSocket::writeRequestHandler sem_post error : {}", strerror(errno));
}
