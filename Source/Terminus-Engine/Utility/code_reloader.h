#pragma once

namespace runtime_compiler
{
    extern bool compile(const char* input_path, const char* source, const char* lib_name, const char* output_path = nullptr);
}
