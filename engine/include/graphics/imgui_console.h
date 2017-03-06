#pragma once

#include <imgui.h>
#include <io/logger.h>

namespace terminus
{
    namespace imgui_console
    {
        extern void initialize();
        extern void shutdown();
        extern void logger_callback(std::string text, LogLevel level);
        extern void log(const char* fmt, ...); //IM_PRINTFARGS(2);
        extern void clear_log();
        extern void draw();
    }
}

