#include "http_c_socket.h"
typedef struct _STRUCT_LOGIN
{
    char username[56]; // 用户名
    char password[40]; // 密码

} STRUCT_LOGIN_T, *STRUCT_LOGIN_PTR;
#pragma pack()
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
        } // 这个属于客户端出错，下面属于服务器出错
        else
        {
            if (errno == EBADF) // #define EBADF   9 /* Bad file descriptor */
            {
                // 多线程偶尔出现这个错误，关闭了文件描述符
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

void CSocket::httpWaitRequestHandlerProcHeader(http_connection_ptr pConn, bool &isFlood)
{
    CMemory *CMemoryPtr = CMemory::GetInstance();
    COMM_PKG_HEADER_PTR pakagePtr = (COMM_PKG_HEADER_PTR)pConn->dataHeadInfo;
    uint16_t pakageLen = ntohs(pakagePtr->pkgLen);
    printf("%hu\n", pakageLen);
    if (pakageLen < PKG_HEADER_LEN) // 数据包的总长度比包头长度还小，肯定是伪造数据包
    {
        printf("pakageLen < PKG_HEADER_LEN error\n");
        pConn->recvBufHeadPtr = pConn->dataHeadInfo;
        pConn->currentStat = _PKG_HD_INIT;
        pConn->needRecvLen = PKG_HEADER_LEN;
        return;
    }

    if (pakageLen >= _PKG_MAX_LENGTH - 1000) // pakage too len
    {
        printf("pakageLen >= _PKG_MAX_LENGTH - 1000 error\n");
        pConn->recvBufHeadPtr = pConn->dataHeadInfo;
        pConn->currentStat = _PKG_HD_INIT;
        pConn->needRecvLen = PKG_HEADER_LEN;
        return;
    }

    if (pakageLen == PKG_HEADER_LEN) // 只有一个包头
    {
        httpWaitRequestHandlerProcBody(pConn, isFlood);
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
    // 开始收包体
    pConn->currentStat = _PKG_BD_INIT;
    pConn->recvBufHeadPtr = recvBuff;
    pConn->needRecvLen = pakageLen - PKG_HEADER_LEN;
    return;
}

void CSocket::httpWaitRequestHandlerProcBody(http_connection_ptr pConn, bool &isFlood)
{
    /*
    donothing
    */
    testHandle(pConn);
    pConn->recvMemoryPtr = NULL;
    pConn->recvBufHeadPtr = pConn->dataHeadInfo;
    pConn->needRecvLen = PKG_HEADER_LEN;
    pConn->currentStat = _PKG_HD_INIT;
    return;
}

void CSocket::testHandle(http_connection_ptr pConn)
{
    char *temp = pConn->recvMemoryPtr + MSG_HEADER_LEN;
    COMM_PKG_HEADER_PTR tempHeaderPtr = (COMM_PKG_HEADER_PTR)temp;
    printf("PkgLen = %hu, MsgCode = %hu, crc32 = %d\n", ntohs(tempHeaderPtr->pkgLen), ntohs(tempHeaderPtr->msgCode), 
    ntohl(tempHeaderPtr->crc32));
    temp += PKG_HEADER_LEN;
    STRUCT_LOGIN_PTR tempLoginPtr = (STRUCT_LOGIN_PTR)temp;
    printf("usrname : %s, password : %s\n", tempLoginPtr->username, tempLoginPtr->password);
}

void CSocket::writeRequestHandler(http_connection_ptr pConn)
{
}
