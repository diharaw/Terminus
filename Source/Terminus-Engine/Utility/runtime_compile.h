#pragma once

namespace runtime_compile
{
    extern bool compile(const char* input_path, const char* source, const char* output_path, const char* include_paths, const char* flags);
}
