#ifndef SPLOG_H
#define SPLOG_H

#include "SpEngine.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

namespace spe{
    class SpLog{
    public:
        SpLog() = delete;
        SpLog(const SpLog&) = delete;
        SpLog operator=(const SpLog&) = delete;
        static void Init();
        static spdlog::logger* GetLoggerInstance(){
            assert(sLoggerInstance && "Logger instance is null, maybe you have not execute SpLog::Init()");
            return sLoggerInstance.get();
        }
    private:
        static std::shared_ptr<spdlog::logger> sLoggerInstance;
    };

#define SpLogTrace(...) SPDLOG_LOGGER_TRACE(spe::SpLog::GetLoggerInstance(), __VA_ARGS__)
}

#endif