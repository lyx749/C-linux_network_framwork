#ifndef HTTP_C_LOGIC_H
#define HTTP_C_LOGIC_H
#include "http_c_socket.h"
// typedef bool(CLogicSocket::*logicHandler)(http_connection_ptr, STRUCT_MSG_HEADER_PTR, COMM_PKG_HEADER_PTR, char *, int);
// std::vector<logicHandler> handlerVector;
class CLogicSocket : public CSocket
{
public:
    CLogicSocket();
    virtual ~CLogicSocket(){};
    virtual bool Initialize();
    virtual void threadRecvProcFunc(char *pMsgBuf);

public:
    bool logicHandlerLogin(http_connection_ptr pConn, STRUCT_MSG_HEADER_PTR pMsgHeader, COMM_PKG_HEADER_PTR pPkgHeader, char *pkgPtr, int pkgLen);
};

#endif