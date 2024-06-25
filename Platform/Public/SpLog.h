#ifndef SPLOG_H
#define SPLOG_H

#include "SpEngine.h"

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
//#include "spdlog/spdlog.h"
#include "spdlog/common.h"

namespace spe{
    //日志管理类
    class SpLog{
    public:
        SpLog() = delete;
        SpLog(const SpLog&) = delete;
        SpLog& operator=(const SpLog&) = delete;
        static void Init();
        static SpLog* GetLoggerInstance(){
            return &sLoggerInstance;
        }

        template<typename... Args>
        void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args&& ...args){
            spdlog::memory_buf_t buf;
            fmt::vformat_to(fmt::appender(buf), fmt, fmt::make_format_args(args...));
            Log(loc, lvl, buf);
        }


    private:

        void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const spdlog::memory_buf_t& buffer);

        static SpLog sLoggerInstance;
    };

#define SP_LOG_LOGGER_CALL(spLog, level, ...) \
        (spLog)->Log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)
//日志调用的宏定义封装
#define SpLogTrace(...) SP_LOG_LOGGER_CALL(spe::SpLog::GetLoggerInstance(), spdlog::level::trace, __VA_ARGS__)
#define SpLogDebug(...) SP_LOG_LOGGER_CALL(spe::SpLog::GetLoggerInstance(), spdlog::level::debug, __VA_ARGS__)
#define SpLogInfo(...) SP_LOG_LOGGER_CALL(spe::SpLog::GetLoggerInstance(), spdlog::level::info, __VA_ARGS__)
#define SpLogWarning(...) SP_LOG_LOGGER_CALL(spe::SpLog::GetLoggerInstance(), spdlog::level::warn, __VA_ARGS__)
#define SpLogError(...) SP_LOG_LOGGER_CALL(spe::SpLog::GetLoggerInstance(), spdlog::level::err, __VA_ARGS__)
}

#endif