#include "http_c_socket.h"
#include "../../build/httpServerConfig.h"
#include "http_c_memory.h"
#include "http_global.h"
void CSocket::httpEventAccept(http_connection_ptr oldc)
{
    struct sockaddr clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int level;

    int s;
    http_connection_ptr newc; // 代表连接池的一个连接

    do
    {
        s = accept(oldc->fd, &clientAddr, &clientAddrLen);
        if (s == -1)
        {
            // 对accept、send和recv而言，事件未发生时errno通常被设置成EAGAIN（意为“再来一次”）或者EWOULDBLOCK（意为“期待阻塞”）
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            level = HTTP_LOG_ALERT;

            /*
            该错误被描述为“software caused connection abort”，即“软件引起的连接中止”。原因在于当服务和客户进程在完成用于 TCP 连接的“三次握手”后，
            客户 TCP 却发送了一个 RST （复位）分节，在服务进程看来，就在该连接已由 TCP 排队，等着服务进程调用 accept 的时候 RST 却到达了。
            POSIX 规定此时的 errno 值必须 ECONNABORTED。源自 Berkeley 的实现完全在内核中处理中止的连接，服务进程将永远不知道该中止的发生。
            服务器进程一般可以忽略该错误，直接再次调用accept。
            */
            // ECONNRESET错误则发生在对方意外关闭套接字后【您的主机中的软件放弃了一个已建立的连接--由于超时或者其它失败而中止接连(用户插拔网线就可能有这个错误出现)】
            if (errno == ECONNABORTED)
                level = HTTP_LOG_ERR;

            /*
            EMFILE：进程fd已用尽{已达到系统所允许单一进程所能打开的文件/套接字总数}
            需要扩充文件描述符
            */
            else if (errno == EMFILE)
                level = HTTP_LOG_CRIT;

            if (errno == ECONNABORTED) // 对方关闭套接字
            {
                // 这个错误因为可以忽略，所以不用干啥
                // do nothing
            }
            // if (useAceept4 && errno == ENOSYS) // accpet4()函数未实现
            // {
            //     useAceept4 = 0;
            //     continue;
            // }
            return;
        }

        // 处理恶意连接的代码
        if (connectionPool.size() > (worker_connections)*5)
        {
            // 比如你允许同时最大2048个连接，但连接池却有了 2048*5这么大的容量，这肯定是表示短时间内 产生大量连接/断开，因为延迟回收机制，这里连接还在垃圾池里没有被回收
            if (freeConnectionPool.size() < workerConnections)
            {
                close(s);
                return;
            }
        }
        if (fcntl(s, F_SETFL, O_NONBLOCK) == -1)
        {
            perror("CSocket::httpEventAccept's fcntl error");
            close(s);
            return;
        }
        newc = getAConnectionFromPool(s);

        memcpy(&newc->clienAddr, &clientAddr, clientAddrLen);
        newc->listening = oldc->listening; // 将连接对象和监听端口绑定

        newc->readHandler = &CSocket::readRequestHandler;
        newc->writeHandler = &CSocket::writeRequestHandler;

        if (httpEpollOperEvent(s, EPOLL_CTL_ADD, EPOLLIN | EPOLLRDHUP, 0, newc) == -1)
        {
            closeConnection(newc);
            return;
        }
        ++onlineUserCount;
        break;
    } while (1);
}
