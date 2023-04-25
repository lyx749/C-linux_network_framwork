#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <map>
#include <stdio.h>
#include "src/include/http_c_crc32.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <vector>
#include <thread>
#pragma pack(1) // 内存对齐1个字节
typedef struct _COMM_PKG_HEADER
{
    unsigned short pkgLen;  // “包头加包体”的总长度
    unsigned short msgCode; // 消息类型
    int crc32;              // CRC32位校验码
} COMM_PKG_HEADER_T, *COMM_PKG_HEADER_PTR;

typedef struct _STRUCT_LOGIN
{
    char username[56]{}; // 用户名
    char password[40]{}; // 密码

} STRUCT_LOGIN_T, *STRUCT_LOGIN_PTR;
#pragma pack()

int PKG_HEADER_LEN = sizeof(COMM_PKG_HEADER_T);

std::pair<char *, ssize_t> makeHttpPackage()
{
    int fd = open("2.txt", O_RDONLY);
    char *msg = new char[1000]{};
    ssize_t n = read(fd, msg, 1000);
    return std::make_pair(msg, n + 1);
}

class client
{
private:
    static client *clientInterface;
    client(){};

public:
    ~client() {}
    static client *getInterface()
    {
        if (!clientInterface)
        {
            clientInterface = new client();
            static deleteInterface del;
        }
        return clientInterface;
    }

    class deleteInterface
    {
    public:
        ~deleteInterface()
        {
            delete clientInterface;
            clientInterface = NULL;
        }
    };

    std::pair<char *, ssize_t> initPingPackage()
    {
        char *sendBuff = new char[PKG_HEADER_LEN]{};
        COMM_PKG_HEADER_PTR packageHeaderPtr = (COMM_PKG_HEADER_PTR)sendBuff;
        packageHeaderPtr->crc32 = 0;
        packageHeaderPtr->msgCode = htons(1);
        packageHeaderPtr->pkgLen = htons(PKG_HEADER_LEN);
        return std::make_pair(sendBuff, PKG_HEADER_LEN);
    }

    std::pair<char *, ssize_t> initPackage()
    {
        char *temp = new char[PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T)]{};
        COMM_PKG_HEADER_PTR packageHeaderPtr = (COMM_PKG_HEADER_PTR)temp;
        packageHeaderPtr->pkgLen = htons(PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T));

        packageHeaderPtr->msgCode = htons(0);

        STRUCT_LOGIN_PTR loginPtr = (STRUCT_LOGIN_PTR)(temp + PKG_HEADER_LEN);
        strcpy(loginPtr->password, "123456");
        strcpy(loginPtr->username, "lyx");
        int calcCrc = CCRC32::GetInstance()->Get_CRC((unsigned char *)loginPtr, sizeof(STRUCT_LOGIN_T));
        packageHeaderPtr->crc32 = htonl(calcCrc);
        return std::make_pair(temp, PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T));
    }

    std::pair<char *, ssize_t> initHttpPackage()
    {
        std::pair<char *, ssize_t> http = makeHttpPackage();
        char *temp = new char[PKG_HEADER_LEN + http.second]{};
        COMM_PKG_HEADER_PTR packageHeaderPtr = (COMM_PKG_HEADER_PTR)temp;
        packageHeaderPtr->pkgLen = htons(PKG_HEADER_LEN + http.second);

        packageHeaderPtr->msgCode = htons(2);
        char *next = (char *)(temp + PKG_HEADER_LEN);
        memcpy(next, http.first, http.second);
        delete[] http.first;
        int calcCrc = CCRC32::GetInstance()->Get_CRC((unsigned char *)next, http.second);
        printf("%d\n", calcCrc);
        packageHeaderPtr->crc32 = htonl(calcCrc);
        return std::make_pair(temp, PKG_HEADER_LEN + http.second);
    }

    std::vector<int> initSocket()
    {
        std::vector<int> sockVec(200, 0);
        for (int i = 0; i < 21; ++i)
        {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1)
            {
                perror("socked error");
                exit(1);
            }
            sockVec[i] = sockfd;
        }
        struct sockaddr_in serverAddr;
        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8848);
        serverAddr.sin_addr.s_addr = inet_addr("47.94.222.192");
        for (auto &e : sockVec)
        {
            if (connect(e, (sockaddr *)&serverAddr, sizeof(sockaddr)) < 0)
            {
                perror("connect error");
                exit(1);
            }
        }
        return sockVec;
    }

    int sendPackage(std::pair<char *, ssize_t> &buff, int sockfd)
    {
        ssize_t needSendLen = buff.second;
        ssize_t sendedLen = 0;
        ssize_t tempLen = 0;
        // printf("needSendLen = %ld\n", needSendLen);
        while (sendedLen < needSendLen)
        {
            if ((tempLen = send(sockfd, buff.first, needSendLen, 0)) < 0)
            {
                perror("send error");
                exit(1);
            }

            sendedLen += tempLen;
            // printf("SendLen = %ld\n", sendedLen);
        }
        delete[] buff.first;
        buff.first = NULL;
        return sendedLen;
    }

    int recvPackage(char *recvBuff, int fd)
    {
        ssize_t n = recv(fd, recvBuff, sizeof(recvBuff), 0);
        if (n < 0)
            perror("recv error");

        return n;
    }
};
client *client::clientInterface = NULL;

void threadFunc()
{
    client *cPtr = client::getInterface();
    std::vector<int> sockVec = cPtr->initSocket();
    while (1)
    {
        for (auto &e : sockVec)
        {
            std::pair<char *, ssize_t> buff = cPtr->initPackage();
            cPtr->sendPackage(buff, e);
        }
        for (auto &e : sockVec)
        {
            std::pair<char *, ssize_t> buff = cPtr->initPingPackage();
            cPtr->sendPackage(buff, e);
        }
        sleep(1);
    }
}

int main()
{
    std::vector<std::thread> threadPool;
    for (int i = 0; i <93; ++i)
    {
        threadPool.push_back(std::thread(threadFunc));
    }

    for (auto &e : threadPool)
    {
        if (e.joinable())
            e.join();
    }
    // close(sockfd);
}