#ifndef SPLOG_H
#define SPLOG_H

#include "SpEngine.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

namespace spe{
    //日志管理类
    class SpLog{
    public:
        SpLog() = delete;
        SpLog(const SpLog&) = delete;
        SpLog& operator=(const SpLog&) = delete;
        static void Init();
        static spdlog::logger* GetLoggerInstance(){
            assert(sLoggerInstance && "Logger instance is null, maybe you have not execute SpLog::Init()");
            return sLoggerInstance.get();
        }
    private:
        static std::shared_ptr<spdlog::logger> sLoggerInstance;
    };

//日志调用的宏定义封装
#define SpLogTrace(...) SPDLOG_LOGGER_TRACE(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
#define SpLogDebug(...) SPDLOG_LOGGER_DEBUG(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
#define SpLogInfo(...) SPDLOG_LOGGER_INFO(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
#define SpLogWarning(...) SPDLOG_LOGGER_WARN(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
#define SpLogError(...) SPDLOG_LOGGER_ERROR(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
}

#endif