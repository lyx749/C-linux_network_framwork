#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
#include <stdio.h>
#include "src/include/http_c_crc32.h"
#pragma pack(1) // 内存对齐1个字节
typedef struct _COMM_PKG_HEADER
{
    unsigned short pkgLen;  // “包头加包体”的总长度
    unsigned short msgCode; // 消息类型
    int crc32;              // CRC32位校验码
} COMM_PKG_HEADER_T, *COMM_PKG_HEADER_PTR;

typedef struct _STRUCT_LOGIN
{
    char username[56]; // 用户名
    char password[40]; // 密码

} STRUCT_LOGIN_T, *STRUCT_LOGIN_PTR;
#pragma pack()

int PKG_HEADER_LEN = sizeof(COMM_PKG_HEADER_T);

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

    std::pair<char *, ssize_t> initPakage()
    {
        char *temp = new char[PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T)]{};
        COMM_PKG_HEADER_PTR pakageHeaderPtr = (COMM_PKG_HEADER_PTR)temp;
        pakageHeaderPtr->pkgLen = htons(PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T));

        pakageHeaderPtr->msgCode = htons(1);

        STRUCT_LOGIN_PTR loginPtr = (STRUCT_LOGIN_PTR)(temp + PKG_HEADER_LEN);
        strcpy(loginPtr->password, "123456");
        strcpy(loginPtr->username, "lyx");
        int calcCrc = CCRC32::GetInstance()->Get_CRC((unsigned char *)loginPtr, sizeof(STRUCT_LOGIN_T));
        printf("%d\n", calcCrc);
        pakageHeaderPtr->crc32 = htonl(calcCrc);
        return std::make_pair(temp, PKG_HEADER_LEN + sizeof(STRUCT_LOGIN_T));
    }

    int initSocket()
    {
        char IP[INET_ADDRSTRLEN]{};
        uint16_t port;
        scanf("%s %hu", IP, &port);
        struct sockaddr_in serverAddr;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("socket error");
            exit(1);
        }
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_addr.s_addr = inet_addr(IP);
        serverAddr.sin_family = AF_INET;

        serverAddr.sin_port = htons(port);
        if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            perror("connect error");
            exit(1);
        }
        printf("connect to IP = %s port = %hu\n", IP, port);
        return sockfd;
    }

    int sendPakage(std::pair<char *, ssize_t> buff, int sockfd)
    {
        ssize_t needSendLen = buff.second;
        ssize_t sendedLen = 0;
        ssize_t tempLen = 0;
        printf("needSendLen = %ld\n", needSendLen);
        while (sendedLen < needSendLen)
        {
            if ((tempLen = send(sockfd, buff.first, needSendLen, 0)) < 0)
            {
                perror("send error");
                exit(1);
            }

            sendedLen += tempLen;
            printf("SendLen = %ld\n", sendedLen);
        }
        delete[] buff.first;
        return sendedLen;
    }
};
client *client::clientInterface = NULL;
int main()
{
    client *cPtr = client::getInterface();
    int sockfd = cPtr->initSocket();
    std::pair<char *, ssize_t> buff = cPtr->initPakage();
    cPtr->sendPakage(buff, sockfd);
}