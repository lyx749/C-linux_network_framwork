
#include "http_log.h"
#include "httpServerConfig.h"
#include <iostream>
myLog *myLog::logInterface = NULL;
void myLog::init()
{
    this->initFile();
    this->initLogger();
}

void myLog::initFile()
{
    mkdir("../ServerLog", 0777);
    this->rootPath = "../ServerLog/";
    this->erroLogPath = ErrorLog;
    this->InfoLogPath = Log;
}

void myLog::initLogger()
{
    this->inforLog = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(this->rootPath + this->InfoLogPath, 5 * 5 * 1024, 10);
    this->errLog = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(this->rootPath + this->erroLogPath, 5 * 5 * 1024, 10);
    this->inforLog->set_level(spdlog::level::info);
    this->errLog->set_level(spdlog::level::err);
    this->sinks.push_back(this->inforLog);
    this->sinks.push_back(this->errLog);
    this->allLoggers = std::make_shared<spdlog::logger>("printLogInterface", sinks.begin(), sinks.end());
    this->allLoggers->set_pattern("[%l] [%Y-%m-%d %H:%M:%S,%e] [Precess:%P] [Thread:%t] - %v");
    this->allLoggers->flush_on(spdlog::level::info);
    spdlog::register_logger(this->allLoggers);
    spdlog::flush_every(std::chrono::seconds(10));
}

std::shared_ptr<spdlog::logger> myLog::getLogger()
{
    return this->allLoggers;
}

// void test()
// {
//     spdlog::init_thread_pool(10000, 1);
//     auto file_logger = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../ServerLog/test.log", true);
//     file_logger->set_level(spdlog::level::debug);
//     //auto a = spdlog::basic_logger_mt("unique_logger", "nihao", false);
//     auto console_logger = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
//     console_logger->set_level(spdlog::level::warn);
//     spdlog::sinks_init_list sink_list = {file_logger, console_logger};
//     spdlog::logger loggers("multi_sink", sink_list.begin(), sink_list.end());
//     loggers.set_pattern("[%l] [%Y-%m-%d %H:%M:%S,%e] [Process:%P] - %v");
//     loggers.error("error {}", "test");
//     loggers.warn("hello {}!", "world");
//     spdlog::drop_all();
// }

// char *getNowTime()
// {
//     char *timeBuff = (char *)CMemory::GetInstance()->AllocMemory(26, true);
//     struct timeval tv;
//     struct tm correctTime;
//     gettimeofday(&tv, NULL);
//     localtime_r(&tv.tv_sec, &correctTime);
//     strcpy(timeBuff, asctime(&correctTime));
//     timeBuff[24] = '\0';
//     return timeBuff;
// }

// void httpErrorLog(char *fmt, ...)
// {
//     CMemory *memoryPtr = CMemory::GetInstance();
//     char *fmtBuff = (char *)memoryPtr->AllocMemory(HTTP_MAX_ERROR_STR, true);
//     char *last = fmtBuff + HTTP_MAX_ERROR_STR;
//     va_list argus;
//     va_start(argus, fmt);
//     http_vslprintf(fmtBuff, last, fmt, argus);
//     char *timeBuff = getNowTime(); // 24 + 7 + 3    time : [error]
//     fprintf(stderr, "%s : [error] %s\n", timeBuff, fmtBuff);
//     va_end(argus);
//     memoryPtr->FreeMemory(timeBuff);
//     memoryPtr->FreeMemory(fmtBuff);
// }

// void httpCommonLog(char *fmt, ...)
// {
//     CMemory *memoryPtr = CMemory::GetInstance();
//     char *fmtBuff = (char *)memoryPtr->AllocMemory(HTTP_MAX_ERROR_STR, true);
//     char *last = fmtBuff + HTTP_MAX_ERROR_STR;
//     va_list argus;
//     va_start(argus, fmt);
//     http_vslprintf(fmtBuff, last, fmt, argus);
//     char *timeBuff = getNowTime(); // 24 + 7 + 3    time : [error]
//     fprintf(stderr, "%s : [log] %s\n", timeBuff, fmtBuff);
//     va_end(argus);
//     memoryPtr->FreeMemory(timeBuff);
//     memoryPtr->FreeMemory(fmtBuff);
// }