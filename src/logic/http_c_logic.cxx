#include "http_macro.h"
#include "http_c_crc32.h"
#include "http_global.h"
CLogicSocket::CLogicSocket()
{
    // handlerVector.push_back(&CLogicSocket::logicHandlerLogin);
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

    // if(imsgCode >= handlerVector.size())
    // {
    //     printf("error msgCode\n");
    //     return;
    // }

    // (this->*(handlerVector[imsgCode]))(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen);
    logicHandlerLogin(pConn, pMsgHeader, pPkgHeader, (char *)pPkgBody, pkglen);
}

bool CLogicSocket::logicHandlerLogin(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, COMM_PKG_HEADER_PTR pPkgHeader, char *pkgPtr, int pkgLen)
{
    printf("PkgLen = %hu, MsgCode = %hu, crc32 = %d\n", ntohs(pPkgHeader->pkgLen), ntohs(pPkgHeader->msgCode),
           pPkgHeader->crc32);
    STRUCT_LOGIN_PTR tempLoginPtr = (STRUCT_LOGIN_PTR)pkgPtr;
    printf("usrname : %s, password : %s\n", tempLoginPtr->username, tempLoginPtr->password);
    return true;
}