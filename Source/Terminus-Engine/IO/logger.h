#pragma once

#include <string>

#define T_LOG_INFO(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::INFO)
#define T_LOG_WARNING(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::WARNING)
#define T_LOG_ERROR(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::ERROR)
#define T_LOG_FATAL(x) logger::log(x, std::string(__FILE__), __LINE__, LogLevel::FATAL)

// log verbosity options
// - timestamp = 1
// - level     = 2
// - file      = 4
// - line      = 8



enum class LogLevel
{
    INFO     = 0,
    WARNING  = 1,
    ERROR    = 2,
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
    
    extern void initialize();
    extern void set_verbosity(int flags);
    extern void open_file_stream();
    extern void open_console_stream();
    extern void open_custom_stream(CustomStreamCallback callback);
    extern void close_file_stream();
    extern void close_console_stream();
    extern void close_custom_stream();
    extern void log(std::string text, std::string file, int line, LogLevel level);
    extern void flush();
}
