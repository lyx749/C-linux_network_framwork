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
    printf("imsgCode = %d handlerVector.szie() = %d\n", imsgCode, handlerVector.size());
    if (imsgCode >= handlerVector.size())
    {
        printf("error msgCode\n");
        return;
    }
    if (!(this->*(handlerVector[imsgCode]))(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen))
    {
        perror("CLogicSocket::threadRecvProcFunc handlerVector[imsgCode] error");
        return;
    }
    // logicHandlerLogin(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen);
}

bool CLogicSocket::logicHandlerLogin(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, COMM_PKG_HEADER_PTR pPkgHeader, char *pkgBodyPtr, int pkgLen)
{
    if (pkgBodyPtr == NULL)
        return false;

    int iRecvLen = sizeof(STRUCT_LOGIN_T);

    if (iRecvLen != pkgLen - sizeof(COMM_PKG_HEADER_T))
        return false;

    std::lock_guard<std::mutex> lk(pConn->logicProcMutex);

    STRUCT_LOGIN_PTR p_RecvInfo = (STRUCT_LOGIN_PTR)pkgBodyPtr;
    /*
    因为还没连接数据库，这个地方可以做一个用户名和密码匹配，成功返回成功连接，失败返回失败
    所以为了测试方便默认正确
    p_RecvInfo->username[sizeof(p_RecvInfo->username)-1]=0;
    p_RecvInfo->password[sizeof(p_RecvInfo->password)-1]=0;
    */
    COMM_PKG_HEADER_PTR tempHeaderPtr;
    CCRC32 *p_crc32 = CCRC32::GetInstance();
    CMemory *memoryPtr = CMemory::GetInstance();
    char *sendBuff = (char *)memoryPtr->AllocMemory(MSG_HEADER_LEN + PKG_HEADER_LEN + 100, true);   //因为还没确定回包的结构，这里先给100个字节
    memcpy(sendBuff, pMsgHeader, MSG_HEADER_LEN);
    tempHeaderPtr = (COMM_PKG_HEADER_PTR)(sendBuff + MSG_HEADER_LEN);

    pPkgHeader->msgCode = htons(10);    //同样是未确定

    pPkgHeader->pkgLen = htons(PKG_HEADER_LEN + 100);

    char *backPackage = (char *)(sendBuff + MSG_HEADER_LEN + PKG_HEADER_LEN);
    strcpy(backPackage, "success login");
    pPkgHeader->crc32 = htonl(p_crc32->Get_CRC((unsigned char *)backPackage, 100));
    msgSend(sendBuff);
    return true;
}