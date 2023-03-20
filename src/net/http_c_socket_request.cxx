#include "http_c_socket.h"

void CSocket::readRequestHandler(http_connection_ptr pConn)
{
    
}


void CSocket::writeRequestHandler(http_connection_ptr pConn)
{

}


 ssize_t CSocket::recvProc(http_connection_ptr pConn, char *buff, ssize_t buflen)
 {
     ssize_t n = recv(pConn->fd, buff, buflen, 0);
     if(n == 0) // client close connetion
     {
         closeConnection(pConn);
     }
 }