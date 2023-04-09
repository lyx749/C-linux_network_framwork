#include "http_macro.h"
#include "http_c_crc32.h"
#include "http_global.h"
#include "http_c_memory.h"
#include "http_log.h"
#include "http_parser.h"
#include <iostream>

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
    p_RecvInfo->username[sizeof(p_RecvInfo->username) - 1] = 0;
    p_RecvInfo->password[sizeof(p_RecvInfo->password) - 1] = 0;
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
    // httpCommonLog("accept ping\n");
    pConn->lastPingTime = time(NULL);

    SendNoBodyPkgToClient(pMsgHeader, 1);
    return true;
}

bool CLogicSocket::logicHandlerHttp(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen)
{
    httpParser http(pkgBodyPtr);
    std::vector<std::pair<std::string, std::string>> package = http.getHttp();
    for(auto e : package)
    {
        std::cout << e.first << " " << e.second << std::endl;
    }
    return true;
}
