#pragma once

#include <string>
#include <Core/macro.h>

#define T_LOG_INFO(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::INFO)
#define T_LOG_WARNING(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::WARNING)
#define T_LOG_ERROR(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::ERR)
#define T_LOG_FATAL(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::FATAL)

enum class LogLevel
{
    INFO     = 0,
    WARNING  = 1,
    ERR	     = 2,
    FATAL    = 3
};

namespace logger
{
    enum LogVerbosity
    {
        VERBOSITY_BASIC     = 0x00,
        VERBOSITY_TIMESTAMP = 0x01,
        VERBOSITY_LEVEL     = 0x02,
        VERBOSITY_FILE      = 0x04,
        VERBOSITY_LINE      = 0x08,
        VERBOSITY_ALL       = 0x0f
    };
    
    typedef void(*CustomStreamCallback)(std::string, LogLevel);
    
    extern TERMINUS_API void initialize();
    extern TERMINUS_API void set_verbosity(int flags);
    extern TERMINUS_API void open_file_stream();
    extern TERMINUS_API void open_console_stream();
    extern TERMINUS_API void open_custom_stream(CustomStreamCallback callback);
    extern TERMINUS_API void close_file_stream();
    extern TERMINUS_API void close_console_stream();
    extern TERMINUS_API void close_custom_stream();
    extern TERMINUS_API void log(std::string text, std::string file, int line, LogLevel level);
    
    // simplified api for scripting
    extern TERMINUS_API void log_info(std::string text);
    extern TERMINUS_API void log_error(std::string text);
    extern TERMINUS_API void log_warning(std::string text);
    extern TERMINUS_API void log_fatal(std::string text);
    
    extern TERMINUS_API void flush();
}
