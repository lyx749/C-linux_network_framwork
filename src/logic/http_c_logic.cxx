#include "http_macro.h"
#include "http_c_crc32.h"
#include "http_global.h"
#include "http_c_memory.h"
std::vector<logicHandler> handlerVector;
CLogicSocket::CLogicSocket()
{
}

void CLogicSocket::addThreadFuncToVector()
{
    handlerVector.push_back(&CLogicSocket::logicHandlerLogin);
    handlerVector.push_back(&CLogicSocket::logicHandlerPing);
}

bool CLogicSocket::Initialize()
{
    bool bParentInit = CSocket::Initialize();
    return bParentInit;
}

void CLogicSocket::threadRecvProcFunc(char *pMsgBuf)
{
    STRUCT_MSG_HEADER_PTR pMsgHeader = (STRUCT_MSG_HEADER_PTR)pMsgBuf;
    COMM_PKG_HEADER_PTR pPkgHeader = (COMM_PKG_HEADER_PTR)(pMsgBuf + MSG_HEADER_LEN);

    void *pPkgBody;

    uint16_t pkglen = ntohs(pPkgHeader->pkgLen);

    if (pkglen == PKG_HEADER_LEN)
    {
        if (pPkgHeader->crc32 != 0) // 只有包头的crc值给0
        {
            return;
        }
        pPkgBody = NULL;
    }
    else
    {
        pPkgHeader->crc32 = ntohl(pPkgHeader->crc32);
        pPkgBody = (void *)(pMsgBuf + PKG_HEADER_LEN + MSG_HEADER_LEN);

        int calcCrc = CCRC32::GetInstance()->Get_CRC((unsigned char *)pPkgBody, pkglen - PKG_HEADER_LEN);
        if (calcCrc != pPkgHeader->crc32)
        {
            perror("crc32 is not right");
            return;
        }

        else
        {
            printf("success get client package\n");
        }
    }
    // crc校验ok
    uint16_t imsgCode = ntohs(pPkgHeader->msgCode);
    http_connection_ptr pConn = pMsgHeader->pConn;

    if (pConn->inCurrsequence != pMsgHeader->inCurrsequence)
    {
        return;
    }
    // printf("imsgCode = %d handlerVector.szie() = %d\n", imsgCode, handlerVector.size());
    if (imsgCode >= handlerVector.size())
    {
        printf("error msgCode\n");
        return;
    }
    if (!(this->*(handlerVector[imsgCode]))(pConn, pMsgHeader, (char *)pPkgBody, pkglen))
    {
        perror("CLogicSocket::threadRecvProcFunc handlerVector[imsgCode] error");
        return;
    }
    // logicHandlerLogin(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen);
}

void CLogicSocket::procPingTimeOutChecking(STRUCT_MSG_HEADER_PTR pMsgHeader, time_t currentTime)
{
    CMemory *memoryPtr = CMemory::GetInstance();
    http_connection_ptr pConn = pMsgHeader->pConn;
    if (pConn->inCurrsequence == pMsgHeader->inCurrsequence)
    {
        if (this->ifOpenTimeoutKick == 1) // 要求超时就关闭连接
        {
            zdCloseSocketProc(pConn);
        }

        else if ((currentTime - pConn->lastPingTime) >= (3 * waitTime + 10)) // 超时踢的判断标准就是 每次检查的时间间隔*3，超过这个时间没发送心跳包，就踢
        {
            zdCloseSocketProc(pConn);
        }
    }
    else
        memoryPtr->FreeMemory(pMsgHeader);
}

// 发送心跳回应包给客户端
void CLogicSocket::SendNoBodyPkgToClient(STRUCT_MSG_HEADER_PTR pMsgHeader, unsigned short msgCode)
{
    CMemory *memoryPtr = CMemory::GetInstance();

    char *sendBuff = (char *)memoryPtr->AllocMemory(PKG_HEADER_LEN + MSG_HEADER_LEN, false);
    char *tmpBuff = sendBuff;

    memcpy(tmpBuff, pMsgHeader, MSG_HEADER_LEN);
    tmpBuff += MSG_HEADER_LEN;

    COMM_PKG_HEADER_PTR pPkgHeader = (COMM_PKG_HEADER_PTR)tmpBuff;
    pPkgHeader->pkgLen = htons(PKG_HEADER_LEN);
    pPkgHeader->crc32 = 0; // 因为没有包体
    pPkgHeader->msgCode = htons(msgCode);
    msgSend(sendBuff);
}

bool CLogicSocket::logicHandlerLogin(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen)
{
    if (pkgBodyPtr == NULL)
        return false;

    int iRecvLen = sizeof(STRUCT_LOGIN_T);

    if (iRecvLen != pkgLen - sizeof(COMM_PKG_HEADER_T))
        return false;

    std::lock_guard<std::mutex> lk(pConn->logicProcMutex); // 为了保证同一条连接的多个数据包按顺序处理

    STRUCT_LOGIN_PTR p_RecvInfo = (STRUCT_LOGIN_PTR)pkgBodyPtr;
    /*
    因为还没连接数据库，这个地方可以做一个用户名和密码匹配，成功返回成功连接，失败返回失败
    所以为了测试方便默认正确
    */
    p_RecvInfo->username[sizeof(p_RecvInfo->username)-1]=0;
    p_RecvInfo->password[sizeof(p_RecvInfo->password)-1]=0;
    COMM_PKG_HEADER_PTR tempHeaderPtr;
    CCRC32 *p_crc32 = CCRC32::GetInstance();
    CMemory *memoryPtr = CMemory::GetInstance();
    char *sendBuff = (char *)memoryPtr->AllocMemory(MSG_HEADER_LEN + PKG_HEADER_LEN + 100, true); // 因为还没确定回包的结构，这里先给100个字节
    memcpy(sendBuff, pMsgHeader, MSG_HEADER_LEN);
    tempHeaderPtr = (COMM_PKG_HEADER_PTR)(sendBuff + MSG_HEADER_LEN);

    tempHeaderPtr->msgCode = htons(10); // 同样是未确定

    tempHeaderPtr->pkgLen = htons(PKG_HEADER_LEN + 100);

    char *backPackage = (char *)(sendBuff + MSG_HEADER_LEN + PKG_HEADER_LEN);
    strcpy(backPackage, "success login");
    tempHeaderPtr->crc32 = htonl(p_crc32->Get_CRC((unsigned char *)backPackage, 100));
    msgSend(sendBuff);
    // printf("success push send package\n");
    return true;
}

bool CLogicSocket::logicHandlerPing(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen)
{
    if (pkgLen - PKG_HEADER_LEN != 0) // 有包体则认为是非法包
        return false;

    std::lock_guard<std::mutex> lk(pConn->logicProcMutex);
    printf("accept ping\n");
    pConn->lastPingTime = time(NULL);

    SendNoBodyPkgToClient(pMsgHeader, 1);
    return true;
}
