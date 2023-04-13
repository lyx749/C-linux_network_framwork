#ifndef HTTP_C_LOGIC_H
#define HTTP_C_LOGIC_H
#include "http_c_socket.h"
class CLogicSocket;
typedef bool (CLogicSocket::*logicHandler)(http_connection_ptr, STRUCT_MSG_HEADER_PTR, char *, int);
class CLogicSocket : public CSocket
{
private:
    std::vector<logicHandler> handlerVector;

public:
    CLogicSocket();
    virtual ~CLogicSocket(){};
    virtual bool Initialize();
    virtual void threadRecvProcFunc(char *pMsgBuf);
    virtual void procPingTimeOutChecking(STRUCT_MSG_HEADER_PTR pMsgHeader, time_t currrntTime); // 重载父类函数
    virtual void addThreadFuncToVector();

private:
    // 业务逻辑处理函数
    bool logicHandlerLogin(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen);
    bool logicHandlerPing(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen);
    bool logicHandlerHttp(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, char *pkgBodyPtr, int pkgLen);

private:
    void SendNoBodyPkgToClient(STRUCT_MSG_HEADER_PTR pMsgHeader, unsigned short msgCode);
    char *buildHttpPackage();
};

#endif