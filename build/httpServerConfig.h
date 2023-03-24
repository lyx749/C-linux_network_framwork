//配置文件
#pragma once


/*[log]*/
//log 日志目录和文件名
#define Log "error.log" 
//只打印日志等级 <= LogLevel 的日志到日志文件中
#define LogLevel 8

/*[Proc]*/
//需启动的 worker 进程数量
#define WorkerProcesses 1
//是否以守护进程方式运行
#define Daemon 1

//处理接收到的消息的线程池中线程数量
#define ProcMsgRecvWorkThreadCount 12

/*[Net]*/
//监听端口数量
#define ListenPortCount 1
//数量取决于ListenPortCount，以0开始
#define ListenPort 7777

//epoll连接的最大数
#define worker_connections 2048

//为确保系统稳定socket关闭后资源不会立即收回，连接池资源延迟回收
#define Sock_RecyConnectionWaitTime 150


//是否开启踢人时钟，1：开启   0：不开启
#define Sock_WaitTimeEnable 1
//多少秒检测一次心跳包，只有当Sock_WaitTimeEnable = 1时，有效
#define Sock_MaxWaitTime 20
//当时间到达Sock_MaxWaitTime指定的时间时，直接把客户端踢出去，只有当Sock_WaitTimeEnable = 1时生效
/* #undef Sock_TimeOutKick */

/*[NetSecurity]*/
//Flood攻击检测是否开启,1：开启   0：不开启
#define Sock_FloodAttackKickEnable 1
//Sock_FloodTimeInterval表示每次收到数据包的时间间隔是100(单位：毫秒)
#define Sock_FloodTimeInterval 100
//Sock_FloodKickCounter表示计算到连续10次，每次100毫秒时间间隔内发包，踢出此客户端
#define Sock_FloodKickCounter 10
