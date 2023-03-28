macro(SetConfigure)
    set(ErrorLog "error.log")
    set(Log "log")
    set(LogLevel 8)
    set(WorkerProcesses 1)
    set(Daemon 1)
    set(ProcMsgRecvWorkThreadCount 120)
    set(ListenPortCount 1)
    set(ListenPort 7777)
    set(worker_connections 2048)
    set(Sock_RecyConnectionWaitTime 150)
    set(Sock_WaitTimeEnable 1)
    set(Sock_MaxWaitTime 5)
    set(Sock_TimeOutKick 0)
    #message("Sock_TimeOutKick = ${Sock_TimeOutKick}")
    set(Sock_FloodAttackKickEnable 1)
    set(Sock_FloodTimeInterval 100)
    set(Sock_FloodKickCounter 10)
    configure_file(httpSeverConfig.h.in httpServerConfig.h @ONLY)
endmacro()
