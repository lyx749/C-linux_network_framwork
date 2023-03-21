#include "http_c_socket.h"

void CSocket::testHandle(http_connection_ptr pConn)
{
    char buff[4096]{};
    ssize_t n = recv(pConn->fd, buff, 4, 0);
    if (n == 0)
    {
        return;
    }
    printf("recive message from IP = %s, port = %d, message size is %lu, message is %s\n", inet_ntoa(pConn->clienAddr.sin_addr),
           ntohs(pConn->clienAddr.sin_port), n, buff);
}

void CSocket::readRequestHandler(http_connection_ptr pConn)
{
    bool isFlood = false;
    ssize_t recvLen = recvProc(pConn, pConn->dataHeadInfo, pConn->needRecvLen);
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
        if (recvLen = pConn->needRecvLen)
        {
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
        if (recvLen = pConn->needRecvLen)
        {
            httpWaitRequestHandlerProcBody(pConn, isFlood);
        }
        else
        {
            pConn->recvBufHeadPtr += recvLen;
            pConn->needRecvLen -= recvLen;
        }
    }

    if (isFlood)
    {
        closeConnection(pConn);
    }

    return;
}

ssize_t CSocket::recvProc(http_connection_ptr pConn, char *buff, ssize_t buflen)
{
    ssize_t n = recv(pConn->fd, buff, buflen, 0);
    if (n == 0) // client close connetion
    {
        printf("client closed connect\n");
        closeConnection(pConn);
        return -1;
    }

    if (n == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // 因为采用LT模式，这个错误一般时ET模式下出的，ET模式不断地读就会出现这个错误
        {
            perror("CSocket::recvProc's recv errno == EAGAIN || errno == EWOULDBLOCK");
            return -1;
        }
        if (errno == EINTR) // 慢系统调用，阻塞
        {
            perror("CSocket::recvProc's recv errno == EINTR");
            return -1;
        }
        // 以上均不属于异常，不需要将客户从连接池移出

        if (errno == ECONNRESET) // #define ECONNRESET 104 /* Connection reset by peer */
        {
            // 如果客户端没有正常关闭socket连接，却关闭了整个运行程序【真是够粗暴无理的，应该是直接给服务器发送rst包而不是4次挥手包完成连接断开】，那么会产生这个错误
            // 10054(WSAECONNRESET)--远程程序正在连接的时候关闭会产生这个错误--远程主机强迫关闭了一个现有的连接
            perror("CSocket::recvProc's recv errno == ECONNRESET");
        }//这个属于客户端出错，下面属于服务器出错
        else
        {
            if(errno == EBADF)// #define EBADF   9 /* Bad file descriptor */
            {
                //多线程偶尔出现这个错误，关闭了文件描述符
                perror("CSocket::recvProc's recv errno == EBADF");
            }
            else
            {
                fprintf(stderr, "CSocket::recvProc's recv errno = %s", strerror(errno));
            }
        }
        closeConnection(pConn);
        return -1;
    }
    return n;
}

void CSocket::httpWaitRequestHandlerProcHeader(http_connection_ptr pConn, bool isFlood)
{
    
}

void CSocket::httpWaitRequestHandlerProcBody(http_connection_ptr pConn, bool isFlood)
{

}
void CSocket::writeRequestHandler(http_connection_ptr pConn)
{

}
