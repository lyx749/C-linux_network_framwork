macro(SetConfigure)
    set(Log "error.log")
    set(LogLevel 8)
    set(WorkerProcesses 1)
    set(Daemon 1)
    set(ProcMsgRecvWorkThreadCount 120)
    set(ListenPortCount 1)
    set(ListenPort 80)
    set(worker_connections 2048)
    set(Sock_RecyConnectionWaitTime 150)
    set(Sock_WaitTimeEnable 1)
    set(Sock_MaxWaitTime 20)
    set(Sock_TimeOutKick 0)
    set(Sock_FloodAttackKickEnable 1)
    set(Sock_FloodTimeInterval 100)
    set(Sock_FloodKickCounter 10)
    configure_file(httpSeverConfig.h.in httpServerConfig.h @ONLY)
endmacro()
