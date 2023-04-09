#include "http_macro.h"
#include "http_c_crc32.h"
#include "http_global.h"
#include "http_c_memory.h"
#include "http_log.h"
CLogicSocket::CLogicSocket()
{
}

void CLogicSocket::addThreadFuncToVector()
{
    handlerVector.push_back(&CLogicSocket::logicHandlerLogin);
    handlerVector.push_back(&CLogicSocket::logicHandlerPing);
    handlerVector.push_back(&CLogicSocket::logicHandlerHttp);
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
            // perror("crc32 is not right");
            return;
        }
    }
    // crc校验ok
    uint16_t imsgCode = ntohs(pPkgHeader->msgCode);
    http_connection_ptr pConn = pMsgHeader->pConn;

    if (pConn->inCurrsequence != pMsgHeader->inCurrsequence)
    {
        return;
    }
    if (imsgCode >= handlerVector.size())
    {
        // printf("error msgCode\n");
        return;
    }
    if (!(this->*(handlerVector[imsgCode]))(pConn, pMsgHeader, (char *)pPkgBody, pkglen))
    {
        // perror("CLogicSocket::threadRecvProcFunc handlerVector[imsgCode] error");
        return;
    }
    // logicHandlerLogin(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen);
}

void CLogicSocket::procPingTimeOutChecking(STRUCT_MSG_HEADER_PTR pMsgHeader, time_t currentTime)
{
    myLog *logPtr = myLog::getInterface();
    CMemory *memoryPtr = CMemory::GetInstance();
    http_connection_ptr pConn = pMsgHeader->pConn;
    if (pConn->inCurrsequence == pMsgHeader->inCurrsequence)
    {
        if (this->ifOpenTimeoutKick == 1) // 要求超时就关闭连接
        {
            zdCloseSocketProc(pConn);
            logPtr->getLogger()->warn("user IP = {} port = {} does not send heartbeat packet rejection for a long time, kick off!", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
        }

        else if ((currentTime - pConn->lastPingTime) >= (3 * waitTime + 10)) // 超时踢的判断标准就是 每次检查的时间间隔*3，超过这个时间没发送心跳包，就踢
        {
            logPtr->getLogger()->warn("user IP = {} port = {} does not send heartbeat packet rejection for a long time, kick off!", inet_ntoa(pConn->clienAddr.sin_addr), ntohs(pConn->clienAddr.sin_port));
            zdCloseSocketProc(pConn);
        }
    }
    else
        memoryPtr->FreeMemory(pMsgHeader);
}
