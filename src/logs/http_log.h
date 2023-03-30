#ifndef HTTP_LOG_H
#define HTTP_LOG_H
#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/sinks/rotating_file_sink.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
class myLog
{
private:
    static myLog *logInterface;
    myLog()
    {
        this->init();
    }
    ~myLog()
    {
        spdlog::drop_all();
    }

public:
    static myLog *getInterface()
    {
        if (logInterface == NULL)
        {
            logInterface = new myLog();
        }
        static freeInterface f;
        return logInterface;
    }

    std::shared_ptr<spdlog::logger> getLogger();

    class freeInterface
    {
    public:
        ~freeInterface()
        {
            if (logInterface != NULL)
                delete logInterface;
            logInterface = NULL;
        }
    };

private:
    void init();
    void initFile();
    void initLogger();

private:
    std::string rootPath;
    std::string erroLogPath;
    std::string InfoLogPath;
    std::shared_ptr<spdlog::logger> allLoggers;
    std::vector<spdlog::sink_ptr> sinks;
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> errLog;
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> inforLog;
};
#endif